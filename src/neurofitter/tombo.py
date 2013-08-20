"""
  Contains functions to set up the environment on the tombo server to run scripts in the 'simulate' 
  package
  
  @author Tom Close 
"""
import sys
if float(sys.version[0:3]) < 2.7:
    raise Exception("This script requires python version 2.7 or greater, you should add python " \
                    "2.7 to your path (i.e. PATH=$PATH:/apps/python/272/bin)")
import os
import time
import shutil
import subprocess
from copy import copy
from nineline.cells.build import path_to_exec

PYTHON_INSTALL_DIR='/apps/python/272'
OPEN_MPI_INSTALL_DIR='/opt/mpi/gnu/openmpi-1.6.3'
NEURON_INSTALL_DIR='/apps/DeschutterU/NEURON-7.3'
NEST_INSTALL_DIR='/apps/DeschutterU/nest-2.2.1'
SUNDIALS_INSTALL_DIR='/apps/DeschutterU/sundials-2.5.0'

def get_project_dir():
    """
    Returns the root directory of the project
    """
    # Root directory of the project code
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')) 

def create_work_dir(script_name, output_dir_parent=None, required_dirs=['src', 'xml'], 
                    dependencies=[('../PyNN/src', 'pyNN'), ('../nine/src/nine', 'nine')]):
    """
    Generates unique paths for the work and output directories, creating the work directory in the 
    process.
    
    @param script_name: The name of the script, used to name the directories appropriately
    @param output_dir_parent: The name of the parent directory in which the output directory """ \
    "will be created (defaults to $HOME/Output)." \
    """@param required_dirs: The sub-directories that need to be copied into the work directory    
    """
    if not output_dir_parent:
        output_dir_parent = os.path.join(os.environ['HOME'], 'output')
    work_dir_parent = os.path.realpath(os.path.join(os.environ['HOME'], 'work'))
    if not os.path.exists(work_dir_parent):
        raise Exception("Symbolic link to work directory is missing from your home directory " \
                        "(i.e. $HOME/work). A symbolic link should be created that points to " \
                        "an appropriate directory in your units sub-directory of '/work' " \
                        "(i.e. ln -s /work/<unit-name>/<user-name> $HOME/work)")
    if not work_dir_parent.startswith('/work'):
        raise Exception("$HOME/work be a symbolic link to a sub-directory of the " \
                        "high-performance filesystem mounted at '/work' (typically "\
                        "/work/<unit-name>/<user-name>).")
    # Automatically generate paths
    # Unique time for distinguishing runs    
    time_str = time.strftime('%Y-%m-%d-%A_%H-%M-%S', time.localtime()) 
    # Working directory path
    work_dir = os.path.join(work_dir_parent, script_name + "." + time_str + ".1") 
    #Ensure that working directory is unique
    created_work_dir=False
    count = 1
    while not created_work_dir:
        try:
            created_work_dir = not os.makedirs(work_dir) 
        except IOError as e:
            count += 1
            if count > 1000:
                print "Something has gone wrong, can't create directory '{}' after 1000 " \
                      "attempts".format(work_dir)
                raise e
            # Replace old count at the end of work directory with new count
            work_dir = '.'.join(work_dir.split('.')[:-1] + [str(count)]) 
    output_dir = os.path.join(output_dir_parent, os.path.split(work_dir)[1])
    init_work_dir(work_dir, required_dirs, time_str, dependencies)   
    return work_dir, output_dir

def init_work_dir(work_dir, required_dirs, time_str, dependencies):
    """
    Copies directories from the project directory to the work directory
    
    @param work_dir: The destination work directory
    @param required_dirs: The required sub-directories to be copied to the work directory
    """   
    # Copy snapshot of selected subdirectories to working directory
    for directory in required_dirs:
        print "Copying '{}' sub-directory to work directory".format(directory)
#        try:
#            subprocess.check_call("cd {project}; tar -czf {dir}.tar.gz {dir}; mv {dir}.tar.gz {work}/;"
#                                  "cd {work}; tar -xzf {work}/{dir}.tar.gz; rm {work}/{dir}.tar.gz"
#                                  .format(project=get_project_dir(), dir=directory, work=work_dir), 
#                                  shell=True) 
#        except IOError as e:
#            raise Exception("Could not copy directory '{}' to work directory ({})"
#                            .format(directory, e))
        shutil.copytree(os.path.join(get_project_dir(),directory), 
                        os.path.join(work_dir,directory), symlinks=True)
    if dependencies:
        dependency_dir = os.path.join(work_dir, 'depend') 
        os.mkdir(dependency_dir)
        for from_, to_ in dependencies:
            # If not an absolute path prepend the project directory as a relative path (useful for
            # getting dependencies from directories installed alongside the project directory
            if not from_.startswith('/'):
                from_ = get_project_dir() + os.path.sep + from_
            shutil.copytree(from_, os.path.join(dependency_dir, to_))
        
    # Make output directory for the generated files
    os.mkdir(os.path.join(work_dir, 'output'))
    # Save the git revision in the output folder for reference
    subprocess.call('cd {}; git rev-parse HEAD > {}'.\
                    format(get_project_dir(), os.path.join(work_dir, 'output', 'git_revision')), 
                    shell=True)
    # Write time string to file for future reference
    f = open(os.path.join(work_dir, 'output', 'time_stamp'), 'w')
    f.write(time_str + '\n')
    f.close()

def create_env(work_dir):
    """
    Creates a dictionary containing the appropriate environment variables
    
    @param work_dir: The work directory to set the envinroment variables for
    """
    env = os.environ.copy()
    env['PATH'] = (env['PATH'] + os.pathsep +
                   os.path.join(PYTHON_INSTALL_DIR, 'bin') + os.pathsep +
                   os.path.join(OPEN_MPI_INSTALL_DIR, 'bin') + os.pathsep +
                   os.path.join(NEURON_INSTALL_DIR, 'x86_64', 'bin') + os.pathsep +
                   os.path.join(NEST_INSTALL_DIR, 'bin') + os.pathsep +
                   os.path.join(SUNDIALS_INSTALL_DIR, 'bin'))
    env['PYTHONPATH'] = (os.path.join(NEST_INSTALL_DIR, 'lib', 'python2.7', 'dist-packages') +
                         os.pathsep + os.path.join(work_dir, 'src') + os.pathsep +
                         os.path.join(work_dir, 'depend'))
    env['LD_LIBRARY_PATH'] = (os.path.join(OPEN_MPI_INSTALL_DIR, 'lib') + os.pathsep +
                              os.path.join(NEST_INSTALL_DIR, 'lib', 'nest') + os.pathsep +
                              os.path.join(SUNDIALS_INSTALL_DIR, 'lib'))
    env['NINEML_SRC_PATH'] = os.path.join(work_dir, 'src')
    return env
    
def compile_nine(script_name, work_dir, env=None, script_dir='simulate', script_args='',
                    simulator='neuron'):
    """
    Compiles objects in the work directory that are required by the NINEML+ network
    
    @param script_name: Name of the script in the 'simulate' directory
    @param work_dir: The work directory in which the network is compiled
    @param env: The required environment variables (defaults to those generated by 'create_env(work_dir)')
    @param script_dir: The directory that the script is stored in (default: 'simulate')
    """
    if not env:
        env = create_env(work_dir)
    else:
        env = copy(env)
    env['NINEMLP_MPI'] = '1'
    # Remove NMODL build directory for pyNN neuron so it can be recompiled in script
    if simulator == 'neuron':
        pynn_nmodl_path = os.path.join(work_dir, 'depend', 'pyNN','neuron', 'nmodl')
        if os.path.exists(os.path.join(pynn_nmodl_path, 'x86_64')):
            shutil.rmtree(os.path.join(pynn_nmodl_path, 'x86_64'))
        subprocess.check_call('cd {}; {}'.format(pynn_nmodl_path, path_to_exec('nrnivmodl')), 
                              shell=True)
    print "Compiling required NINEML+ objects"
    print 'python {} {} --build build_only --simulator {}'.\
           format(os.path.join(work_dir, 'src', script_dir, script_name + '.py'),
                  script_args, simulator)
    subprocess.check_call('python {} {} --build build_only --simulator {}'.\
                           format(os.path.join(work_dir, 'src', script_dir, script_name + '.py'),
                                  script_args, simulator), shell=True, env=env)

def compile_custom(script_name, work_dir, env=None, script_dir='test', script_args='', 
                   simulator='neuron'):
    """
    Compiles objects in the work directory that are required by the custom script
    
    @param script_name: Name of the script in the 'simulate' directory
    @param work_dir: The work directory in which the network is compiled
    @param env: The required environment variables (defaults to those generated by 'create_env(work_dir)')
    @param script_dir: The directory that the script is stored in (default: 'simulate')
    """
    if not env:
        env = create_env(work_dir)
    else:
        env = copy(env)
    # Remove NMODL build directory for pyNN neuron so it can be recompiled in script
    print "Compiling required objects"
    subprocess.check_call('python {} {} --build build_only --simulator {}'.\
                          format(os.path.join(work_dir, 'src', script_dir, script_name + '.py'),
                                  script_args, simulator),
                          shell=True, env=env)

def submit_job(script_name, cmds, np, work_dir, output_dir, que_name='longP', max_memory='4g',
               virtual_memory='3g', time_limit=None, env=None, copy_to_output=['xml'], 
               strip_build_from_copy=True, name=None):
    """
    Create a jobscript in the work directory and then submit it to the tombo que
    
    @param script_name: The name of the script (used to give a meaningful name to the job)
    @param cmds: The commands to run on the cluster
    @param np: The number of processors to request for the job
    @param work_dir: The working directory to run the script from
    @param output_dir: The output directory to copy the results to
    @param time_limit [str]: The hard time limit for the job in "HH:MM:SS" format (increases the preference assigned by the scheduler)
    @param env: The required environment variables (defaults to those generated by 'create_env(work_dir)')    
    @param copy_to_output: Directories to copy into the output directory
    @param strip_build_from_copy: Removes all files and directories to be copied that have the name 'build'
    @param name: Records a name for the run (when generating multiple runs) so that the output directory can be easily renamed to a more informative name after it is copied to its destination via the command "mv <output_dir> `cat <output_dir>/name`"
    """
    if not env:
        env = create_env(work_dir)
    else:
        env = copy(env)
    copy_cmd = ''
    for to_copy in copy_to_output:
        origin = work_dir + os.path.sep + to_copy
        if strip_build_from_copy:
            copy_cmd+='find {origin} -name build -exec rm -r {{}} \; 2>/dev/null\n'.\
                      format(origin=origin)
        destination = output_dir + os.path.sep + to_copy
        base_dir = os.path.dirname(destination[:-1] if destination.endswith('/') else destination)
        copy_cmd += """
mkdir -p {base_dir}
cp -r {origin} {destination}
""".format(base_dir=base_dir, origin=origin, destination=destination)
    #Create jobscript
    if time_limit:
        if type(time_limit) != str or len(time_limit.split(':')) != 3:
            raise Exception("Poorly formatted time limit string '{}' passed to submit job"
                            .format(time_limit))
        time_limit_option= "\n# Set the maximum run time\n#$ -l h_rt {}\n".format(time_limit)
    else:
        time_limit_option=''
    if name:
        name_cmd = "echo '{name}' > {output_dir}/name"
    else:
        name_cmd = ""
    jobscript_path = os.path.join(work_dir, script_name + '.job')
    f = open(jobscript_path, 'w')
    f.write("""#!/usr/bin/env sh
    
# Parse the job script using this shell
#$ -S /bin/bash

# Send stdout and stderr to the same file.
#$ -j y

# Standard output and standard error files
#$ -o {work_dir}/output_stream
#$ -e {work_dir}/output_stream

# Name of the queue
#$ -q {que_name}

# use OpenMPI parallel environment with {np} processes
#$ -pe openmpi {np}
{time_limit}
# Set the memory limits for the script
#$ -l h_vmem={max_mem}
#$ -l virtual_free={virt_mem}

# Export the following env variables:
#$ -v HOME
#$ -v PATH
#$ -v PYTHONPATH
#$ -v LD_LIBRARY_PATH
#$ -v NINEMLP_SRC_PATH
#$ -v NINEMLP_MPI
#$ -v BREP_DEVEL
#$ -v PARAMDIR
#$ -v VERBOSE

###################################################
### Copy the model to all machines we are using ###
###################################################

# Set up the correct paths 
export PATH={path}:$PATH
export PYTHONPATH={pythonpath}
export LD_LIBRARY_PATH={ld_library_path}
export NINEMLP_SRC_PATH={nine_src_path}
export NINEMLP_MPI=1

echo "============== Starting mpirun ===============" 

cd {work_dir}
{cmds}

echo "============== Mpirun has ended =============="

echo "Copying files to output directory '{output_dir}'"
cp -r {work_dir}/output {output_dir}
cp {jobscript_path} {output_dir}/job
cp {work_dir}/output_stream {output_dir}/output
{name_cmd}
{copy_cmd}

echo "============== Done ===============" 
""".format(work_dir=work_dir, path=env['PATH'], pythonpath=env['PYTHONPATH'],
      ld_library_path=env['LD_LIBRARY_PATH'], nine_src_path=os.path.join(work_dir,'src'), np=np,
      que_name=que_name, max_mem=max_memory, virt_mem=virtual_memory, cmds=cmds, 
      output_dir=output_dir, name_cmd=name_cmd, copy_cmd=copy_cmd, jobscript_path=jobscript_path, 
      time_limit=time_limit_option))
    f.close()
    # Submit job
    print "Submitting job '%s' to que" % jobscript_path
    subprocess.check_call('qsub %s' % jobscript_path, shell=True)
    print "Your job '%s' has been submitted" % jobscript_path
    print "The output stream can be viewed by:"
    print "less " + os.path.join(work_dir, 'output_stream')
    print "Once completed the output files (including the output stream and job script) of this job will be copied to:"
    print output_dir
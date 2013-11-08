#!/usr/bin/env python
"""
 This script prepares/copies the required settings files and directories to the tombo scratch 
 filesystem and creates a job script before submitting it to the job que

 Author: Tom Close (tclose@oist.jp)
 Created: 20/8/2013
"""
#Name of the script for the output directory and submitted mpi job
SCRIPT_NAME = 'neurofitter'
# Required imports
import argparse
import os.path
import neurofitter.tombo
# Arguments to the script
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--np', type=int, default=256, 
                    help="The the number of processes to use for the simulation "
                         "(default: %(default)s)")
parser.add_argument('--que_name', type=str, default='short', 
                    help="The the que to submit the job to(default: %(default)s)")
parser.add_argument('--output_dir', default=None, type=str, 
                    help="The parent directory in which the output directory will be created "
                         "(defaults to $HOME/Output)")
parser.add_argument('--max_memory', type=str, default='3g', 
                    help="The maximum memory allocated to run the network (when tested the neuron "
                         "version required 1~1.5Gb and the NEST version ~500Mb so 2G is set as the "
                         "safe default")
parser.add_argument('--virtual_memory', type=str, default='2g', 
                    help="The average memory usage required by the program, decides when the "
                         "scheduler is able to run the job")
parser.add_argument('--name', type=str, default=None, 
                    help="Saves a file within the output directory with the name 'name' for easy "
                         "renaming of the output directory after it is copied to its final "
                         "destination, via the command 'mv <output_dir> `cat <output_dir>/name`'")
args = parser.parse_args()
# Create work directory and get path for output directory
work_dir, output_dir = neurofitter.tombo.create_work_dir(SCRIPT_NAME, args.output_dir, 
                                             required_dirs=required_dirs)
cmd_line = ("time mpirun mpineurofitter {work_dir}/settings.xml".format(script_name=SCRIPT_NAME,
                                                                        work_dir=work_dir))
# Submit job to que
if not args.dry_run:
    neurofitter.tombo.submit_job(SCRIPT_NAME, cmd_line, args.np, work_dir, output_dir, 
                                 copy_to_output=copy_to_output, que_name=args.que_name, 
                                 strip_build_from_copy=(not args.keep_build), name=args.name,
                                 max_memory=args.max_memory, virtual_memory=args.virtual_memory)
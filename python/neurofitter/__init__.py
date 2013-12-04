import urllib
from lxml import etree
from lxml.builder import E
from lxml.etree import tostring
import re
from copy import deepcopy
import os
import shutil

white_space_delim = re.compile('[ \n]+')
           
def copy_unique_files(files, dest_dir):
    # Turn filename into a list of filenames
    if isinstance(files, str):
        files = [files]
        returnstring = True
    else:
        returnstring = False
    if len(files) != len(set(files)):
        raise Exception("Original file list, {}, contains duplicates".format(files))
    # Calculate the minimum path depth that needs to be used to differentiate the file list
    for num_trunc in xrange(min([len(os.path.split(f)) for f in files])):
        trunc_files = [os.path.join(os.path.split(f)[-(num_trunc+1)]) 
                            for f in files] 
        if len(trunc_files) == len(set(trunc_files)):
            break
    # Create a list to hold the new file names
    new_files = []
    for f in files:
        # Get the destination path, which is the original filename plus the number of parent 
        # directories required so that all files are unique
        f_dest = os.path.join(*([dest_dir] + f.split(os.path.sep)[-(num_trunc+1):]))
        f_dest_dir = os.path.dirname(f_dest)
        # Create required parent directories
        if not os.path.exists(f_dest_dir):
            os.makedirs(f_dest_dir)
        shutil.copy(f, f_dest)
        new_files.append(f_dest)
    return new_files if not returnstring else new_files[0]

from .fitter import Fitter, RandomFitter
from .model import Model, ExecutableModel
from .experiment import Experiment, FakeExperiment
from .traces_reader import TracesReader, NormalTracesReader
from .error_value_calculator import ErrorValueCalculator, MPIErrorValueCalculator


class Settings(object):
    
    def __init__(self, program_name, dimensions=1, verbose_level=1, seed=500, sample_frequency=50, 
                 starting_points=(0.2,), bounds=((0.001, 0.2),), work_dir=None, fitter=RandomFitter(), 
                 traces_reader=NormalTracesReader(), model=ExecutableModel(), 
                 experiment=FakeExperiment(), error_value_calculator=MPIErrorValueCalculator(), 
                 print_parameter_file=False):
        """
        Creates a libNeuroFitterML settings object, which can be used to generate the appropriate
        xml for a NeuroFitter experiment. 
        
        `dimensions` -- This is an integer describing the number of model parameters you want to fit, 
                      so it is the number of dimensions of the hyperspace the optimization algorithm
                      will search in.
        `verbose_level` -- This integer tells how much output Neurofitter will write to stdout while it
                        is running. The higher the integer, the more output. At verboselevel 0 
                        Neurofitter will show no output.
        `seed` -- The random seed used in the stochastic optimization algorithms.
                SamplingFrequency -- The sampling frequency of your experimental data and the model 
                output. The units of this value should match these of the timestamps used in your 
                data files.
        `starting_points` -- The parameter sets that should be used initialize the optimization 
                          algorithm, this will not work for every optimization algorithm
        `bounds` -- The bounds of your model parameters. [list(list())] One line per parameter. 
                    E.g. [[1.0, 10000.0], [1,.0 50000.0]]
                    Put's the bounds of parameter 1 at [1.0 10000.0] and parameter 2 at [1.0 50000.0].
        `work_dir` -- Location where you want to let Neurofitter write its output
        `fitter` -- Determines the optimization algorithm used
        `traces_reader` -- Determines the output data reader used
        `model` -- Determines the model type (software) used
        `experiment` -- Determines the way experimental data is read by Neurofitter
        `error_value_calculator` -- Determines the algorithm by Neurofitter to convert output data 
                                    into a error value
        `print_parameter_file` -- If this is set to 1 it means Neurofitter will write the settings file
                                to stdout before starting. This is handy if you will save the
                                Neurofitter stdout output for later, and you still want to know the
                                settings file you used.  If set to 0, Neurofitter won't print the
                                settings.xml file                                    
        """
        try:
            if traces_reader.num_runs != experiment.num_runs:
                raise Exception("Number of runs in traces reader ({}) does not match the number implied"
                                " by the experiment ({})".format(traces_reader.num_runs, 
                                                                 experiment.num_runs))
        except AttributeError:
            pass # only if experiment and traces_readers both have number of runs arguments
        self.program_name = program_name
        self.dimensions = int(dimensions)
        self.verbose_level = verbose_level
        self.seed = int(seed)
        self.sample_frequency = float(sample_frequency)
        self.starting_points = [float(p) for p in starting_points]
        if len(bounds) != self.dimensions:
            raise Exception("Number of bounds ({}) does not match provided dimensions ({})"
                            .format(len(bounds), self.dimensions))
        self.bounds = []
        for dim_bounds in bounds:
            if len(dim_bounds) != 2:
                raise Exception("Two bounds must be provided for each dimension (found {})"
                                .format(len(dim_bounds)))
            self.bounds.append([float(b) for b in dim_bounds])
        self.work_dir = work_dir
        self.fitter = fitter
        self.traces_reader = traces_reader
        self.model = model
        self.experiment = experiment
        self.error_value_calculator = error_value_calculator
        self.print_parameter_file = bool(int(print_parameter_file))

    def to_xml(self):    
        element_list = [self.program_name,
                        E('Dimensions', str(self.dimensions)),
                        E('VerboseLevel', str(self.verbose_level)),
                        E('Seed', str(self.seed)),
                        E('SamplingFrequency', str(self.sample_frequency)),
                        E('StartingPoints', ' '.join([str(p) for p in self.starting_points])),
                        E('Bounds', '\n'.join([' '.join([str(b) for b in dim_bounds]) 
                                               for dim_bounds in self.bounds])),
                        E('WorkingDirectory', self.work_dir),
                        E('PrintParameterFile', str(int(self.print_parameter_file)))]
        element_list.extend(self.fitter.to_xml())
        element_list.extend(self.traces_reader.to_xml())
        element_list.extend(self.model.to_xml())
        element_list.extend(self.experiment.to_xml())
        element_list.extend(self.error_value_calculator.to_xml())
        return E(*element_list)
        
    def to_string(self):
        return ('<?xml version="1.0" encoding="UTF-8"?>\n' + 
                tostring(self.to_xml(), pretty_print=True))
            
    def set_work_directory(self, work_dir, proc_dir=None):
        self.work_dir = work_dir
        if proc_dir is None:
            proc_dir = work_dir
        for child in (self.fitter, self.traces_reader, self.model, self.experiment, 
                      self.error_value_calculator):
            try:
                child.set_work_directory(work_dir, proc_dir)
            except AttributeError:
                pass # Attempt to set the work directory of the child elements of the settings object
        
    @classmethod
    def from_xml(cls, element):
        program_name = element.tag
        dimensions = element.find('Dimensions').text.strip()
        verbose_level = element.find('VerboseLevel').text.strip()
        seed = element.find('Seed').text.strip()
        sample_frequency = element.find('SamplingFrequency').text.strip()
        starting_points = white_space_delim.split(element.find('StartingPoints').text.strip())
        bounds = []
        for dim_bounds in element.find('Bounds').text.strip().split('\n'):
            bounds.append(white_space_delim.split(dim_bounds.strip()))
        work_dir = element.find('WorkingDirectory').text.strip()
        print_parameter_file = element.find('PrintParameterFile').text.strip()
        fitter = Fitter.from_xml(element.find(Fitter.element_name + 'Type'),
                                 element.find(Fitter.element_name + 'Parameters'))
        traces_reader = TracesReader.from_xml(
                 element.find(TracesReader.element_name + 'Type'),
                 element.find(TracesReader.element_name + 'Parameters'))
        model = Model.from_xml(element.find(Model.element_name + 'Type'),
                               element.find(Model.element_name + 'Parameters'))
        experiment = Experiment.from_xml(element.find(Experiment.element_name + 'Type'),
                                         element.find(Experiment.element_name + 'Parameters'))
        error_value_calculator = ErrorValueCalculator.from_xml(
                 element.find(ErrorValueCalculator.element_name + 'Type'),
                 element.find(ErrorValueCalculator.element_name + 'Parameters'))
        return cls(program_name, dimensions, verbose_level, seed, sample_frequency, starting_points, 
                   bounds, work_dir, fitter, traces_reader, model, experiment, error_value_calculator, 
                   print_parameter_file)
    
    def save(self, filename):
        with open(filename, 'w') as f:
            f.write(self.to_string()) 
           
    @classmethod
    def load(cls, url):
        """
        Read a NineML user-layer file and return a Model object.
    
        If the URL does not have a scheme identifier, it is taken to refer to a
        local file.
        """
        if not isinstance(url, file):
            f = urllib.urlopen(url)
            doc = etree.parse(f)
            f.close()
        else:
            doc = etree.parse(url)
        root = doc.getroot()
        return Settings.from_xml(root)
    
    
def prepare_work_dir(work_dir, settings, num_processes=1):
    """
    Prepare the work directory for running neurofitter, particularly when using MPI
    
    `work_dir` -- the path of the work directory to prepare for a Neurofitter run
    `settings` -- the `Settings` object containing the complete settings for the Neurofitter run
    `num_processes` -- the number of processes to be used by Neurofitter
    """
    os.makedirs(work_dir)
    settings = deepcopy(settings)
    settings.set_work_directory(work_dir)
    settings.save(os.path.join(work_dir, 'settings.xml'))
    if num_processes > 1:
        for i in xrange(num_processes):
            proc_dir = os.path.join(work_dir, str(i))
            os.mkdir(proc_dir)
            # Copy settings to allow the work directory to be set without affecting the passed object
            proc_settings = deepcopy(settings) 
            proc_settings.set_work_directory(work_dir, proc_dir)
            proc_settings.save(os.path.join(work_dir, 'settings.xml_{}'.format(i)))

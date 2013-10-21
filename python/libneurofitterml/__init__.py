import urllib
from lxml import etree
from lxml.builder import E
from lxml.etree import tostring
import re

white_space_delim = re.compile('[ \n]+')

from .fitter import Fitter
from .model import Model
from .experiment import Experiment
from .traces_reader import TracesReader
from .error_value_calculator import ErrorValueCalculator    


class Settings(object):
    
    def __init__(self, program_name, dimensions, verbose_level, seed, sample_frequency, 
                 starting_points, bounds, work_dir, fitter, traces_reader, model, experiment, 
                 error_value_calculator, print_parameter_file):
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
        settings_xml = E(*element_list)
        return '<?xml version="1.0" encoding="UTF-8"?>\n' + tostring(settings_xml, 
                                                                     pretty_print=True)
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
           

def parse(url):
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
    
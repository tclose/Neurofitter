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
        self.program_name = program_name
        self.dimensions = int(dimensions)
        self.verbose_level = verbose_level
        self.seed = int(seed)
        self.sample_frequency = float(sample_frequency)
        self.starting_points = [float(p) for p in starting_points]
        self.bounds = [float(b) for b in bounds]
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
                        E('Bounds', ' '.join([str(b) for b in self.bounds])),
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
        bounds = white_space_delim.split(element.find('Bounds').text.strip())
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
    
from lxml.builder import E
from . import white_space_delim

        
class Experiment(object):
    
    element_name='Experiment'
    
    def to_xml(self):
        if isinstance(self, FileListExperiment):
            type_xml = E(self.element_name + 'Type', 'FileList')
        elif isinstance(self, FakeExperiment):
            type_xml = E(self.element_name + 'Type', 'Fake')
        else:
            assert(False), "The class '{}' is not listed in Experiment.to_xml()".format(type(self))
        return (type_xml, self._to_xml())
        
    @classmethod
    def from_xml(cls, type_element, parameters_element):
        experiment_type = type_element.text.strip()
        if experiment_type == 'FileList':
            experiment = FileListExperiment.from_xml(parameters_element)
        elif experiment_type == 'Fake':
            experiment = FakeExperiment.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(experiment_type))
        return experiment
    
    
class FileListExperiment(Experiment):
    
    def __init__(self, files_list):
        self.files_list = files_list 
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('FilesList', '\n'.join(self.files_list)))

    @classmethod
    def from_xml(cls, element):
        parameters = white_space_delim.split(element.find('FilesList').text.strip())
        return cls(parameters)

    
class FakeExperiment(Experiment):
    
    def __init__(self, parameters):
        self.parameters = parameters
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('Parameters', ' '.join(self.parameters)))

    @classmethod
    def from_xml(cls, element):
        parameters = white_space_delim.split(element.find('Parameters').text.strip())
        return cls(parameters)
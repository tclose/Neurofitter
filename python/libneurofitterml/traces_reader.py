from lxml.builder import E
from . import white_space_delim


class TracesReader(object):
   
    element_name='TracesReader'

    def to_xml(self):
        if isinstance(self, NormalTracesReader):
            type_xml = E(self.element_name + 'Type', 'Normal')
        else:
            assert(False), "The class '{}' is not listed in TracesReader.to_xml()".format(type(self))
        return (type_xml, self._to_xml())
        
    @classmethod
    def from_xml(cls, type_element, parameters_element):
        traces_reader_type = type_element.text.strip()
        if traces_reader_type == 'Normal':
            traces_reader = NormalTracesReader.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(traces_reader_type))
        return traces_reader 
   
   
class NormalTracesReader(TracesReader):
    
    def __init__(self, num_runs, num_run_params, run_params, num_periods, periods, num_record_sites,
                 record_sites, output):
        self.num_runs = int(num_runs)
        self.num_run_params = int(num_run_params) 
        self.run_params = [float(p) for p in run_params]
        self.num_periods = int(num_periods)
        self.periods = [float(p) for p in periods]
        self.num_record_sites = int(num_record_sites)
        self.record_sites = record_sites
        self.output = output
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('NumberOfRuns', str(self.num_runs)),
                 E('NumberOfRunParameters', str(self.num_run_params)),
                 E('RunParameters', ' '.join([str(p) for p in self.run_params])),
                 E('NumberOfPeriods', str(self.num_periods)),
                 E('Periods', ' '.join([str(p) for p in self.periods])),
                 E('NumberOfRecordSites', str(self.num_record_sites)),
                 E('RecordSites', ' '.join(self.record_sites)),
                 E('OutputFilePrefix', self.output))
        
    @classmethod
    def from_xml(cls, element):
        num_runs = element.find('NumberOfRuns').text.strip()
        num_run_params = element.find('NumberOfRunParameters').text.strip()
        run_params = white_space_delim.split(element.find('RunParameters').text.strip())
        num_periods = element.find('NumberOfPeriods').text.strip()
        periods = white_space_delim.split(element.find('Periods').text.strip())
        num_record_sites = element.find('NumberOfRecordSites').text.strip()
        record_sites = element.find('RecordSites').text.strip()
        output = element.find('OutputFilePrefix').text.strip()
        return cls(num_runs, num_run_params, run_params, num_periods, periods, num_record_sites,
                   record_sites, output)

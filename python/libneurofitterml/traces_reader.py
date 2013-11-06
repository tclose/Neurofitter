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
        """
        `num_runs ` -- The number of times the model has to be run for every error value evaluation.
                       E.g. for different injection strengths you want to run the model a multiple 
                       of times.
        `num_run_params ` -- The number of parameters for every run, e.g. if you inject at two 
                             locations in a cell, you would want to put "2" here
        `run_params ` -- The actual run parameters, so e.g. the different injection strength, 1 line
                         per run, the last value of every line should be the weight you want to give
                         to that run, compared to the others:
        `num_periods ` -- The number of time periods you want to use in your traces
        `periods ` -- The start and stop times of the periods, 1 line for every period, the third 
                      number is the weight
        `num_record_sites ` -- Number of recording sites in the cell, every recording site should be
                               1 column in the model output files and experimental files
        `record_sites ` -- The weight of every recording site
        `output ` -- The prefix that will be used by Neurofitter to read the experimental and model 
                     output data. E.g. the file for run 2 would be "output_Run2.dat"
        """
        self.num_runs = int(num_runs)
        self.num_run_params = int(num_run_params) 
        if len(run_params) != self.num_run_params:
            raise Exception("Number of run_params ({}) does not match provided num_runs ({})"
                            .format(len(run_params), self.num_runs))
        self.run_params = []            
        for rparams in run_params:
            self.run_params.append([float(p) for p in rparams])
        self.num_periods = int(num_periods)
        if len(periods) != self.num_periods:
            raise Exception("Number of periods ({}) does not match provided num_periods ({})"
                            .format(len(periods), self.num_periods))
        self.periods = []            
        for prd in periods:
            self.periods.append([float(p) for p in prd])
        self.num_record_sites = int(num_record_sites)
        self.record_sites = record_sites
        self.output = output
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('NumberOfRuns', str(self.num_runs)),
                 E('NumberOfRunParameters', str(self.num_run_params)),
                 E('RunParameters', '\n'.join([' '.join([str(p) for p in rparam]) 
                                               for rparam in self.run_params])),
                 E('NumberOfPeriods', str(self.num_periods)),
                 E('Periods', '\n'.join([' '.join([str(p) for p in prd]) 
                                               for prd in self.periods])),
                 E('NumberOfRecordSites', str(self.num_record_sites)),
                 E('RecordSites', ' '.join(self.record_sites)),
                 E('OutputFilePrefix', self.output))
        
    @classmethod
    def from_xml(cls, element):
        num_runs = element.find('NumberOfRuns').text.strip()
        num_run_params = element.find('NumberOfRunParameters').text.strip()
        run_params = []
        for rparam in element.find('RunParameters').text.strip().split('\n'):
            run_params.append(white_space_delim.split(rparam.strip()))
        num_periods = element.find('NumberOfPeriods').text.strip()
        periods = []
        for prd in element.find('Periods').text.strip().split('\n'):
            periods.append(white_space_delim.split(prd.strip()))
        num_record_sites = element.find('NumberOfRecordSites').text.strip()
        record_sites = element.find('RecordSites').text.strip()
        output = element.find('OutputFilePrefix').text.strip()
        return cls(num_runs=num_runs, num_run_params=num_run_params, run_params=run_params, 
                   num_periods=num_periods, periods=periods, num_record_sites=num_record_sites,
                   record_sites=record_sites, output=output)

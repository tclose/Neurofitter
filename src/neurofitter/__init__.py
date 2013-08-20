from __future__ import absolute_import
from lxml.builder import E
from lxml.etree import tostring


class Filter(object):
    
    element_name='TracesReader'
    
    def __init__(self, filter_type, num_points):
        self.type = filter_type
        self.num_points = num_points 
        
    def xml_elements(self):
        return (E(self.element_name + 'Type', self.type),
                E('FilterParameters',
                  E('NumberOfPoints', str(self.num_points))))

   
class TracesReader(object):
    
    element_name='TracesReader'
    
    def __init__(self, reader_type, num_runs, run_params, periods, record_sites, output_prefix):
        self.type = reader_type
        self.num_runs = num_runs 
        self.run_params = run_params  
        self.periods = periods
        self.record_sites = record_sites  
        self.output_prefix = output_prefix 
        
    def xml_elements(self):
        return (E(self.element_name + 'Type', self.type),
                E(self.element_name + 'Parameters',
                  E('NumberOfRuns', str(self.num_runs)),
                  E('NumberOfRunParameters', str(len(self.run_params))),
                  E('RunParameters', ' '.join([str(p) for p in self.run_params])),
                  E('NumberOfPeriods', str(len(self.periods))),
                  E('Periods', ' '.join([str(p) for p in self.periods])),
                  E('NumberOfRecordSites', str(1)),
                  E('RecordSites', str(1)),
                  E('OutputFilePrefix', 'output')))
        
   
class Model(object):
    
    element_name='Model'
    
    def __init__(self, model_type, command, output_dir, parameter_file, show_output):
        self.type = model_type
        self.command = command 
        self.output_dir = output_dir  
        self.parameter_file = parameter_file  
        self.show_output = show_output  
        
    def xml_elements(self):
        return (E(self.element_name + 'Type', self.type),
                E(self.element_name + 'Parameters',
                  E('ExecuteCommand', self.command),
                  E('OutputDirectory', self.output_dir),
                  E('ParameterFile', self.parameter_file),
                  E('ShowExecuteOutput', str(int(self.show_output)))))
        
        
class Experiment(object):
    
    element_name='Experiment'
    
    def __init__(self, exp_type, files_list):
        self.type = exp_type
        self.files_list = files_list 
        
    def xml_elements(self):
        return (E(self.element_name + 'Type', self.type), 
                E(self.element_name + 'Parameters', E('FilesList', '\n'.join(self.files_list))))


class ErrorValueCalculator(object):
    
    element_name='ErrorValueCalculator'
    
    def __init__(self, error_val_type, calculator=None, export_file=None):
        self.type = error_val_type
        self.calculator = calculator 
        self.export_file = export_file 
        
    def xml_elements(self):
        element_list = [self.element_name +'Parameters']
        if self.calculator is not None:
            element_list.extend(self.calculator.xml_elements())
        if self.export_file is not None:
            element_list.append(E('enableFileExport', '1'))
            element_list.append(E('exportFile', self.export_file))
        else:
            element_list.append(E('enableFileExport', '0'))
        return E(self.element_name + 'Type', self.type), E(*element_list)


def create_settings_xml(program_name, dimensions, verbose_level, seed, sample_frequency, 
                        starting_points, bounds, work_dir, filt, traces_reader, model, 
                        experiment, error_value_calculator, print_parameter_file=True):

        
    element_list = [program_name,
                    E('Dimensions', str(dimensions)),
                    E('VerboseLevel', str(verbose_level)),
                    E('Seed', str(seed)),
                    E('SamplingFrequency', str(sample_frequency)),
                    E('StartingPoints', ' '.join([str(p) for p in starting_points])),
                    E('Bounds', ' '.join([str(b) for b in bounds])),
                    E('WorkingDirectory', work_dir),
                    E('PrintParameterFile', str(int(print_parameter_file)))]
    element_list.extend(filt.xml_elements())
    element_list.extend(traces_reader.xml_elements())
    element_list.extend(model.xml_elements())
    element_list.extend(experiment.xml_elements())
    element_list.extend(error_value_calculator.xml_elements())
    xml_settings = E(*element_list)
    return tostring(xml_settings, pretty_print=True)
   
                                              
if __name__ == '__main__':
    
    filt = Filter('Random', 2000)
    traces_reader = TracesReader('Normal', num_runs=5, run_params=[1], periods=[2], record_sites=1, 
                                 output_prefix='output')
    model = Model('Executable', 
                  """unset DISPLAY; cd SCRATCH/model; x86_64/special -nogui
                     runModel3_Nannuli2_dia_0_4.hoc""",
                  output_dir='outputs', parameter_file='/model/param.dat', show_output=False)
    experiment = Experiment('FileList', 
                            ['SHAREDDIR/data/Testg18e-6_Nannuli2_dia_0_4_Jan08.dat',
                             'SHAREDDIR/data/Testg32e-6_Nannuli2_dia_0_4_Jan08.dat',
                             'SHAREDDIR/data/Testg54e-6_Nannuli2_dia_0_4_Jan08.dat',
                             'SHAREDDIR/data/Testg81e-6_Nannuli2_dia_0_4_Jan08.dat',
                             'SHAREDDIR/data/Testg111e-6_Nannuli2_dia_0_4_Jan08.dat'])
    calculator = ErrorValueCalculator('RMS', export_file='ErrorValue.dat')
    error_value_calculator = ErrorValueCalculator('MPI', calculator, 
            export_file='/users/anwar/TuningCaDynamicsNeurofitterDend_depthOnly/Dia0_4/work/pfs/BestFit.dat.JOBID')
    
    print create_settings_xml(program_name='TestProgram', dimensions=1, verbose_level=1, seed=500, 
                              sample_frequency=50, starting_points=[0.2], bounds=[0.001, 0.2], 
                              work_dir='/home/tclose/', filt=filt, traces_reader=traces_reader, 
                              model=model, experiment=experiment, 
                              error_value_calculator=error_value_calculator)

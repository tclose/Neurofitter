from __future__ import absolute_import
from lxml.builder import E
from lxml.etree import tostring

#===================================================================================================
# Fitter 
#===================================================================================================

class Fitter(object): 
    
    element_name = "Fitter"

    def to_xml(self):
        if isinstance(self, RandomFitter):
            type_xml = E(self.element_name + 'Type', 'Random')
        elif isinstance(self, EOFitter):
            type_xml = E(self.element_name + 'Type', 'EO')
        else:
            assert(False)
        return (type_xml, self._to_xml())

    @classmethod
    def from_xml(cls, type_element, parameters_element):
        fitter_type = type_element.text
        if fitter_type == 'Random':
            obj = RandomFitter.from_xml(parameters_element)
        elif fitter_type == 'EOFitter':
            obj = EOFitter.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of fitter".format(fitter_type))
        return obj


class RandomFitter(Fitter):
    
    def __init__(self, num_points):
        """
        num_points -- The number of random points to use 
        """
        self.num_points = num_points

    def _to_xml(self):
        return E(self.element_name + 'Parameters', 
                 E('NumberOfPoints', str(self.num_points)))

    @classmethod
    def from_xml(cls, element):
        num_points = int(element.find('NumberOfPoints'))
        return cls(num_points)


class EOFitter(Fitter):
    
    valid_replacement = ('Plus',)
    valid_cross_type = ('global', 'local')
    valid_cross_obj = ('intermediate',)
    
    def __init__(self, pop_size, num_offspring, replacement, max_gen, min_gen, max_eval, 
                 target_fitness, steady_gen, cross_type, cross_obj, tau_loc, tau_glob, beta):
        """
        pop_size -- Number of individuals in population 
        num_offspring -- Number of offspring 
        replacement -- Replacement strategy 
        max_gen -- Maximal number of generations 
        min_gen -- Minimal number of generations 
        max_eval -- Maximal number of evaluation 
        target_fitness -- Target fitness below which ES stops 
        steady_gen -- Maximal number of generations without improvement 
        cross_type -- number of parents for cross-over (global or local) 
        cross_obj -- type of cross-over 
        tau_loc -- internal parameter for self-evolution 
        tau_glob -- internal parameter for self-evolution 
        beta -- internal parameter for self-evolution 
        """
        if replacement not in self.valid_replacement:
            raise Exception("Invalid cross-over type '{}', can be '{}'".format(replacement, 
                    "', '".join(self.valid_replacement)))
        if cross_type not in self.valid_cross_type:
            raise Exception("Invalid cross-over type '{}', can be '{}'".format(cross_type, 
                    "', '".join(self.valid_cross_type)))
        if cross_obj not in self.valid_cross_obj:
            raise Exception("Invalid cross-over type '{}', can be '{}'".format(cross_obj, 
                    "', '".join(self.valid_cross_obj)))
        self.pop_size = int(pop_size)
        self.num_offspring = int(num_offspring)
        self.replacement = replacement
        self.max_gen = int(max_gen)
        self.min_gen = int(min_gen)
        self.max_eval = int(max_eval)
        self.target_fitness = target_fitness
        self.steady_gen = int(steady_gen)
        self.cross_type = cross_type
        self.cross_obj = cross_obj
        self.tau_loc = float(tau_loc)
        self.tau_glob = float(tau_glob)
        self.beta = float(beta)
    
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('popSize', self.pop_size),
                 E('nbOffspring', self.num_offspring),
                 E('replacement', self.replacement),
                 E('maxGen', self.max_gen),
                 E('minGen', self.min_gen),
                 E('maxval', self.max_eval),
                 E('targetFitness', self.target_fitness),
                 E('steadyGen', self.steady_gen),
                 E('crossType', self.cross_type),
                 E('crossObj', self.cross_obj),
                 E('TauLoc', self.tau_loc),
                 E('TauGlob', self.tau_glob),
                 E('Beta', self.beta))
    
    @classmethod
    def from_xml(cls, element):
        pop_size = element.find('popSize')
        num_offspring = element.find('nbOffspring')
        replacement = element.find('replacement')
        max_gen = element.find('maxGen')
        min_gen = element.find('minGen')
        max_eval = element.find('maxval')
        target_fitness = element.find('targetFitness')
        steady_gen = element.find('steadyGen')
        cross_type = element.find('crossType')
        cross_obj = element.find('crossObj')
        tau_loc = element.find('TauLoc')
        tau_glob = element.find('TauGlob')
        beta = element.find('Beta')
        return cls(pop_size, num_offspring, replacement, max_gen, min_gen, max_eval, target_fitness, 
                   steady_gen, cross_type, cross_obj, tau_loc, tau_glob, beta)


#===================================================================================================
# Traces Reader
#===================================================================================================
   
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
        traces_reader_type = type_element.text
        if traces_reader_type == 'Normal':
            obj = RandomFitter.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(traces_reader_type))
        return obj 
   
   
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
                 E('NumberOfPeriods', str(len(self.periods))),
                 E('Periods', ' '.join([str(p) for p in self.periods])),
                 E('NumberOfRecordSites', str(self.num_record_sites)),
                 E('RecordSites', ' '.join(self.record_sites)),
                 E('OutputFilePrefix', self.output))
        
    @classmethod
    def from_xml(cls, element):
        num_runs = element.find('NumberOfRuns')
        num_run_params = element.find('NumberOfRunParameters')
        run_params = element.find('RunParameters').split(' ')
        num_periods = element.find('NumberOfPeriods')
        periods = element.find('Periods').split(' ')
        num_record_sites = element.find('NumberOfRecordSites')
        record_sites = element.find('RecordSites')
        output = element.find('OutputFilePrefix')
        return cls(num_runs, num_run_params, run_params, num_periods, periods, num_record_sites,
                   record_sites, output)


#===================================================================================================
# Model   
#===================================================================================================
   
class Model(object):
    
    element_name='Model'
    
    def to_xml(self):
        if isinstance(self, GenesisModel):
            type_xml = E(self.element_name + 'Type', 'Genesis')
        elif isinstance(self, NeuronModel):
            type_xml = E(self.element_name + 'Type', 'Neuron')
        elif isinstance(self, ExecutableModel):
            type_xml = E(self.element_name + 'Type', 'Executable')
        else:
            assert(False), "The class '{}' is not listed in Model.to_xml()".format(type(self))
        return (type_xml, self._to_xml())
        
    @classmethod
    def from_xml(cls, type_element, parameters_element):
        model_type = type_element.text
        if model_type == 'Genesis':
            obj = GenesisModel.from_xml(parameters_element)
        elif model_type == 'Neuron':
            obj = NeuronModel.from_xml(parameters_element)
        elif model_type == 'Executable':
            obj = ExecutableModel.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(traces_reader_type))
        return obj


    
class ExecutableModel(Model):
    
    def __init__(self, command, output_dir, parameter_file, show_output):
        self.command = command 
        self.output_dir = output_dir  
        self.parameter_file = parameter_file  
        self.show_output = show_output  
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('ExecuteCommand', self.command),
                 E('OutputDirectory', self.output_dir),
                 E('ParameterFile', self.parameter_file),
                 E('ShowExecuteOutput', str(int(self.show_output))))

    @classmethod
    def from_xml(cls, element):
        command = element.find('ExecuteCommand')
        output_dir = element.find('OutputDirectory')
        parameter_file = element.find('ParameterFile')
        show_output = element.find('ShowExecuteOutput')
        return cls(command, output_dir, parameter_file, show_output)
    
    
class GenesisModel(Model):
    
    def __init__(self, genesis_location, model_dir, output_dir, model_src, param_file, 
                 show_exec_output):
        self.genesis_location = genesis_location 
        self.model_dir = model_dir  
        self.output_dir = output_dir  
        self.model_src = model_src
        self.param_file = param_file
        self.show_exec_output = show_exec_output
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('GenesisLocation', self.genesis_location),
                 E('ModelDirectory', self.model_dir),
                 E('OutputDirectory', self.output_dir),
                 E('ModelSource', str(self.model_src),
                 E('ParameterFile', str(self.param_file),
                 E('ShowExecuteOutput', str(self.show_exec_output)))

    @classmethod
    def from_xml(cls, element):
        genesis_location = element.find('GenesisLocation')
        model_dir = element.find('ModelDirectory')
        output_dir = element.find('OutputDirectory')
        model_src = element.find('ModelSource')
        param_file = element.find('ParameterFile')
        show_exec_output = element.find('ShowExecuteOutput')
        return cls(genesis_location, model_dir, output_dir, model_src, param_file, show_exec_output)     
        
        
class NeuronModel(Model):
    
    def __init__(self):
        raise NotImplementedError

    def _to_xml(self):
        raise NotImplementedError

    @classmethod
    def from_xml(cls, element):
        raise NotImplementedError
        
        
#===================================================================================================
# Experiment
#===================================================================================================
        
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
        traces_reader_type = type_element.text
        if traces_reader_type == 'FileList':
            obj = FileListExperiment.from_xml(parameters_element)
        elif traces_reader_type == 'Fake':
            obj = FakeExperiment.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(traces_reader_type))
        return obj
    
    
class FileListExperiment(Experiment):
    
    def __init__(self, files_list):
        self.files_list = files_list 
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters', E('FilesList', '\n'.join(self.files_list)))

    @classmethod
    def from_xml(cls, element):
        raise NotImplementedError


#===================================================================================================
# Error value calculator
#===================================================================================================

class ErrorValueCalculator(object):
    
    element_name='ErrorValueCalculator'
    
    def __init__(self, error_val_type, calculator=None, export_file=None, 
                 enable_traces_export=None):
        self.type = error_val_type
        self.calculator = calculator 
        self.export_file = export_file
        self.enable_traces_export = enable_traces_export 
        
    def to_xml(self):
        element_list = [self.element_name +'Parameters']
        if self.calculator is not None:
            element_list.extend(self.calculator.to_xml())
        if self.export_file is not None:
            element_list.append(E('enableFileExport', '1'))
            element_list.append(E('exportFile', self.export_file))
        else:
            element_list.append(E('enableFileExport', '0'))
        if self.enable_traces_export is not None:
            element_list.append(E('enableTracesExport', str(self.enable_traces_export)))
        return E(self.element_name + 'Type', self.type), E(*element_list)


#===================================================================================================
# Complete Settings object
#===================================================================================================

class Settings(object):
    
    
    def __init__(self, program_name, dimensions, verbose_level, seed, sample_frequency, 
                 starting_points, bounds, work_dir, fitter, traces_reader, model, experiment, 
                 error_value_calculator, print_parameter_file=True):
        self.program_name = program_name
        self.dimensions = dimensions
        self.verbose_level = verbose_level
        self.seed = seed
        self.sample_frequency = sample_frequency
        self.starting_points = starting_points
        self.bounds = bounds
        self.work_dir = work_dir
        self.fitter = fitter
        self.traces_reader = traces_reader
        self.model = model
        self.experiment = experiment
        self.error_value_calculator = error_value_calculator
        self.print_parameter_file = print_parameter_file

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
    def from_xml(self):
        pass
                                              
if __name__ == '__main__':
    
    fitter = Fitter('Random', 2000)
    traces_reader = TracesReader('Normal', num_runs=5, 
                                 run_params=[0.000018, 2000, 0.0000326, 1000, 0.0000545, 500, 
                                             0.0000815, 250, 0.000111, 125], 
                                 periods=[500, 550, 1.0, 550, 5000, 1.0], record_sites=1, 
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
    calculator = ErrorValueCalculator('RMS', export_file='ErrorValue.dat', enable_traces_export=0)
    error_value_calculator = ErrorValueCalculator('MPI', calculator, 
            export_file='/users/anwar/TuningCaDynamicsNeurofitterDend_depthOnly/Dia0_4/work/pfs/BestFit.dat.JOBID')
    settings = Settings(program_name='TestProgram', dimensions=1, verbose_level=1, seed=500, 
                        sample_frequency=50, starting_points=[0.2], bounds=[0.001, 0.2], 
                        work_dir='/home/tclose/', fitter=fitter, traces_reader=traces_reader, 
                        model=model, experiment=experiment, 
                        error_value_calculator=error_value_calculator)
    print settings.to_xml()
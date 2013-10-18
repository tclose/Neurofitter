from __future__ import absolute_import
import urllib
from lxml import etree
from lxml.builder import E
from lxml.etree import tostring
import re

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
        self.num_points = int(num_points)

    def _to_xml(self):
        return E(self.element_name + 'Parameters', 
                 E('NumberOfPoints', str(self.num_points)))

    @classmethod
    def from_xml(cls, element):
        num_points = element.find('NumberOfPoints').text
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
        pop_size = element.find('popSize').text
        num_offspring = element.find('nbOffspring').text
        replacement = element.find('replacement').text
        max_gen = element.find('maxGen').text
        min_gen = element.find('minGen').text
        max_eval = element.find('maxval').text
        target_fitness = element.find('targetFitness').text
        steady_gen = element.find('steadyGen').text
        cross_type = element.find('crossType').text
        cross_obj = element.find('crossObj').text
        tau_loc = element.find('TauLoc').text
        tau_glob = element.find('TauGlob').text
        beta = element.find('Beta').text
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
        num_runs = element.find('NumberOfRuns').text
        num_run_params = element.find('NumberOfRunParameters').text
        run_params = element.find('RunParameters').text.split(' ')
        num_periods = element.find('NumberOfPeriods').text
        periods = element.find('Periods').text.split(' ')
        num_record_sites = element.find('NumberOfRecordSites').text
        record_sites = element.find('RecordSites').text
        output = element.find('OutputFilePrefix').text
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
            raise Exception("Unrecognised type '{}' of traces reader".format(model_type))
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
        command = element.find('ExecuteCommand').text
        output_dir = element.find('OutputDirectory').text
        parameter_file = element.find('ParameterFile').text
        show_output = element.find('ShowExecuteOutput').text
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
                 E('ModelSource', str(self.model_src)),
                 E('ParameterFile', str(self.param_file)),
                 E('ShowExecuteOutput', str(self.show_exec_output)))

    @classmethod
    def from_xml(cls, element):
        genesis_location = element.find('GenesisLocation').text
        model_dir = element.find('ModelDirectory').text
        output_dir = element.find('OutputDirectory').text
        model_src = element.find('ModelSource').text
        param_file = element.find('ParameterFile').text
        show_exec_output = element.find('ShowExecuteOutput').text
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
    
    file_list_delimeter = re.compile('[ \n]+')
    
    def __init__(self, files_list):
        self.files_list = files_list 
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('FilesList', '\n'.join(self.files_list)))

    @classmethod
    def from_xml(cls, element):
        parameters = cls.file_list_delimeter.split(element.find('FilesList').text)
        return cls(parameters)

    
class FakeExperiment(Experiment):
    
    def __init__(self, parameters):
        self.parameters = parameters
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('Parameters', ' '.join(self.parameters)))

    @classmethod
    def from_xml(cls, element):
        parameters = element.find('Parameters').text.split(' ')
        return cls(parameters)
        

#===================================================================================================
# Error value calculator
#===================================================================================================

class ErrorValueCalculator(object):
    
    element_name='ErrorValueCalculator'

    def to_xml(self):
        if isinstance(self, MPIErrorValueCalculator):
            type_xml = E(self.element_name + 'Type', 'MPI')
        elif isinstance(self, RMSErrorValueCalculator):
            type_xml = E(self.element_name + 'Type', 'RMS')
        elif isinstance(self, MatrixErrorValueCalculator):
            type_xml = E(self.element_name + 'Type', 'Matrix')
        else:
            assert(False), ("The class '{}' is not listed in ErrorValueCalculator.to_xml()"
                            .format(type(self)))
        return (type_xml, self._to_xml())
        
    @classmethod
    def from_xml(cls, type_element, parameters_element):
        error_value_calculator = type_element.text
        if error_value_calculator == 'MPI':
            obj = MPIErrorValueCalculator.from_xml(parameters_element)
        elif error_value_calculator == 'RMS':
            obj = RMSErrorValueCalculator.from_xml(parameters_element)
        elif error_value_calculator == 'Matrix':
            obj = MatrixErrorValueCalculator.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(error_value_calculator))
        return obj


class MatrixErrorValueCalculator(ErrorValueCalculator):    
        
    class VdVdtMatrix(object):
        
        element_name = 'VdVdtMatrix'
            
        def to_xml(self):
            if isinstance(self, MatrixErrorValueCalculator.DirectVdVdtMatrix):
                type_xml = E(self.element_name + 'Type', 'Direct')
            else:
                assert(False), ("The class '{}' is not listed in VdVdtMatrix.to_xml()"
                                .format(type(self)))
            return (type_xml, self._to_xml())
            
        @classmethod
        def from_xml(cls, type_element, parameters_element):
            vdVdt_matrix = type_element.text
            if vdVdt_matrix == 'Direct':
                obj = MatrixErrorValueCalculator.DirectVdVdtMatrix.from_xml(parameters_element)
            else:
                raise Exception("Unrecognised type '{}' of traces reader".format(vdVdt_matrix))
            return obj
        
    class DirectVdVdtMatrix(MatrixErrorValueCalculator.VdVdtMatrix):
        
        def __init__(self, v_length, dvdt_length, minimal_v, maximal_v, compare_precision,
                     numeric_output_format, sum_of_square_roots):
            self.v_length = int(v_length)
            self.dvdt_length = int(dvdt_length)
            self.minimal_v = float(minimal_v)
            self.maximal_v = float(maximal_v)
            self.compare_precision = float(compare_precision)
            self.numeric_output_format = numeric_output_format
            self.sum_of_square_roots = sum_of_square_roots
            
        def _to_xml(self):
            return E(self.element_name + 'Parameters',
                    E('vLength', self.v_length),
                    E('dVdtLength', self.dvdt_length),
                    E('minimalV', self.minimal_v),
                    E('maximalV', self.maximal_v),
                    E('comparePrecision', self.compare_precision),
                    E('numericOutputFormat', self.numeric_output_format),
                    E('SumOfSquareRoots', self.sum_of_square_roots))

        @classmethod
        def from_xml(cls, element):
            v_length = element.find('vLength').text
            dvdt_length = element.find('dVdtLength').text
            minimal_v= element.find('minimalV').text
            maximal_v= element.find('maximalV').text
            compare_precision= element.find('comparePrecision').text
            numeric_output_format= element.find('numericOutputFormat').text
            sum_of_square_roots = element.find('SumOfSquareRoots').text
            return cls(v_length, dvdt_length, minimal_v, maximal_v, compare_precision,
                       numeric_output_format, sum_of_square_roots)
            
    def __init__(self, vdVdt_matrix, enable_file_export, export_file):
        self.vdVdt_matrix = vdVdt_matrix
        self.enable_file_export = enable_file_export
        self.export_file = export_file
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 *(self.vdVdt_matrix.to_xml() + 
                   (E('enableFileExport', self.enable_file_export),
                   E('exportFile', self.export_file))))

    @classmethod
    def from_xml(cls, element):
        vdVdt_matrix = cls.VdVdtMatrix.from_xml(
                  element.find(cls.VdVdtMatrix.element_name + 'Type'), 
                  element.find(cls.VdVdtMatrix.element_name + 'Parameters'))
        enable_file_export = element.find('enableFileExport').text
        export_file = element.find('exportFile').text
        return cls(vdVdt_matrix, enable_file_export, export_file)
    

class RMSErrorValueCalculator(ErrorValueCalculator):
    
    def __init__(self, enable_file_export, export_file, enable_traces_export):
        self.export_file = export_file
        self.enable_file_export = enable_file_export
        self.enable_traces_export = enable_traces_export 
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('enableFileExport', self.enable_file_export),
                 E('exportFile', self.export_file),
                 E('enableTracesExport', self.enable_traces_export))

    @classmethod
    def from_xml(cls, element):
        enable_file_export = element.find('enableFileExport').text
        export_file = element.find('exportFile').text
        enable_traces_export = element.finde('enableTracesExport').text
        return cls(enable_file_export, export_file, enable_traces_export)


class MPIErrorValueCalculator(ErrorValueCalculator):
        
    def __init__(self, calculator, enable_file_export, export_file):
        self.calculator = calculator 
        self.export_file = export_file
        self.enable_file_export = enable_file_export
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 *(self.calculator.to_xml() + 
                   (E('enableFileExport', self.enable_file_export),
                    E('exportFile', self.export_file))))

    @classmethod
    def from_xml(cls, element):
        calculator = ErrorValueCalculator.from_xml(element.find(ErrorValueCalculator.element_name + 'Type'), 
                                                     element.find(ErrorValueCalculator.element_name + 'Parameters'))
        enable_file_export = element.find('enableFileExport').text
        export_file = element.find('exportFile').text
        return cls(calculator, enable_file_export, export_file)


#===================================================================================================
# Complete Settings object
#===================================================================================================

class Settings(object):
    
    def __init__(self, program_name, dimensions, verbose_level, seed, sample_frequency, 
                 starting_points, bounds, work_dir, fitter, traces_reader, model, experiment, 
                 error_value_calculator, print_parameter_file):
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
    def from_xml(cls, element):
        program_name = element.tag
        dimensions = element.find('Dimensions').text
        verbose_level = element.find('VerboseLevel').text
        seed = element.find('Seed').text
        sample_frequency = element.find('SamplingFrequency').text
        starting_points = element.find('StartingPoints').text
        bounds = element.find('Bounds').text
        work_dir = element.find('WorkingDirectory').text
        print_parameter_file = element.find('PrintParameterFile').text
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
           
                                              
if __name__ == '__main__':
    
#     fitter = Fitter('Random', 2000)
#     traces_reader = TracesReader('Normal', num_runs=5, 
#                                  run_params=[0.000018, 2000, 0.0000326, 1000, 0.0000545, 500, 
#                                              0.0000815, 250, 0.000111, 125], 
#                                  periods=[500, 550, 1.0, 550, 5000, 1.0], record_sites=1, 
#                                  output_prefix='output')
#     model = Model('Executable', 
#                   """unset DISPLAY; cd SCRATCH/model; x86_64/special -nogui
#                      runModel3_Nannuli2_dia_0_4.hoc""",
#                   output_dir='outputs', parameter_file='/model/param.dat', show_output=False)
#     experiment = Experiment('FileList', 
#                             ['SHAREDDIR/data/Testg18e-6_Nannuli2_dia_0_4_Jan08.dat',
#                              'SHAREDDIR/data/Testg32e-6_Nannuli2_dia_0_4_Jan08.dat',
#                              'SHAREDDIR/data/Testg54e-6_Nannuli2_dia_0_4_Jan08.dat',
#                              'SHAREDDIR/data/Testg81e-6_Nannuli2_dia_0_4_Jan08.dat',
#                              'SHAREDDIR/data/Testg111e-6_Nannuli2_dia_0_4_Jan08.dat'])
#     calculator = ErrorValueCalculator('RMS', export_file='ErrorValue.dat', enable_traces_export=0)
#     error_value_calculator = ErrorValueCalculator('MPI', calculator, 
#             export_file='/users/anwar/TuningCaDynamicsNeurofitterDend_depthOnly/Dia0_4/work/pfs/BestFit.dat.JOBID')
#     settings = Settings(program_name='TestProgram', dimensions=1, verbose_level=1, seed=500, 
#                         sample_frequency=50, starting_points=[0.2], bounds=[0.001, 0.2], 
#                         work_dir='/home/tclose/', fitter=fitter, traces_reader=traces_reader, 
#                         model=model, experiment=experiment, 
#                         error_value_calculator=error_value_calculator)
#     print settings.to_xml()

    settings = parse('/home/tclose/git/neurofitter/example/roimpisettings.xml')
    print settings.to_xml()
    
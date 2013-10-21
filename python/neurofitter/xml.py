from __future__ import absolute_import
import urllib
from lxml import etree
from lxml.builder import E
from lxml.etree import tostring
import re

    
_white_space_delim = re.compile('[ \n]+')

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
        fitter_type = type_element.text.strip()
        if fitter_type == 'Random':
            fitter = RandomFitter.from_xml(parameters_element)
        elif fitter_type == 'EO':
            fitter = EOFitter.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of fitter".format(fitter_type))
        return fitter


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
        num_points = element.find('NumberOfPoints').text.strip()
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
                 E('popSize', str(self.pop_size)),
                 E('nbOffspring', str(self.num_offspring)),
                 E('replacement', self.replacement),
                 E('maxGen', str(self.max_gen)),
                 E('minGen', str(self.min_gen)),
                 E('maxEval', str(self.max_eval)),
                 E('targetFitness', self.target_fitness),
                 E('steadyGen', str(self.steady_gen)),
                 E('crossType', self.cross_type),
                 E('crossObj', self.cross_obj),
                 E('TauLoc', str(self.tau_loc)),
                 E('TauGlob', str(self.tau_glob)),
                 E('Beta', str(self.beta)))
    
    @classmethod
    def from_xml(cls, element):
        pop_size = element.find('popSize').text.strip()
        num_offspring = element.find('nbOffspring').text.strip()
        replacement = element.find('replacement').text.strip()
        max_gen = element.find('maxGen').text.strip()
        min_gen = element.find('minGen').text.strip()
        max_eval = element.find('maxEval').text.strip()
        target_fitness = element.find('targetFitness').text.strip()
        steady_gen = element.find('steadyGen').text.strip()
        cross_type = element.find('crossType').text.strip()
        cross_obj = element.find('crossObj').text.strip()
        tau_loc = element.find('TauLoc').text.strip()
        tau_glob = element.find('TauGlob').text.strip()
        beta = element.find('Beta').text.strip()
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
        run_params = _white_space_delim.split(element.find('RunParameters').text.strip())
        num_periods = element.find('NumberOfPeriods').text.strip()
        periods = _white_space_delim.split(element.find('Periods').text.strip())
        num_record_sites = element.find('NumberOfRecordSites').text.strip()
        record_sites = element.find('RecordSites').text.strip()
        output = element.find('OutputFilePrefix').text.strip()
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
        model_type = type_element.text.strip()
        if model_type == 'Genesis':
            model = GenesisModel.from_xml(parameters_element)
        elif model_type == 'Neuron':
            model = NeuronModel.from_xml(parameters_element)
        elif model_type == 'Executable':
            model = ExecutableModel.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(model_type))
        return model


    
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
        command = element.find('ExecuteCommand').text.strip()
        output_dir = element.find('OutputDirectory').text.strip()
        parameter_file = element.find('ParameterFile').text.strip()
        show_output = element.find('ShowExecuteOutput').text.strip()
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
        genesis_location = element.find('GenesisLocation').text.strip()
        model_dir = element.find('ModelDirectory').text.strip()
        output_dir = element.find('OutputDirectory').text.strip()
        model_src = element.find('ModelSource').text.strip()
        param_file = element.find('ParameterFile').text.strip()
        show_exec_output = element.find('ShowExecuteOutput').text.strip()
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
        parameters = _white_space_delim.split(element.find('FilesList').text.strip())
        return cls(parameters)

    
class FakeExperiment(Experiment):
    
    def __init__(self, parameters):
        self.parameters = parameters
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('Parameters', ' '.join(self.parameters)))

    @classmethod
    def from_xml(cls, element):
        parameters = _white_space_delim.split(element.find('Parameters').text.strip())
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
        error_value_calculator_type = type_element.text.strip()
        if error_value_calculator_type == 'MPI':
            error_value_calculator = MPIErrorValueCalculator.from_xml(parameters_element)
        elif error_value_calculator_type == 'RMS':
            error_value_calculator = RMSErrorValueCalculator.from_xml(parameters_element)
        elif error_value_calculator_type == 'Matrix':
            error_value_calculator = MatrixErrorValueCalculator.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader"
                            .format(error_value_calculator_type))
        return error_value_calculator


class VdVdtMatrix(object):
    
    element_name = 'VdVdtMatrix'
        
    def to_xml(self):
        if isinstance(self, DirectVdVdtMatrix):
            type_xml = E(self.element_name + 'Type', 'Direct')
        else:
            assert(False), ("The class '{}' is not listed in VdVdtMatrix.to_xml()"
                            .format(type(self)))
        return (type_xml, self._to_xml())
        
    @classmethod
    def from_xml(cls, type_element, parameters_element):
        vdVdt_matrix = type_element.text.strip()
        if vdVdt_matrix == 'Direct':
            obj = DirectVdVdtMatrix.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(vdVdt_matrix))
        return obj
    
class DirectVdVdtMatrix(VdVdtMatrix):
    
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
                E('vLength', str(self.v_length)),
                E('dVdtLength', str(self.dvdt_length)),
                E('minimalV', str(self.minimal_v)),
                E('maximalV', str(self.maximal_v)),
                E('comparePrecision', str(self.compare_precision)),
                E('numericOutputFormat', self.numeric_output_format),
                E('SumOfSquareRoots', self.sum_of_square_roots))

    @classmethod
    def from_xml(cls, element):
        v_length = element.find('vLength').text.strip()
        dvdt_length = element.find('dVdtLength').text.strip()
        minimal_v= element.find('minimalV').text.strip()
        maximal_v= element.find('maximalV').text.strip()
        compare_precision= element.find('comparePrecision').text.strip()
        numeric_output_format= element.find('numericOutputFormat').text.strip()
        sum_of_square_roots = element.find('SumOfSquareRoots').text.strip()
        return cls(v_length, dvdt_length, minimal_v, maximal_v, compare_precision,
                   numeric_output_format, sum_of_square_roots)


class MatrixErrorValueCalculator(ErrorValueCalculator):    
                   
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
        vdVdt_matrix = VdVdtMatrix.from_xml(
                  element.find(VdVdtMatrix.element_name + 'Type'), 
                  element.find(VdVdtMatrix.element_name + 'Parameters'))
        enable_file_export = element.find('enableFileExport').text.strip()
        export_file = element.find('exportFile').text.strip()
        return cls(vdVdt_matrix, enable_file_export, export_file)
    

class RMSErrorValueCalculator(ErrorValueCalculator):
    
    def __init__(self, enable_file_export, export_file, enable_traces_export):
        self.export_file = export_file
        self.enable_file_export = enable_file_export
        self.enable_traces_export = enable_traces_export 
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('enableFileExport', self.enable_file_export),
                 E('enableTracesExport', self.enable_traces_export),
                 E('exportFile', self.export_file))

    @classmethod
    def from_xml(cls, element):
        enable_file_export = element.find('enableFileExport').text.strip()
        export_file = element.find('exportFile').text.strip()
        enable_traces_export = element.find('enableTracesExport').text.strip()
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
        enable_file_export = element.find('enableFileExport').text.strip()
        export_file = element.find('exportFile').text.strip()
        return cls(calculator, enable_file_export, export_file)


#===================================================================================================
# Complete Settings object
#===================================================================================================

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
        starting_points = _white_space_delim.split(element.find('StartingPoints').text.strip())
        bounds = _white_space_delim.split(element.find('Bounds').text.strip())
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
           
                                              
if __name__ == '__main__':
    
    settings = parse('/home/tclose/git/neurofitter/example/butera.xml')
    print settings.to_xml()
    
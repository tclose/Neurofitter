from lxml.builder import E
import os
import shutil
   
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
    
    def set_work_dir(self, work_dir):
        self.work_dir = work_dir


class ExecutableModel(Model):
    
    def __init__(self, command=None, output_dir=None, parameter_file=None, show_output=False):
        """
        Interface to a generic model program 
    
        `command` -- The command to be executed
        `output_dir` -- The directory that will contain the output files of the model
        `param_file` -- The filename that Neurofitter has to use to write the file that contains the
                        information for Executable
        `show_exec_output` -- Set to 1 if you want to see the stdout output (handy for debugging),
                              0 if not.
        """ 
        self.command = command 
        self.output_dir = output_dir  
        self.parameter_file = parameter_file  
        self.show_output = show_output  
        
    def _to_xml(self):
        # Insert parameter file before first optional argument
        cmdlist = self.command.split(' -', 1)
        command = ' -'.join([cmdlist[0] + ' ' + self.parameter_file] + cmdlist[1:])
        return E(self.element_name + 'Parameters',
                 E('ExecuteCommand', command),
                 E('OutputDirectory', self.output_dir),
                 E('ParameterFile', self.parameter_file),
                 E('ShowExecuteOutput', str(int(self.show_output))))

    @classmethod
    def from_xml(cls, element):
        command = element.find('ExecuteCommand').text.strip()
        output_dir = element.find('OutputDirectory').text.strip()
        parameter_file = element.find('ParameterFile').text.strip()
        show_output = element.find('ShowExecuteOutput').text.strip()
        return cls(command=command, output_dir=output_dir, parameter_file=parameter_file, 
                   show_output=show_output)
        
    def set_work_directory(self, work_dir, proc_dir=None): #@UnusedVariable proc_dir is used instead of work dir    
        self.output_dir = os.path.join(proc_dir, 'outputs')
        os.mkdir(self.output_dir)
        os.mkdir(os.path.join(proc_dir, 'model'))
        self.parameter_file = os.path.join(os.path.join(proc_dir, 'model', 'parameters.txt'))
    
    
class GenesisModel(Model):
    
    def __init__(self, genesis_location, model_dir, output_dir, model_src, param_file, 
                 show_exec_output):
        """
        Interface to the Genesis simulator.
    
        `genesis_location` -- The location of the genesis binary
        `model_dir` -- The directory that contains the genesis model files            
        `output_dir` -- The directory that will contain the output files of the model
        `model_src` -- The root source file (.g) of the model
        `param_file` -- The filename that Neurofitter has to use to write the file that contains the
                        information for Genesis
        `show_exec_output` -- Set to 1 if you want to see Neuron stdout output (handy for debugging),
                              0 if not. 
        """        
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
        return cls(genesis_location=genesis_location, model_dir=model_dir, output_dir=output_dir, 
                   model_src=model_src, param_file=param_file, show_exec_output=show_exec_output)     
        
        
class NeuronModel(Model):
    
    def __init__(self, neuron_location, model_dir, output_dir, model_src, param_file, 
                 show_exec_output):
        """
        Interface to the Neuron simulator.
    
        `neuron_location` -- The location of the Neuron special file
        `model_dir` -- The directory that contains the Neuron model files            
        `output_dir` -- The directory that will contain the output files of the model
        `model_src` -- The root source file (.hoc) of the model
        `param_file` -- The filename that Neurofitter has to use to write the file that contains the
                        information for Neuron
        `show_exec_output` -- Set to 1 if you want to see Neuron stdout output (handy for debugging),
                              0 if not. 
        """
        self.neuron_location = neuron_location.replace(' -nogui', '') # No gui is stripped as it is automatically included
        self.model_dir = model_dir  
        self.output_dir = output_dir  
        self.model_src = model_src
        self.param_file = param_file
        self.show_exec_output = show_exec_output
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('SpecialLocation', self.neuron_location + ' -nogui'),
                 E('ModelDirectory', self.model_dir),
                 E('OutputDirectory', self.output_dir),
                 E('ModelSource', str(self.model_src)),
                 E('ParameterFile', str(self.param_file)),
                 E('ShowExecuteOutput', str(self.show_exec_output)))

    @classmethod
    def from_xml(cls, element):
        neuron_location = element.find('SpecialLocation').text.strip()
        model_dir = element.find('ModelDirectory').text.strip()
        output_dir = element.find('OutputDirectory').text.strip()
        model_src = element.find('ModelSource').text.strip()
        param_file = element.find('ParameterFile').text.strip()
        show_exec_output = element.find('ShowExecuteOutput').text.strip()
        return cls(neuron_location=neuron_location, model_dir=model_dir, output_dir=output_dir, 
                   model_src=model_src, param_file=param_file, show_exec_output=show_exec_output) 
        
from lxml.builder import E

   
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
        
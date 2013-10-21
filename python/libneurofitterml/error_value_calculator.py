from lxml.builder import E
from . import white_space_delim


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

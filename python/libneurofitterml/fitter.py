from lxml.builder import E


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


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


class Mesh(Fitter):
    
    def __init__(self):
        pass
    
    def _to_xml(self):
        return E(self.element_name + 'Parameters')
    
    @classmethod
    def from_xml(cls, element): #@UnusedVariable
        return cls()
    

class RandomFitter(Fitter):
    
    def __init__(self, num_points):
        """
        `num_points` -- The number of random points to use 
        """
        self.num_points = int(num_points)

    def _to_xml(self):
        return E(self.element_name + 'Parameters', 
                 E('NumberOfPoints', str(self.num_points)))

    @classmethod
    def from_xml(cls, element):
        num_points = element.find('NumberOfPoints').text.strip()
        return cls(num_points)


class FileFitter(Fitter):
    
    def __init__(self, num_points, filename):
        """
        `num_points` -- The number of random points to use 
        """
        self.num_points = int(num_points)
        self.filename = filename

    def _to_xml(self):
        return E(self.element_name + 'Parameters', 
                 E('NumberOfPoints', str(self.num_points)),
                 E('Filename', self.filename))

    @classmethod
    def from_xml(cls, element):
        num_points = element.find('NumberOfPoints').text.strip()
        filename = element.find('Filename').text.strip()
        return cls(num_points, filename)


class SwarmFitter(Fitter):
    
    def __init__(self, num_runs):
        """
        `num_runs` -- Number of "evolutions" you want the swarm optimization to run
        """
        self.num_runs = num_runs
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('NumberOfRuns', str(self.num_runs)))
        
    @classmethod
    def from_xml(cls, element):
        num_runs = element.find('NumberOfRuns').text.strip()
        cls(num_runs)


class EOFitter(Fitter):
    
    valid_replacement = ('Plus',)
    valid_cross_type = ('global', 'local')
    valid_cross_obj = ('intermediate',)
    
    def __init__(self, pop_size, num_offspring, replacement, max_gen, min_gen, max_eval, 
                 target_fitness, steady_gen, cross_type, cross_obj, tau_loc, tau_glob, beta):
        """
        `pop_size` -- Number of individuals in population 
        `num_offspring` -- Number of offspring 
        `replacement` -- Replacement strategy 
        `max_gen` -- Maximal number of generations 
        `min_gen` -- Minimal number of generations 
        `max_eval` -- Maximal number of evaluation 
        `target_fitness` -- Target fitness below which ES stops 
        `steady_gen` -- Maximal number of generations without improvement 
        `cross_type` -- number of parents for cross-over (global or local) 
        `cross_obj` -- type of cross-over 
        `tau_loc` -- internal parameter for self-evolution 
        `tau_glob` -- internal parameter for self-evolution 
        `beta` -- internal parameter for self-evolution 
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
    
    def _to_xml(self, tag_name=None):
        if tag_name is None:
            tag_name = self.element_name
        return E(tag_name + 'Parameters',
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



class NOMADFitter(Fitter):
    
    def __init__(self, coarsening_exponent, initial_poll_size, poll_basis, refining_exponent, 
                 max_poll_size, poll_order, poll_complete, poll_directions, initial_search, 
                 initial_complete, initial_points, iterative_search, iterative_complete, 
                 iterative_points, speculative_search, poll_size_term, iterations, truth_evals, 
                 new_truth_evals, consecutive_fails, filter_relative, hmax, hmin, filter_norm, 
                 surrogate_tolerance, scaling_choice, gen_cons_nb, use_surrogate, start_pt_file, 
                 bounds_file, results_file):
        """
        `coarsening_exponent` --  Exponent to enlarge poll size in case of failure 
        `initial_poll_size` -- Initial poll size
        `poll_basis` -- ??
        `refining_exponent` -- ?? 
        `max_poll_size` -- Maximum poll size
        `poll_order` --  Dynamic 0/1 
        `poll_complete` --  Opportunistic 0/1 
        `poll_directions` --  MADS directions 0/1/2/3/4
        `initial_search` --  No initial search 0/1/3 
        `initial_complete` --  Opportunistic 0/1 
        `initial_points` --  Number of points after initial search 
        `iterative_search` --  Random search at start every iteration 0/1/2 
        `iterative_complete` --  Opportunistic 0/1 
        `iterative_points` --   Number of points generated by iteration 
        `speculative_search` --  Do a search in poll direction (only MADS) 0/1 
        `poll_size_term` --  Terminate when poll size is smaller than this value 
        `iterations` -- Max number of iterations
        `truth_evals` --   Max number of truth evaluations 
        `new_truth_evals` --  Max number of non-cache truth evaluations 
        `consecutive_fails` --  Max number of consecutive better-solution-failures 
        `filter_relative` --  HMax is relative to starting point 0/1 
        `hmax` -- ??
        `hmin` -- ??
        `filter_norm` --  L-2 squared norm 0/1/2 
        `surrogate_tolerance` -- ?? 
        `scaling_choice` --  No scaling 0/1/2/3 
        `gen_cons_nb` --  Number of constraints 
        `use_surrogate` --  Use surrogate 0/1 
        `start_pt_file` -- ??
        `bounds_file` -- ??
        `results_file` -- ??
        """
        self.coarsening_exponent = coarsening_exponent
        self.initial_poll_size = initial_poll_size
        self.poll_basis = poll_basis
        self.refining_exponent = refining_exponent
        self.max_poll_size = max_poll_size
        self.poll_order = poll_order
        self.poll_complete = poll_complete
        self.poll_directions = poll_directions
        self.initial_search = initial_search
        self.initial_complete = initial_complete
        self.initial_points = initial_points
        self.iterative_search = iterative_search
        self.iterative_complete = iterative_complete
        self.iterative_points = iterative_points
        self.speculative_search = speculative_search
        self.poll_size_term = poll_size_term
        self.iterations = iterations
        self.truth_evals = truth_evals
        self.new_truth_evals = new_truth_evals
        self.consecutive_fails = consecutive_fails
        self.filter_relative = filter_relative
        self.hmax = hmax
        self.hmin = hmin
        self.filter_norm = filter_norm
        self.surrogate_tolerance = surrogate_tolerance
        self.scaling_choice = scaling_choice
        self.gen_cons_nb = gen_cons_nb
        self.use_surrogate = use_surrogate
        self.start_pt_file = start_pt_file
        self.bounds_file = bounds_file
        self.results_file = results_file
    
    def _to_xml(self, tag_name=None):
        if tag_name is None:
            tag_name = self.element_name
        return E(tag_name + 'Parameters',
                E('COARSENING_EXPONENT', str(self.coarsening_exponent)),                          
                E('INITIAL_POLL_SIZE', str(self.initial_poll_size)),     
                E('POLL_BASIS', str(self.poll_basis)),            
                E('REFINING_EXPONENT', str(self.refining_exponent)),     
                E('MAX_POLL_SIZE', str(self.max_poll_size)),         
                E('POLL_ORDER', str(self.poll_order)),            
                E('POLL_COMPLETE', str(self.poll_complete)),         
                E('POLL_DIRECTIONS', str(self.poll_directions)),       
                E('INITIAL_SEARCH', str(self.initial_search)),        
                E('INITIAL_COMPLETE', str(self.initial_complete)),      
                E('INITIAL_POINTS', str(self.initial_points)),        
                E('ITERATIVE_SEARCH', str(self.iterative_search)),      
                E('ITERATIVE_COMPLETE', str(self.iterative_complete)),    
                E('ITERATIVE_POINTS', str(self.iterative_points)),      
                E('SPECULATIVE_SEARCH', str(self.speculative_search)),    
                E('POLL_SIZE_TERM', str(self.poll_size_term)),        
                E('ITERATIONS', str(self.iterations)),    
                E('TRUTH_EVALS', str(self.truth_evals)),           
                E('NEW_TRUTH_EVALS', str(self.new_truth_evals)),       
                E('CONSECUTIVE_FAILS', str(self.consecutive_fails)),     
                E('FILTER_RELATIVE', str(self.filter_relative)),       
                E('HMAX', str(self.hmax)),                  
                E('HMIN', str(self.hmin)),                  
                E('FILTER_NORM', str(self.filter_norm)),           
                E('SURROGATE_TOLERANCE', str(self.surrogate_tolerance)),   
                E('SCALING_CHOICE', str(self.scaling_choice)),        
                E('GEN_CONS_NB', str(self.gen_cons_nb)),           
                E('USE_SURROGATE', str(self.use_surrogate)),         
                E('START_PT_FILE', str(self.start_pt_file)),
                E('BOUNDS_FILE', str(self.bounds_file)),
                E('RESULTS_FILE', str(self.results_file)))
    
    @classmethod
    def from_xml(cls, element):
        coarsening_exponent = element.find('COARSENING_EXPONENT').text.strip()                           
        initial_poll_size = element.find('INITIAL_POLL_SIZE').text.strip()      
        poll_basis = element.find('POLL_BASIS').text.strip()             
        refining_exponent = element.find('REFINING_EXPONENT').text.strip()      
        max_poll_size = element.find('MAX_POLL_SIZE').text.strip()          
        poll_order = element.find('POLL_ORDER').text.strip()             
        poll_complete = element.find('POLL_COMPLETE').text.strip()          
        poll_directions = element.find('POLL_DIRECTIONS').text.strip()        
        initial_search = element.find('INITIAL_SEARCH').text.strip()         
        initial_complete = element.find('INITIAL_COMPLETE').text.strip()       
        initial_points = element.find('INITIAL_POINTS').text.strip()         
        iterative_search = element.find('ITERATIVE_SEARCH').text.strip()       
        iterative_complete = element.find('ITERATIVE_COMPLETE').text.strip()     
        iterative_points = element.find('ITERATIVE_POINTS').text.strip()       
        speculative_search = element.find('SPECULATIVE_SEARCH').text.strip()     
        poll_size_term = element.find('POLL_SIZE_TERM').text.strip()         
        iterations = element.find('ITERATIONS').text.strip()     
        truth_evals = element.find('TRUTH_EVALS').text.strip()            
        new_truth_evals = element.find('NEW_TRUTH_EVALS').text.strip()        
        consecutive_fails = element.find('CONSECUTIVE_FAILS').text.strip()      
        filter_relative = element.find('FILTER_RELATIVE').text.strip()        
        hmax = element.find('HMAX').text.strip()                   
        hmin = element.find('HMIN').text.strip()                   
        filter_norm = element.find('FILTER_NORM').text.strip()            
        surrogate_tolerance = element.find('SURROGATE_TOLERANCE').text.strip()    
        scaling_choice = element.find('SCALING_CHOICE').text.strip()         
        gen_cons_nb = element.find('GEN_CONS_NB').text.strip()            
        use_surrogate = element.find('USE_SURROGATE').text.strip()          
        start_pt_file = element.find('START_PT_FILE').text.strip() 
        bounds_file = element.find('BOUNDS_FILE').text.strip() 
        results_file = element.find('RESULTS_FILE').text.strip() 
        return cls(coarsening_exponent, initial_poll_size, poll_basis, refining_exponent, 
                 max_poll_size, poll_order, poll_complete, poll_directions, initial_search, 
                 initial_complete, initial_points, iterative_search, iterative_complete, 
                 iterative_points, speculative_search, poll_size_term, iterations, truth_evals, 
                 new_truth_evals, consecutive_fails, filter_relative, hmax, hmin, filter_norm, 
                 surrogate_tolerance, scaling_choice, gen_cons_nb, use_surrogate, start_pt_file, 
                 bounds_file, results_file)
        
        
class EONOMAD(Fitter):
    
    def __init__(self, eo, nomad):
        """
        A combined EO and NOMAD fitter
        
        `eo` -- The EO fitter
        `nomad` -- The NOMAD fitter
        """
        self.eo = eo
        self.nomad = nomad
        
    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 self.eo._to_xml('EOFitter'),
                 self.nomad._to_xml('NOMADFitter'))
        
    @classmethod
    def from_xml(cls, element):
        eo = EOFitter.from_xml(element.find('EOFitterParameters'))
        nomad = EOFitter.from_xml(element.find('NOMADFitterParameters'))
        cls(eo, nomad)


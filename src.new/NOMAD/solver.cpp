/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* École Polytechnique de Montréal - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montréal (Québec) H3C 3A7 Canada             *
* Tel:  (514) 340-6053 x6928                                                  *
* Email: Gilles.Couture@gerad.ca                                              *
* Fax: (514) 340-5665                                                         *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation. You can find the              *
* GNU General Public License at:                                              *
*             http://www.gnu.org/copyleft/gpl.html                            *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program; if not, write to the Free Software                 *
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. *
*                                                                             *
* You can find information on the NOMAD software at:                          *
*             http://www.gerad.ca/NOMAD/                                      *
******************************************************************************/

// solver.cpp -- Solver class methods.

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;
#include "defines.h"
#include "evaluator.h"
#include "mesh.h"
#include "pollcontrol.h"
#include "searchcontrol.h"
#include "solver.h"
#include "surrogatecontrol.h"
#include "terminator.h"

/* Private methods.
******************************************************************************/

// This method updates the variables after every iteration.
bool Solver::update(const unsigned int search_success,
		    const unsigned int poll_success)
{
  // The iteration was a success if the search or the poll was a success.
  unsigned int iteration_success =
    (search_success >= poll_success) ? search_success : poll_success;
  /* The boolean variable 'feasible' is updated according to
     'iteration_success'. */
  switch (iteration_success)
    {
      /* Case '0' is an iteration failure, '1' is partially successful: in
	 these cases we alternate the poll center between 'feasible' and 'not
	 feasible'. */
      case 0  :
      case 1  : feasible = !feasible;
                break;
      /* Case '2' is successful, a new least infeasible incumbent was found.
	 The next poll center will be infeasible. */
      case 2  : feasible = false;
	        break;
      /* Case '3' is successful, a new best feasible incumbent was found. The
	 next poll center will be feasible. */
      case 3  : feasible = true;
	        break;
      default : cerr << "Wrong value for 'iteration_success'!\n";
    }

  if (display_factor >= 5)
    {
      cout << "\nIteration success = " << iteration_success << ", feasible = ";
      (feasible == true) ? cout << "true\n" : cout << "false\n";
    }

  // If the iteration was successful...
  if (iteration_success >= 2)
    {
      // A new incumbent was found. The mesh is coarsened.
      mesh->coarsen();
      // The surrogate information is deleted when there's a new incumbent.
      if (surr_control != NULL)
	surr_control->cleanSurrogate();
      // The iteration was successful.
      return true;
    }
  else
    {
      /* If both the search and the poll failed ('0' is returned in this case)
	 we have an iteration failure. */
      if ((iteration_success == 0) && (terminate == false))
	{
	  /* The mesh is refined, and compared to the mesh termination
	     criterion of Terminator. */
	  if (terminator->compareMeshSize(mesh->refine()) == true)
	    terminate = true;
	}
      // We tell SurrogateManager to update the surrogate.
      if (surr_control != NULL)
	surr_control->update(evaluator->getIncumbent());
      // The iteration is unsuccessful, or partially successful.
      return false;
    }
}

/* This method builds the output index once, at the start of a run. This index
   indicates the kinds of output that are calculated in the run: the objective
   function value (f(x)), the general constraints functions values (gi(x)),
   the transition values and the surrogate function value. */
void Solver::buildOutputIndex(const bool surr_used,
			      const unsigned int gen_cons_nb)
{
  if (surr_used)
    output_dimension++;
  if (gen_cons_nb > 0)
    output_dimension += gen_cons_nb;
  // The 'output_index' array is created.
  output_index = new unsigned int[output_dimension];
  // The first position contains the truth function value (represented by '0').
  output_index[0] = 0;
  if (output_dimension > 1)
    {
      unsigned int i = 1;
      /* If there's a surrogate, its output will go in second position
	 (represented by '1'). */
      if (surr_used)
	output_index[i++] = 1;
      /* All other array positions will contain a '2' (representing a general
	 constraint output). */
      if (gen_cons_nb > 0)
	while (i < output_dimension)
	  output_index[i++] = 2;
    }
  if (display_factor >= 9)
    {
      cout << "In 'Solver::buildOutputIndex(...)'\n";
      cout << "Output dimension: " << output_dimension << "\n";
      cout << "Output index: ";
      for (unsigned int j = 0; j < output_dimension; j++)
	cout << output_index[j] << " ";
      cout << "\n\n";
    }
}

/* This method creates the surrogate, and passes a surrogate reference to
   SearchControl and PollControl. */
void Solver::createSurrogate()
{
  /* The SurrogateControl object is created: it'll store the points generated
     by the search and poll, and pass them to the surrogate for
     recalibration. */
  surr_control = new SurrogateControl(max_dimension, output_dimension);
  // SearchControl and PollControl are linked to SurrogateManager.
  searchcontrol->linkWithSurrogate(* surr_control);
  pollcontrol->linkWithSurrogate(* surr_control);
}

/* This method writes a lot of stuff in the output files just before the
   problem is solved. */
void Solver::prepareOutputFiles(ofstream & fout, ofstream & fgraph,
				ofstream & history)
{
  /* The number of function evaluations and the truth value of the starting
     point are written in the output file. */
  fout << "\n\nRun-time information...\n\n";
  fout << "feval #   f(x)   Point\n";
  fout << "-------   ----   -----\n";
  fout.precision(8);

  // This condition tests if the starting point is feasible.
  if (evaluator->feasible())
    {
      // The starting point is written in the output file.
      unsigned int teval = evaluator->getTruthEvaluations();
      fout << "   " << teval << "    ";
      evaluator->showIncumbent(fout);
      // The starting point is written in the graph file.
      double inc_truth = evaluator->getTruth();
      fgraph << teval << "  " << inc_truth << "\n";
      // The variable 'feasible' is set to true.
      feasible = true;
    }

  // A legend is written in the history file. First the xi are written...
  for (unsigned int i = 1; i <= max_dimension; i++)
    history << "x" << i << "\t";
  // ... Then f(x) is written.
  history << "f(x)" << "\t";
  // This method will write the rest of the legend and the starting point.
  evaluator->writeHistoryLegend(history);
}

/* Public methods.
******************************************************************************/

// Default constructor. The Solver object is created at Start-up.
Solver::Solver()
{
  // Variables are initialized.
  feasible = false;
  terminate = false;
  display_factor = 0;
  iteration_count = 0;
  max_dimension = 0;
  output_dimension = 1;
  random_seed = 1;
  output_index = NULL;
  /* The Mesh, Terminator, Evaluator, PollControl and SearchControl objects
     are created with default values. */
  mesh = new Mesh();
  terminator = new Terminator(* this);
  evaluator = new Evaluator(* terminator);
  pollcontrol = new PollControl(* evaluator);
  searchcontrol = new SearchControl(* evaluator);
  surr_control = NULL;
}

// Destructor. The created objects are destroyed.
Solver::~Solver()
{
  delete searchcontrol;
  delete evaluator;
  delete mesh;
  delete pollcontrol;
  delete terminator;
  if (surr_control != NULL)
    delete surr_control;
  if (output_index != NULL)
    delete [] output_index;
}

// This method returns the current mesh size.
double Solver::getMeshSize()
{
  return mesh->getMeshSize();
}

// This method returns the incumbent point's truth value.
double Solver::getTruth()
{
  return evaluator->getTruth();
}

// This method returns a pointer to the Filter object.
Filter * Solver::getFilter()
{
  return evaluator->getFilter();
}

// This method returns a pointer to the General Constraints object.
GeneralConstraints * Solver::getGenCons()
{
  return evaluator->getGenCons();
}

/* This method evaluates the truth value of the starting point. It's called
   just before 'solveProblem()'. */
int Solver::evaluateStartingPoint() const
{
  // 'sp' is a pointer to the starting point.
  AugmentedPoint * sp = evaluator->getStartingPoint();

  /* If the starting point is null, it means that the user wants to continue a
     run; the starting point is already evaluated, there's nothing to do but
     return. */
  if (sp == 0)
    return 1;
  else
    // The starting point exists, its truth value is calculated.
    {
      // We get the user's choice of poll directions.
      int direction_choice = pollcontrol->getChoice();
      /* If the user chose standard n+1 directions or uniform n+1 directions,
	 PollControl has to build the epsilon array. */
      if ((direction_choice == 1) || (direction_choice == 2))
	evaluator->buildEpsilonArray(max_dimension, terminator->getMeshSize(),
				     mesh->getMeshSize(), mesh->getBasis());
      /* The starting point's truth value is evaluated; we also check if the
	 point is within the bounds. If it's outside the bounds,
	 'evaluate_Truth(...)' returns '-1' . */
      int value = evaluator->evaluateTruth(max_dimension, sp, true);
      /* We check if the starting point is feasible or not (if we have general
	 constraints, the starting point could be infeasible). */
      evaluator->isStartingPointFeasible();
      // If the starting point was within the bounds...
      if (value != -1)
	{
	  /* ... We tell Evaluator to update the proximity array, which gives
	     us information about how close the starting point is from the
	     bounds. */
	  evaluator->updateProximityArray(max_dimension);
	  /* Pollcontrol will possibly update the direction set, according to
	     the updated proximity vector. */
	  pollcontrol->updateDirectionSet(max_dimension);
	}
      return value;
    }
}

// This method returns the # of new truth evaluations.
int Solver::returnNewTruthEvaluations()
{
  return evaluator->getNewTruthEvaluations();
}

// This method returns the # of cache evaluations.
int Solver::returnPermCacheHits()
{
  return evaluator->getPermCacheHits();
}

// This method returns the # of cache evaluations.
int Solver::returnTempCacheHits()
{
  return evaluator->getTempCacheHits();
}

// This method asks Terminator for the active termination criterion.
int Solver::returnTerminationFlag()
{
  return terminator->getFlag();
}

// This method returns the # of truth evaluations.
int Solver::returnTruthEvaluations()
{
  return evaluator->getTruthEvaluations();
}

// The user wants to continue the previous run.
void Solver::continueRun()
{
  // The boolean variable 'terminate' is reset.
  terminate = false;
  // We alert pointed objects, so they can reinitialize themselves.
  evaluator->continueRun();
  searchcontrol->continueRun();
  // The surrogate's recalibration list is deleted.
  if (surr_control != NULL)
    surr_control->cleanList();
}

/* This method is called from Terminator. When one of the termination criteria
   is exceeded, we have to stop the program. */
void Solver::endProgram()
{
  // We tell Evaluator to stop what it's doing.
  evaluator->endProgram();
  // This will halt solveProblem(...)'s main loop.
  terminate = true;
}

/* This function is used when the truth, surrogate and general constraints
   functions are black boxes, separated from Nomad. The directory and
   executable names for the truth and surrogate functions are sent to
   Evaluator. */
void Solver::enterBlackBoxes(const char * dir, const char * input,
			     const char * truthF,
			     std::vector<const char *> & gen_constraints,
			     const char * surrF)
{
  unsigned int cons_nb = gen_constraints.size();
  if (display_factor >= 9)
    {
      cout << "\nIn 'Solver::enterBlackBoxes(...)'\n";
      cout << "Truth string: " << truthF << "\n";
      cout << "Surrogate string: ";
      (surrF == NULL) ? cout << "No surrogate\n" : cout << surrF << "\n";
      cout << "Number of general constraints: " << cons_nb << "\n";
      if (cons_nb > 0)
	{
	  std::vector<const char *>::iterator current;
	  for (current = gen_constraints.begin();
	       current != gen_constraints.end(); current++)
	    cout << * current << "\n";
	}
    }
  // A new output index is built.
  bool surr_used = (surrF != NULL) ? true : false;
  buildOutputIndex(surr_used, cons_nb);
  evaluator->enterBlackBoxes(dir, input, truthF, gen_constraints, surrF);
  // If the problem uses a surrogate (there's a surrogate string, 'surrF') ...
  if (surrF != NULL)
    // The surrogate is created.
    createSurrogate();
}

// The path of the permanent cache is received, and passed to Evaluator.
void Solver::enterCache(const char * perm)
{
  evaluator->enterCache(max_dimension, output_dimension, perm);
}

// The filter parameters are sent to Evaluator.
void Solver::enterFilter(const bool choice, const double h_max,
			 const double h_min, const unsigned int norm) const
{
  evaluator->enterFilter(choice, h_max, h_min, norm);
}

// The mesh variables are received and sent to Mesh.
void Solver::enterMesh(const double poll_size, const double basis,
		       const double max_poll_size, const unsigned int coarsen,
		       const int refine) const
{
  mesh->set(poll_size, basis, max_poll_size, coarsen, refine);
}

/* This function is used if the user functions are put into Nomad (i.e.
   there are no black boxes). */
void Solver::enterOutputTypes(const bool surr_used,
			      const unsigned int gen_cons_nb)
{
  // A new output index is built.
  buildOutputIndex(surr_used, gen_cons_nb);
  evaluator->enterOutputTypes(surr_used, gen_cons_nb);
  // If the problem uses a surrogate ...
  if (surr_used)
    // The surrogate is created.
    createSurrogate();
}

// This method receives the user's choices for poll strategy and directions:
// - fixed: Are the poll directions fixed, or ordered by success.
// - complete: Is the poll going to evaluate points in all directions, even if
//             a better point has been found.
// - choice: User's choice for poll directions.
void Solver::enterPoll(bool fixed, const bool complete,
		       const unsigned int choice) const
{
  // Choices #3 and #4 are for 'MADS - dense 2 * n' and 'MADS - dense n + 1'
  // poll directions respectively.
  if (choice > 2)
    {
      // Dense sets are used: the poll directions are recreated every
      // iteration, thus they can't be ordered by success.
      fixed = true;
      // We tell SearchControl to build a speculative search.
      searchcontrol->createSpeculativeSearch();
      // The poll directions choice is sent to Terminator; it'll be used to
      // calculate the mesh size termination criterion.
      terminator->calculateMeshSize(max_dimension, choice);
    }
  // The poll directions choice is sent to Mesh; it'll be used to calculate the
  // initial mesh size.
  mesh->calculateMeshSizes(max_dimension, choice);
  // The parameters are sent to the PollControl object.
  pollcontrol->set(max_dimension, fixed, complete, choice);
}

/* The problem dimension, starting point, bound arrays and scaling parameters
   are received and passed along to Evaluator. */
void Solver::enterProblem(const unsigned int max_dim, double * coord,
			  const double * lower, const double * upper,
			  const unsigned int scaling, const double * scales,
			  const double low, const double up)
{
  max_dimension = max_dim;
  evaluator->enterProblem(max_dimension, output_dimension, output_index, coord,
			  lower, upper, scaling, scales, low, up);
}

// The random seed is received.
void Solver::enterRandomSeed(const unsigned int seed)
{
  // The seed value is stored.
  random_seed = seed;
  srand(seed);
}

// The search variables are received and passed to SearchControl.
void Solver::enterSearch(const unsigned int ini_type, const bool ini_complete,
			 const unsigned int ini_pts,
			 const unsigned int iter_type,
			 const bool iter_complete, const unsigned int iter_pts,
			 const bool speculative) const
{
  searchcontrol->createSearchers(ini_type, ini_complete, ini_pts, iter_type,
				 iter_complete, iter_pts, speculative);
}

// The surrogate parameters are sent to Evaluator.
void Solver::enterSurrogate(const double surr_tol) const
{
  evaluator->enterSurrogate(surr_tol);
}

// The termination criteria are received and passed to Terminator.
void Solver::enterTerminationCriteria(const double poll_size_term,
				      const int consecutive_failures,
				      const int truth_evaluations,
				      const int iterations,
				      const int new_truth_evaluations) const
{
  terminator->set(poll_size_term, consecutive_failures, truth_evaluations,
		  iterations, new_truth_evaluations);
}

/* This method passes the received parameter to Evaluator, along with the
   problem dimension. */
void Solver::fillWithBestPoint(double * coord)
{
  evaluator->fillWithBestPoint(max_dimension, coord);
}

// The display factor is set.
void Solver::setDisplay(const unsigned int display)
{
  display_factor = display;
  // The display factor must be changed in these objects.
  evaluator->setDisplay(display_factor);
  mesh->setDisplay(display_factor);
  pollcontrol->setDisplay(display_factor);
  searchcontrol->setDisplay(display_factor);
  if (surr_control != NULL)
    surr_control->setDisplay(display_factor);
}

// This method outputs the problem's data in an output file, 'fout'.
void Solver::showInput(ofstream & fout) const
{
  if (!fout.is_open())
    return;
  fout << "Problem specifications:\n";
  fout << "-----------------------\n";
  fout << "Problem dimension: " << max_dimension << ", ";
  fout << "number of outputs: " << output_dimension << "\n";
  fout << "Random seed: " << random_seed << "\n";
  // We output the pointed objects' data.
  if (evaluator != NULL)
    evaluator->showInput(max_dimension, fout);
  if (searchcontrol != NULL)
    searchcontrol->showInput(fout);
  if (mesh != NULL)
    mesh->showInput(fout);
  if (pollcontrol != NULL)
    pollcontrol->showInput(fout);
  if (terminator != NULL)
    terminator->showInput(fout);
}

// The problem solution is written in an output file, 'fout'.
void Solver::showOutput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  fout << "\nProblem solution:\n";
  fout << "-----------------\n";
  // The final incumbent is written in the file.
  fout << "Best point found: ";
  evaluator->showIncumbent(fout);
  // The final mesh size is written.
  mesh->showOutput(fout);
  fout << "Number of iterations: " << iteration_count << "\n";
  // The function, truth and cache counters are written.
  evaluator->showOutput(fout);
}

// Data entry is completed and the starting point is ok: it's time to solve
// the problem. All problem outputs are written in a file, 'fout'.
void Solver::solveProblem(ofstream & fout)
{
  if (display_factor > 0)
    {
      cout << "\n\n********************\n* START OF THE RUN *";
      cout << "\n********************\n\n";
    }

  // We create a new output file, 'fgraph', where we'll write the data needed
  // for the GUI to draw a graph.
  ofstream fgraph("LIBRARY/graph.txt");
  // The history file will contain all the points visited by the algorithm.
  ofstream history("LIBRARY/history.txt");
  // This method will prepare the output files.
  prepareOutputFiles(fout, fgraph, history);

  // This variable will determine the success of an iteration.
  bool iteration_success = false;
  // This variable will hold the current mesh size.
  double mesh_size;
  // This array will be the poll center.
  double * poll_center = NULL;
  // These two variables will help determine the success of an iteration.
  unsigned int search_success, poll_success;

  // 'solveProblem()' main loop. Every pass through the loop is an iteration
  // of the algorithm. The boolean variable "terminate" starts out false.
  while (!(terminate))
    {
      if (display_factor > 0)
	{
	  cout << "\nNew iteration: " << evaluator->getTruthEvaluations();
	  cout << " truth evaluations\n--------------\n";
	}
      // The surrogate manager's list of points is deleted at every iteration.
      if (surr_control != NULL)
	surr_control->cleanList();
      // We ask Mesh for the current mesh size;
      mesh_size = mesh->getMeshSize();
      // We ask Evaluator for a poll center.
      poll_center = evaluator->getPollCenter(feasible);
      // This variable is reset at every pass through the loop.
      poll_success = 0;

      // First we do a search.
      search_success = searchcontrol->search
	(max_dimension, mesh_size, poll_center, iteration_success,
	 fout, fgraph, history);
      // If the search doesn't fail... (it returns '0' or '1'  when it does)
      if (search_success >= 2)
	// We tell PollControl to check if the new incumbent is near the
	// bounds. If it is, the direction set has to be updated.
	pollcontrol->updateDirectionSet(max_dimension);
      else   // The search did not find a new incumbent: we poll.
	poll_success = pollcontrol->poll(max_dimension, mesh_size,
					 mesh->getBasis(), poll_center,
					 fout, fgraph, history);
      // Update at the end of an iteration.
      iteration_success = update(search_success, poll_success);
      // The iteration counter is incremented.
      terminator->compareIterationsCount(++iteration_count);
      if (display_factor > 0)
	{
	  cout << "Iteration #" << iteration_count << " was ";
	  iteration_success == true ? cout << "successful\n" :
	    cout << "unsuccessful\n";
	}
    }

  // We're out of the loop: first we get the last value of the truth
  // evaluations counter.
  unsigned int teval = evaluator->getTruthEvaluations();
  // Then we get the value of the truth evaluations counter the last time
  // a new incumbent was found. If the 2 values are equal it means that the
  // last point evaluated was the new incumbent. Thus the final best point is
  // already written in the output file; in this case we do nothing.
  if (evaluator->getTeval() != teval)
    {
      // The last point evaluated is written in the output file.
      fout << "   " << teval << "    ";
      evaluator->showIncumbent(fout);
      if (evaluator->feasible())
	fgraph << teval << "  " << evaluator->getTruth() << "\n";
    }

  // The graph files are closed.
  fgraph.close();
  history.close();
  // The problem solution is written in the output file.
  showOutput(fout);
  // The caches are closed.
  evaluator->closeCaches();
}

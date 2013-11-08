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

// evaluator.cpp -- Evaluator class methods.

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
#include "augmentedpoint.h"
#include "constraints.h"
#include "evaluator.h"
#include "filter.h"
#include "functioncontrol.h"

/* Private methods:
******************************************************************************/

// This method initializes the object's variables.
void Evaluator::initialize()
{
  terminate = false;
  surrogate_tolerance = 0.0;
  consecutive_fails = polldir_index = teval = 0;
}

// This method updates the run statistics. It's called when one of the
// incumbents is improved.
void Evaluator::updateStatistics(const int sent_from, const unsigned int count,
				 const double fx_decrease)
{
  // This switch calls the correct Statistics method, according to 'sent_from'.
  switch (sent_from)
    {
      // The poll generated a better iterate.
      case 0  : stats.addToPollDecrease(fx_decrease);
                break;
      // The initial search generated a better iterate.
      case 1  : stats.addToInitialDecrease(fx_decrease);
	        break;
      // The iterative search generated a better iterate.
      case 2  : stats.addToIterativeDecrease(fx_decrease);
          	break;
      // The dense search generated a better iterate.
      case 3  : stats.addToDenseDecrease(fx_decrease);
		break;
      // This case is trickier, because the search was a concatenation of a one
      // point dense search and an iterative search.
      case 6  : if (count == 0)
		  // The dense search generated a better iterate (the first
		  // point in the list was the dense search point).
		  stats.addToDenseDecrease(fx_decrease);
		else
		  // The iterative search generated a better iterate.
		  stats.addToIterativeDecrease(fx_decrease);
	        break;
      default : cerr << "Can't update statistics: Parameter incorrect.\n";
    }
}

/* Public methods:
******************************************************************************/

// Default constructor called by the Solver object at Start-up.
Evaluator::Evaluator(Terminator & t)
{
  // The variables are initialized to default values.
  display_factor = 0;
  initialize();
  // We don't know if the problem has bound constraints at this time.
  constraints = NULL;
  // FunctionControl is created using default values.
  f_control = new FunctionControl(t);
  incumbent = NULL;
  // We'll have information about the starting point later.
  starting_point = NULL;
  filter = NULL;
}

// Destructor.
Evaluator::~Evaluator()
{
  // The created objects are destroyed.
  if (constraints != NULL)
    delete constraints;
  if (f_control != NULL)
    delete f_control;
  if (incumbent != NULL)
    delete incumbent;
  if (starting_point != NULL)
    delete starting_point;
  if (filter != NULL)
    delete filter;
}

// This method returns true if the starting point is feasible, false otherwise.
bool Evaluator::feasible()
{
  // If 'incumbent' exists, it means that the starting point was feasible.
  if (incumbent != NULL)
    return true;
  else
    return false;
}

// This method returns 'true' if the Constraints object exists.
bool Evaluator::getConstraints()
{
  if (constraints != NULL)
    return true;
  else
    return false;
}

// This method returns the incumbent's f(x) value.
double Evaluator::getTruth()
{
  /* If 'incumbent' is NULL, it means that no feasible point was found by NOMAD
     up to this point. A very large number is returned. */
  if (incumbent == NULL)
    return INFINIT;
  else
    return incumbent->getOutput(0);
}

// This method returns Constraints' lower bounds array.
double * Evaluator::getLowerBounds()
{
  return constraints->getLowerBounds();
}

// This method returns a poll center.
double * Evaluator::getPollCenter(const bool feasible)
{
  // First case: there are no general constraints in the problem.
  if ((incumbent != NULL) && (filter == NULL))
    {
      if (display_factor >= 5)
	cout << "No general constraints - poll center is best feasible.\n";
      // The incumbent point will be the poll center.
      return incumbent->getPoint();
    }
  else
    {
      // Second case: no feasible points were found up to this point.
      if ((incumbent == NULL) && (filter != NULL))
	{
	  if (display_factor >= 5)
	    cout << "No feasible point - poll center is least infeasible.\n";
	  // The least infeasible point will be the poll center.
	  return filter->getPollCenter();
	}
      else
	/* Third case: polling is done alternatively between the incumbent and
	   the least feasible point, according to the 'feasible' variable. */
	{
	  if (feasible)
	    {
	      if (display_factor >= 5)
		cout << "Poll center is best feasible.\n";
	      // The incumbent point will be the poll center.
	      return incumbent->getPoint();
	    }
	  else
	    {
	      if (display_factor >= 5)
		cout << "Poll center is least infeasible.\n";
	      // The least infeasible point will be the poll center.
	      return filter->getPollCenter();
	    }
	}
    }
}

// This method returns Constraints' upper bounds array.
double * Evaluator::getUpperBounds()
{
  return constraints->getUpperBounds();
}

// This method returns a pointer to the GeneralConstraints object.
GeneralConstraints * Evaluator::getGenCons()
{
  return f_control->getGenCons();
}

/* This method first asks the Constraints object if the trial point 'ap'
   satisfies the bound constraints. If it does, 'truth' is evaluated. */
int Evaluator::evaluateTruth(const unsigned int dim, AugmentedPoint * ap,
			     bool startingPoint)
{
  // If there are bound constraints, and the point is outside the bounds...
  if ((constraints != NULL) && (!constraints->check(dim, ap->getPoint())))
    {
      // ... truth is set to a very large number.
      ap->setOutput(0, INFINIT);
      /* Returning '-1' tells 'Solver' that the starting point is outside the
	 bounds. */
      return -1;
    }
  // If there's a surrogate and 'ap' is the starting point...
  if ((startingPoint) && (ap->getPosition(1) != -1))
    // ... the starting point's surrogate value is evaluated.
    f_control->evaluateFlow(* ap);
  /* In all other cases (there are no bound constraints, or the point is
     inside the bounds), we evaluate truth. */
  return f_control->evaluateTruth(* ap, startingPoint);
}


#ifdef PARANOMAD

//WVG Added this function for parallelization
/* This method first asks the Constraints object if the trial point 'ap'
   satisfies the bound constraints. If it does, 'truth' is evaluated. */
vector< int > Evaluator::parallelEvaluateTruth(const unsigned int dim, vector< AugmentedPoint * > points, bool startingPoint) {
    vector<AugmentedPoint *> toEvalPoints;
    // If there are bound constraints, and the point is outside the bounds...
    for (unsigned int i = 0; i < points.size(); i++) {
		if (display_factor >= 10) {cout << "Checking constraints of point: "; points[i]->show(); cout << endl;} 
        if ((constraints != NULL) && (!constraints->check(dim, points[i]->getPoint()))) {
            // ... truth is set to a very large number.
            points[i]->setOutput(0, INFINIT);
            /* Returning '-1' tells 'Solver' that the starting point is outside the
            bounds. */
            continue;
        }

        // If there's a surrogate and 'ap' is the starting point...
        if ((startingPoint) && (points[i]->getPosition(1) != -1)) {
            // ... the starting point's surrogate value is evaluated.
            f_control->evaluateFlow(*(points[i]));
        }

        /* In all other cases (there are no bound constraints, or the point is
        inside the bounds), we evaluate truth. */

		if (display_factor >= 10) {cout << "Evaluating: "; points[i]->show(); cout << endl;} 
        toEvalPoints.push_back(points[i]);
    }

    return f_control->parallelEvaluateTruth(toEvalPoints);
}

#endif

// This method returns Constraints' proximity array.
int * Evaluator::getProximityArray()
{
  return constraints->getProximityArray();
}

// Returns FunctionControl's number of new truth evaluations.
unsigned int Evaluator::getNewTruthEvaluations()
{
  return f_control->getNewTruthEvaluations();
}

// Returns the number of hits in the permanent cache.
unsigned int Evaluator::getPermCacheHits()
{
  return f_control->getPermCacheHits();
}

// Returns the number of hits in the temporary cache.
unsigned int Evaluator::getTempCacheHits()
{
  return f_control->getTempCacheHits();
}

// Returns FunctionControl's number of truth evaluations.
unsigned int Evaluator::getTruthEvaluations()
{
  return f_control->getTruthEvaluations();
}

// This method relays the call to build the epsilon array to Constraints.
void Evaluator::buildEpsilonArray(const unsigned int dim, const double deltaT,
				  const double delta, const double factor)
{
  if (constraints != NULL)
    constraints->buildEpsilonArray(dim, deltaT, delta, factor);
}

// The caches must be closed at the end of a run.
void Evaluator::closeCaches()
{
  f_control->closeCaches();
}

// The user wants to continue the previous run from where it ended.
void Evaluator::continueRun()
{
  // The object's attributes are initialized.
  initialize();
  // The Constraints and FunctionControl objects are alerted.
  if (constraints != NULL)
    constraints->continueRun();
  f_control->continueRun();
  // The run statistics are reset.
  stats.reset();
}

// This method stops the evaluation of the list of points in
// 'process_this_list(...)' when one of the termination criteria is met.
void Evaluator::endProgram()
{
  terminate = true;
  // TEMP!!!
  if (filter != NULL)
    filter->writeInGraphFile();
}

/* The directory and executable names for the truth and surrogate functions are
   sent to FunctionControl. */
void Evaluator::enterBlackBoxes(const char * dir, const char * input,
				const char * truthF,
				std::vector<const char *> & gen_constraints,
				const char * surrF)
{
  if ((gen_constraints.size() > 0) && (filter == NULL))
    filter = new Filter();
  f_control->enterBlackBoxes(dir, input, truthF, gen_constraints, surrF);
}

// This method is called when the user wants to use a cache.
void Evaluator::enterCache(const unsigned int dim,
			   const unsigned int out_dim, const char * perm)
{
  f_control->makeDatabases(dim, out_dim, perm);
}

// The filter parameters are entered.
void Evaluator::enterFilter(const bool choice, const double h_max,
			    const double h_min, const unsigned int norm)
{
  f_control->enterFilter(choice, h_max, h_min, norm);
}

/* The output types are sent to FunctionControl (this method is used if the
   user functions are not in black box form. */
void Evaluator::enterOutputTypes(const bool surr_used,
				 const unsigned int gen_cons_nb)
{
  if ((gen_cons_nb > 0) && (filter == NULL))
    filter = new Filter();
  f_control->enterOutputTypes(surr_used, gen_cons_nb);
}

// The problem data is received from Solver.
void Evaluator::enterProblem(const unsigned int dim,
			     const unsigned int out_dim,
			     const unsigned int * out_index, double * coord,
			     const double * lower, const double * upper,
			     const unsigned int scaling, const double * scales,
			     const double low, const double up)
{
  // The FunctionControl object has to build the scales arrays.
  f_control->buildScales(dim, coord, lower, upper, scaling, scales, low, up);
  // The scales arrays are built; we assign pointers to them.
  double * scales_add = f_control->getScalesAdd();
  double * scales_mult = f_control->getScalesMult();
  
  // The starting point is created.
  starting_point = new AugmentedPoint(dim, out_dim, out_index, coord,
				      scales_add, scales_mult);

  /* The constraints object is created, with the bounds and scales arrays (the
     bounds have to be scaled). */
  if ((lower != NULL) && (upper != NULL))
    constraints = new Constraints(dim, lower, upper, scales_add, scales_mult);
}

// The surrogate parameter is entered.
void Evaluator::enterSurrogate(const double surr_tol)
{
  surrogate_tolerance = surr_tol;
}

/* This method receives a list of points and sends them to 'FunctionControl'
   so that their Flow values are calculated. */
void Evaluator::evaluateFlow(const unsigned int dim,
			     std::vector<AugmentedPoint *> & list_of_points)
{
  std::vector<AugmentedPoint *>::iterator temp;
  /* Each point is checked by the Constraints object to see if it satisfies
     the bound constraints. If it does, Flow is evaluated. */
  for (temp = list_of_points.begin(); temp != list_of_points.end(); temp++)
    {
      // If there are bound constraints...
      if ((constraints != NULL) &&
	  // ... we check if the point satisfies them.
	  (constraints->check(dim, (* temp)->getPoint()) == false))
	// If it doesn't, Flow is set to a very large number.
	(* temp)->setLow(INFINIT);
      else   // There are no bound constraints, or the point is inside.
	// The point's Flow value is calculated.
	f_control->evaluateFlow(** temp);
    }
}

/* This method passes the received parameters to FunctionControl, along with
   the incumbent point's coordinates. */
void Evaluator::fillWithBestPoint(const unsigned int dim, double * coord)
{
  f_control->fillWithBestPoint(dim, coord, incumbent->getPoint());
}

// This method checks if the starting point is feasible or not.
void Evaluator::isStartingPointFeasible()
{
  /* We have an infeasible starting point in this case:
     - If there's a Filter object, it means that there are general constraints
       in the current problem (the starting point might be infeasible).
     - If Hx is greater than zero, the starting point is infeasible. */
  if ((filter != NULL) && (starting_point->getHx() > 0.0))
    // The starting point copy will populate the filter.
    filter->populate(* starting_point);
  else   // The starting point is feasible.
    // The starting point is copied in the 'incumbent' AugmentedPoint object.
    incumbent = new AugmentedPoint(* starting_point);
}

// The 'linkWithSurrogate(...)' call is relayed to FunctionControl.
void Evaluator::linkWithSurrogate(SurrogateControl & surr)
{
  f_control->linkWithSurrogate(surr);
}

// This method sets the display factor.
void Evaluator::setDisplay(const unsigned int display)
{
  display_factor = display;
  // The display factor is reset in these objects.
  if (constraints != NULL)
    constraints->setDisplay(display_factor);
  if (filter != NULL)
    filter->setDisplay(display_factor);
  f_control->setDisplay(display_factor);
}

// The incumbent point outputs itself.
void Evaluator::showIncumbent()
{
  if (incumbent == NULL)
    cout << "NA (no feasible point was found)\n";
  else
    incumbent->show();
}

// The incumbent point outputs itself in the output file.
void Evaluator::showIncumbent(ofstream & fout)
{
  if (incumbent == NULL)
    fout << "NA (no feasible point was found)\n";
  else
    incumbent->showResults(fout, f_control->getScalesAdd(),
			   f_control->getScalesMult());
}

// Outputs Evaluator's variables.
void Evaluator::showInput(const unsigned int dim, ofstream & fout)
{
  if (!fout.is_open())
    return;

  bool feasible_start_pt = feasible();
  double * scales_add = f_control->getScalesAdd();
  double * scales_mult = f_control->getScalesMult();

  fout << "Use the caches? ";
  f_control->cachesUsed() ? fout << "Yes\n" : fout << "No\n";
  if (surrogate_tolerance != 0.0)
    fout << "Surrogate tolerance: " << surrogate_tolerance << "\n";
  // The starting point is shown.
  fout << "Starting point ";
  feasible_start_pt == false ? fout << "(infeasible):" : fout << "(feasible):";
  fout << "  ";
  // If 'starting_point' exists it's shown.
  if (starting_point != NULL)
    starting_point->showResults(fout, scales_add, scales_mult);
  else   // If it doesn't exist, it means that the user chose to continue a
         // run. The starting point is the best feasible point.
    if (feasible_start_pt)
      incumbent->showResults(fout, scales_add, scales_mult);
    else   // If there's no best feasible point, the least infeasible is the
           // starting point.
      (filter->getIncumbent())->showResults(fout, scales_add, scales_mult);

  // Now we show the problem's constraints.
  fout << "\nConstraints:\n------------\n";
  if (constraints != NULL)
    // The constraints variables are shown.
    constraints->showInput(dim, fout, scales_add, scales_mult);
  // FunctionControl's variables are shown.
  f_control->showInput(dim, fout);
}

// Once the problem is solved, we output the solution, and counters.
void Evaluator::showOutput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  // FunctionControl's counters are shown.
  f_control->showOutput(fout);
  fout << "Number of consecutive failures: ";
  // The consecutive failures counter is shown.
  fout << consecutive_fails << "\n\n";
  // The run statistics are written in the file.
  fout << stats << "\n";
}

// This method is called only once, from 'Solver::evaluateStartingPoint()'.
void Evaluator::updateProximityArray(const unsigned int dimension)
{
  /* This method tells Constraints to update the proximity array according to
   the starting point. */
  if (constraints != NULL)
    constraints->updateProximityArray(dimension, starting_point->getPoint());
}

// This method writes the legend and the starting point in the history file.
void Evaluator::writeHistoryLegend(ofstream & history)
{
  // Pointers to these arrays are needed to unscale the starting point (the
  // user must see all the points visited by NOMAD in coordinates he can
  // understand, so we have to unscale them).
  double * scales_add = f_control->getScalesAdd();
  double * scales_mult = f_control->getScalesMult();

  // FunctionControl will finish writing the history file legend.
  f_control->writeHistoryLegend(history);

  // The 'starting point' exists it's written in the history file.
  if (starting_point != NULL)
    {
      starting_point->showHistory(history, scales_add, scales_mult);
      // The starting point was copied in either the filter list (if it was
      // infeasible), or the AugmentedPoint 'incumbent' (if it was feasible).
      // It has also been written in the output and history files. It can be
      // safely destroyed, having served its purposes.
      delete starting_point;
      starting_point = NULL;
    }
  else   // If it doesn't exist, it means that the user chose to continue a
         // run. The starting point is the best feasible point.
    if (feasible())
      incumbent->showHistory(history, scales_add, scales_mult);
    else   // If there's no best feasible point, the least infeasible is the
           // starting point.
      (filter->getIncumbent())->showHistory(history, scales_add, scales_mult);
}


#ifndef PARANOMAD

/* This method evaluates truth for a list of points received as parameters. It
   returns true if a new best iterate was found, false otherwise. */
unsigned int Evaluator::processThisList(const unsigned int dimension,
		                std::vector<AugmentedPoint *> & list_of_points,
					bool entire_list, ofstream & fout,
					ofstream & fgraph, ofstream & history,
					const int sent_from)
{
  if (display_factor >= 3)
    cout << "Evaluating the list of points...\n";
  // Local variables we'll need.
  bool feasible_trial = true;
  // This variable will hold the decrease of f(x) when a better iterate is
  // found.
  double fx_decrease = 0.0;
  double inc_truth, trial_truth;
  double * scales_add = f_control->getScalesAdd();
  double * scales_mult = f_control->getScalesMult();
  int output_ok = 1;
  // 'count' is incremented by one every pass in the loop.
  unsigned int count = 0, iteration_success = 0, trial_success = 0;
  std::vector<AugmentedPoint *>::iterator trial;
  // Main loop.
  for (trial = list_of_points.begin(); trial != list_of_points.end(); trial++)
    {
      // The variable is reset for each point.
      fx_decrease = 0.0;
      /* If we reached a termination criterion, like the number of truth
	 evaluations, 'terminate' equals true and we stop evaluating points. */
      if (terminate)
	{
	  if (display_factor >= 3)
	    cout << "\nA termination criterion was reached.\n";
	  break;
	}
      /* If the trial point's surrogate function value is greater than the
	 incumbent's surrogate function value by a certain factor, we stop
	 evaluating Truth for this list of points. (This is safe because the
	 points are ordered by surrogate function value). */
      if ((sent_from > 0) && ((* trial)->getPosition(1) != -1) &&
	  (incumbent != NULL) && (abs((* trial)->getLow()) >
			       abs(surrogate_tolerance * incumbent->getLow())))
	{
	  if (display_factor >= 5)
	    {
	      cout << "|Trial's s(x) value| is greater than\n";
	      cout << "|surrogate_tolerance * incumbent's s(x) value|\n";
	      cout << "We stop evaluating the list of points.\n";
	      cout << abs((* trial)->getLow()) << " > ";
	      cout << abs(surrogate_tolerance * incumbent->getLow()) << endl;
	    }
	  break;
	}
      else   // The trial point's truth value is evaluated.
	output_ok = evaluateTruth(dimension, * trial);
      if (display_factor >= 3)
	{
	  cout << count + 1 << ": ";
	  (* trial)->show();
	}

      /* 'output_ok' is equal to '1' if the point was within the bounds, and
	 not in the temporary cache. */
      if (output_ok == 1)
	{
	  // Every trial point si unsuccessful at first.
	  trial_success = 0;
	  // The newly evaluated point is written in the history file.
	  (* trial)->showHistory(history, scales_add, scales_mult);
	  /* 'feasible_trial' will tell us if the trial point is feasible or
	     not. */
	  if (filter != NULL)
	    feasible_trial = ((* trial)->getHx() == 0.0) ? true : false;
	  // If the trial point is infeasible...
	  if (feasible_trial == false)
	    // ... it's sent to populate the filter.
	    trial_success = filter->populate(** trial);
	  else   // The trial point is feasible.
	    {
	      /* We get incumbent's and trial's truth values to compare them.
		 Note that if the entire list of points is to be evaluated
		 (we're doing a complete poll or search), the best iterate can
		 change many times. That's why the line
		 'double inc_truth = incumbent->getOutput(0);' is inside the
		 loop. */
	      trial_truth = (* trial)->getOutput(0);
	      /* If 'incumbent' doesn't exist (no feasible point was found up
		 to this point), 'inc_truth' is set to a very large number. */
	      if (incumbent == NULL)
		inc_truth = INFINIT;
	      else   // If 'incumbent' exists, we get its truth value.
		inc_truth = incumbent->getOutput(0);

	      // if (trial's truth value < incumbent's truth value)...
	      if (trial_truth < inc_truth)
		{
		  // ...Success! We have a new best iterate!
		  trial_success = 3;
		  /* If 'incumbent' is NULL, it means that no feasible point
		     was found up to now. */
		  if (incumbent == NULL)
		    // It's created by copying the trial point.
		    incumbent = new AugmentedPoint(** trial);
		  else
		    {
		      // The decrease in f(x) is calculated.
		      fx_decrease = inc_truth - trial_truth;
		      // We copy the trial point in 'incumbent'.
		      incumbent->update(* trial);
		    }
		  // The new best iterate is written in the output files.
		  teval = f_control->getTruthEvaluations();
		  fout << "   " << teval << "    ";
		  incumbent->showResults(fout, scales_add, scales_mult);
		  fgraph << teval << "  " << trial_truth << "\n";
		}
	    }

	  // The iteration success variable is updated.
	  if (trial_success > iteration_success)
	    iteration_success = trial_success;
	  // If a new incumbent was found, these tasks are done:
	  if (trial_success >= 2)
	    {
	      if ((display_factor >= 5) && (entire_list) &&
		  (trial_success == 3))
		cout << "New best feasible incumbent found!\n";
	      /* The counter value is put into 'polldir_index'. It's used
		 by PollControl when the poll directions are ordered by
		 success.*/
	      polldir_index = count;
	      // The consecutive failures counter is put to zero.
	      consecutive_fails = 0;
	      /* We tell the 'Constraints' object to update the proximity
		 vector. */
	      if (constraints != NULL)
		constraints->updateProximityArray(dimension,
						  (* trial)->getPoint());
	      // The run statistics are updated.
	      updateStatistics(sent_from, count, fx_decrease);
	      /* If 'entire_list' is false, it means we stop evaluating
		 points as soon as a better one is found. */
	      if (entire_list == false)
		break;
	    }
	  else   // Failure! We don't have a new best iterate.
	    /* The consecutive failures counter is incremented, and compared to
	       the termination criterion. */
	    f_control->compare(++consecutive_fails);
	}
      // The counter of loop passes is incremented.
      count++;
    }

  if (display_factor >= 5)
    {
      (sent_from > 0) ? cout << "\nSearch " : cout << "\nPoll ";
      switch (iteration_success)
	{
          case 0  : cout << "failure.\n";
	            break;
          case 1  : cout << "partly successful:";
	            cout << " New point(s) added to the filter.\n";
		    break;
          case 2  : cout << "success:";
	            cout << " A new least infeasible incumbent was found.\n";
		    break;
          case 3  : cout << "success:";
	            cout << " A new best feasible incumbent was found.\n";
		    break;
          default : cerr << "indeterminate: Return value incorrect!\n";
	}
    }
  return iteration_success;
}


#else

/// WVG changed for parallelization
/* This method evaluates truth for a list of points received as parameters. It
   returns true if a new best iterate was found, false otherwise. */
unsigned int Evaluator::processThisList(const unsigned int dimension,
		                std::vector<AugmentedPoint *> & list_of_points,
					bool entire_list, ofstream & fout,
					ofstream & fgraph, ofstream & history,
					const int sent_from) {
	if (display_factor >= 3) cout << "Evaluating a list of points in parallel...\n";
  	
	// Local variables we'll need.
  	bool feasible_trial = true;
  	
	// This variable will hold the decrease of f(x) when a better iterate is
  	// found.
  	double fx_decrease = 0.0;
  	double inc_truth, trial_truth;
  	double * scales_add = f_control->getScalesAdd();
  	double * scales_mult = f_control->getScalesMult();
  	int output_ok = 1;
	int i = 0;
  	
	// 'count' is incremented by one every pass in the loop.
  	unsigned int count = 0, iteration_success = 0, trial_success = 0;
  	std::vector<AugmentedPoint *>::iterator trial;
  	
	// Main loop.
  	vector< int > evalOutput = parallelEvaluateTruth(dimension, list_of_points);

	for (trial = list_of_points.begin(); trial != list_of_points.end(); trial++) {
		// The variable is reset for each point.
		fx_decrease = 0.0;
		/* If we reached a termination criterion, like the number of truth
	 	evaluations, 'terminate' equals true and we stop evaluating points. */
		if (terminate) {
	  		if (display_factor >= 3) cout << "\nA termination criterion was reached.\n";
	  		break;
		}
     	 /* If the trial point's surrogate function value is greater than the
	 	incumbent's surrogate function value by a certain factor, we stop
	 	evaluating Truth for this list of points. (This is safe because the
	 	points are ordered by surrogate function value). */
      	if ((sent_from > 0) && ((* trial)->getPosition(1) != -1) &&
	  		(incumbent != NULL) && (abs((* trial)->getLow()) >
			       abs(surrogate_tolerance * incumbent->getLow()))) {
	  		if (display_factor >= 5) {
	      		cout << "|Trial's s(x) value| is greater than\n";
	      		cout << "|surrogate_tolerance * incumbent's s(x) value|\n";
	      		cout << "We stop evaluating the list of points.\n";
	      		cout << abs((* trial)->getLow()) << " > ";
	      		cout << abs(surrogate_tolerance * incumbent->getLow()) << endl;
			}
	  		break;
		}
		else {   
			// The trial point's truth value is evaluated.
			output_ok = evalOutput[i++];
		}
      
		if (display_factor >= 3) {
	  		cout << count + 1 << ": ";
	  		(* trial)->show();
		}

      	/* 'output_ok' is equal to '1' if the point was within the bounds, and
	 	not in the temporary cache. */
      	if (output_ok == 1) {
	  		// Every trial point si unsuccessful at first.
	  		trial_success = 0;
	  		
			// The newly evaluated point is written in the history file.
	  		(* trial)->showHistory(history, scales_add, scales_mult);
	  			/* 'feasible_trial' will tell us if the trial point is feasible or
	     		not. */
	  		if (filter != NULL) {
	    		feasible_trial = ((* trial)->getHx() == 0.0) ? true : false;
			}
	  		// If the trial point is infeasible...
	  		if (feasible_trial == false) {
	    		// ... it's sent to populate the filter.
	    		trial_success = filter->populate(** trial);
			}
	  		else  {  // The trial point is feasible.
	 			/* We get incumbent's and trial's truth values to compare them.
		 		Note that if the entire list of points is to be evaluated
		 		(we're doing a complete poll or search), the best iterate can
		 		change many times. That's why the line
		 		'double inc_truth = incumbent->getOutput(0);' is inside the
		 		loop. */
	      		
				trial_truth = (* trial)->getOutput(0);
	      		/* If 'incumbent' doesn't exist (no feasible point was found up
		 		to this point), 'inc_truth' is set to a very large number. */
	      		
				if (incumbent == NULL) {
					inc_truth = INFINIT;
				}
	      		else {  // If 'incumbent' exists, we get its truth value.
					inc_truth = incumbent->getOutput(0);
				}

	      		// if (trial's truth value < incumbent's truth value)...
	      		if (trial_truth < inc_truth) {
		  			// ...Success! We have a new best iterate!
		  			trial_success = 3;
		  			/* If 'incumbent' is NULL, it means that no feasible point
		     		was found up to now. */
		  
					if (incumbent == NULL) {
		    			// It's created by copying the trial point.
		    			incumbent = new AugmentedPoint(** trial);
					}
		  			else {
		      			// The decrease in f(x) is calculated.
		      			fx_decrease = inc_truth - trial_truth;
		      			// We copy the trial point in 'incumbent'.
		      			incumbent->update(* trial);
		   			}
		 	 		// The new best iterate is written in the output files.
		  			teval = f_control->getTruthEvaluations();
		  			fout << "   " << teval << "    ";
		  			incumbent->showResults(fout, scales_add, scales_mult);
		  			fgraph << teval << "  " << trial_truth << "\n";
				}
	    	}

	  		// The iteration success variable is updated.
	  		if (trial_success > iteration_success) {
				iteration_success = trial_success;
			}
	  		// If a new incumbent was found, these tasks are done:
	  		if (trial_success >= 2) {
	      		if ((display_factor >= 5) && (entire_list) &&
		  			(trial_success == 3)) {
 					cout << "New best feasible incumbent found!\n";
	      					/* The counter value is put into 'polldir_index'. It's used
		 					by PollControl when the poll directions are ordered by
		 					success.*/
				}
	      		polldir_index = count;
	      		// The consecutive failures counter is put to zero.
	      		consecutive_fails = 0;
	      		/* We tell the 'Constraints' object to update the proximity
		 		vector. */
	      		if (constraints != NULL) {
					constraints->updateProximityArray(dimension,
						  (* trial)->getPoint());
				}
	      		// The run statistics are updated.
	      		updateStatistics(sent_from, count, fx_decrease);
	      		/* If 'entire_list' is false, it means we stop evaluating
		 		points as soon as a better one is found. */
	      		if (entire_list == false) {
					break;
				}
	    	}
	  		else {   // Failure! We don't have a new best iterate.
	    		/* The consecutive failures counter is incremented, and compared to
	       		the termination criterion. */
	    		f_control->compare(++consecutive_fails);
			}
		}
      	// The counter of loop passes is incremented.
      	count++;
    }

	if (display_factor >= 5) {
      (sent_from > 0) ? cout << "\nSearch " : cout << "\nPoll ";
      switch (iteration_success) {
          case 0  : cout << "failure.\n";
	            break;
          case 1  : cout << "partly successful:";
	            cout << " New point(s) added to the filter.\n";
		    break;
          case 2  : cout << "success:";
	            cout << " A new least infeasible incumbent was found.\n";
		    break;
          case 3  : cout << "success:";
	            cout << " A new best feasible incumbent was found.\n";
		    break;
          default : cerr << "indeterminate: Return value incorrect!\n";
		}
    }

	return iteration_success;

}

#endif

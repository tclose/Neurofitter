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

/* solver.h -- class definition for Solver. This class acts as a control box
   for the program. It receives all system operation messages from the user
   interface, and dispatches the tasks to the program objects. The Solver
   object is the heart of the optimization part of Nomad, with the
   "solveProblem(...)" function. */

#ifndef _SOLVER_H_
#define _SOLVER_H_
#include <vector>
using namespace std;
#include "filter.h"
#include "generalconstraints.h"

class Evaluator;
class Mesh;
class PollControl;
class SearchControl;
class SurrogateControl;
class Terminator;
//class TruthFunction; //Added to allow setting TruthFunction WVG

class Solver
{
/* The Solver object has the following variables:
   - "display_factor" is used for debugging.
   - a boolean variable, "terminate". The main loop of the "solveProblem(...)"
     function stops when "terminate" is true.
   - "max_dimension" is the number of variables of the problem.
   - "output_dimension" is the number of outputs of the problem.
   - pointers to these objects: SearchControl, Evaluator, Mesh, PollControl,
     Terminator and SurrogateManager. They process the various tasks
     dispatched to them. */
 private:
  bool feasible, terminate;
  unsigned int display_factor, iteration_count, max_dimension,
    output_dimension, random_seed;
  unsigned int * output_index;
  Evaluator * evaluator;
  Mesh * mesh;
  PollControl * pollcontrol;
  SearchControl * searchcontrol;
  SurrogateControl * surr_control;
  Terminator * terminator;
//  TruthFunction * truth;

// The class methods are described in "solver.cpp".
  bool update(const unsigned int search_success,
	      const unsigned int poll_success);
  void buildOutputIndex(const bool surr_used, const unsigned int gen_cons_nb);
  void createSurrogate();
  void prepareOutputFiles(ofstream & fout, ofstream & fgraph,
			  ofstream & history);
 public:
  Solver();
//  Solver(TruthFunction *);
  ~Solver();
  double getMeshSize();
  double getTruth();
  Filter * getFilter();
  GeneralConstraints * getGenCons();
  int evaluateStartingPoint() const;
  int returnNewTruthEvaluations();
  int returnPermCacheHits();
  int returnTempCacheHits();
  int returnTerminationFlag();
  int returnTruthEvaluations();
  void continueRun();
  void endProgram();
  void enterBlackBoxes(const char * dir, const char * input,
		       const char * truthF,
		       std::vector<const char *> & gen_constraints,
		       const char * surrF = NULL);
  void enterCache(const char * perm = NULL);
  void enterFilter(const bool choice, const double h_max,
		   const double h_min, const unsigned int norm) const;
  void enterMesh(const double poll_size, const double basis,
		 const double max_poll_size, const unsigned int coarsen,
		 const int refine) const;
  void enterOutputTypes(const bool surr_used, const unsigned int gen_cons_nb);
  void enterPoll(bool fixed, const bool complete,
		 const unsigned int choice) const;
  void enterProblem(const unsigned int max_dim, double * coord,
		    const double * lower, const double * upper,
		    const unsigned int scaling, const double * scales,
		    const double low, const double up);
  void enterRandomSeed(const unsigned int seed);
  void enterSearch(const unsigned int ini_type, const bool ini_complete,
		   const unsigned int ini_pts, const unsigned int iter_type,
		   const bool iter_complete, const unsigned int iter_pts,
		   const bool speculative) const;
  void enterSurrogate(const double surr_tol) const;
  void enterTerminationCriteria(const double poll_size_term,
				const int consecutive_failures,
				const int truth_evaluations,
				const int iterations,
				const int new_truth_evaluations) const;
  void fillWithBestPoint(double * coord);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout) const;
  void showOutput(ofstream & fout);
  void solveProblem(ofstream & fout);
};
#endif

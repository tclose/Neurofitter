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

/* terminator.h -- class definition for Terminator.  This class manages the 
   program's termination criteria. */

#ifndef _TERMINATOR_H_
#define _TERMINATOR_H_
#include <fstream>
using namespace std;
#include "solver.h"

class Terminator
{
/* The Terminator object has the following variables:
   - 'poll_size' is the poll size termination criterion.
   - 'consecutive_fails' is the number of function evaluations done since a new
     incumbent was found.
   - 'flag' indicates which criterion was activated for termination.
   - 'function_evals' is the number of function evaluations termination
     criterion.
   - 'truth_evals' is the number of truth evaluations termination criterion.
   - a pointer to the Solver object. */
 private:
  double mesh_size, poll_size;
  int consecutive_fails, flag, iterations, new_truth_evals, truth_evals;
  Solver * solver;

// The class methods are described in "terminator.cpp"
 public:
  Terminator(Solver & s);
  ~Terminator();
  bool compareMeshSize(const double delta_m);
  double getMeshSize();
  int getFlag();
  void calculateMeshSize(const unsigned int dimension,
			 const unsigned int choice);
  void compareConsecutiveFails(const int consecutive_failures);
  void compareIterationsCount(const int iterations_count);
  void compareNewTruthEvals(const int new_truth_evaluations);
  void compareTruthEvals(const int truth_evaluations);
  void set(const double poll_size_term, const int consecutive_failures,
	   const int truth_evaluations, const int max_iter,
	   const int new_truth_evaluations);
  void showInput(ofstream & fout);
};
#endif

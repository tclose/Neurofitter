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

// parameters.h -- class definition for Parameters.

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "solver.h"

class Parameters
{
private:
  // Boolean variables.
  bool filter_relative, initial_complete, iterative_complete, poll_complete,
    poll_order, speculative_search;
  // The path of the parameters file.
  char * parameters_file;
  // Real number (double) variables.
  double hmax, hmin, max_poll_size, poll_basis, poll_size, poll_size_term,
    surrogate_tolerance;
  // Integer variables.
  int consecutive_fails, iterations, new_truth_evals, refining_exponent,
    truth_evals;
  // Unsigned integer variables.
  unsigned int coarsening_exponent, filter_norm, initial_points, initial,
    iterative_points, iterative, poll_directions, random_seed;

  // Private methods.
  void readParameterFile(ifstream & fin);

  // The class methods are described in "parameters.cpp".
public:
  Parameters(const char * file);
  ~Parameters();
  bool getFilterRelative();
  bool getInitialComplete();
  bool getIterativeComplete();
  bool getSpeculativeSearch();
  bool readyToSolve() const;
  char * getFile();
  double getHmax();
  double getHmin();
  double getMaxPollSize();
  double getPollBasis();
  double getPollSize();
  double getPollSizeTerm();
  double getSurrogateTolerance();
  int getConsecutiveFails();
  int getIterations();
  int getNewTruthEvals();
  int getPollDirs();
  int getPollOrder();
  int getPollType();
  int getRefine();
  int getTruthEvals();
  unsigned int getCoarsen();
  unsigned int getFilterNorm();
  unsigned int getInitialPoints();
  unsigned int getIterativePoints();
  unsigned int getInitialSearch();
  unsigned int getIterativeSearch();
  unsigned int getSeed();
  void capture(const unsigned int gen_cons_nb, const bool surrogate_use,
	       Solver & controlbox) const;
  void enterFilter(const bool choice, const double h_max,
		   const double h_min, const unsigned int norm);
  void enterMesh(const double delta_p, const double basis,
		 const double max_delta_p, const unsigned int coarsen,
		 const int refine);
  void enterPoll(bool fixed, const bool complete, unsigned int choice);
  void enterRandomSeed(const unsigned int seed);
  void enterSearch(const unsigned int ini_type, const bool ini_complete,
		   const unsigned int ini_pts, const unsigned int iter_type,
		   const bool iter_complete, const unsigned int iter_pts,
		   const bool speculative);
  void enterSurrogate(const double surr_tol);
  void enterTerminationCriteria(const double poll_size_criterion,
				const int consecutive_failures,
				const int truth_evaluations,
				const int iter_number,
				const int new_truth_evaluations);
  void load(const char * file);
  void setFile(const char * file);
  void show(const bool filter_use, const bool surrogate_use) const;
  void write() const;
};
#endif

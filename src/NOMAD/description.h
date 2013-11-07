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

/* description.h -- class definition for Description. */

#ifndef DESCRIPTION_H
#define DESCRIPTION_H
#include "solver.h"

class Description
{
private:
  // Some boolean variables.
  bool black_box_use, bounds_use, cache_use, surrogate_use;
  // These strings are file names for the various components of NOMAD.
  char * cache, * constraints, * directory, * general_constraints, * input,
    * name, * problem_file, * results, * starting_point, * surrogate, * scales,
    * truth;
  /* These real variables hold the user's choices for lower and upper bounds
     (used for scaling). */
  double lower, upper;
  // An integer variable for the scaling choice;
  int scaling;
  // Some integer variables.
  unsigned int dimension, general_constraints_number;

  // Private methods.
  void captureArrays(Solver & controlbox);
  void captureBlackBoxes(Solver & controlbox);
  void fillArray(ifstream & fin, double * coord);
  void readProblemFile(ifstream & fin);

  // The class methods are described in "description.cpp".
public:
  Description(const char * file = NULL);
  ~Description();
  // 'get' methods.
  bool getBlackBoxUse();
  bool getBoundsUse();
  bool getCacheUse();
  bool getSurrogateUse() const;
  bool readyToSolve();
  char * getCache();
  char * getConstraints();
  char * getDirectory();
  char * getGeneralConstraints();
  char * getInput();
  char * getName() const;
  char * getProblemFile();
  char * getResultsFile() const;
  char * getScales();
  char * getStartingPoint();
  char * getSurrogate();
  char * getTruth();
  double getLowerScale();
  double getUpperScale();
  int getScalingChoice();
  unsigned int getDimension();
  unsigned int getGeneralConstraintsNumber() const;
  void capture(Solver & controlbox);
  void enterBlackBoxes(const bool black_box, const char * truth_exec,
		       const char * general, const char * surr_exec);
  void enterConstraints(const bool bounds, const char * bounds_file,
			const unsigned int gen_cons_nbr);
  void enterFiles(const char * dir, const char * in, const char * res);
  void enterOptions(const bool cache_usage, const char * cache_file,
		    const bool surr_use);
  void enterProblem(const char * prob_name, const unsigned int dim,
		    const char * start_pt);
  void enterScales(const int index, const char * scales_file,
		   const double low, const double up);
  void loadProblemFile(const char * file);
  void setProblemFile(const char * file);
  void show() const;
  void write() const;
};
#endif

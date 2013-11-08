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

/* functioncontrol.h -- class definition for FunctionControl. This class
   controls access to the cache and the user provided functions (truth and
   surrf). */

#ifndef _FUNCTIONCONTROL_H_
#define _FUNCTIONCONTROL_H_
#include <fstream>
using namespace std;
#include "augmentedpoint.h"
#include "surrogatecontrol.h"
#include "terminator.h"

class CacheControl;
class GeneralConstraints;
class SurrFunction;
class TruthFunction;

class FunctionControl
{
/* FunctionControl has the following variables:
   - a counter of function evaluations (truth evaluations + cache hits).
   - a counter of truth evaluations.
   - a few strings, used for calling black boxes.
   - a pointer to the CacheManager object.
   - a pointer to the GeneralConstraints object.
   - a pointer to the SurrogateManager object.
   - a pointer to the Terminator object.
   - pointers to the TruthFunction and SurrFunction objects, used when
     there's no black box in the problem. */
 private:
  char * truth_box, * surrogate_box, * input_file, * output_file;
  double low, up;
  double * scales_add, * scales_mult;
  unsigned int display_factor, new_truth_evaluations, truth_evaluations,
    use_scaling;
  CacheControl * cache_control;
  GeneralConstraints * gen_cons;
  SurrFunction * surr_f;
  SurrogateControl * surr_control;
  Terminator * term;
  TruthFunction * truth;

// The class methods are described in "functioncontrol.cpp".

  char * createBlackBoxString(char * file, const char * dir, const char * box,
			      const char * txt);
  char * createFileString(char * file, const char * dir, const char * txt);
  double giveMeTruth(AugmentedPoint & ap);
  void automaticScaling(const unsigned int dim, double * coord);
  void automaticScaling(const unsigned int dim, double * coord,
			const double * lower, const double * upper);
  void boundScaling(const unsigned int dim, const double * lower,
		    const double * upper, const double low, const double up);
  void openInputFile(const unsigned int dim, const double * coord);
  void showScales(const unsigned int dim, ofstream & fout);

 public:
  FunctionControl(Terminator & t);
  ~FunctionControl();
  bool cachesUsed();
  bool evaluateTruth(AugmentedPoint & ap, bool startingPoint = false);

  #ifdef PARANOMAD
	vector< int > parallelEvaluateTruth(vector< AugmentedPoint * > points);
  #endif

  double * getScalesAdd();
  double * getScalesMult();
  // Returns a pointer to the GeneralConstraints object.
  GeneralConstraints * getGenCons()  {return gen_cons;}
  // Returns the number of truth evaluations.
  unsigned int getNewTruthEvaluations()  {return new_truth_evaluations;}
  unsigned int getPermCacheHits();
  unsigned int getTempCacheHits();
  // Returns the number of truth evaluations.
  unsigned int getTruthEvaluations()  {return truth_evaluations;}
  void buildScales(const unsigned int dim, double * coord,
		   const double * lower, const double * upper,
		   const unsigned int scaling, const double * scale,
		   const double a, const double b);
  /* The "last_incumbent" counter is incremented. We alert the TERMINATOR
     object, in case the maximum number of function evaluations since the last
     incumbent was found is reached. */
  void compare(const unsigned int last_incumbent)
    {term->compareConsecutiveFails(last_incumbent);}
  void closeCaches();
  void continueRun();
  void enterBlackBoxes(const char * dir, const char * input,
		       const char * truthF,
		       std::vector<const char *> & gen_constraints,
		       const char * surrF = NULL);
  void enterFilter(const bool choice, const double h_max,
		   const double h_min, const unsigned int norm);
  void enterOutputTypes(const bool surr_used, const unsigned int gen_cons_nb);
  void evaluateFlow(AugmentedPoint & ap);
  void fillWithBestPoint(const unsigned int dim, double * coord,
			 const double * incumbent);
  void linkWithSurrogate(SurrogateControl & surr);
  void makeDatabases(const unsigned int dim, const unsigned int out,
		     const char * perm);
  void setDisplay(const unsigned int display);
  void showInput(const unsigned int dim, ofstream & fout);
  void showOutput(ofstream & fout);
  void writeHistoryLegend(ofstream & history);
};
#endif

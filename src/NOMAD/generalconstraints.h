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

// generalconstraints.h -- class definition for GeneralConstraints.

#ifndef _GENERALCONSTRAINTS_H_
#define _GENERALCONSTRAINTS_H_
#include <fstream>
#include <vector>
using namespace std;
#include "augmentedpoint.h"
#include "subject.h"

class GeneralConstraints : public Subject
{
/* GeneralConstraints has the following variables:
   -  */
 private:
  bool relative, black_box_used;
  char * input_file, * output_file;
  double hmax, hmin;
  unsigned int display_factor, filter_norm, gen_constraints_nb;
  std::vector<char *> gen_constraints_boxes;
  std::vector<char *>::iterator current;

// The class methods are described in "generalconstraints.cpp".
  char * createBlackBoxString(char * file, const char * dir, const char * box,
			      const char * txt);
  double calculateHx(const double * coord, bool startingPoint,
		     unsigned int pos);
  void blackBoxRoutine(AugmentedPoint & ap, unsigned int pos);
  void initialize(const unsigned int gen_cons_nb);
  void userRoutine(AugmentedPoint & ap, unsigned int pos);

 public:
  GeneralConstraints(const char * dir, const char * input, const char * output,
		     std::vector<const char *> & gen_constraints);
  GeneralConstraints(const unsigned int gen_cons_nb);
  ~GeneralConstraints();
  double getHmax();
  // This method returns the number of general constraints.
  unsigned int getGenConsNb() {return gen_constraints_nb;}
  void enterFilter(const bool choice, const double h_max,
		   const double h_min, const unsigned int norm);
  void evaluate(AugmentedPoint & ap, bool startingPoint = false);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout);
};
#endif

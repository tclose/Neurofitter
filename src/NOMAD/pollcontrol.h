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

/* pollcontrol.h -- class definition for PollControl. This class controls
   the poll algorithm. */

#ifndef _POLLCONTROL_H_
#define _POLLCONTROL_H_
#include <fstream>
#include <vector>
using namespace std;
#include "directionset.h"
#include "evaluator.h"
#include "surrogatecontrol.h"

class AugmentedPoint;
class Pollster;

class PollControl
{
/* The PollControl object has the following variables:
   - "choice" is the user choice for generating poll directions.
   - "fixed_order" is a boolean variable indicating the poll strategy.
   - "polldirs" is a STL container of Direction objects. */
 private:
  bool fixed_order;
  int * prox_array;
  unsigned int display_factor;
  std::vector<AugmentedPoint *> list_of_points;
  std::vector<AugmentedPoint *>::iterator a_point;
  DirectionSet * direction_set;
  Evaluator * eval;
  Pollster * pollster;
  SurrogateControl * surr_control;

// The class methods are described in "pollcontrol.cpp".
  bool equal(const unsigned int max_dimension, int * old, int * _new);
  void cleanList();
  void copy(const unsigned int max_dimension, int * a, int * b);
  void showList();
  void success(const unsigned int index, const unsigned int max_dimension);

 public:
  PollControl(Evaluator & e);
  ~PollControl();
  bool updateDirectionSet(const unsigned int max_dimension);
  int getChoice();
  unsigned int poll(const unsigned int max_dim, const double delta,
		    const double factor, const double * poll_center,
		    ofstream & fout, ofstream & fgraph, ofstream & history);
  void linkWithSurrogate(SurrogateControl & surr);
  void set(const unsigned int dimension, bool fixed, bool complete,
	   const unsigned int ch);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout);
};
#endif

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

// filter.h -- class definition for Filter.

#ifndef _FILTER_H_
#define _FILTER_H_
#include <vector>
using namespace std;
#include "subject.h"

class AugmentedPoint;

class Filter : public Subject
{
  /* Filter has the following data structure:
     - An STL vector of augmented points, 'filter_list'.
     - An STL iterator that will point to 'filter_list', 'current'. */
 private:
  unsigned int display_factor;
  std::vector<AugmentedPoint *> filter_list;
  std::vector<AugmentedPoint *>::iterator current;

  // The class methods are described in "filter.cpp".
  unsigned int insert(const AugmentedPoint & ap, const double Fx,
		      const double Hx);
  void checkDominance(const double Fx, const int pos);
  void cleanList();
  void showList();
  void store(const AugmentedPoint & ap, const int pos = -1);
 public:
  Filter();
  ~Filter();
  const AugmentedPoint * getIncumbent();
  bool isEmpty();
  double * getPollCenter();
  unsigned int populate(AugmentedPoint & trial);
  void setDisplay(const unsigned int display);
  void writeInGraphFile();
};
#endif

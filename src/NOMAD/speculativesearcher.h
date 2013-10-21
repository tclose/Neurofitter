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

/* speculativesearcher.h -- class definition for SpeculativeSearcher, which is
   derived from Searcher. */

#ifndef _SPECULATIVESEARCHER_H_
#define _SPECULATIVESEARCHER_H_
#include <fstream>
using namespace std;
#include "searcher.h"

class AugmentedPoint;

class SpeculativeSearcher : public Searcher
{
/* The SpeculativeSearcher object has the following variables:
   - */
 private:
  double * previous;

// The class methods are described in "speculativesearcher.cpp" .
 public:
  SpeculativeSearcher(const unsigned int how_many_pts, const bool complete);
  ~SpeculativeSearcher();
  bool search(const unsigned int max_dim, const double,
	      const double * poll_center,
	      std::vector<AugmentedPoint *> & list_of_points,
	      Evaluator &);
  void update(const unsigned int max_dim, const double * poll_center);
};
#endif

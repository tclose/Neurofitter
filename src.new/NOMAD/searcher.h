/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* �cole Polytechnique de Montr�al - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montr�al (Qu�bec) H3C 3A7 Canada             *
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

/* searcher.h -- class definition for Searcher, which is an abstract base class
   for all search strategy subclasses (like RandomSearcher). */

#ifndef _SEARCHER_H_
#define _SEARCHER_H_
#include <fstream>
#include <vector>
using namespace std;
#include "augmentedpoint.h"
#include "evaluator.h"

class Searcher
{
/* The Searcher object has the following variables:
   - "display_factor" is used for debugging purposes.
   - "search_pts" is the number of points generated by the search.
   - "complete_search" indicates if the search is complete or not. */
 protected:
  unsigned int display_factor, search_pts;
  bool complete_search;

 public:
  Searcher(const unsigned int pts, const bool complete)
    : search_pts(pts), complete_search(complete) {display_factor = 0;}
  virtual ~Searcher() {}
  void setDisplay(const unsigned int display) {display_factor = display;}
  /* 'search(...)' is a pure virtual function. It receives these parameters:
     - The problem dimension.
     - The current mesh size, "delta".
     - The current best point, "poll_center".
     - A pointer to a list of points (each point being a vector of doubles).
       The list is empty at the start, and every point generated by the
       search is added to the end of the list.
     - A pointer to the Evaluator object: it lets the Search ask Evaluator
       what it needs to know. */
  virtual bool search(const unsigned int max_dim, const double delta,
		      const double * poll_center,
		      std::vector<AugmentedPoint *> & list_of_points,
		      Evaluator & eval) = 0;
  virtual void showInput(ofstream & fout)
    {complete_search == true ? fout << "complete." : fout << "incomplete.";}
  virtual void update(const unsigned int, const double *) {}
};
#endif

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

/* searchcontrol.h -- class definition for SearchControl, which is the context
   object between Solver and Searcher. */

#ifndef _SEARCHCONTROL_H_
#define _SEARCHCONTROL_H_
#include <fstream>
#include <vector>
using namespace std;
#include "evaluator.h"
#include "pollcontrol.h"

class AugmentedPoint;
class Searcher;
class SurrogateControl;

class SearchControl
{
/* The SearchControl object has the following variables:
   - an STL container, 'list_of_points'. It will contain the points generated
     by the search strategy.
   - pointers to Searcher, Evaluator and Surrogate. */
 private:
  bool initial, speculative_ok;
  unsigned int display_factor;
  std::vector<AugmentedPoint *> list_of_points;
  std::vector<AugmentedPoint *>::iterator a_point;
  Evaluator * eval;
  Searcher * initial_search, * iterative_search, * speculative_search;
  SurrogateControl * surr_control;

  // The methods are described in "searchcontrol.cpp"
  void cleanList();
  void deleteSearcherObjects();
  void initialize();
  void mesher(const unsigned int max_dim, const double * incumbent,
	      const double delta);
 public:
  SearchControl(Evaluator & e);
  ~SearchControl();
  unsigned int search(const unsigned int max_dim, const double delta,
		      const double * poll_center, bool success,
		      ofstream & fout, ofstream & fgraph, ofstream & history);
  void continueRun();
  void createSearchers(const unsigned int ini_type, const bool ini_complete,
		       const unsigned int ini_pts,
		       const unsigned int iter_type, const bool iter_complete,
		       const unsigned int iter_pts, const bool speculative);
  void createSpeculativeSearch();
  void linkWithSurrogate(SurrogateControl & surr);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout);
  void showList();
};
#endif

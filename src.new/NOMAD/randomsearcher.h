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

/* randomsearcher.h -- class definition for RandomSearcher, which is derived
   from Searcher. */

#ifndef _RANDOMSEARCHER_H_
#define _RANDOMSEARCHER_H_
#include <fstream>
using namespace std;
#include "searcher.h"

class AugmentedPoint;

class RandomSearcher : public Searcher
{
/* The RandomSearcher object has the following variables:
   - a real variable, 'r_max' is the highest possible number for the random
     number generator. */
 private:
  double r_max;
  double * left, * total;
  const double * lower, * upper;

// The class methods are described in "randomsearcher.cpp" .
  double generateCoordinateInUnboundedSpace(const unsigned int max_dimension,
					    const double poll_center,
					    const double delta);
  double generateCoordinateInBoundedSpace(const double poll_center,
					  const double delta, const double n,
					  const double less_than_poll_center);
  void createArrays(const unsigned int max_dim, const double delta,
		    const double * poll_center);
  double generateHowManyPts(const unsigned int dim);
  void fillTheList(const unsigned int max_dim, const double delta,
		   const double * poll_center, const double nb_of_points,
		   std::vector<AugmentedPoint *> & list_of_points);

 public:
  RandomSearcher(const unsigned int how_many_pts, const bool complete,
		 Evaluator & eval);
  ~RandomSearcher();
  bool search(const unsigned int max_dim, const double delta,
	      const double * poll_center,
	      std::vector<AugmentedPoint *> & list_of_points,
	      Evaluator & eval);
  void showInput(ofstream & fout);
};
#endif

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

// directionset.h -- class definition for DirectionSet. This class controls
// the set of poll directions.

#ifndef _DIRECTIONSET_H_
#define _DIRECTIONSET_H_
#include <deque>
#include <fstream>
#include <vector>
using namespace std;
#include "direction.h"


class DirectionSet
{
  // The DirectionSet object has the following variables:
  // - "choice" is the user choice for generating poll directions.
  // - "fixed_order" is a boolean variable indicating the poll strategy.
  // - "polldirs" is a STL container of Direction objects.
 private:
  double r_max;
  int choice;
  unsigned int display_factor;
  std::deque<Direction> polldirs;
  std::deque<Direction>::iterator direction;
  std::vector<vector<double> > permanent_dirs;

// The class methods are described in "directionset.cpp".
  double roundNumber(double temp);
  int findIndex(const unsigned int max_dim, const double index);
  int makePermanentDir(const unsigned int max_dim, const double index,
		       const double index_factor);
  void buildDirection(const unsigned int dim, const unsigned int pos,
		      const int val);
  void buildHybridPollDirs(int * prox, const unsigned int dim,
			   const unsigned int count);
  void buildOrthogonalPollDirs(int * prox, const unsigned int dim,
			       const unsigned int count);
  void makeDensePollDirs(const unsigned int dimension);
  void makeNPlus1PollDirs(const unsigned int dimension,
			  std::deque<Direction> & p);
  void makePollDir(const std::vector<double> v, std::deque<Direction> & p);
  void makePollDirSet(const unsigned int dimension);
  void make2NPollDirs(const unsigned int dimension);
  void makeUniformPollDirs(const unsigned int dimension,
			   std::deque<Direction> & p);
  void showDirections();
  void showPermanentDirs();

 public:
  DirectionSet(const unsigned int ch, const unsigned int dimension);
  ~DirectionSet();
  int getChoice();
  std::deque<Direction> & getPollDirs();
  void makeDenseSet(const unsigned int max_dim, const double delta,
		    const double factor);
  void rebuildPollDirSet(int * prox, const unsigned int dim);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout);
  void success(const unsigned int index);
};
#endif

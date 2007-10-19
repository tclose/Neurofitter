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

/* continuouspollster.h -- class definition for ContinuousPollster, which is
   derived from Pollster. */

#ifndef _CONTINUOUSPOLLSTER_H_
#define _CONTINUOUSPOLLSTER_H_
#include <fstream>
using namespace std;
#include "pollster.h"

class AugmentedPoint;

class ContinuousPollster : public Pollster
{
/* The ContinuousPollster object has the following variables: */
 private:
 

// The class methods are described in "continuouspollster.cpp".

 public:
  ContinuousPollster(const bool complete);
  ~ContinuousPollster();
  bool poll(const unsigned int max_dim, const double delta,
	    const double * poll_center, std::deque<Direction> & polldirs,
	    std::vector<AugmentedPoint *> & list_of_points);
  void showInput(ofstream & fout) const;
};
#endif

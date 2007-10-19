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

/* pollster.h -- class definition for Pollster, which is an abstract base class
   for all poll strategy subclasses (like ContinuousPollster). */

#ifndef _POLLSTER_H_
#define _POLLSTER_H_
#include <deque>
#include <fstream>
#include <vector>
using namespace std;
#include "augmentedpoint.h"
#include "direction.h"

class Pollster
{
/* The Pollster object has the following variables:
   - "display_factor" is used for debugging purposes.
   - "complete_poll" indicates if the poll is complete or not. */

 protected:
  unsigned int display_factor;
  bool complete_poll;

 public:
  Pollster(const bool complete)
    : complete_poll(complete) {display_factor = 0;}
  virtual ~Pollster() {}
  // This method returns the 'complete_poll' variable.
  bool getComplete() const {return complete_poll;}
  // This method sets 'complete_poll' to 'complete'.
  void set(bool complete) {complete_poll = complete;}
  // This method sets 'display_factor' to 'display'.
  void setDisplay(const unsigned int display) {display_factor = display;}
  /* 'poll(...)' is a pure virtual function. It receives these parameters:
     - The problem dimension, "max_dim".
     - The current mesh size, "delta".
     - The current iterate, "poll_center".
     - The address of a STL list of PollDirection objects, "polldirs".
     - The address of a STL vector of AugmentedPoint objects. */
  virtual bool poll(const unsigned int max_dim, const double delta,
		    const double * poll_center,
		    std::deque<Direction> & polldirs,
		    std::vector<AugmentedPoint *> & list_of_points) = 0;
  virtual void showInput(ofstream & fout) const
    {complete_poll == true ?  fout << "complete.\n" : fout << "opportunistic.\n";}
};
#endif

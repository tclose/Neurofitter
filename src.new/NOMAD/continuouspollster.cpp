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

// continuouspollster.cpp -- ContinuousPollster class methods.

#include <cmath>
#include <iostream>
using namespace std;
#include "continuouspollster.h"
#include "defines.h"

// Private methods.
// ----------------



// Public methods.
// ---------------

// Default constructor.
ContinuousPollster::ContinuousPollster(const bool complete)
  : Pollster(complete)
{}

// Destructor.
ContinuousPollster::~ContinuousPollster()
{}

//
bool ContinuousPollster::poll(const unsigned int max_dim, const double delta,
			      const double * poll_center,
			      std::deque<Direction> & polldirs,
			      std::vector<AugmentedPoint *> & list_of_points)
{
  std::vector<double> dir;
  double * coord = new double[max_dim];
  unsigned int d = 1;
  // We poll the mesh around the incumbent, according to the poll directions.
  for (std::deque<Direction>::iterator direction = polldirs.begin();
       direction != polldirs.end(); direction++)
    {
      int i = 0;
      dir = (* direction).getDirection();
      if (display_factor >= 9)
	cout << "Direction " << d++ << ": ";
      /* "j" will point the current poll direction.
	 This loop evaluates the following vector operation:
	 trial = poll_center + (poll direction * mesh size) */
      for (std::vector<double>::iterator j = dir.begin(); j != dir.end(); j++)
	{
	  if (display_factor >= 9)
	    cout << * j << " ";
	  coord[i] = poll_center[i] + (* j * delta);
	  i++;
	}
      dir.clear();
      if (display_factor >= 9)
	cout << "\n";
      AugmentedPoint * ap = new AugmentedPoint(max_dim, coord);
      list_of_points.push_back(ap);
    }
  // The 'coord' array is erased from memory.
  delete [] coord;
  return complete_poll;
}

// show -- Outputs the poll strategy.
void ContinuousPollster::showInput(ofstream & fout) const
{
  if (!fout.is_open())
    return;
  fout << "Continuous poll, ";
  Pollster::showInput(fout);
}

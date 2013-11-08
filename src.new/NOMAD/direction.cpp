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

/* direction.cpp -- Direction class methods. The methods of this class are
   called by the Mesh and Pollstrategy objects, both of which manage a set of
   directions. */

#include <iostream>
using namespace std;
#include "direction.h"

// Default constructor.
Direction::Direction(const std::vector<double> v)
  : dir(v)
{}

// Destructor.
Direction::~Direction()
{
  dir.clear();
}

// Returns "dir", the vector of real numbers.
std::vector<double> & Direction::getDirection()
{
  return dir;
}

//
double Direction::get(const unsigned int pos)
{
  return dir[pos];
}

//
void Direction::set(const unsigned int pos, const double val)
{
  dir[pos] = val;
}

// show -- Outputs the direction.
void Direction::show()
{
  // We check if the direction is empty.
  if (dir.empty())
    cout << "Empty vector!\n";
  else
    // The direction is shown.
    {
      for (std::vector<double>::iterator i = dir.begin(); i != dir.end(); i++)
	{
	  cout.width(6);
	  cout << * i << " ";
	}
      cout << "\n";
    }
}

// show -- Outputs the direction.
void Direction::show(ofstream & fout)
{
  if (!fout.is_open())
    return;
  if (dir.empty())
    fout << "Empty vector!";
  else
    for (std::vector<double>::iterator i = dir.begin(); i != dir.end(); i++)
      {
	fout.width(6);
	fout << * i << " ";
      }
  fout << "\n";
}

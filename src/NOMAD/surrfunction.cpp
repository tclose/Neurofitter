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

// surrfunction.cpp -- SurrFunction class methods.

#include <cmath>
using namespace std;
#include "defines.h"
#include "surrfunction.h"

// Default constructor.
SurrFunction::SurrFunction()
{
  // Initialization of the constants array.
  y = new double[236];
  y[0] = 0.0;
  for (unsigned int i = 1; i < 236; i++)
    y[i] = 94 + 0.66 * i;
}

// Destructor.
SurrFunction::~SurrFunction()
{
  delete [] y;
}

// Insert the surrogate function here: Hock and Schitkowski 105.
const double SurrFunction::evaluate(const double * coordinates)
{
  double a;
  unsigned int i;
  double truth_value = 0.0;
  for (i = 1; i < 236; i++)
    {
      a  = coordinates[0] / (100.0 * coordinates[5]) * exp(-pow(y[i] - coordinates[2], 2) / (2 * pow(coordinates[5], 2) ) );
      a += coordinates[1] / (100.0 * coordinates[6]) * exp(-pow(y[i] - coordinates[3], 2) / (2 * pow(coordinates[6], 2) ) );
      a += (100.0 - coordinates[1] - coordinates[0]) / (100.0 * coordinates[7]) * exp(-pow(y[i] - coordinates[4], 2) / (2 * pow(coordinates[7], 2) ) );
      truth_value = truth_value - log(a / 2.506628275);
    }
  return truth_value;
}

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

// statistics.cpp -- class methods for Statistics.

#include <iostream>
using namespace std;
#include "statistics.h"

// Private methods
// ****************************************************************************

// This method sets the object's attributes to zero.
void Statistics::initialize()
{
  dense_decrease = initial_decrease = iterative_decrease = poll_decrease = 0.0;
  dense_success = initial_success = iterative_success = poll_success = 0;
}

// Public methods:
//*****************************************************************************

// Class constructor.
Statistics::Statistics()
{
  // The attributes are initialized.
  initialize();
}

// These four methods ('addTo...') add the received parameter to the
// appropriate '...decrease' variable. They also increment by one the
// appropriate '...success' variable.
void Statistics::addToDenseDecrease(const double dense_dec)
{
  dense_decrease += dense_dec;
  dense_success++;
}

void Statistics::addToInitialDecrease(const double initial_dec)
{
  initial_decrease += initial_dec;
  initial_success++;
}

void Statistics::addToIterativeDecrease(const double iterative_dec)
{
  iterative_decrease += iterative_dec;
  iterative_success++;
}

void Statistics::addToPollDecrease(const double poll_dec)
{
  poll_decrease += poll_dec;
  poll_success++;
}

// This method resets the object's attributes to zero.
void Statistics::reset()
{initialize();}

// Non-member functions.
// ****************************************************************************

// This function outputs a Statistics object.
ostream & operator<<(ostream & os, const Statistics & s)
{
  // This variable is needed to translate the f(x) decreases in percentage.
  double total_decrease = s.dense_decrease + s.initial_decrease +
    s.iterative_decrease + s.poll_decrease;

  // In the fixed mode, the precision means the number of digits displayed to
  // the right of the decimal point.
  os.setf(ios_base::fixed);
  os.setf(ios_base::right);
  os.precision(2);

os << "\nRun statistics\n--------------\n\n";

  // The stats are written in table format. These are the column titles.
  os << "\t\tNUMBER OF\tf(x) DECREASE AT\n";
  os << "\t\tSUCCESSES\tFEASIBLE POINTS (%)\n";
  os << "\t\t---------\t-------------------\n";

  // First the poll statistics are displayed.
  os << "POLL\t\t    " << s.poll_success << "\t\t     ";
  (total_decrease > 0.0) ?
    os << s.poll_decrease / total_decrease * 100.0 : os << "0.0";

  // The search statistics are displayed.
  os << "\nSEARCH   \t    ";
  os << s.dense_success + s.initial_success + s.iterative_success << "\t\t     ";
  (total_decrease > 0.0) ?
    os << (total_decrease - s.poll_decrease) / total_decrease * 100.0 : os << "0.0";

  // The initial search statistics are displayed.
  os << "\n   (Initial   \t    " << s.initial_success << "\t\t     ";
  (total_decrease > 0.0) ?
    os << s.initial_decrease / total_decrease * 100.0 : os << "0.0";

  // The iterative search statistics are displayed.
  os << ")\n   (Iterative\t    " << s.iterative_success << "\t\t     ";
  (total_decrease > 0.0) ?
    os << s.iterative_decrease / total_decrease * 100.0 : os << "0.0";

  // The dense search statistics are displayed.
  os << ")\n   (Dense\t    " << s.dense_success << "\t\t     ";
  (total_decrease > 0.0) ?
    os << s.dense_decrease / total_decrease * 100.0 : os << "0.0";
  os << ")\n";

  // Standard notation and precision are restored.
  os.setf(ios_base::floatfield);
  os.setf(ios_base::left);
  os.precision(6);

  return os;
}

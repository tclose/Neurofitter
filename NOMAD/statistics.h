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

// statistics.h -- class definition for Statistics.

#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics
{
private:
  // Real variables.
  double dense_decrease, initial_decrease, iterative_decrease, poll_decrease;
  // Integer variables.
  unsigned int dense_success, initial_success, iterative_success, poll_success;

  // The class methods are described in "statistics.cpp".
  void initialize();

public:
  Statistics();
  // Destructor.
  ~Statistics() {}
  // 'get' methods for all the object's attributes.
  double getDenseDecrease() const {return dense_decrease;}
  double getInitialDecrease() const {return initial_decrease;}
  double getIterativeDecrease() const {return iterative_decrease;}
  double getPollDecrease() const {return poll_decrease;}
  unsigned int getDenseSuccess() const {return dense_success;}
  unsigned int getInitialSuccess() const {return initial_success;}
  unsigned int getIterativeSuccess() const {return iterative_success;}
  unsigned int getPollSuccess() const {return poll_success;}

  void addToDenseDecrease(const double dense_dec);
  void addToInitialDecrease(const double initial_dec);
  void addToIterativeDecrease(const double iterative_dec);
  void addToPollDecrease(const double poll_dec);
  void reset();
  friend ostream & operator<<(ostream & os, const Statistics & s);
};
#endif

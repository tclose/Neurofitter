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

// preferences.h -- class definition for Preferences.

#ifndef PREFERENCES_H
#define PREFERENCES_H
#include "solver.h"

class Preferences
{
private:
  // Boolean variables.
  bool send_email;
  // String variables.
  char * email_address, * send_email_string;
  // Unsigned integer variables.
  unsigned int display_factor;

  // Private methods.
  void createSendEmailString(const char * name);
  void readPreferences(ifstream & fin);
  void setPreferences(const unsigned int display, const bool email,
		      const char * address, const char * name);

  // The class methods are described in "preferences.cpp".
public:
  Preferences();
  
  // Added by WVG to allow reading from specific preference file	
  Preferences(const char *);
  void loadPreferences(const char *);
  
  ~Preferences();
  bool getSendEmail();
  char * getEmailAddress();
  unsigned int getDisplayFactor();
  void capture(const unsigned int display, const bool email_on,
	       const char * address, const char * name, Solver & controlbox);
  void capture(const char * name, Solver & controlbox);
  void endOfRun() const;
  void loadPreferences();
  void show() const;
  void write() const;
};
#endif

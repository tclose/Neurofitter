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

// preferences.cpp -- class methods for Preferences.

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;
#include "preferences.h"

// Private methods
// ****************************************************************************

//
void Preferences::createSendEmailString(const char * name)
{
  if (send_email_string != NULL)
    {
      delete [] send_email_string;
      send_email_string = NULL;
    }
  // The 'send_email_string' variable is created.
  const char * str1 = "echo \"Your Nomad run '";
  const char * str2 = "' has ended\" | mail -s 'Nomad' ";
  send_email_string = new char[strlen(str1) + strlen(str2) + 
			       strlen(name) + strlen(email_address) + 1];
  strcpy(send_email_string, str1);
  strcat(send_email_string, name);
  strcat(send_email_string, str2);
  strcat(send_email_string, email_address);
}

/* This method reads the preferences file, capturing its variables in the
   process. */
void Preferences::readPreferences(ifstream & fin)
{
  char ch;
  char line[80], temp[80];
  bool check_email = false, check_display = false, check_address = false;
  // The file is read line by line in this loop.
  while (!fin.eof())
    {
      // We look at the first character, without putting it in the input buffer
      ch = fin.peek();
      fin >> line;
      /* If the first character is '#', it means that we have a comment line.
	 Nothing is done in that case. */
      if (ch != '#')
	{
	  // The boolean 'send_email' is extracted.
	  if (!check_email && (strcmp(line, "SEND_EMAIL") == 0))
	    {
	      fin >> send_email;
	      check_email = true;
	    }
	  // The display factor is extracted:
	  else if (!check_display && (strcmp(line, "DISPLAY_FACTOR") == 0))
	    {
	      fin >> display_factor;
	      check_display = true;
	    }
	  // The email address is extracted:
	  else if (!check_address && (strcmp(line,"EMAIL_ADDRESS") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'email_address'.
	      if (email_address != NULL)
		delete [] email_address;
	      email_address = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'email_address'.
	      strcpy(email_address, temp);
	      check_address = true;
	    }
	}
    }
}

//
void Preferences::setPreferences(const unsigned int display, const bool email,
				 const char * address, const char * name)
{
  send_email = email;
  display_factor = display;
  if (email_address != NULL)
    {
      delete [] email_address;
      email_address = NULL;
    }
  if ((email == true) && (address != NULL))
    {
      // The memory is reserved for 'email_address'.
      email_address = new char[strlen(address) + 1];
      // 'address' is copied into 'email_address'.
      strcpy(email_address, address);
      // The 'send_email_string' is created.
      createSendEmailString(name);
    }
}

/* Public methods:
******************************************************************************/

// Class constructor.
Preferences::Preferences()
{
  /* All the variables are initialized to default preferences, just in case
     the user forgets to include a given variable in the preferences file. */
  // The boolean variables are initialized.
  send_email = false;
  // The string variable is initialized.
  email_address = NULL;
  send_email_string = NULL;
  // The unsigned variables are initialized.
  display_factor = 0;
  // The problem file must be loaded.
  loadPreferences();
}

///WVG Added to be able to specify the location of the preference file

// Class constructor.
Preferences::Preferences(const char * fileName)
{
  /* All the variables are initialized to default preferences, just in case
     the user forgets to include a given variable in the preferences file. */
  // The boolean variables are initialized.
  send_email = false;
  // The string variable is initialized.
  email_address = NULL;
  send_email_string = NULL;
  // The unsigned variables are initialized.
  display_factor = 0;
  // The problem file must be loaded.
  loadPreferences(fileName);
}



// Destructor.
Preferences::~Preferences()
{
  if (email_address != NULL)
    delete [] email_address;
  if (send_email_string != NULL)
    delete [] send_email_string;
}

bool Preferences::getSendEmail()
{
  return send_email;
}

char * Preferences::getEmailAddress()
{
  return email_address;
}

unsigned int Preferences::getDisplayFactor()
{
  return display_factor;
}

/* This method captures this object's attributes and sends them to Solver. It's
   called when the GUI is in use. */
void Preferences::capture(const unsigned int display, const bool email_on,
			  const char * address, const char * name,
			  Solver & controlbox)
{
  // The display factor is sent to Solver.
  controlbox.setDisplay(display);
  // The preferences are changed.
  setPreferences(display, email_on, address, name);
  // The preferences are written to file.
  write();
}

/* This method captures this object's attributes and sends them to Solver. It's
   called in batch file mode, when the GUI is inactive. */
void Preferences::capture(const char * name, Solver & controlbox)
{
  // The display factor is sent to Solver.
  controlbox.setDisplay(display_factor);
  //
  if ((send_email == true) && (email_address != NULL))
    createSendEmailString(name);
}

// This method sends an email to the user at the end of a run.
void Preferences::endOfRun() const
{
  if ((send_email) && (send_email_string != NULL))
    system(send_email_string);
}

// This method opens the preferences file and loads them in memory.
void Preferences::loadPreferences()
{
  // We open the file.
  ifstream fin("LIBRARY/preferences.dat");
  // If the file has been opened ('file' is a correct path)...
  if (fin.is_open())
    {
      // ...we read the file's data with this method.
      readPreferences(fin);
      // The file is closed.
      fin.close();
    }
  else
    // We alert the user that something is wrong with the path.
    cerr << "Couldn't open preferences file 'LIBRARY/preferences.txt'\n\n";
}


///WVG added to specify location of preference file

// This method opens the preferences file and loads them in memory.
void Preferences::loadPreferences(const char * fileName)
{
  // We open the file.
  ifstream fin(fileName);
  // If the file has been opened ('file' is a correct path)...
  if (fin.is_open())
    {
      // ...we read the file's data with this method.
      readPreferences(fin);
      // The file is closed.
      fin.close();
    }
  else
    // We alert the user that something is wrong with the path.
    cerr << "Couldn't open preferences file '" << fileName <<"'\n\n";
}



// This method prints the object's attributes to standard output.
void Preferences::show() const
{
  cout << "\n**************************************************************\n";
  cout << "Preferences file\n----------------\n\n";

  // The variables are shown.
  cout << "Display factor: " << display_factor;
  cout << "\nSend email ? ";
  (send_email == true) ? cout << "Yes" : cout << "No";
  if (send_email)
    cout << "\nEmail address: " << email_address << "\n";

  cout << "**************************************************************\n\n";
}

// This method writes the object's attributes in the preferences file.
void Preferences::write() const
{
  // First we open the preferences file.
  ofstream fout("LIBRARY/preferences.txt");
  if (!fout.is_open())
    cerr << "Couldn't open preferences file 'LIBRARY/preferences.txt'!\n";
  else   // The file is open; the attributes are written in the file.
    {
      /****************/
      /* Preferences. */
      /****************/
      fout << "#PREFERENCES\n#-----------\n";
      fout << "DISPLAY_FACTOR\t\t" << display_factor << "\n";
      fout << "SEND_EMAIL\t\t" << send_email << "\n";
      if (send_email)
	fout << "EMAIL_ADDRESS\t\t" << email_address << "\n";

      // We close the preferences file.
      fout.close();
    }
}

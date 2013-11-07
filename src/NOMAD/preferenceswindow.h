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

/* preferenceswindow.h -- class definition for PreferencesWindow. It has public
   inheritance from the QDialog class. */

#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H
#include <qdialog.h>
#include <qstring.h>
using namespace std;
#include "solver.h"

class Preferences;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

// PreferencesWindow inherits publicly from QDialog.
class PreferencesWindow: public QDialog
{
private:
  // A pointer to a Preferences object.
  Preferences * preferences;
  /* This check box indicates if the user wants to receive an email when the
     run has ended. */
  QCheckBox * send_email;
  // This combo box holds the display factor variable.
  QComboBox * display_factor;
  // This label asks the user for an email address.
  QLabel * email_address_label;
  // This text editor will hold the user's email address.
  QLineEdit * email_address;
  // Some buttons.
  QPushButton * ok;

  /* The macro "Q_OBJECT" must be included in all classes that contain signals
     and/or slots. The moc compiler must generate a moc file from the .cpp
     file; this moc file will then be compiled normally. */
  Q_OBJECT
  // The class methods are described in "descriptorwindow.cpp".
public:
  PreferencesWindow(QWidget * parent, const char * name = 0);
  ~PreferencesWindow();
  unsigned int getDisplayFactor();
  void capture(Solver & controlbox, const char * name);
  void endOfRun();
  void load();
  void showPreferences();

private slots:
  void cancelDialog();
  void emailEvent();
  void enable(const QString &);
};
#endif

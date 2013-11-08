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

// preferenceswindow.cpp -- class methods for PreferencesWindow.

#include <fstream>
#include <iostream>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
using namespace std;
#include "preferences.h"
#include "preferenceswindow.h"

// Public methods:
// ---------------

// Class constructor.
PreferencesWindow::PreferencesWindow(QWidget * parent, const char * name)
  : QDialog(parent, name)
{
  // A 1x1 grid is created. Its parent is "this" object (PreferencesWindow).
  QGridLayout * grid = new QGridLayout(this, 1, 1, 2, 2);

  // We put some objects on the grid: First row.
  grid->addWidget(new QLabel("Display factor", this), 0, 0);
  display_factor = new QComboBox(this);
  display_factor->insertItem("0");
  display_factor->insertItem("1");
  display_factor->insertItem("2");
  display_factor->insertItem("3");
  display_factor->insertItem("4");
  display_factor->insertItem("5");
  display_factor->insertItem("6");
  display_factor->insertItem("7");
  display_factor->insertItem("8");
  display_factor->insertItem("9");
  display_factor->insertItem("10");
  grid->addWidget(display_factor, 0, 1);

  // Second row.
  grid->addWidget(new QLabel("Send email after\nrun is over ?", this), 1, 0);
  send_email = new QCheckBox("", this);
  grid->addWidget(send_email, 1, 1);
  // The check box is connected to 'emailEvent()'.
  connect(send_email, SIGNAL(clicked()), this, SLOT(emailEvent()));

  // Third row.
  email_address_label = new QLabel("Email address: ", this);
  grid->addWidget(email_address_label, 2, 0);
  email_address = new QLineEdit(this);
  grid->addWidget(email_address, 2, 1);

  // Fourth row.
  ok = new QPushButton("Ok", this);
  grid->addWidget(ok, 3, 0);
  QPushButton * cancel = new QPushButton("Cancel", this);
  grid->addWidget(cancel, 3, 1);
  // When "Cancel" is pressed the dialog is closed.
  connect(cancel, SIGNAL(clicked()), this, SLOT(cancelDialog()));
  /* When 'Ok' is pressed, the dialog is closed and the seed value is
     recorded. */
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));

  // A title is written at the top of the window..
  setCaption("Preferences");

  // The Preferences object is created.
  preferences = new Preferences();
  // The preferences are loaded.
  load();
  // This call insures that the window will be shown correctly.
  emailEvent();
  // We tell the application to resize itself.
  resize(sizeHint());
}

// Destructor.
PreferencesWindow::~PreferencesWindow()
{
  // The Preferences object is destroyed.
  if (preferences != NULL)
    delete preferences;
}

//
unsigned int PreferencesWindow::getDisplayFactor()
{
  return preferences->getDisplayFactor();
}

// The 'capturePreferences' call is passed on to the Preferences object.
void PreferencesWindow::capture(Solver & controlbox, const char * name)
{
  // Is the 'send_email' checkbox checked?
  bool send_email_on = send_email->isChecked();
  /* The string 'address' is relevant only if the user wants to receive an
     email. */
  const char * address = (send_email_on == true) ?
    email_address->text().latin1() : NULL;
  // The call is passed along to 'Preferences'.
  preferences->capture(display_factor->currentItem(), send_email_on, address,
		       name, controlbox);
}

//
void PreferencesWindow::endOfRun()
{
  preferences->endOfRun();
}

// This method shows the preferences on the window.
void PreferencesWindow::load()
{
  preferences->loadPreferences();
  display_factor->setCurrentItem(preferences->getDisplayFactor());
  send_email->setChecked(preferences->getSendEmail());
  if (send_email->isChecked())
    email_address->setText(preferences->getEmailAddress());
}

//
void PreferencesWindow::showPreferences()
{
  preferences->show();
}

// Private slots.
// --------------

/* This method is called whenever the user presses the 'Cancel' button. The
   preferences are reset and the dialog is closed, */
void PreferencesWindow::cancelDialog()
{
  // The preferences are reset.
  display_factor->setCurrentItem(0);
  send_email->setChecked(false);
  email_address->clear();
  // The dialog is closed.
  reject();
}

// This method is called whenever the user clicks on the 'send_email' checkbox.
void PreferencesWindow::emailEvent()
{
  // First we check if the 'send_email' checkbox is checked or not.
  bool send_email_on = send_email->isChecked();
  // If the checkbox is off, the email address is erased.
  if (send_email_on == false)
    email_address->clear();
  // These fields are enabled or disabled according to the checkbox.
  email_address_label->setEnabled(send_email_on);
  email_address->setEnabled(send_email_on);
}

// This method decides when to enable the 'Solve' button.
void PreferencesWindow::enable(const QString &)
{}

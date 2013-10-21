/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* �cole Polytechnique de Montr�al - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montr�al (Qu�bec) H3C 3A7 Canada             *
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

// mainwindow.cpp -- class methods for MainWindow.

#include "mainapplication.h"
#include "mainwindow.h"

/* Class constructor. MainWindow inherits QMainWindow. In the QMainWindow
   constructor, we have the following things:
   - "0" means that the object has no parent object.
   - "GUI main window" is the object's name.
   - "WDestructiveClose" is a flag that tells the object to self-destruct when
     it's closed. */
MainWindow::MainWindow(int argc, char * argv[])
  : QMainWindow(0, "GUI main window", WDestructiveClose)
{
  // The MainApplication object is created. We give it MainWindow's address.
  main_app = new MainApplication(this, argc, argv);

  /* "main_app" is chosen as the main widget for the application. If a user
     closes a main widget, the application exits. */
  setCentralWidget(main_app);

  // We tell the application to resize itself.
  resize(minimumSizeHint());
}

// Destructor. The "main" widget is destroyed automatically by QT.
MainWindow::~MainWindow()
{}

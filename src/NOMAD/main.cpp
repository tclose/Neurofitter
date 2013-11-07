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

/* main.cpp -- main program for NOMAD (with the GUI).
   In NOMAD we use Qt, a cross-platform C++ GUI application framework, to build
   a graphical user interface. For more information see the "Qt Reference
   Documentation" at: "http://doc.trolltech.com/"
*/

#include <qapplication.h>
using namespace std;
#include "mainwindow.h"

// 'main' part of the software.
int main(int argc, char * argv[])
{
  /* We create a QApplication object, which is Qt's main object. There has to
     be exactly one QAplication object in every application that uses Qt. */
  QApplication a(argc, argv);
  a.setFont(QFont("Helvetica", 10, QFont::Normal), true);
  /* The MainWindow object is created: it's the window that will be seen by the
     program user. */
  MainWindow * mw = new MainWindow(argc, argv);
  // We give it a title.
  mw->setCaption
    ("NOMAD - Nonlinear Optimization for Mixed vAriables, with Derivatives");
  /* We set it as the main widget (if it's closed, the application will
     terminate). */
  a.setMainWidget(mw);
  // The main window shows itself.
  mw->show();
  /* "connect()" is perhaps the most central feature of Qt. It establishes a
     direct connection between two Qt objects. Every Qt object can both have
     'signals' (to send messages) and 'slots' (to receive messages). Here we
     tell the application "a" to quit when the last program window is closed.*/
  a.connect(& a, SIGNAL(lastWindowClosed()), & a, SLOT(quit()));
  /* This is where "main()" passes control to Qt. "exec()" will return when the
     application exits. */
  return a.exec();
}

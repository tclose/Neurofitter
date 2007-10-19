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

/* mainapplication.h -- class definition for MainApplication. It has public
   inheritance from the QWidget class. */

#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include <qwidget.h>

class DescriptionWindow;
class FilterGraph;
class Graph;
class ParametersWindow;
class PreferencesWindow;
class QGridLayout;
class QLineEdit;
class QMenuBar;
class QPopupMenu;
class QPushButton;
class Solver;
class TextEditor;

// MainApplication inherits publicly from QWidget.
class MainApplication: public QWidget
{
private:
  /* The MainApplication object has the following variables:
******************************************************************************/
  bool ready_to_solve;
  DescriptionWindow * description_win;
  FilterGraph * filter_graph;
  Graph * graph;
  ParametersWindow * parameters_win;
  PreferencesWindow * preferences_win;
  /* An object of the QGridLayout class, "grid". The QGridLayout class lays
     out widgets in a grid. */
  QGridLayout * grid;
  /* Many objects of the QLineEdit class. The QLineEdit widget is a one-line
     text editor: These text editors will display the problem's solution. */
  QLineEdit * ceval, * criterion, * function_value, * nteval, * point_coord;
  QMenuBar * menu;
  QPopupMenu * description_m, * strategies;
  QPushButton * continue_run, * new_run, * results_pb, * solve;
  Solver * controlbox;
  // Pointer to a text editor.
  TextEditor * results_edit;

  /* The macro "Q_OBJECT" must be included in all classes that contain signals
     and/or slots. The moc compiler must generate a moc file from the .cpp
     file; this moc file will then be compiled normally. */
  Q_OBJECT

  // The class methods are described in "mainapplication.cpp".
  void clearOutputs();
  void enableSolve();
  void outputSolution();
  void solveProblem();

public:
  MainApplication(QWidget * parent, int argc, char * argv[],
		  const char * name = 0);
  ~MainApplication();

private slots:
  // void clearGraph();
  void continueRun();
  void defaults();
  void editDescription();
  void editPreferences();
  void loadDescription();
  void loadParameters();
  void newDescription();
  void newRun();
  void quitProgram();
  void solveClicked();
  void viewResults();
};
#endif

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

// mainapplication.cpp -- class methods for MainApplication.

#include <fstream>
#include <iostream>
#include <qapplication.h>
#include <qdialog.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qvalidator.h>
using namespace std;
#include "descriptionwindow.h"
#include "filtergraph.h"
#include "graph.h"
#include "mainapplication.h"
#include "parameterswindow.h"
#include "preferenceswindow.h"
#include "solver.h"
#include "texteditor.h"

/* Private methods:
******************************************************************************/

// This method clears the problem outputs.
void MainApplication::clearOutputs()
{
  // The GUI's outputs are reset.
  ceval->clear();
  criterion->clear();
  function_value->clear();
  point_coord->clear();
  nteval->clear();
  // The graph is erased.
  graph->setOutput(false);
  // The filter graph is destroyed.
  if (filter_graph != NULL)
    {
      delete filter_graph;
      filter_graph = NULL;
    }
}

// This method manages the 'solve' button.
void MainApplication::enableSolve()
{
  /* The 'solve' button is enabled if these booleans are true, disabled
     otherwise. */
  if ((ready_to_solve) && (description_win->readyToSolve()) &&
      (parameters_win->readyToSolve()))
    solve->setEnabled(true);
  else
    solve->setEnabled(false);
}

// This method displays the problem results (including graphs).
void MainApplication::outputSolution()
{
  // Some local variables we'll need.
  QString qs, qs_buffer;
  unsigned int i;
  unsigned int dimension = description_win->getDimension();

  /* Once the problem is solved, we have to write the correct data in the
     output window, and to draw the graph. */
  

  /* In the next block the best iterate's truth value is written in the text
     editor 'function_value', and the best iterate's coordinates are written in
     'point_coord'. */
  double truth = controlbox->getTruth();
  // If 'truth' is infinite, No feasible point was found in the run.
  if (truth == INFINIT)
    {
      point_coord->setText("No feasible point was found");
      function_value->setText("N/A");
    }
  else
    {
      // "coord" will be the best point found, in unscaled coordinates.
      double * coord = new double[dimension];
      // 'coord' is initialized.
      for (i = 0; i < dimension; i++)
	coord[i] = 0.0;
      /* We send it to Solver: the unscaled values of the best point found will
	 be put into it. */
      controlbox->fillWithBestPoint(coord);

      // The 'coord' array is copied into a QString object, 'qs'.
      qs.append("(");
      for (i = 0; i < dimension; i++)
	{
	  qs.append(qs_buffer.setNum(coord[i], 'g', 3));
	  if (i < (dimension - 1))
	    qs.append(", ");
	  else
	    qs.append(")");
	}
      // The strings are built, and written in the text editors.
      point_coord->setText(qs);
      function_value->setText(qs_buffer.setNum(truth, 'g', 6));

      // The memory is freed.
      delete [] coord;
    }

  // We output the number of new truth evaluations in the text editor "nteval".
  int new_truth_eval = controlbox->returnNewTruthEvaluations();
  nteval->setText(qs_buffer.setNum(new_truth_eval));
  /* We output the number of cache evaluations in the text editor "ceval".
     The format is 'temporary cache hits / permanent cache hits'. */
  qs.setNum(controlbox->returnTempCacheHits());
  qs.append(" / ");
  qs_buffer.setNum(controlbox->returnPermCacheHits());
  qs.append(qs_buffer);
  ceval->setText(qs);

  /* We output the activated termination criterion in the 'criterion'
     'QLineEdit' object. */
  switch(controlbox->returnTerminationFlag())
    {
      case 0  : criterion->setText("Poll size");
                break;
      case 1  : criterion->setText("Function evaluations");
                break;
      case 2  : criterion->setText("Consecutive failures");
                break;
      case 3  : criterion->setText("Truth evaluations");
                break;
      default : cerr << "No termination criterion activated!\n";
    }

  // Finally the graph is drawn.
  graph->setOutput(true);
}

/* This method checks if the problem parameters are correct, then tells Solver
   to solve the current problem. */
void MainApplication::solveProblem()
{
  /* We check if there's an error with the parameters file. If everything is
     ok... */
  if (parameters_win->readyToSolve())
    {
      unsigned int gen_cons_nb =
	description_win->getGeneralConstraintsNumber();
      // First the fields in the parameters' window are sent to Parameters.
      parameters_win->sendParameters();
      // Then the parameters are captured and sent to Solver.
      parameters_win->capture(gen_cons_nb, description_win->getSurrogateUse(),
			      * controlbox);
      // The preferences are sent to Solver.
      preferences_win->capture(* controlbox, description_win->getName());
      /* A 'FilterGraph' object is created; it's linked with the Filter object
	 of NOMAD. */
      if ((gen_cons_nb > 0) && (filter_graph == NULL))
	filter_graph = new FilterGraph(controlbox->getFilter(),
				       controlbox->getGenCons());
      // The starting point is evaluated. If everything is ok ...
      if (controlbox->evaluateStartingPoint() > 0)
	{
	  // 'fout' is the results file for the current run.
	  ofstream fout(description_win->getResultsFile());
	  // The problem's input is written in 'fout'.
	  controlbox->showInput(fout);
	  // The problem is solved by the optimization module.
	  controlbox->solveProblem(fout);
	  // The solution is displayed on the GUI.
	  outputSolution();
	  // The results file is closed.
	  fout.close();
	  // The 'Preferences' object must be alerted that the run is over.
	  preferences_win->endOfRun();
	  // The 'Continue run' and 'New run' buttons are enabled.
	  continue_run->setEnabled(true);
	  new_run->setEnabled(true);
	  /* The boolean 'ready_to_solve' is put to 'false', so the user can't
	     push the 'solve' button many times in a row. */
	  ready_to_solve = false;
	  enableSolve();
	  // The poll size text editor is disabled.
	  parameters_win->enablePollSize(false);
	}
      else
	{
	  switch(QMessageBox::critical(this, "NOMAD",
				   "The starting point is outside the bounds!\n"
				   "Please correct before running NOMAD.\n"
				   "See the program documentation in the\n"
				   "DOC directory."))
	    {
	      case QMessageBox::Ok : close();
				     break;
	    }
	}
    }
  else   // We alert the user that something's wrong with the parameters.
    {
      switch(QMessageBox::critical(this, "NOMAD",
				   "Something is wrong with the parameters!\n"
				   "Please correct before running NOMAD.\n"
				   "See the program documentation in the\n"
				   "DOC directory."))
	{
	  case QMessageBox::Ok : close();
				 break;
	}
    }
}

/* Public methods:
******************************************************************************/

/* Class constructor. In the QWidget constructor we have the following things:
   - The parent object, which is the MainWindow object (see "mainwindow.cpp").
   - The object's name.
   - "0" means that no flag is used in the constructor. */
MainApplication::MainApplication(QWidget * parent, int argc, char * argv[],
				 const char * name)
  : QWidget(parent, name, 0)
{
  // These pointers are initialized to NULL.
  filter_graph = NULL;
  results_edit = NULL;

  // The DescriptionWindow object is created (but not shown at the start).
  if (argc > 1)
    {
      // A Description file was given in the command line.
      description_win = new DescriptionWindow(this, argv[1]);
      // The 'solve' button can be pushed right away.
      ready_to_solve = true;
    }
  else
    {
      // No Description file.
      description_win = new DescriptionWindow(this);
      /* The 'solve' button can't be pushed; a problem description must be
	 loaded first. */
      ready_to_solve = false;
    }

  // The PreferencesWindow object is created, but not shown.
  preferences_win = new PreferencesWindow(this);

  // 'controlbox' points to the Solver object, which is NOMAD's controller.
  controlbox = new Solver();

  // In this section we build the items that will be on the menu bar.
  // The problem description menu.
  description_m = new QPopupMenu(this);
  description_m->insertItem("New ...", this, SLOT(newDescription()));
  description_m->insertItem("Load ...", this, SLOT(loadDescription()));
  description_m->insertItem("Edit ...", this, SLOT(editDescription()));

  // The Parameters menu.
  QPopupMenu * parameters_m = new QPopupMenu(this);
  parameters_m->insertItem("Load ...", this, SLOT(loadParameters()));
  parameters_m->insertItem("Set to defaults", this, SLOT(defaults()));

  // The Results menu.
  // QPopupMenu * results_m = new QPopupMenu(this);
  // results_m->insertItem("Clear graph", this, SLOT(clearGraph()));

  // The Preferences menu.
  QPopupMenu * preferences_m = new QPopupMenu(this);
  preferences_m->insertItem("Edit", this, SLOT(editPreferences()));

  // We put the items on the menu bar.
  menu = new QMenuBar(this);
  menu->insertItem("Description", description_m);
  menu->insertItem("Parameters", parameters_m);
  menu->insertItem("Preferences", preferences_m);

  // A 1x1 grid is created. Its parent is "this" object (the "main" widget).
  grid = new QGridLayout(this, 1, 1, 2, 2);
  // The grid has to be aware of the menu bar to place its objects properly.
  grid->setMenuBar(menu);

  /*************/
  /* LEFT SIDE */
  /*************/

  // The ParametersWindow object is created and put on the grid..
  if (argc > 2)
    {
      // A Parameters file was given in the command line.
      parameters_win = new ParametersWindow
	(this, description_win->getSurrogateUse(),
	 description_win->getGeneralConstraintsNumber(), argv[2]);
    }
  else
    // No Parameters file.
    parameters_win = new ParametersWindow(this, false, 0);
  grid->addMultiCellWidget(parameters_win, 0, 20, 0, 3);

  // Vertical separator between left and right side.
  QGroupBox * separator = new QGroupBox(this);
  separator->setPaletteBackgroundColor(Qt::gray);
  separator->setMinimumWidth(15);
  separator->setMaximumWidth(15);
  grid->addMultiCellWidget(separator, 0, 20, 4, 4);

  /**************/
  /* RIGHT SIDE */
  /**************/

  // Section title.
  QGroupBox * results_box = new QGroupBox("Problem results", this);
  results_box->setPaletteBackgroundColor(Qt::gray);
  results_box->setAlignment(Qt::AlignHCenter);
  results_box->setFlat(true);
  results_box->setMinimumHeight(20);
  results_box->setMaximumHeight(20);
  grid->addMultiCellWidget(results_box, 0, 0, 5, 8);

  /* On the second row we put two text editors where the best point found by
     Nomad and its truth value will be displayed. */
  grid->addWidget(new QLabel("Best point:", this), 1, 5);
  point_coord = new QLineEdit(this);
  point_coord->setReadOnly(TRUE);
  point_coord->setFrame(FALSE);
  grid->addWidget(point_coord, 1, 6);
  grid->addWidget(new QLabel("Objective function\n"
			     "value at best point:", this), 1, 7);
  function_value = new QLineEdit(this);
  function_value->setReadOnly(TRUE);
  function_value->setFrame(FALSE);
  grid->addWidget(function_value, 1, 8);

  // Third row.
  grid->addWidget(new QLabel("New truth\n"
			     "evaluations:", this), 2, 5);
  nteval = new QLineEdit(this);
  nteval->setReadOnly(TRUE);
  nteval->setFrame(FALSE);
  grid->addWidget(nteval, 2, 6);
  grid->addWidget(new QLabel("Temporary/permanent\n"
			     "cache hits:", this), 2, 7);
  ceval = new QLineEdit(this);
  ceval->setReadOnly(TRUE);
  ceval->setFrame(FALSE);
  grid->addWidget(ceval, 2, 8);

  // 4th row.
  grid->addWidget(new QLabel("Termination:", this), 3, 5);
  criterion = new QLineEdit(this);
  criterion->setReadOnly(true);
  criterion->setFrame(false);
  grid->addWidget(criterion, 3, 6);
  results_pb = new QPushButton("View results", this);
  grid->addWidget(results_pb, 3, 8);
  // The 'results_pb' button is connected to the 'viewResults()' slot.
  connect(results_pb, SIGNAL(clicked()), this, SLOT(viewResults()));

  // Section title.
  QGroupBox * graph_box = new QGroupBox("Output graph", this);
  graph_box->setPaletteBackgroundColor(Qt::gray);
  graph_box->setAlignment(Qt::AlignHCenter);
  graph_box->setFlat(true);
  graph_box->setMinimumHeight(20);
  graph_box->setMaximumHeight(20);
  grid->addMultiCellWidget(graph_box, 4, 4, 5, 8);

  /* We put a graph, displaying the objective function value according to the
     number of truth evaluations. */
  graph = new Graph("LIBRARY/graph.txt", "Number of function evaluations ",
  		    "Objective function value ", false, this, 0);
  grid->addMultiCellWidget(graph, 5, 18, 5, 8);

  // A section separator is put on the 20th row.
  QGroupBox * separator_gb = new QGroupBox(this);
  separator_gb->setPaletteBackgroundColor(Qt::gray);
  separator_gb->setMinimumHeight(15);
  separator_gb->setMaximumHeight(15);
  grid->addMultiCellWidget(separator_gb, 19, 19, 5, 8);

  // 21st row.
  solve = new QPushButton("Solve", this);
  grid->addWidget(solve, 20, 5);
  continue_run = new QPushButton("Continue run", this);
  continue_run->setDisabled(true);
  grid->addWidget(continue_run, 20, 6);
  new_run = new QPushButton("New run", this);
  new_run->setDisabled(true);
  grid->addWidget(new_run, 20, 7);
  QPushButton * quit = new QPushButton("Quit", this);
  grid->addWidget(quit, 20, 8);
  // The buttons are connected to functions.
  connect(solve, SIGNAL(clicked()), this, SLOT(solveClicked()));
  connect(continue_run, SIGNAL(clicked()), this, SLOT(continueRun()));
  connect(new_run, SIGNAL(clicked()), this, SLOT(newRun()));
  // If the user presses "Quit", the QApplication quits.
  connect(quit, SIGNAL(clicked()), this, SLOT(quitProgram()));

  // We tell the application to resize itself.
  resize(minimumSizeHint());

  /* This call will enable or disable the 'solve' button, according to the
     status of the description and parameters files. */
  enableSolve();
}

// Destructor. All child objects are destroyed automatically.
MainApplication::~MainApplication()
{
  if (description_win != NULL)
    delete description_win;
  if (preferences_win != NULL)
    delete preferences_win;
  if (filter_graph != NULL)
    delete filter_graph;
  if (parameters_win != NULL)
    delete parameters_win;
  delete controlbox;
  if (results_edit != NULL)
    delete results_edit;
}

//*****************************************************************************
// Private slots: These methods are called when an event happens in the GUI
//                (the user clicks a button for example).
//*****************************************************************************

// This method clears the standard output graph.
// void MainApplication::clearGraph()
// {
//  graph->setOutput(false);
// }

// This method allows users to continue a run.
void MainApplication::continueRun()
{
  // First the problem outputs are cleared.
  clearOutputs();
  // We tell 'Solver' that there's a new run.
  controlbox->continueRun();
  // The problem is solved.
  solveProblem();
  // This function call will enable and disable certain buttons.
  ready_to_solve = false;
  enableSolve();
}

// This method resets the problem parameters to defaults.
void MainApplication::defaults()
{
  parameters_win->defaults();
}

// This method lets the user edit a problem file in the problem dialog window.
void MainApplication::editDescription()
{
  description_win->load();
}

// This method lets the user edit the program preferences.
void MainApplication::editPreferences()
{
  preferences_win->show();
}

/* This method opens a file dialog window, so the user can choose a problem
   file. */
void MainApplication::loadDescription()
{
  /* The function 'getOpenFileName(...)' won't let the user choose a file
     name that doesn't exist. */
  QString file = QFileDialog::getOpenFileName(QString::null,
					      QString::null, this);
  // The problem file is loaded.
  if (file != NULL)
    {
      description_win->load(file.latin1());
      // The 'solve' button is enabled.
      ready_to_solve = true;
      enableSolve();
    }
}

/* This method opens a file dialog window, so the user can choose a parameters
   file. */
void MainApplication::loadParameters()
{
  /* The function 'getOpenFileName(...)' won't let the user choose a file
     name that doesn't exist. */
  QString file = QFileDialog::getOpenFileName(QString::null,
					      QString::null, this);
  // The parameters file is loaded.
  if (file != NULL)
    parameters_win->load(description_win->getSurrogateUse(),
			 description_win->getGeneralConstraintsNumber(),
			 file.latin1());
}

// This method creates a new problem description.
void MainApplication::newDescription()
{
  description_win->newDescription();
}

/* This method allows users to do a new run, either with the same or with a new
   problem. */
void MainApplication::newRun()
{
  // First the problem outputs are cleared.
  clearOutputs();

  // The Solver object is destroyed, then recreated.
  delete controlbox;
  controlbox = new Solver();

  // The 'Continue run' and 'New run' buttons are disabled.
  continue_run->setDisabled(true);
  new_run->setDisabled(true);

  // The 'solve' button is enabled.
  ready_to_solve = true;
  enableSolve();
  // The description item in the menu is enabled, along with the poll size text
  // editor.
  description_m->setEnabled(true);
  parameters_win->enablePollSize(true);
}

// This method is called when the user pushes the 'Quit' button.
void MainApplication::quitProgram()
{
  // The 'filter_graph' is closed.
  if (filter_graph != NULL)
    filter_graph->close();
  // Closing the parent widget terminates the program.
  parentWidget()->close();
}

// The user just pushed the 'solve' button: the problem is solved.
void MainApplication::solveClicked()
{
  // First we check if the description file is complete.
  if (description_win->readyToSolve())
    {
      // The problem description is sent to Solver.
      description_win->capture(* controlbox);
      // The problem is solved.
      solveProblem();
      // The description item in the menu is deactivated.
      description_m->setDisabled(true);
    }
  else   // The description file is incomplete: the user is alerted.
    {
      switch(QMessageBox::critical(this, "NOMAD",
				   "The description file is incomplete!\n"
				   "Please complete it before running NOMAD.\n"
				   "See the program documentation in the\n"
				   "DOC directory."))
	{
	  case QMessageBox::Ok : close();
				 break;
	}
    }
}

// This method lets the user view the current results file.
void MainApplication::viewResults()
{
  const char * results_file = description_win->getResultsFile();
  // The text editor is created.
  if (results_edit == NULL)
    results_edit = new TextEditor(results_file);
  // The results file is loaded in the text editor.
  results_edit->load(results_file);
  // The text editor is shown.
  results_edit->show();
}

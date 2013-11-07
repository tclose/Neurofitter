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

// descriptionwindow.cpp -- class methods for DescriptionWindow.

#include <fstream>
#include <iostream>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qvalidator.h>
using namespace std;
#include "description.h"
#include "descriptionwindow.h"
#include "texteditor.h"

// Public methods:
// ---------------

// Class constructor.
DescriptionWindow::DescriptionWindow(QWidget * parent, const char * file,
				   const char * name)
  : QDialog(parent, name)
{
  // This boolean variable starts out false.
  dialog_edited = false;
  // The text editors are null at the start.
  bounds_edit = NULL;
  general_edit = NULL;
  scales_edit = NULL;
  start_edit = NULL;

  // A 1x1 grid is created. Its parent is "this" object (problem dialog).
  QGridLayout * grid = new QGridLayout(this, 1, 1, 2, 2);

  // A section title is put on the first row.
  QGroupBox * general_gb = new QGroupBox("General description", this);
  general_gb->setPaletteBackgroundColor(Qt::gray);
  general_gb->setAlignment(Qt::AlignHCenter);
  general_gb->setFlat(true);
  general_gb->setMinimumHeight(20);
  general_gb->setMaximumHeight(20);
  grid->addMultiCellWidget(general_gb, 0, 0, 0, 3);

  // Second row.
  grid->addWidget(new QLabel("Problem name:", this), 1, 0);
  problem_name = new QLineEdit(this);
  grid->addWidget(problem_name, 1, 1);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(problem_name, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));

  // Third row.
  grid->addWidget(new QLabel("Problem dimension:", this), 2, 0);
  dimension = new QLineEdit(this);
  dimension->setValidator(new QIntValidator(1, 99, dimension));
  grid->addWidget(dimension, 2, 1);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(dimension, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));

  // 4th row.
  cache_cb = new QCheckBox("Use caches?", this);
  grid->addWidget(cache_cb, 3, 0);
  cache = new QLineEdit(this);
  grid->addWidget(cache, 3, 1);
  cache_enter = new QPushButton("Enter", this);
  grid->addWidget(cache_enter, 3, 2);
  // The check box is connected to 'cacheEvent()'.
  connect(cache_cb, SIGNAL(clicked()), this, SLOT(cacheEvent()));
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(cache_cb, SIGNAL(clicked()), this, SLOT(dialogEdited()));
  connect(cache, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterCacheFile()' function.
  connect(cache_enter, SIGNAL(clicked()), this, SLOT(enterCacheFile()));

  // A section title is put on the 5th row.
  QGroupBox * constraints_gb = new QGroupBox("Constraints", this);
  constraints_gb->setPaletteBackgroundColor(Qt::gray);
  constraints_gb->setAlignment(Qt::AlignHCenter);
  constraints_gb->setFlat(true);
  constraints_gb->setMinimumHeight(20);
  constraints_gb->setMaximumHeight(20);
  grid->addMultiCellWidget(constraints_gb, 4, 4, 0, 3);

  // 6th row.
  grid->addWidget(new QLabel("Number of general\n"
			     "constraints:", this), 5, 0);
  gen_cons = new QLineEdit(this);
  gen_cons->setValidator(new QIntValidator(0, 99, gen_cons));
  grid->addWidget(gen_cons, 5, 1);
  // The text editor is connected to 'genConsEvent(...)'.
  connect(gen_cons, SIGNAL(textChanged(const QString &)),
	  this, SLOT(genConsEvent(const QString &)));
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(gen_cons, SIGNAL( textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));

  // 7th row.
  bounds_cb = new QCheckBox("Use bounds?", this);
  grid->addWidget(bounds_cb, 6, 0);
  bounds = new QLineEdit(this);
  grid->addWidget(bounds, 6, 1);
  bounds_enter = new QPushButton("Enter", this);
  grid->addWidget(bounds_enter, 6, 2);
  bounds_view = new QPushButton("View", this);
  grid->addWidget(bounds_view, 6, 3);
  // The check box is connected to 'boundsEvent()'.
  connect(bounds_cb, SIGNAL(clicked()), this, SLOT(boundsEvent()));
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(bounds_cb, SIGNAL(clicked()), this, SLOT(dialogEdited()));
  connect(bounds, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  /* The 'bounds' QLineEdit object is connected to the 'boundsEdited(...)'
     function. */
  connect(bounds, SIGNAL(textChanged(const QString &)),
	  this, SLOT(boundsEdited(const QString &)));
  // The button is connected to the 'enterBoundConstraints()' function.
  connect(bounds_enter, SIGNAL(clicked()),
	  this, SLOT(enterBoundConstraints()));
  // The button is connected to the 'editBounds()' function.
  connect(bounds_view, SIGNAL(clicked()), this, SLOT(editBounds()));

  // 8th row.
  QGroupBox * files_gb = new QGroupBox("Problem files", this);
  files_gb->setPaletteBackgroundColor(Qt::gray);
  files_gb->setAlignment(Qt::AlignHCenter);
  files_gb->setFlat(true);
  files_gb->setMinimumHeight(20);
  files_gb->setMaximumHeight(20);
  grid->addMultiCellWidget(files_gb, 7, 7, 0, 3);

  // 9th row.
  grid->addWidget(new QLabel("Directory:", this), 8, 0);
  directory = new QLineEdit(this);
  grid->addWidget(directory, 8, 1);
  QPushButton * dir_b = new QPushButton("Enter", this);
  grid->addWidget(dir_b, 8, 2);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(directory, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterProblemDirectory()' function.
  connect(dir_b, SIGNAL(clicked()), this, SLOT(enterProblemDirectory()));

  // 10th row.
  grid->addWidget(new QLabel("Starting point file:", this), 9, 0);
  start_pt = new QLineEdit(this);
  grid->addWidget(start_pt, 9, 1);
  QPushButton * start_b = new QPushButton("Enter", this);
  grid->addWidget(start_b, 9, 2);
  start_view = new QPushButton("View", this);
  grid->addWidget(start_view, 9, 3);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(start_pt, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  /* The 'start_pt' QLineEdit object is connected to the
     'startingPointEdited(...)' function. */
  connect(start_pt, SIGNAL(textChanged(const QString &)),
	  this, SLOT(startingPointEdited(const QString &)));
  // The button is connected to the 'enterStartingPoint()' function.
  connect(start_b, SIGNAL(clicked()), this, SLOT(enterStartingPoint()));
  // The button is connected to the 'editStartingPoint()' function.
  connect(start_view, SIGNAL(clicked()), this, SLOT(editStartingPoint()));

  // 11th row.
  grid->addWidget(new QLabel("Results file:", this), 10, 0);
  results = new QLineEdit(this);
  grid->addWidget(results, 10, 1);
  QPushButton * res_b = new QPushButton("Enter", this);
  grid->addWidget(res_b, 10, 2);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(results, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterResultsFile()' function.
  connect(res_b, SIGNAL(clicked()), this, SLOT(enterResultsFile()));

  // 12th row.
  black_box = new QGroupBox("Black boxes", this);
  black_box->setPaletteBackgroundColor(Qt::gray);
  black_box->setAlignment(Qt::AlignHCenter);
  black_box->setFlat(true);
  black_box->setMinimumHeight(20);
  black_box->setMaximumHeight(20);
  black_box->setCheckable(true);
  grid->addMultiCellWidget(black_box, 11, 11, 0, 3);
  // The check box is connected to 'blackBoxEvent()'.
  connect(black_box, SIGNAL(toggled(bool)), this, SLOT(blackBoxEvent(bool)));
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(black_box, SIGNAL(toggled(bool)), this, SLOT(dialogEdited(bool)));

  // 13th row.
  grid->addWidget(new QLabel("Input file:", this), 12, 0);
  input = new QLineEdit(this);
  grid->addWidget(input, 12, 1);
  input_enter = new QPushButton("Enter", this);
  grid->addWidget(input_enter, 12, 2);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(input, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterInputFile()' function.
  connect(input_enter, SIGNAL(clicked()), this, SLOT(enterInputFile()));

  // 14th row.
  grid->addWidget(new QLabel("Truth executable:", this), 13, 0);
  truth = new QLineEdit(this);
  grid->addWidget(truth, 13, 1);
  truth_enter = new QPushButton("Enter", this);
  grid->addWidget(truth_enter, 13, 2);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(truth, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterTruthExecutable()' function.
  connect(truth_enter, SIGNAL(clicked()), this, SLOT(enterTruthExecutable()));

  // 15th row.
  surrogate_cb = new QCheckBox("Use surrogate?", this);
  grid->addWidget(surrogate_cb, 14, 0);
  surrogate = new QLineEdit(this);
  grid->addWidget(surrogate, 14, 1);
  surrogate_enter = new QPushButton("Enter", this);
  grid->addWidget(surrogate_enter, 14, 2);
  // The check box is connected to 'surrogateEvent()'.
  connect(surrogate_cb, SIGNAL(clicked()), this, SLOT(surrogateEvent()));
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(surrogate_cb, SIGNAL(clicked()), this, SLOT(dialogEdited()));
  connect(surrogate, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  // The button is connected to the 'enterSurrogateExecutable()' function.
  connect(surrogate_enter, SIGNAL(clicked()),
	  this, SLOT(enterSurrogateExecutable()));

  // 16th row.
  grid->addWidget(new QLabel("General constraints:", this), 15, 0);
  general = new QLineEdit(this);
  grid->addWidget(general, 15, 1);
  general_enter = new QPushButton("Enter", this);
  grid->addWidget(general_enter, 15, 2);
  general_view = new QPushButton("View", this);
  grid->addWidget(general_view, 15, 3);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(general, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  /* The 'general' QLineEdit object is connected to the 'genConsEdited(...)'
     function. */
  connect(general, SIGNAL(textChanged(const QString &)),
	  this, SLOT(genConsEdited(const QString &)));
  // The button is connected to the 'enterGeneralConstraints()' function.
  connect(general_enter, SIGNAL(clicked()),
	  this, SLOT(enterGeneralConstraints()));
  // The button is connected to the 'editGenCons()' function.
  connect(general_view, SIGNAL(clicked()), this, SLOT(editGenCons()));

  // A section title is put on the 17th row.
  QGroupBox * scaling_gb = new QGroupBox("Scaling", this);
  scaling_gb->setPaletteBackgroundColor(Qt::gray);
  scaling_gb->setAlignment(Qt::AlignHCenter);
  scaling_gb->setFlat(true);
  scaling_gb->setMinimumHeight(20);
  scaling_gb->setMaximumHeight(20);
  grid->addMultiCellWidget(scaling_gb, 16, 16, 0, 3);

  // 18th row.
  grid->addWidget(new QLabel("Scale variables?", this), 17, 0);
  scaling_cb = new QComboBox(this);
  scaling_cb->insertItem("No scaling");
  scaling_cb->insertItem("Automatic");
  scaling_cb->insertItem("User vector");
  scaling_cb->insertItem("User bounds");
  grid->addWidget(scaling_cb, 17, 1);
  // The combo box is connected to 'scalingEvent(int)'.
  connect(scaling_cb, SIGNAL(activated(int)), this, SLOT(scalingEvent(int)));

  // 19th row.
  user_vector_lbl = new QLabel("User vector:", this);
  grid->addWidget(user_vector_lbl, 18, 0);
  scaling_vector = new QLineEdit(this);
  grid->addWidget(scaling_vector, 18, 1);
  scales_enter = new QPushButton("Enter", this);
  grid->addWidget(scales_enter, 18, 2);
  scales_view = new QPushButton("View", this);
  grid->addWidget(scales_view, 18, 3);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(scaling_vector, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  /* the 'scaling_vector' QLineEdit object is connected to the 'scalesEdited()'
     function. */
  connect(scaling_vector, SIGNAL(textChanged(const QString &)),
	  this, SLOT(scalesEdited(const QString &)));
  // The 'Enter' button is connected to the 'enterScales()' function.
  connect(scales_enter, SIGNAL(clicked()), this, SLOT(enterScales()));
  // The 'View' button is connected to the 'editScales()' function.
  connect(scales_view, SIGNAL(clicked()), this, SLOT(editScales()));

  // 20th row.
  lower_bound_lbl = new QLabel("Lower bound:", this);
  grid->addWidget(lower_bound_lbl, 19, 0);
  lower = new QLineEdit(this);
  grid->addWidget(lower, 19, 1);
  upper_bound_lbl = new QLabel("Upper bound:", this);
  grid->addWidget(upper_bound_lbl, 19, 2);
  upper = new QLineEdit(this);
  grid->addWidget(upper, 19, 3);
  /* In this dialog window, when the user changes anything the
     'dialogEdited(...)' slot is called. */
  connect(lower, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));
  connect(upper, SIGNAL(textChanged(const QString &)),
	  this, SLOT(dialogEdited(const QString &)));

  // A section separator is put on the 21st row.
  QGroupBox * separator_gb = new QGroupBox(this);
  separator_gb->setPaletteBackgroundColor(Qt::gray);
  separator_gb->setMinimumHeight(15);
  separator_gb->setMaximumHeight(15);
  grid->addMultiCellWidget(separator_gb, 20, 20, 0, 3);

  // 22nd row.
  save_pb = new QPushButton("Save", this);
  grid->addWidget(save_pb, 21, 0);
  save_as = new QPushButton("Save as", this);
  grid->addWidget(save_as, 21, 1);
  close_pb = new QPushButton("Close", this);
  grid->addWidget(close_pb, 21, 3);
  // The buttons are connected to functions.
  connect(save_pb, SIGNAL(clicked()), this, SLOT(save()));
  connect(save_as, SIGNAL(clicked()), this, SLOT(saveAs()));
  connect(close_pb, SIGNAL(clicked()), this, SLOT(closeDialog()));

  // The path of the description file is shown at the top of the window.
  if (file == NULL)
    // No description file was given by the user.
    setCaption("Problem description");
  else
    // The path is shown.
    setCaption(file);
    
  // The Description object is created.
  description = new Description(file);

  // We tell the application to resize itself.
  resize(minimumSizeHint());
}

// Destructor.
DescriptionWindow::~DescriptionWindow()
{
  // The Description object is destroyed.
  if (description != NULL)
    delete description;
  // The TextEditor objects are destroyed.
  if (bounds_edit != NULL)
    delete bounds_edit;
  if (general_edit != NULL)
    delete general_edit;
  if (scales_edit != NULL)
    delete scales_edit;
  if (start_edit != NULL)
    delete start_edit;
}

// This method asks 'Description' for the 'surrogate_use' variable.
bool DescriptionWindow::getSurrogateUse()
{
  return description->getSurrogateUse();
}

/* This method asks the description object if the problem is ready to be
   solved. */
bool DescriptionWindow::readyToSolve()
{
  return description->readyToSolve();
}

// This method asks 'Description' for the problem name.
char * DescriptionWindow::getName()
{
  return description->getName();
}

// This method asks 'Description' for the path of the problem file.
char * DescriptionWindow::getProblemFile()
{
  return description->getProblemFile();
}

// This method asks 'Description' for the path of the results file.
char * DescriptionWindow::getResultsFile()
{
  return description->getResultsFile();
}

// This method asks 'Description' for the problem dimension.
unsigned int DescriptionWindow::getDimension()
{
  return description->getDimension();
}

// This method asks 'Description' for the number of general constraints.
unsigned int DescriptionWindow::getGeneralConstraintsNumber()
{
  return description->getGeneralConstraintsNumber();
}

/* This method is called when the user wants to solve the problem. The problem
   description is captured and sent to the 'Description' object. */
void DescriptionWindow::capture(Solver & controlbox)
{
  // Then we tell 'Description' to send its attributes to 'controlbox'.
  description->capture(controlbox);
}

// This method shows the problem description on the window.
void DescriptionWindow::load(const char * file)
{
  // The user might want to load a new problem file.
  if (file != NULL)
    description->loadProblemFile(file);

  // The description window is updated.
  bool bounds_use = description->getBoundsUse();
  bool cache_use = description->getCacheUse();
  bool surrogate_use = description->getSurrogateUse();
  int scaling_choice = description->getScalingChoice();
  unsigned int gen_cons_nb = description->getGeneralConstraintsNumber();
  QString temp;
  // First we show the problem name.
  problem_name->setText(description->getName());
  // We show the problem dimension.
  temp = temp.setNum(description->getDimension());
  dimension->setText(temp);
  // We show the user's scaling choice.
  scaling_cb->setCurrentItem(scaling_choice);
  if (scaling_choice == 2)
    // This is the user's path for the scaling vector.
    scaling_vector->setText(description->getScales());
  else
    // There's no user-defined scaling vector.
    scaling_vector->setText("N/A");
  if (scaling_choice == 3)
    {
      temp = temp.setNum(description->getLowerScale());
      lower->setText(temp);
      temp = temp.setNum(description->getUpperScale());
      upper->setText(temp);
    }
  // The checkbox is checked if the problem uses the caches.
  cache_cb->setChecked(cache_use);
  // If the problem uses the caches...
  if (cache_use)
    // ...we show the permanent cache file.
    cache->setText(description->getCache());
  else
    cache->setText("No cache");

  // We show the number of general constraints.
  temp = temp.setNum(gen_cons_nb);
  gen_cons->setText(temp);
  // The checkbox is checked if the problem has bounds constraints.
  bounds_cb->setChecked(bounds_use);
  // If the problem has bounds constraints...
  if (bounds_use)
    // ...we show the the bounds file.
    bounds->setText(description->getConstraints());
  else
    bounds->setText("No bound constraints");

  // We show the problem directory.
  directory->setText(description->getDirectory());
  // We show the starting point file.
  start_pt->setText(description->getStartingPoint());
  // We show the results file.
  results->setText(description->getResultsFile());

  // We show a boolean, telling us if there's a surrogate black box.
  surrogate_cb->setChecked(surrogate_use);
  // If the problem uses black boxes...
  black_box->setChecked(description->getBlackBoxUse());
  if (black_box->isChecked())
    {
      // ...we show the input file.
      input->setText(description->getInput());
      // The truth executable is shown.
      truth->setText(description->getTruth());
      if (surrogate_use)
	// The surrogate executable is shown.
	surrogate->setText(description->getSurrogate());
      else
	surrogate->setText("No surrogate");
      if (gen_cons_nb > 0)
	// The general constraints file is shown.
	general->setText(description->getGeneralConstraints());
      else
	general->setText("No general constraints");
    }

  /* We call these methods so that the widgets are enabled or disabled
     properly. */
  blackBoxEvent(true);
  boundsEvent();
  cacheEvent();
  scalingEvent(scaling_cb->currentItem());

  // This boolean variable starts out false.
  dialog_edited = false;
  // The object is shown.
  show();
}

/* This method is called when the user wants to create a new problem
   description. */
void DescriptionWindow::newDescription()
{
  // The Description object is destroyed and recreated.
  delete description;
  description = new Description();
  /* The problem description is loaded in the window. Since there's currently
     no description file, the window's fields will be blank. */
  load();
}

// Private slots.
// --------------

/* This method is called whenever there's an event with the black box check
   box. */
void DescriptionWindow::blackBoxEvent(bool)
{
  // Is the check box checked?
  bool black_box_use = black_box->isChecked();
  // If it's unchecked, black boxes are not used in the problem.
  if (!black_box_use)
    {
      input->setText("NA");
      truth->setText("NA");
      surrogate->setText("NA");
      general->setText("NA");
    }
  /* The editors and buttons linked to the truth black box and input file are
     enabled or disabled, according to the check box. */
  truth->setEnabled(black_box_use);
  truth_enter->setEnabled(black_box_use);
  input->setEnabled(black_box_use);
  input_enter->setEnabled(black_box_use);
  /* These function calls will alert the widgets linked to the surrogate and
     general constraints that there's a new event. */
  surrogateEvent();
  genConsEvent("");
}

/* This method determines if the 'bounds_view' button is enabled or not,
   according to the 'str' string. */
void DescriptionWindow::boundsEdited(const QString & str)
{
  if ((str == "") || (str == "No bound constraints"))
    bounds_view->setDisabled(true);
  else
    bounds_view->setEnabled(true);
}

// This method is called whenever there's an event with the bounds check box.
void DescriptionWindow::boundsEvent()
{
  // Is the check box checked?
  bool bounds_use = bounds_cb->isChecked();
  // If it's unchecked, there are no bounds in the problem.
  if (!bounds_use)
    bounds->setText("No bound constraints");
  /* The text editor and the buttons linked to the bounds are enabled or
     disabled, according to the check box. */
  bounds->setEnabled(bounds_use);
  bounds_enter->setEnabled(bounds_use);
  bounds_view->setEnabled(bounds_use);
  /* The 'boundsEdited(...)' slot must be called, to make sure that
     'bounds_view' is properly enabled. */
  boundsEdited(bounds->text());
}

// This method is called whenever there's an event with the cache check box.
void DescriptionWindow::cacheEvent()
{
  // Is the check box checked?
  bool cache_use = cache_cb->isChecked();
  // If it's unchecked, the problem doesn't use the caches.
  if (!cache_use)
    cache->setText("No cache file");
  /* The text editor and the button linked to the cache are enabled or
     disabled, according to the check box. */
  cache->setEnabled(cache_use);
  cache_enter->setEnabled(cache_use);
}

// This method is called whenever the user wants to close the dialog window.
void DescriptionWindow::closeDialog()
{
  if (dialog_edited)
    {
      switch(QMessageBox::warning(this, "NOMAD",
				  "The problem dialog has been edited.\n"
				  "Do you want to save it?",
				  QMessageBox::Yes, QMessageBox::No,
				  QMessageBox::Cancel))
	{
	  case QMessageBox::Yes : save();
	                          close();
				  break;
	  case QMessageBox::No : close();
			         break;
	  case QMessageBox::Cancel : break;
	}
    }
  else   // Everything is ok: the dialog window can be closed.
    close();
}

/* This method is called whenever the user edits the problem dialog (by
   clicking on a button). */
void DescriptionWindow::dialogEdited()
{
  dialog_edited = true;
}

/* This method is called whenever the user edits the problem dialog (by
   toggling a check box on/off). */
void DescriptionWindow::dialogEdited(bool)
{
  dialog_edited = true;
}

/* This method is called whenever the user edits the problem dialog (by
   changing the text in a QLineEdit object). */
void DescriptionWindow::dialogEdited(const QString & str)
{
  dialog_edited = true;
  bool enable_pb = (str == "") ? false : true;
  /* If the string 'str' is empty, the buttons are disabled, otherwise they are
     enabled. */
  close_pb->setEnabled(enable_pb);
  save_pb->setEnabled(enable_pb);
  save_as->setEnabled(enable_pb);
}

// This method allows the user to edit the bound constraints.
void DescriptionWindow::editBounds()
{
  const char * constraints_file = bounds->text().latin1();
  // The text editor is created.
  if (bounds_edit == NULL)
    bounds_edit = new TextEditor(constraints_file);
  // The constraints file is loaded in the text editor.
  bounds_edit->load(constraints_file);
  // The text editor is shown.
  bounds_edit->show();
}

// This method allows the user to edit the general constraints file.
void DescriptionWindow::editGenCons()
{
  const char * gen_cons_file = general->text().latin1();
  // The text editor is created.
  if (general_edit == NULL)
    general_edit = new TextEditor(gen_cons_file);
  // The general constraints file is loaded in the text editor.
  general_edit->load(gen_cons_file);
  // The text editor is shown.
  general_edit->show();
}

// This method allows the user to edit the scales file.
void DescriptionWindow::editScales()
{
  const char * scaling_file = scaling_vector->text().latin1();
  // The text editor is created.
  if (scales_edit == NULL)
    scales_edit = new TextEditor(scaling_file);
  // The scaling file is loaded in the text editor.
  scales_edit->load(scaling_file);
  // The text editor is shown.
  scales_edit->show();
}

// The user can edit the starting point in this method.
void DescriptionWindow::editStartingPoint()
{
  const char * starting_point_file = start_pt->text().latin1();
  // The text editor is created.
  if (start_edit == NULL)
    start_edit = new TextEditor(starting_point_file);
  // The starting point file is loaded in the text editor.
  start_edit->load(starting_point_file);
  // The text editor is shown.
  start_edit->show();
}

void DescriptionWindow::enterBoundConstraints()
{
  QString str_bounds = QFileDialog::getSaveFileName(QString::null,
						    QString::null, this, 0,
					       "Enter bound constraints file");
  if (str_bounds != NULL)
    bounds->setText(str_bounds);
}

void DescriptionWindow::enterCacheFile()
{
  QString str_cache = QFileDialog::getSaveFileName(QString::null,
						   QString::null, this, 0,
						   "Enter cache file");
  if (str_cache != NULL)
    cache->setText(str_cache);
}

void DescriptionWindow::enterGeneralConstraints()
{
  QString str_gen = QFileDialog::getSaveFileName(QString::null, QString::null,
						 this, 0,
					     "Enter general constraints file");
  if (str_gen != NULL)
    general->setText(str_gen);
}

void DescriptionWindow::enterInputFile()
{
  QString str_in = QFileDialog::getSaveFileName(QString::null, QString::null,
						this, 0, "Enter input file");
  if (str_in != NULL)
    input->setText(str_in);
}

void DescriptionWindow::enterProblemDirectory()
{
  QString str_dir = QFileDialog::getExistingDirectory(QString::null, this, 0,
					      "Enter problem directory", true);
  if (str_dir != NULL)
    directory->setText(str_dir);
}

void DescriptionWindow::enterResultsFile()
{
  QString str_out = QFileDialog::getSaveFileName(QString::null, QString::null,
						 this, 0,"Enter results file");
  if (str_out != NULL)
    results->setText(str_out);
}

void DescriptionWindow::enterScales()
{
  QString str_scales = QFileDialog::getSaveFileName(QString::null,
						    QString::null, this, 0,
						    "Enter scales file");
  if (str_scales != NULL)
    scaling_vector->setText(str_scales);
}

void DescriptionWindow::enterStartingPoint()
{
  QString str_point = QFileDialog::getSaveFileName(QString::null,
						   QString::null, this, 0,
						  "Enter starting point file");
  if (str_point != NULL)
    start_pt->setText(str_point);
}

void DescriptionWindow::enterSurrogateExecutable()
{
  QString str_surr = QFileDialog::getOpenFileName(QString::null,
						  QString::null, this, 0,
					    "Enter surrogate executable file");
  if (str_surr != NULL)
    surrogate->setText(str_surr);
}

void DescriptionWindow::enterTruthExecutable()
{
  QString str_truth = QFileDialog::getOpenFileName(QString::null,
						   QString::null, this, 0,
						"Enter truth executable file");
  if (str_truth != NULL)
    truth->setText(str_truth);
}

/* This method determines if the 'general_view' button is enabled or not,
   according to the 'str' string. */
void DescriptionWindow::genConsEdited(const QString & str)
{
  if ((str == "") || (str == "No general constraints") || (str == "NA"))
    general_view->setDisabled(true);
  else
    general_view->setEnabled(true);
}

/* This method is called whenever there's an event with the black box check
   box or general constraints text editor. */
void DescriptionWindow::genConsEvent(const QString &)
{
  // Is the black box check box checked?
  bool black_box_use = black_box->isChecked();
  // The number of general constraints is extracted from the text editor.
  unsigned int gen_cons_nb;
  if (gen_cons->text().isEmpty())
    gen_cons_nb = 0;
  else
    gen_cons_nb = gen_cons->text().toUInt();
  // The problem doesn't have general constraints in this case.
  if (gen_cons_nb == 0)
    general->setText("No general constraints");
  /* The text editor and the button linked to the general constraints are
     enabled or disabled. */
  general->setEnabled((gen_cons_nb > 0) && black_box_use);
  general_enter->setEnabled((gen_cons_nb > 0) && black_box_use);
  general_view->setEnabled((gen_cons_nb > 0) && black_box_use);
  /* The 'genConsEdited(...)' slot must be called, to be sure that
     'general_view' is properly enabled. */
  genConsEdited(general->text());
}

/* This method gathers all data from the description window, and sends it to
   'Description'. */
void DescriptionWindow::save()
{
  // The 'Description' object gives us the path of the description file.
  char * problem_file = description->getProblemFile();
  /* If there's no 'problem_file' string, we don't know where to save the
     problem data: we call 'saveAs()'. */
  if (problem_file == NULL)
    {
      saveAs();
      return;
    }
  /* The text editor's data is gathered and sent to 'Description' through these
     function calls. */
  description->enterProblem((problem_name->text()).latin1(),
			    (dimension->text()).toUInt(),
			    (start_pt->text()).latin1());
  description->enterFiles((directory->text()).latin1(),
			  (input->text()).latin1(),
			  (results->text()).latin1());
  description->enterOptions(cache_cb->isChecked(), cache->text().latin1(),
			    surrogate_cb->isChecked());
  description->enterConstraints(bounds_cb->isChecked(),
				(bounds->text()).latin1(),
				(gen_cons->text()).toUInt());
  description->enterBlackBoxes(black_box->isChecked(),
			       (truth->text()).latin1(),
			       (general->text()).latin1(),
			       (surrogate->text()).latin1());
  description->enterScales(scaling_cb->currentItem(),
			   scaling_vector->text().latin1(),
			   lower->text().toDouble(), upper->text().toDouble());
  // All the attributes are set: it's time to write them in the problem file.
  description->write();
  // The boolean is reset to 'false'.
  dialog_edited = false;
}

/* This method lets the user chooses the path where the problem description
   will be saved. */
void DescriptionWindow::saveAs()
{
  // The path is chosen in this file dialog.
  QString fn = QFileDialog::getSaveFileName(QString::null,
					    QString::null, this);
  // If the user chose a path...
  if (!fn.isEmpty())
    {
      const char * problem_file = fn.latin1();
      setCaption(problem_file);
      // ... it is sent to 'Description'.
      description->setProblemFile(problem_file);
      // This function gathers the data.
      save();
    }
}

/* This method determines if the 'scales_view' button is enabled or not,
   according to the 'str' string. */
void DescriptionWindow::scalesEdited(const QString & str)
{
  if (str == "")
    scales_view->setDisabled(true);
  else
    scales_view->setEnabled(true);
}

// This method is called whenever there's an event with the scaling combo box.
void DescriptionWindow::scalingEvent(int index)
{
  dialog_edited = true;
  /* If index is equal to '2', the scaling is done according to a vector
     provided by the user, and these widgets are enabled. */
  bool user_defined = (index == 2) ? true : false;
  user_vector_lbl->setEnabled(user_defined);
  scaling_vector->setEnabled(user_defined);
  scales_enter->setEnabled(user_defined);
  scales_view->setEnabled(user_defined);
  /* If 'index' is equal to '3', the scaling is done with user provided bounds,
     and these widgets are enabled. */
  user_defined = (index == 3) ? true : false;
  lower_bound_lbl->setEnabled(user_defined);
  upper_bound_lbl->setEnabled(user_defined);
  lower->setEnabled(user_defined);
  upper->setEnabled(user_defined);
}

/* This method determines if the 'start_view' button is enabled or not,
   according to the 'str' string. */
void DescriptionWindow::startingPointEdited(const QString & str)
{
  if (str == "")
    start_view->setDisabled(true);
  else
    start_view->setEnabled(true);
}

/* This method is called whenever there's an event with the black box or
   surrogate check boxes. */
void DescriptionWindow::surrogateEvent()
{
  // Is the surrogate check box checked?
  bool surrogate_use = surrogate_cb->isChecked();
  // Is the black box check box checked?
  bool black_box_use = black_box->isChecked();
  /* If the surrogate check box is unchecked, the problem doesn't use a
     surrogate. */
  if (!surrogate_use)
    surrogate->setText("No surrogate");
  /* The text editor and the button linked to the surrogate are enabled or
     disabled, according to both check boxes. */
  surrogate->setEnabled(surrogate_use && black_box_use);
  surrogate_enter->setEnabled(surrogate_use && black_box_use);
}

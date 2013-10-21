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

// parameterswindow.cpp -- class methods for ParametersWindow.

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
#include "parameters.h"
#include "parameterswindow.h"
#include "texteditor.h"

// Private methods:
// ----------------

// This method checks that the parameters window is properly displayed.
void ParametersWindow::checkDisplay(const bool surr_use,
				    const unsigned int gen_cons_nb)
{
  // This method 
  bool gen_cons_use = (gen_cons_nb > 0) ? true : false;
  filter_box->setChecked(gen_cons_use);
  filterBoxEvent(gen_cons_use);
  initialChanged(initial->currentItem());
  iterativeChanged(iterative->currentItem());
  pollDirsChanged(poll_dirs->currentItem());
  surrogate_box->setChecked(surr_use);
  surrogateBoxEvent(surr_use);
}

// Public methods:
// ---------------

// Class constructor.
ParametersWindow::ParametersWindow(QWidget * parent, const bool surr_use,
				   const unsigned int gen_cons_nb,
				   const char * file, const char * name)
  : QWidget(parent, name)
{
  // A 1x1 grid is created. Its parent is "this" object (ParametersWindow).
  QGridLayout * grid = new QGridLayout(this, 1, 1, 2, 2);

  /*******************/
  /* Mesh parameters */
  /*******************/

  QGroupBox * mesh_box = new QGroupBox("Mesh parameters", this);
  mesh_box->setPaletteBackgroundColor(Qt::gray);
  mesh_box->setAlignment(Qt::AlignHCenter);
  mesh_box->setFlat(true);
  mesh_box->setMinimumHeight(20);
  mesh_box->setMaximumHeight(20);
  grid->addMultiCellWidget(mesh_box, 0, 0, 0, 3);

  // We put some objects on the grid: Second row.
  grid->addWidget(new QLabel("Initial poll\nsize", this), 1, 0);
  poll_size = new QLineEdit("1.0", this);
  poll_size->setValidator(new QDoubleValidator(0.0, 1000.0, 16, poll_size));
  grid->addWidget(poll_size, 1, 1);
  grid->addWidget(new QLabel("Coarsening\nexponent", this), 1, 2);
  coarsen = new QLineEdit("1", this);
  coarsen->setValidator(new QIntValidator(1, 10, coarsen));
  grid->addWidget(coarsen, 1, 3);

  // Third row.
  grid->addWidget(new QLabel("Poll size\nbasis", this), 2, 0);
  poll_basis = new QLineEdit("2.0", this);
  poll_basis->setValidator(new QDoubleValidator(1.0, 100.0, 16, poll_basis));
  grid->addWidget(poll_basis, 2, 1);
  grid->addWidget(new QLabel("Refining\nexponent", this), 2, 2);
  refine = new QLineEdit("-1", this);
  refine->setValidator(new QIntValidator(-10, -1, refine));
  grid->addWidget(refine, 2, 3);

  // Fourth row.
  grid->addWidget(new QLabel("Max poll size", this), 3, 0);
  max_poll_size = new QLineEdit("100000", this);
  grid->addWidget(max_poll_size, 3, 1);

  /* The mesh QLineEdit objects are connected to the "enable(...)" slot,
     which observes if the data entry fields are completed. When they
     are, the "Solve" button is enabled. */
  connect(poll_size, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable(const QString &)));
  connect(poll_basis, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable(const QString &)));
  connect(coarsen, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable(const QString &)));
  connect(refine, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable(const QString &)));
  connect(max_poll_size, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable(const QString &)));

  /*******************/
  /* Poll parameters */
  /*******************/

  QGroupBox * poll_box = new QGroupBox("Poll parameters", this);
  poll_box->setPaletteBackgroundColor(Qt::gray);
  poll_box->setAlignment(Qt::AlignHCenter);
  poll_box->setFlat(true);
  poll_box->setMinimumHeight(20);
  poll_box->setMaximumHeight(20);
  grid->addMultiCellWidget(poll_box, 4, 4, 0, 3);

  // 6th row.
  grid->addWidget(new QLabel("Poll order", this), 5, 0);
  poll_order = new QComboBox(FALSE, this);
  poll_order->insertItem("Dynamic");
  poll_order->insertItem("Fixed");
  grid->addWidget(poll_order, 5, 1);
  grid->addWidget(new QLabel("Poll type", this), 5, 2);
  poll_type = new QComboBox(FALSE, this);
  poll_type->insertItem("Opportunistic");
  poll_type->insertItem("Complete");
  grid->addWidget(poll_type, 5, 3);

  // 7th row.
  grid->addWidget(new QLabel("Poll directions", this), 6, 0);
  poll_dirs = new QComboBox(FALSE, this);
  poll_dirs->insertItem("GPS - 2*n");
  poll_dirs->insertItem("GPS - n+1");
  poll_dirs->insertItem("GPS - uniform");
  poll_dirs->insertItem("MADS - 2*n");
  poll_dirs->insertItem("MADS - n+1");
  grid->addWidget(poll_dirs, 6, 1);
  grid->addWidget(new QLabel("Random seed:", this), 6, 2);
  random_seed = new QLineEdit("1", this);
  random_seed->setValidator(new QIntValidator(0, 1000, random_seed));
  grid->addWidget(random_seed, 6, 3);

  /*********************/
  /* Search parameters */
  /*********************/

  QGroupBox * search_box = new QGroupBox("Search parameters", this);
  search_box->setPaletteBackgroundColor(Qt::gray);
  search_box->setAlignment(Qt::AlignHCenter);
  search_box->setFlat(true);
  search_box->setMinimumHeight(20);
  search_box->setMaximumHeight(20);
  grid->addMultiCellWidget(search_box, 7, 7, 0, 3);

  // 9th row.
  grid->addWidget(new QLabel("Initial search", this), 8, 0);
  initial = new QComboBox(false, this);
  initial->insertItem("No search");
  initial->insertItem("Random");
  initial->insertItem("Latin hypercube");
  grid->addWidget(initial, 8, 1);
  // The 'initial' combo box is connected to the 'initialChanged(...)' slot.
  connect(initial, SIGNAL(activated(int)), this, SLOT(initialChanged(int)));
  grid->addWidget(new QLabel("Iterative search", this), 8, 2);
  iterative = new QComboBox(false, this);
  iterative->insertItem("No search");
  iterative->insertItem("Random");
  iterative->insertItem("Latin hypercube");
  grid->addWidget(iterative, 8, 3);
  /* The 'iterative' combo box is connected to the 'iterativeChanged(...)'
     slot. */
  connect(iterative, SIGNAL(activated(int)),
	  this, SLOT(iterativeChanged(int)));

  // 10th row.
  grid->addWidget(new QLabel("Initial type", this), 9, 0);
  init_complete = new QComboBox(false, this);
  init_complete->insertItem("Opportunistic");
  init_complete->insertItem("Complete");
  grid->addWidget(init_complete, 9, 1);
  grid->addWidget(new QLabel("Iterative type", this), 9, 2);
  iter_complete = new QComboBox(false, this);
  iter_complete->insertItem("Opportunistic");
  iter_complete->insertItem("Complete");
  grid->addWidget(iter_complete, 9, 3);

  // 11th row.
  grid->addWidget(new QLabel("Initial points", this), 10, 0);
  initial_pts = new QLineEdit("0", this);
  initial_pts->setValidator(new QIntValidator(initial_pts));
  grid->addWidget(initial_pts, 10, 1);
  grid->addWidget(new QLabel("Iterative points", this), 10, 2);
  iterative_pts = new QLineEdit("0", this);
  iterative_pts->setValidator(new QIntValidator(0, 1000, iterative_pts));
  grid->addWidget(iterative_pts, 10, 3);

  // 12th row.
  dynamic_label = new QLabel("Speculative search", this);
  grid->addWidget(dynamic_label, 11, 2);
  // This checkbox holds the boolean variable for MADS dynamic search.
  speculative = new QCheckBox("(MADS)", this);
  grid->addWidget(speculative, 11, 3);
  // This method is called whenever the 'poll_dirs' combo box is modified.
  connect(poll_dirs, SIGNAL(activated(int)), this, SLOT(pollDirsChanged(int)));

  /**************************/
  /* Termination parameters */
  /**************************/

  QGroupBox * terminator_box = new QGroupBox("Termination parameters", this);
  terminator_box->setPaletteBackgroundColor(Qt::gray);
  terminator_box->setAlignment(Qt::AlignHCenter);
  terminator_box->setFlat(true);
  terminator_box->setMinimumHeight(20);
  terminator_box->setMaximumHeight(20);
  grid->addMultiCellWidget(terminator_box, 12, 12, 0, 3);

  // 14th row.
  grid->addWidget(new QLabel("Poll size", this), 13, 0);
  ps_term = new QLineEdit("0", this, "0");
  ps_term->setValidator(new QDoubleValidator(-1.0, 1000.0, 16, ps_term));
  grid->addWidget(ps_term, 13, 1);
  grid->addWidget(new QLabel("Iterations", this), 13, 2);
  iter_term = new QLineEdit("0", this, "0");
  iter_term->setValidator(new QIntValidator(-1, 1000000, iter_term));
  grid->addWidget(iter_term, 13, 3);

  // 15th row.
  grid->addWidget(new QLabel("Truth evals", this), 14, 0);
  te_term = new QLineEdit("50", this, "50");
  te_term->setValidator(new QIntValidator(-1, 1000000, te_term));
  grid->addWidget(te_term, 14, 1);
  grid->addWidget(new QLabel("New truth evals", this), 14, 2);
  nt_term = new QLineEdit("0", this, "0");
  nt_term->setValidator(new QIntValidator(-1, 1000000, nt_term));
  grid->addWidget(nt_term, 14, 3);

  // 16th row.
  grid->addWidget(new QLabel("Consecutive\nfailures", this), 15, 0);
  li_term = new QLineEdit("0", this, "0");
  li_term->setValidator(new QIntValidator(-1, 10000, li_term));
  grid->addWidget(li_term, 15, 1);

  /*********************/
  /* Filter parameters */
  /*********************/

  filter_box = new QGroupBox("Filter parameters", this);
  filter_box->setPaletteBackgroundColor(Qt::gray);
  filter_box->setAlignment(Qt::AlignHCenter);
  filter_box->setFlat(true);
  filter_box->setMinimumHeight(20);
  filter_box->setMaximumHeight(20);
  filter_box->setCheckable(true);
  grid->addMultiCellWidget(filter_box, 16, 16, 0, 3);
  // The check box is connected to 'filterBoxEvent()'.
  connect(filter_box, SIGNAL(toggled(bool)), this, SLOT(filterBoxEvent(bool)));

  // 18th row.
  grid->addWidget(new QLabel("Hmax", this), 17, 0);
  hmax_relative = new QComboBox(false, this);
  hmax_relative->insertItem("Fixed");
  hmax_relative->insertItem("Relative");
  grid->addWidget(hmax_relative, 17, 1);
  grid->addWidget(new QLabel("Hmax value", this), 17, 2);
  hmax = new QLineEdit("100.0", this);
  hmax->setValidator(new QDoubleValidator(0.0, 1000000.0, 16, hmax));
  grid->addWidget(hmax, 17, 3);

  // 19th row.
  grid->addWidget(new QLabel("Filter norm", this), 18, 0);
  filter_norm = new QComboBox(false, this);
  filter_norm->insertItem("L-Infinity");
  filter_norm->insertItem("L-1");
  filter_norm->insertItem("L-2 squared");
  filter_norm->setCurrentItem(2);
  grid->addWidget(filter_norm, 18, 1);
  grid->addWidget(new QLabel("Hmin value", this), 18, 2);
  hmin = new QLineEdit("0.0001", this);
  hmin->setValidator(new QDoubleValidator(0.000001, 0.1, 16, hmin));
  grid->addWidget(hmin, 18, 3);

  /************************/
  /* Surrogate parameters */
  /************************/

  surrogate_box = new QGroupBox("Surrogate parameters", this);
  surrogate_box->setPaletteBackgroundColor(Qt::gray);
  surrogate_box->setAlignment(Qt::AlignHCenter);
  surrogate_box->setFlat(true);
  surrogate_box->setMinimumHeight(20);
  surrogate_box->setMaximumHeight(20);
  surrogate_box->setCheckable(true);
  grid->addMultiCellWidget(surrogate_box, 19, 19, 0, 3);
  // The check box is connected to 'surrogateBoxEvent()'.
  connect(surrogate_box, SIGNAL(toggled(bool)),
	  this, SLOT(surrogateBoxEvent(bool)));

  // 21st row.
  grid->addWidget(new QLabel("Surrogate\ntolerance", this), 20, 0);
  surr_tol = new QLineEdit("5.0", this);
  surr_tol->setValidator(new QDoubleValidator(1.0, 1000.0, 16, surr_tol));
  grid->addWidget(surr_tol, 20, 1);

  // A section separator is put on the 22nd row.
  QGroupBox * separator_gb = new QGroupBox(this);
  separator_gb->setPaletteBackgroundColor(Qt::gray);
  separator_gb->setMinimumHeight(15);
  separator_gb->setMaximumHeight(15);
  grid->addMultiCellWidget(separator_gb, 21, 21, 0, 3);

  /****************/
  /* Some buttons */
  /****************/

  // 23rd row.
  save_pb = new QPushButton("Save", this);
  grid->addWidget(save_pb, 22, 0);
  QPushButton * save_as = new QPushButton("Save as", this);
  grid->addWidget(save_as, 22, 1);
  // The buttons are connected to functions.
  connect(save_pb, SIGNAL(clicked()), this, SLOT(save()));
  connect(save_as, SIGNAL(clicked()), this, SLOT(saveAs()));

  // The Parameters object is created.
  parameters = new Parameters(file);
  if (file != NULL)
    // If the file is defined the parameters are loaded.
    load(surr_use, gen_cons_nb);
  else
    // This method will make sure that the window is displayed properly.
    checkDisplay(surr_use, gen_cons_nb);

  // We tell the application to resize itself.
  resize(minimumSizeHint());
}

// Destructor.
ParametersWindow::~ParametersWindow()
{
  // The Parameters object is destroyed.
  if (parameters != NULL)
    delete parameters;
}

// This method relays the 'readyToSolve()' call to Parameters.
bool ParametersWindow::readyToSolve()
{
  return parameters->readyToSolve();
}

// The 'captureParameters' signal is passed on to the Parameters object.
void ParametersWindow::capture(const unsigned int gen_cons_nb,
			       const bool surrogate_use, Solver & controlbox)
{
  parameters->capture(gen_cons_nb, surrogate_use, controlbox);
}

// This method resets all parameters to default values.
void ParametersWindow::defaults()
{
  /* We send the 'pollDirsChanged(0)' signal, which will reset the mesh
     parameters. */
  pollDirsChanged(0);

  // The poll parameters are reset
  poll_order->setCurrentItem(0);
  poll_type->setCurrentItem(0);
  poll_dirs->setCurrentItem(0);

  // The search parameters are reset.
  initial->setCurrentItem(0);
  // This signal will take care of the other variables.
  initialChanged(0);
  iterative->setCurrentItem(0);
  // This signal will take care of the other variables.
  iterativeChanged(0);

  // The termination parameters are reset.
  te_term->setText("50");
  iter_term->setText("0");
  li_term->setText("0");
  ps_term->setText("1e-4");
  nt_term->setText("0");

  // The filter parameters are reset if necessary.
  if (filter_box->isChecked())
    {
      hmax_relative->setCurrentItem(0);
      filter_norm->setCurrentItem(2);
      hmax->setText("5.0");
      hmin->setText("0.0001");
    }

  // The surrogate parameters are reset if necessary.
  if (surrogate_box->isChecked())
    surr_tol->setText("5.0");
}

// This method enables or disables the poll size text editor according to the
// boolean parameter. This is done to indicate to the user that he can't change
// the initial poll size before pressing the 'Continue run' button.
void ParametersWindow::enablePollSize(const bool poll_size_enabled)
{
  if (poll_size_enabled == true)
    poll_size->setEnabled(true);
  else
    poll_size->setDisabled(true);
}

// This method shows the parameters on the window.
void ParametersWindow::load(const bool surr_use,
			    const unsigned int gen_cons_nb, const char * file)
{
  // The user might want to load a new parameters file.
  if (file != 0)
    parameters->load(file);
  QString temp;
  // The mesh parameters are loaded.
  temp = temp.setNum(parameters->getCoarsen());
  coarsen->setText(temp);
  temp = temp.setNum(parameters->getPollBasis());
  poll_basis->setText(temp);
  temp = temp.setNum(parameters->getPollSize());
  poll_size->setText(temp);
  temp = temp.setNum(parameters->getMaxPollSize());
  max_poll_size->setText(temp);
  temp = temp.setNum(parameters->getRefine());
  refine->setText(temp);
  // The poll parameters are loaded.
  poll_dirs->setCurrentItem(parameters->getPollDirs());
  poll_order->setCurrentItem(parameters->getPollOrder());
  poll_type->setCurrentItem(parameters->getPollType());
  // The random seed is loaded.
  temp = temp.setNum(parameters->getSeed());
  random_seed->setText(temp);
  // The search parameters are loaded.
  initial->setCurrentItem(parameters->getInitialSearch());
  init_complete->setCurrentItem(parameters->getInitialComplete());
  iterative->setCurrentItem(parameters->getIterativeSearch());
  iter_complete->setCurrentItem(parameters->getIterativeComplete());
  temp = temp.setNum(parameters->getInitialPoints());
  initial_pts->setText(temp);
  temp = temp.setNum(parameters->getIterativePoints());
  iterative_pts->setText(temp);
  speculative->setChecked(parameters->getSpeculativeSearch());
  // The termination parameters are loaded.
  temp = temp.setNum(parameters->getTruthEvals());
  te_term->setText(temp);
  temp = temp.setNum(parameters->getIterations());
  iter_term->setText(temp);
  temp = temp.setNum(parameters->getConsecutiveFails());
  li_term->setText(temp);
  temp = temp.setNum(parameters->getPollSizeTerm());
  ps_term->setText(temp);
  temp = temp.setNum(parameters->getNewTruthEvals());
  nt_term->setText(temp);
  // The filter parameters are loaded.
  filter_norm->setCurrentItem(parameters->getFilterNorm());
  hmax_relative->setCurrentItem(parameters->getFilterRelative());
  temp = temp.setNum(parameters->getHmax());
  hmax->setText(temp);
  temp = temp.setNum(parameters->getHmin());
  hmin->setText(temp);
  // The surrogate parameters are loaded.
  temp = temp.setNum(parameters->getSurrogateTolerance());
  surr_tol->setText(temp);

  // This method will make sure that the window is displayed properly.
  checkDisplay(surr_use, gen_cons_nb);
}

// The text editor's data is gathered and sent to 'Parameters'.
void ParametersWindow::sendParameters()
{
  parameters->enterFilter(hmax_relative->currentItem(),
			  hmax->text().toDouble(), hmin->text().toDouble(),
			  filter_norm->currentItem());
  parameters->enterMesh(poll_size->text().toDouble(),
			poll_basis->text().toDouble(),
			max_poll_size->text().toDouble(),
			coarsen->text().toUInt(), refine->text().toInt());
  parameters->enterPoll(poll_order->currentItem(), poll_type->currentItem(),
			poll_dirs->currentItem());
  parameters->enterRandomSeed(random_seed->text().toUInt());
  parameters->enterSearch(initial->currentItem(), init_complete->currentItem(),
			  initial_pts->text().toUInt(),
			  iterative->currentItem(),
			  iter_complete->currentItem(),
			  iterative_pts->text().toUInt(),
			  speculative->isChecked());
  parameters->enterSurrogate(surr_tol->text().toDouble());
  parameters->enterTerminationCriteria(ps_term->text().toDouble(),
				       li_term->text().toInt(),
				       te_term->text().toInt(),
				       iter_term->text().toInt(),
				       nt_term->text().toInt());
}

//
void ParametersWindow::showParameters()
{
  parameters->show(filter_box->isChecked(), surrogate_box->isChecked());
}

// Private slots.
// --------------

// This method decides when to enable the 'Solve' button.
void ParametersWindow::enable(const QString &)
{
  // The "done" button is enabled if the text editors are not empty.
  if (!poll_size->text().isEmpty() &&
      !poll_basis->text().isEmpty() &&
      !coarsen->text().isEmpty() &&
      !refine->text().isEmpty() &&
      !max_poll_size->text().isEmpty())
    save_pb->setEnabled(true);
  else
    save_pb->setEnabled(false);
}

/* This method enables or disables widgets related to the Filter group box,
   according to 'checked'. */
void ParametersWindow::filterBoxEvent(bool checked)
{
  if (checked)
    {
      hmax_relative->setEnabled(true);
      filter_norm->setEnabled(true);
      hmin->setEnabled(true);
      hmax->setEnabled(true);
    }
  else
    {
      hmax_relative->setDisabled(true);
      filter_norm->setDisabled(true);
      hmin->setDisabled(true);
      hmax->setDisabled(true);
    }
}

// This method is called whenever the user changes the initial search.
void ParametersWindow::initialChanged(int index)
{
  // If there's no initial search, the parameters are reset.
  if (index == 0)
    {
      init_complete->setCurrentItem(0);
      initial_pts->setText("0");
      init_complete->setDisabled(true);
      initial_pts->setDisabled(true);
    }
  else
    {
      init_complete->setEnabled(true);
      initial_pts->setEnabled(true);
    }
}

// This method is called whenever the user changes the iterative search.
void ParametersWindow::iterativeChanged(int index)
{
  // If there's no iterative search, the parameters are reset.
  if (index == 0)
    {
      iter_complete->setCurrentItem(0);
      iterative_pts->setText("0");
      iter_complete->setDisabled(true);
      iterative_pts->setDisabled(true);
    }
  else
    {
      iter_complete->setEnabled(true);
      iterative_pts->setEnabled(true);
    }
}

// This method is called whenever the user makes a change to the poll
// directions' combo box.
void ParametersWindow::pollDirsChanged(int index)
{
  // The user chose a MADS set of poll directions: the speculative search is
  // enabled.
  if (index > 2)
    {
      dynamic_label->setEnabled(true);
      speculative->setEnabled(true);
    }
  else
    // The user chose a GPS set of poll directions: the speculative search is
    // disabled.
    {
      dynamic_label->setDisabled(true);
      speculative->setDisabled(true);
      speculative->setChecked(false);
     }
}

/* This method gathers the parameters from the GUI, and sends them to the
   Parameters object. */
void ParametersWindow::save()
{
  // The 'Descriptor' object gives us the path of the problem description file.
  char * parameters_file = parameters->getFile();
  /* If there's no 'problem_file' string, we don't know where to save the
     problem data: we call 'saveAs()'. */
  if (parameters_file == NULL)
    {
      saveAs();
      return;
    }
  // The window's contents are sent to Parameters.
  sendParameters();
  /* All the attributes are set: it's time to write them in the parameters
     file. */
  parameters->write();
}

/* This method lets the user chooses the path where the problem parameters
   will be saved. */
void ParametersWindow::saveAs()
{
  // The path is chosen in this file dialog.
  QString fn = QFileDialog::getSaveFileName(QString::null,
					    QString::null, this);
  // If the user chose a path...
  if (!fn.isEmpty())
    {
      const char * parameters_file = fn.latin1();
      setCaption(parameters_file);
      // ... it is sent to 'Parameters'.
      parameters->setFile(parameters_file);
      // This function gathers the data.
      save();
    }
}

/* This method enables or disables widgets related to the Surrogate group box,
   according to 'checked'. */
void ParametersWindow::surrogateBoxEvent(bool checked)
{
  if (checked)
    surr_tol->setEnabled(true);
  else
    surr_tol->setDisabled(true);
}

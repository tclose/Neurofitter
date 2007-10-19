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

/* parameterswindow.h -- class definition for ParametersWindow. It has public
   inheritance from the QWidget class. */

#ifndef PARAMETERSWINDOW_H
#define PARAMETERSWINDOW_H
#include <qstring.h>
#include <qwidget.h>
using namespace std;
#include "solver.h"

class Parameters;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

// ParametersWindow inherits publicly from QWidget.
class ParametersWindow: public QWidget
{
private:
  // A pointer to a Parameters object.
  Parameters * parameters;
  // This check box holds a search parameter.
  QCheckBox * speculative;
  // These combo boxes are for the poll parameters.
  QComboBox * poll_dirs, * poll_order, * poll_type,
    // These combo boxes hold search parameters.
    * initial, * init_complete, * iterative, * iter_complete,
    // These combo boxes hold some filter parameters.
    * filter_norm, * hmax_relative;
  //
  QGroupBox * filter_box, * surrogate_box;
  // These text editors are for the mesh parameters:
  //
  QLabel * dynamic_label;
  // These text editors display the mesh variables.
  QLineEdit * coarsen, * max_poll_size, * poll_basis, * poll_size, * refine,
    // This text editor displays the random seed.
    * random_seed,
    // These text editors display some search variables.
    * initial_pts, * iterative_pts,
    // These text editors display the termination criteria.
    * iter_term, * li_term, * nt_term, * ps_term, * te_term,
    // These text editors display some filter parameters.
    * hmax, * hmin,
    // This text editor displays a surrogate parameter.
    * surr_tol;
  // Some buttons.
  QPushButton * save_pb;

  /* The macro "Q_OBJECT" must be included in all classes that contain signals
     and/or slots. The moc compiler must generate a moc file from the .cpp
     file; this moc file will then be compiled normally. */
  Q_OBJECT

  // The class methods are described in "descriptorwindow.cpp".
  void checkDisplay(const bool surr_use, const unsigned int gen_cons_nb);

public:
  ParametersWindow(QWidget * parent, const bool surr_use,
		   const unsigned int gen_cons_nb, const char * file = NULL,
		   const char * name = 0);
  ~ParametersWindow();
  bool readyToSolve();
  void capture(const unsigned int gen_cons_nb, const bool surrogate_use,
	       Solver & controlbox);
  void defaults();
  void enablePollSize(const bool poll_size_enabled);
  void load(const bool surr_use, const unsigned int gen_cons_nb,
	    const char * file = NULL);
  void sendParameters();
  void showParameters();

private slots:
  void enable(const QString &);
  void filterBoxEvent(bool checked);
  void initialChanged(int index);
  void iterativeChanged(int index);
  void pollDirsChanged(int index);
  void save();
  void saveAs(); 
  void surrogateBoxEvent(bool checked);
};
#endif

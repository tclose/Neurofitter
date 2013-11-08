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

/* descriptionwindow.h -- class definition for DescriptionWindow. It has public
   inheritance from the QDialog class. */

#ifndef DESCRIPTIONWINDOW_H
#define DESCRIPTIONWINDOW_H
#include <qdialog.h>
#include <qstring.h>
using namespace std;
#include "solver.h"

class Description;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class TextEditor;

// DescriptionWindow inherits publicly from QDialog.
class DescriptionWindow: public QDialog
{
private:
  // A boolean to know if the dialog has been edited or not.
  bool dialog_edited;
  // A link to a Description object.
  Description * description;
  // Some check boxes.
  QCheckBox * bounds_cb, * cache_cb, * surrogate_cb;
  // A combo box describing the scaling options.
  QComboBox * scaling_cb;
  // Some group boxes (acting as separators between sections).
  QGroupBox * black_box;
  // These labels are enabled or disabled as needed.
  QLabel * lower_bound_lbl, * upper_bound_lbl, * user_vector_lbl;
  // These text editors contain the problem description.
  QLineEdit * bounds, * cache, * dimension, * directory, * gen_cons,
    * general, * input, * lower, * problem_name, * results, * scaling_vector,
    * start_pt, * surrogate, * truth, * upper;
  // Some buttons.
  QPushButton * bounds_enter, * bounds_view, * cache_enter, * close_pb,
    * general_enter, * general_view, * input_enter, * save_pb, * save_as,
    * scales_enter, * scales_view, * start_view, * surrogate_enter,
    * truth_enter;
  // Pointers to text editors.
  TextEditor * bounds_edit, * general_edit, * scales_edit, * start_edit;

  /* The macro "Q_OBJECT" must be included in all classes that contain signals
     and/or slots. The moc compiler must generate a moc file from the .cpp
     file; this moc file will then be compiled normally. */
  Q_OBJECT
  // The class methods are described in "descriptionwindow.cpp".
public:
  DescriptionWindow(QWidget * parent, const char * file = NULL,
		   const char * name = 0);
  ~DescriptionWindow();
  bool getSurrogateUse();
  bool readyToSolve();
  char * getName();
  char * getProblemFile();
  char * getResultsFile();
  unsigned int getDimension();
  unsigned int getGeneralConstraintsNumber();
  void capture(Solver & controlbox);
  void load(const char * file = NULL);
  void newDescription();

private slots:
  void blackBoxEvent(bool);
  void boundsEdited(const QString & str);
  void boundsEvent();
  void cacheEvent();
  void closeDialog();
  void dialogEdited();
  void dialogEdited(bool);
  void dialogEdited(const QString & str);
  void editBounds();
  void editGenCons();
  void editScales();
  void editStartingPoint();
  void enterBoundConstraints();
  void enterCacheFile();
  void enterGeneralConstraints();
  void enterInputFile();
  void enterProblemDirectory();
  void enterResultsFile();
  void enterScales();
  void enterStartingPoint();
  void enterSurrogateExecutable();
  void enterTruthExecutable();
  void genConsEdited(const QString & str);
  void genConsEvent(const QString &);
  void save();
  void saveAs();
  void scalesEdited(const QString & str);
  void scalingEvent(int index);
  void startingPointEdited(const QString & str);
  void surrogateEvent();
};
#endif

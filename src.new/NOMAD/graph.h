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

/* graph.h -- class definition for Graph. It has public inheritance from the
   QWidget class. */

#ifndef GRAPH_H
#define GRAPH_H
#include <qwidget.h>
#include <qpainter.h>
#include <qpoint.h>

// Graph publicly inherits QWidget.
class Graph : public QWidget
{
  /* The macro "Q_OBJECT" must be included in all classes that contain signals
     and/or slots. The moc compiler must generate a moc file from the .cpp
     file; this moc file will then be compiled normally. */
  Q_OBJECT
  // The class methods are described in "graph.cpp".
public:
  Graph(const char * file, const char * x_axis, const char * y_axis,
	bool filter = false, QWidget * parent = 0, const char * name = 0);
  ~Graph();
  void set_hmax(const double h);
  void drawAxis(QPainter * p);
  void setScales();
  void drawScales(QPainter * p);
  void drawGraph(QPainter * p);
  QPoint calculatePoint(double x, double y);
  int calculateX(double x);
  int calculateY(double y);
  QSizePolicy sizePolicy() const;
  void setOutput(bool output);

protected:
  void paintEvent(QPaintEvent *);

/* The Graph object has the following variables (they are used to calculate
   the graph's scale):
   - "nmax" is the total number of function evaluations needed to solve the
     problem.
   - "fzero" is the first truth value found while solving the problem.
   - "fmin" is the last truth value found while solving the problem. */
private:
  bool output_solution, filter_graph;
  double hmax, nmax, fmax, fmin;
  const char * file_name, * xstart, * ystart;
};
#endif

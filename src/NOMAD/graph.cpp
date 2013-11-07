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

// graph.cpp -- class methods for GRAPH.

#include <iostream>
#include <fstream>
#include <qstring.h>
#include <math.h>
using namespace std;
#include "graph.h"

// Class constructor.
Graph::Graph(const char * file, const char * x_axis, const char * y_axis,
	     bool filter, QWidget * parent, const char * name)
  : QWidget(parent, name)
{
  // The variables are initialized.
  file_name = file;
  output_solution = false;
  filter_graph = filter;
  hmax = nmax = fmax = fmin = 0.0;
  xstart = x_axis;
  ystart = y_axis;
  // The graph's background color will be white.
  setPalette(QPalette(white));
}

// Destructor.
Graph::~Graph()
{}

//
void Graph::set_hmax(const double h)
{
  hmax = h;
}

/* This virtual function is called by Qt whenever a widget needs to update
   itself (i.e. paint the widget's surface). */
void Graph::paintEvent(QPaintEvent *)
{
  // The QPainter class paints on paint devices.
  QPainter p(this);
  /* The QPainter::translate() function translates the coordinate system of
     the QPainter, i.e. moves it by an offset. Here we set the (0,0) point to
     the bottom left corner of the widget. The x and y directions remain
     unchanged, i.e. all the y coordinates inside the widget are now negative
     (see "The Coordinate System" in the web documentation for more information
     about Qt's coordinate system). */
  p.translate(0, rect().bottom());

  // This function draws the x and y axis.
  drawAxis(& p);
  // These three functions do most of the drawing job: they are described below
  if (output_solution)
    {
      setScales();
      drawScales(& p);
      drawGraph(& p);
    }
}

// This method draws the x and y axis.
void Graph::drawAxis(QPainter * p)
{
  QString xcoord(xstart);
  QString ycoord(ystart);
  // These functions return the height and width of the Graph object.
  int H = height();
  int W = width();

  // Text legends are written for each axis.
  p->drawText(width() - 275, -15, xcoord);
  // We rotate the axis to write the text vertically.
  p->rotate(-90);
  p->drawText(height() - 275, 15, ycoord);
  // We rotate the axis back to normal.
  p->rotate(90);

  // We draw the axes for x and y.
  p->drawLine(50, -50, 50, -(H - 20));
  p->drawLine(50, -50, W - 20, -50);
 
  // We draw small arrows at both axes' ends.
  p->drawLine(47, -(H - 23), 50, -(H - 20));
  p->drawLine(53, -(H - 23), 50, -(H - 20));
  p->drawLine(W - 23, -47, W - 20, -50);
  p->drawLine(W - 23, -53, W - 20, -50);
}

// This method sets values for the 3 variables nmax, fmax and fmin.
void Graph::setScales()
{
  // cout << "In 'Graph::set()'\n";
  double x, y;
  // We open the file where the points are written.
  ifstream fin(file_name);
  fin >> x;
  fin >> y;
  // 'fmax' is the first truth value written in the file.
  fmax = y;
  // We go to the end of the file.
  while (fin)
    {
      fin >> x;
      fin >> y;
    }
  // 'nmax' is the last number of function evaluations written in the file.
  nmax = x;
  // 'fmin' is the last truth value written in the file.
  fmin = y;
  //
  if (filter_graph)
    {
      fmax = fmax + 0.1 * (fmax - fmin);
      double n = 1.1 * nmax;
      if (hmax != 0.0)
	nmax = (n < hmax) ? n : hmax;
    }
  /* If (fmax == fmin), the y axis won't have a scale. In this case we give
     arbitrary values to fmax and fmin. */
  if (fmax == fmin)
    {
      // If they're equal to 0 we give them 1 and -1 values.
      if (fmax == 0)
	{
	  fmax = 1;
	  fmin = -1;
	}
      else
	{
	  // We separate them by 2 times "0.1 * abs(fmax)" (fmax == fmin).
	  fmax = fmax + 0.1 * abs(fmax);
	  fmin  = fmin  - 0.1 * abs(fmin);
	}
    }
  // We close the file.
  fin.close();
  // cout << "End of 'Graph::set()'\n";
}

// This method writes the x and y scales on the graph.
void Graph::drawScales(QPainter * p)
{
  // cout << "In 'Graph::scale(...)'\n";
  /* We put the scale on the x axis. The general idea is this: we'll draw a
     small number of lines on the x axis (between 1 and 10 lines), and for
     each line its number of function evaluations. */
  double lgx = floor(log10(nmax));
  /* The number of lines drawn is equal to the last number of function
     evaluations divided by the largest multiple of 10 it (nmax) contains. For
     example, if nmax is 389, log10(389) = 2, and 389/100 = 3 (it's an integer
     division). So we'd have to draw 3 lines on the x axis. */
  double nx = floor(nmax / pow(10, lgx));
  // In this loop the lines are drawn and the numbers put on the x axis.
  for (unsigned int i = 0; i <= nx; i++)
    {
      // We put the correct x coordinate in "scale".
      unsigned int scale = calculateX(i * pow(10, lgx));
      // The line is drawn.
      p->drawLine(scale, -53, scale, -47);
      // The number is written.
      p->drawText(scale - 5, -30, QString::number(i));
    }

  /* If "nx" is equal to one, we put other lines at 0.5 and 1.5, within the
     graph boundaries. */
  if (nx == 1)
    {
      /* We insert a line at '0.5'. It's in the graph boundaries, since we
	 already have 0 and 1. */
      int scale = calculateX(0.5 * pow(10, lgx));
      p->drawLine(scale, -53, scale, -47);
      p->drawText(scale - 5, -30, QString::number(0.5, 'g', 6));
      // We insert a line at '1.5'.
      scale = calculateX(1.5 * pow(10, lgx));
      // We have to check if '1.5' is still within the graph boundaries.
      if (scale <= (width() - 20))
	{
	// If it is, the line is inserted.
	  p->drawLine(scale, -53, scale, -47);
	  p->drawText(scale - 5, -30, QString::number(1.5, 'g', 6));
	}
    }

  /* Now the string "Number of function evaluations (*10e'lgx')" is written
     under the x axis. In the example above, 'lgx' is equal to 2. */
  QString xcoord(xstart);
  xcoord.append("(*10e");
  xcoord.append(QString::number(lgx));
  xcoord.append(")");
  p->drawText(width() - 275, -15, xcoord);

  // We put the scale on the y axis.
  double lgy, zero, min, reallgy;
  // "lgy" is the base 10 logarithm of the truth values' difference.
  lgy = floor(log10(fmax - fmin));
  // "reallgy" is calculated as "lgy", except it's a real number.
  reallgy = log10(fmax - fmin);
  /* If the truth values' difference is smaller than 1, we decrement "lgy", so
     it goes from 0 to -1, or -1 to -2, etc. */
  if ((reallgy -lgy < 0.4) && ((reallgy - lgy) != 0))
    lgy--;

  // The two variables, "zero" and "min" are calculated.
  zero = floor(fmax / pow(10, lgy));
  min = floor(fmin / pow(10, lgy));

  // In this loop the lines are drawn on the y axis, and the scales (numbers)
  // are written. At the bottom of the axis is 'min', and at the top is 'zero'.
  double l_value = min;
  while (l_value <= zero)
    {
      /* The "calulateY(...) method returns the correct y coordinate, which is
         put into "scale". */
      int scale = calculateY(l_value * pow(10, lgy));
      // If "scale" is within the graph boundaries in the output window...
      if ((scale <= -50) && (scale >= -(height() - 25)))
	{
	  // ... we draw the line and write the scale.
	  p->drawLine(47, scale, 53, scale);
	  p->drawText(25, scale, QString::number(l_value));
	}
      l_value += 1.0;
    }

  /* If there's only a difference of one between the truth values, we put
     other lines at .5 intervals, within the graph boundaries. */
  if (zero == (min + 1.0))
    {
      // The three y coordinates are calculated.
      int scalemin  = calculateY(min * pow(10, lgy));
      int scalezero = calculateY(zero * pow(10, lgy));
      int scale = (scalemin + scalezero) / 2;

      // The first line and scale are drawn between the truth values.
      p->drawLine(47, scale, 53, scale);
      double temp = min + zero;
      temp = temp / 2;
      p->drawText(25, scale, QString::number(temp, 'g', 6));

      /* The second line is drawn above "fmax", if it's still within the graph
	 boundaries. */
      scale = scalezero + (scalezero - scalemin) / 2;
      // If "scale" is not above the top of the graph ...
      if (scale >= -(height() - 25))
	{
	  // ... the line is drawn.
	  p->drawLine(47, scale, 53, scale);
	  // ... we calculate what number to write.
	  temp = zero - min;      
	  temp = temp / 2;
	  temp += zero;
	  // ... the number is drawn.
	  p->drawText(25, scale, QString::number(temp, 'g', 6));
	}

      /* The third line is drawn below "fmin", if it's still within the graph
	 boundaries. */
      scale = scalemin - (scalezero - scalemin) / 2;
      // If "scale" is not below the bottom of the graph...
      if (scale <= -50)
	{
	  // ... the line is drawn.
	  p->drawLine(47, scale, 53, scale);
	  // ... we calculate what number to write.
	  temp = -zero + min;      
	  temp = temp / 2;
	  temp += min;
	  // ... the number is drawn.
	  p->drawText(25, scale, QString::number(temp, 'g', 6));
	}
    }

  /* Now the string "Objective function value (*10e'lgx')" is written besides
     the y axis. */
  QString ycoord(ystart);
  ycoord.append("(*10e");
  ycoord.append(QString::number(lgy));
  ycoord.append(")");
  // We rotate the axis to write the text vertically.
  p->rotate(-90);
  p->drawText(height() - 275, 15, ycoord);
  // We rotate the axis back to normal.
  p->rotate(90);
  // cout << "End of 'Graph::scale(...)'\n";
}

// This method draws the actual graph line.
void Graph::drawGraph(QPainter * p)
{
  // cout << "In 'Graph::drawGraph(...)'\n";
  // The program outputs needed to draw the graph are in the file.
  ifstream fin(file_name);
  double x, y;
  QPoint p1, p2, middle, temp;
  // We read the first line.
  fin >> x;
  fin >> y;
  // cout << "x = " << x << ", y = " << y << "\n";
  // The "calculatePoint(...) method returns the correct x and y coordinates.
  p1 = calculatePoint(x, y);
  // cout << "p1: x = " << p1.x() << ", y = " << p1.y() << "\n";
  // The graph line will be drawn in blue.
  p->setPen(blue);

  //
  if (filter_graph)
    {
      temp = calculatePoint(x, fmax);
      // cout << "temp: x = " << temp.x() << ", y = " << temp.y() << "\n";
      p->drawLine(temp, p1);
    }
  // While we have an input ...
  while (fin)
    {
      // ... we read a line, recording the integer and double in x and y.
      fin >> x;
      fin >> y;
      // cout << "x = " << x << ", y = " << y << "\n";
      // If we have an input ...
      if (fin)
	{
	  // ... p2's coordinates are calculated.
	  p2 = calculatePoint(x, y);
	  // cout << "p2: x = " << p2.x() << ", y = " << p2.y() << "\n";
	  // ... we set the middle point's coordinates.
	  middle.setX(p2.x());
	  middle.setY(p1.y());
	  /* ... the graph line is drawn in two parts. This gives a staircase
	     look to the graph. */
	  p->drawLine(p1, middle);
	  p->drawLine(middle, p2);
	  /* ... in the next iteration, the first point will be the last point
	     of the last iteration. */
	  p1 = p2;
	}
    }
  //
  if ((filter_graph) && (hmax != 0.0))
    {
      if (nmax < hmax)
	{
	  p2 = calculatePoint(nmax, y);
	  p->drawLine(p1, p2);
	}
      else
	{
	  p2.setX(calculateX(hmax));
          p2.setY(-50);
	  middle.setX(p2.x());
	  middle.setY(p1.y());
	  p->drawLine(p1, middle);
	  p->drawLine(middle, p2);
	}
    }
  // We change back the pen color to black.
  p->setPen(black);
  // The file is closed.
  fin.close();
  // cout << "End of 'Graph::drawGraph(...)'\n";
}

/* This method, given a number of function evaluations x and a truth value y
   returns a QPoint object with the correct coordinates. */
QPoint Graph::calculatePoint(double x, double y)
{
  // "H" is the total vertical space available to plot the graph.
  int H = -(height() - 90);
  // "W" is the total horizontal space available to plot the graph.
  int W = width() - 80;
  int a, b;
  /* "a" is the exact x coordinate on the graph, calculated like this:
     'x / nmax' is the current number of function evaluations divided by the
                total number of function evaluations. It's a number between 0
	        and 1 (0 < (x/nmax) <= 1).
     '(x / nmax) * W' gives a coordinate in the horizontal space.
     '((x / nmax) * W) + 50' We add 50 because there's a margin of 50 to the
                             left of the graph. */
  a = (int)floor(x * W / nmax + 50);
  /* "b" is the exact y coordinate on the graph, calculated like this:
     'y - fmin' is the current truth value minus the last truth value.
     'fmax - fmin' is the initial truth value minus the last truth value.
     '(y - fmin) / (fmax - fmin)' gives a number between 0 and 1.
     '(y - fmin) / (fmax - fmin) * H' gives a coordinate in the vertical space
     '((y - fmin) / (fmax - fmin) * H) - 60' We substract 60 because there's a
                                    margin of 60 to the bottom of the graph. */
  b = (int)floor((y - fmin) * H / (fmax - fmin) - 60);
  return QPoint(a, b);
}

/* This method, given a number of function evaluations x, returns the correct
   horizontal coordinate. */
int Graph::calculateX(double x)
{
  int W = width() - 80;
  return (int)floor(x * W / nmax + 50);
}

// This method, given a truth value y, returns the correct vertical coordinate.
int Graph::calculateY(double y)
{
  int H = -(height() - 90);
  return (int)floor((y - fmin) * H / (fmax - fmin) - 60);
}

/* This virtual method returns the default layout behavior of the widget. It
   is used automatically when the object repaints itself. */
QSizePolicy Graph::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

// This method will redraw the graph if 'output' is equal to 'true'.
void Graph::setOutput(bool output)
{
  output_solution = output;
  repaint();
}

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

// filtergraph.cpp -- class methods for FilterGraph.

#include <iostream>
#include <qlayout.h>
using namespace std;
#include "filtergraph.h"
#include "graph.h"

// Public methods:
// ---------------

//
FilterGraph::FilterGraph(Filter * s1, GeneralConstraints * s2,
			 QWidget * parent, const char * name)
  : QMainWindow(parent, name, 0), Observer()
{
  //
  _subject_one = s1;
  _subject_one->attach(this);
  _subject_two = s2;
  _subject_two->attach(this);
  //
  graph = new Graph("LIBRARY/filter_graph.txt", "h(x) ", "f(x) ",
		    true, this, 0);
  setCaption("Filter graph");
  setCentralWidget(graph);
  resize(600,480);
  show();
}

//
FilterGraph::~FilterGraph()
{
  _subject_one->detach(this);
  _subject_two->detach(this);
}

//
void FilterGraph::update(Subject * theChangedSubject)
{
  if (_subject_one == theChangedSubject)
    {
      graph->setOutput(true);
      setActiveWindow();
      raise();
      setUpdatesEnabled(true);
    }
  else   // (_subject_two == theChangedSubject)
    graph->set_hmax(_subject_two->getHmax() );
}

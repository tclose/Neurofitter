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

/* filter.cpp -- Filter class methods. The filter is used when there are
   general constraints in the optimization problem. It stores AugmentedPoint
   objects according to a rule of dominance. */

#include <cmath>
#include <iostream>
using namespace std;
#include "augmentedpoint.h"
#include "defines.h"
#include "filter.h"

/* Private methods.
******************************************************************************/

/* This method does two things: First it finds the place where 'ap' could be
   inserted in the filter list; then if 'ap' meets certain conditions, it is
   inserted by calling the 'store(...)' method.
   - 'ap' is the Augmented Point to (possibly) insert in the filter list.
   - 'Fx' is the truth function value of 'ap'.
   - 'Hx' is the constraint violation function value of 'ap'. */
unsigned int Filter::insert(const AugmentedPoint & ap, const double Fx,
			    const double Hx)
{
  /* The variable 'pos' will indicate the insertion position of 'ap' in the
     filter list. */
  int pos = 0;
  /* The variable 'success' can have these values:
     - 0: Failure: 'ap' wasn't inserted in the filter list.
     - 1: Partial success: 'ap' was inserted in the middle or at the end of the
          filter list.
     - 2: Success: 'ap' was inserted at the start of the filter list. */
  unsigned int success = 0;
  // If the list is empty, the point is inserted.
  if (filter_list.empty())
    {
      if (display_factor >= 5)
	cout << "Insertion at the start: the filter list was empty.\n";
      store(ap);
      success = 2;
    }
  else
    {
      /* 'current_Hx' is the Hx value for the current point in the filter list.
	 'current_Fx' is the Fx value for the current point in the filter list.
	 'previous_Fx' is the Fx value for the previous point in the filter
	 list. */
      double current_Hx, current_Fx;
      // At the beginning 'previous_Fx' is set to a very large number.
      double previous_Fx = INFINIT;
      // The stopping criterion is handled inside the loop.
      for (current = filter_list.begin(); current <= filter_list.end();
	   current++)
	{
	  // Particular case: we're at the end of the filter list.
	  if (current == filter_list.end())
	    {
	      // We check only the previous point's truth value.
	      if (Fx < previous_Fx)
		{
		  if (display_factor >= 5)
		    cout << "Insertion at the end of the filter list.\n";
		  store(ap);
		  success = 1;
		  break;
		}
	    }
	  else   // General case.
	    {
	      // The current point's function values are captured.
	      current_Hx = (* current)->getHx();
	      current_Fx = (* current)->getOutput(0);
	      /* 'Hx' is compared to the current point's constraint violation
		 function value:
		 1- If 'Hx' is greater than 'current_Hx', nothing is done, we
		    go back to the top of the loop.
		 2- If 'Hx' is inferior to 'current_Hx', 'Fx' is compared to
		    the previous point's truth function value. If 'Fx' is
		    inferior it's not dominated and can be inserted.
		 3- If 'Hx' is equal to 'current_Hx', 'Fx' is compared to the
		    current point's truth function value. If 'Fx' is inferior
		    it's not dominated and can be inserted. */
	      if (((Hx < current_Hx) && (Fx < previous_Fx)) ||
		  ((Hx == current_Hx) && (Fx < current_Fx)))
		{
		  // 'ap' is inserted in the filter list.
		  if (display_factor >= 5)
		    {
		      if (pos == 0)
			cout << "Insertion at the start of the filter list.\n";
		      else
			cout << "Insertion in the middle of the filter list.\n";
		    }
		  store(ap, pos);
		  // Success!
		  if (pos == 0)
		    success = 2;
		  else   // Partial success.
		    success = 1;
		  /* We must check if the points in the rest of the list are
		     dominated (they have a superior truth function value) by
		     the newly inserted point. */
		  checkDominance(Fx, pos);
		  break;
		}
	      // We update 'previous_Fx'.
	      previous_Fx = current_Fx;
	    }
	  // The counter 'pos' is incremented.
	  pos++;
	}
    }
  return success;
}

/* This method is called after an augmented point is inserted in the filter. If
   the points that follow the inserted point have an higher truth value, they
   are dominated and thus erased from the list. */
void Filter::checkDominance(const double Fx, const int pos)
{
  int i;
  if (display_factor >= 5)
    cout << "Checking dominance, Fx = " << Fx << ", pos = " << pos << "\n";
  current = filter_list.begin();
  /* The 'current' iterator is advanced to the point that follows the new
     inserted point. */
  for (i = 0; i < pos + 1; i++)
    current++;
  /* We check all the points following the inserted point, as long as we're not
      at the end of the list (obviously) and their truth values are higher or
      equal to 'Fx' (the inserted point's truth value, received as parameter).
  */
  while ((current != filter_list.end()) && ((* current)->getOutput(0) >= Fx))
    {
      if (display_factor >= 5)
	{
	  cout << "Deleting ";
	  (* current)->show();
	}
      // The pointed object is deleted.
      delete (* current);
      /* The element is erased. The 'erase' function returns an iterator to the
	 element that had followed the erased one, that's how 'current'
	 advances to the next element. */
      current = filter_list.erase(current);
    }
}

// This method deletes the list of points from memory.
void Filter::cleanList()
{
  if (!filter_list.empty())
    {
      // The AugmentedPoint objects in 'filter_list' are destroyed.
      for (current = filter_list.begin(); current != filter_list.end();
	   current++)
	delete * current;
      // The memory for the pointers to AugmentedPoint objects is freed.
      filter_list.clear();
    }
}

// Outputs Filter's list of points.
void Filter::showList()
{
  cout << "Filter list: ";
  cout << filter_list.size() << " point(s), capacity = ";
  cout << filter_list.capacity() << "\n";
  if (filter_list.empty())
    cout << "List of points empty!\n";
  else
    {
      // We go through the list of points.
      for (current = filter_list.begin(); current != filter_list.end();
	   current++)
	// Each point is displayed.
	(* current)->show();
    }
}

// This method stores the AugmentedPoint 'ap' into the filter list.
void Filter::store(const AugmentedPoint & ap, const int pos)
{
  // A copy of 'ap' is made with the copy constructor.
  AugmentedPoint * temp = new AugmentedPoint(ap);
  //
  if (pos == -1)
    filter_list.push_back(temp);
  else
    {
      (* current) = filter_list.at(pos);
      /* The copy is inserted into 'filter_list', before the 'current'
	 iterator. */
      filter_list.insert(current, temp);
    }
}

/* Public methods:
******************************************************************************/

// Default constructor.
Filter::Filter()
  : Subject()
{
  display_factor = 0;
  current = filter_list.begin();
}

// Destructor.
Filter::~Filter()
{
  cleanList();
}

// This method returns the incumbent (AugmentedPoint object). The Filter
// incumbent is the least infeasible point.
const AugmentedPoint * Filter::getIncumbent()
{
  // The filter list isn't supposed to be empty if this method is called. This
  // is a check just to be thorough.
  if (filter_list.empty())
    {
      cerr << "Error: the method 'Filter::getIncumbent()' can't return the\n";
      cerr << "least infeasible point because the filter list is empty!\n";
      return 0;
    }
  else
    {
      // The least infeasible point is the first point in the filter list.
      current = filter_list.begin();
      return (* current);
    }
}

//
bool Filter::isEmpty()
{
  return filter_list.empty();
}

/* This method returns the least infeasible point, which is the first point in
   the filter list. */
double * Filter::getPollCenter()
{
  current = filter_list.begin();
  return (* current)->getPoint();
}

/* An AugmentedPoint object is received, if it meets certain critera it will be
   added to the filter list. */
unsigned int Filter::populate(AugmentedPoint & trial)
{
  double Fx, Hx;
  /* This variable will be returned, with these possible return values:
     - 0: The point wasn't added to the filter list.
     - 1: The point was added somewhere in the middle or at the end of the
          filter list.
     - 2: The point was added at the start of the filter list. It means that we
          have a new least infeasible point, which is one type of incumbent. */
  unsigned int success = 0;

  /* We get the truth and constraint violation function values of the trial
     point. */
  Fx = trial.getOutput(0);
  Hx = trial.getHx();
  /* The point is not inserted in the filter list in these cases:
     1- If 'Fx' equals 'INFINIT', the point is outside the bound
        constraints.
     2- If 'Hx' equals 'INFINIT', the point's constraint violation
        function value is greater than 'Hmax'.
     3- If 'Hx' equals 0, the point is feasible. */
  if ((Fx != INFINIT) && (Hx != INFINIT) && (Hx != 0.0))
    success = insert(trial, Fx, Hx);

  if (display_factor >= 5)
    {
      if (success > 0)
	showList();
    }
  /* The filter list is populated. This function call will update the filter
     graph. */
  // if (success)
  // writeInGraphFile();
  return success;
}

// This method sets the display factor.
void Filter::setDisplay(const unsigned int display)
{
  display_factor = display;
}

/* This method writes the points of the filter list in a file, to be read later
   when the filter graph is updated. */
void Filter::writeInGraphFile()
{
  // cout << "In 'Filter::writeInGraphFile()'\n";
  // The filter graph file is opened.
  ofstream fout("LIBRARY/filter_graph.txt");
  // We go through the list of points.
  for (current = filter_list.begin(); current != filter_list.end(); current++)
    // Each point's H(x) and f(x) is written in the filter graph file.
    fout << (* current)->getHx() << "  " << (* current)->getOutput(0) << "\n";
  // The filter graph file is closed.
  fout.close();
  // This Subject class method notifies the filter graph to update itself.
  notify();
}

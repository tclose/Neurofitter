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

// surrogatecontrol.cpp -- SurrogateControl class methods.

#include <iostream>
using namespace std;
#include "augmentedpoint.h"
#include "surrogatecontrol.h"

// Private methods.
// ****************************************************************************

//
int SurrogateControl::partition(std::vector<AugmentedPoint *> & unsorted_list,
				int m, int p)
{
  int i = m, j = p;
  double surr_i = 0.0, surr_j = 0.0;
  double surr_m = unsorted_list[m]->getLow();
  do
    {
      do 
	{
	  i++;
	  surr_i = unsorted_list[i]->getLow();
	}
      while (surr_i < surr_m);
      do
	{
	  j--;
	  surr_j = unsorted_list[j]->getLow();
	}
      while (surr_j > surr_m);
      if (i < j)
	swap(unsorted_list, i, j);
    }
  while (i < j);
  if (m != j)
    swap(unsorted_list, m, j);
  return j;
}

//
void SurrogateControl::quicksort(std::vector<AugmentedPoint *> & unsorted_list,
				 int p, int q)
{
  // If there are more than one element in the list.
  if (p < q)
    {
      /* Divide into two subproblems ('j' is the position of the partitioning
	 element). */
      int j = partition(unsorted_list, p, q + 1);
      // Solve the subproblems.
      quicksort(unsorted_list, p, j - 1);
      quicksort(unsorted_list, j + 1, q);
    }
}

//
void SurrogateControl::swap(std::vector<AugmentedPoint *> & unsorted_list,
			    int i, int j)
{
  AugmentedPoint * ap = new AugmentedPoint(* unsorted_list[i]);
  * unsorted_list[i] = * unsorted_list[j];
  * unsorted_list[j] = * ap;
  delete ap;
}

// Public methods.
// ****************************************************************************

//
SurrogateControl::SurrogateControl(const unsigned int dim,
				   const unsigned int truth_dim)
{
  display_factor = 0;
  surrogate = new Surrogate(dim, truth_dim);
  a_point = list_of_points.begin();
}

//
SurrogateControl::~SurrogateControl()
{
  if (surrogate != NULL)
    delete surrogate;
  // The old list is erased.
  cleanList();
}

// This method deletes the list of points from memory.
void SurrogateControl::cleanList()
{
  if (!list_of_points.empty() )
    {
      // The AugmentedPoint objects of the last iteration are destroyed.
      for (a_point = list_of_points.begin(); a_point != list_of_points.end();
	   a_point++)
	delete * a_point;
      list_of_points.clear();
    }
}

// This method cleans the surrogate data.
void SurrogateControl::cleanSurrogate()
{
  cleanList();
  surrogate->clean();
}

// The display factor is reset.
void SurrogateControl::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// Outputs the list of points.
void SurrogateControl::showList()
{
  cout << "List of points in Observer:\n";
  if (list_of_points.empty() )
    cout << "List of points empty!\n";
  else
    {
      // We traverse the list of points in this loop.
      for (a_point = list_of_points.begin();
	   a_point != list_of_points.end(); a_point++)
	(* a_point)->show();
    }
  cout << "\n";
}

//
void SurrogateControl::sortThisList(
				 std::vector<AugmentedPoint *> & unsorted_list)
{
  std::vector<AugmentedPoint *>::iterator temp;
  if (display_factor >= 5)
    {
      cout << "\nUnsorted list:\n**************\n";
      for (temp = unsorted_list.begin(); temp < unsorted_list.end(); temp++)
	(* temp)->show();
      cout << "\n";
    }
  // We create a point at the end of the list, for the quicksort algorithm.
  temp = unsorted_list.begin();
  unsorted_list.push_back(new AugmentedPoint((*temp)->getDimension(),
					     (*temp)->getPoint() ) );
  // This variable holds the number of elements in the list.
  int size = unsorted_list.size();
  quicksort(unsorted_list, 0, size - 1);
  // We destroy the point that we created at the end of the list.
  unsorted_list.erase(unsorted_list.end() - 1);
  if (display_factor >= 5)
    {
      cout << "\nSorted list:\n************\n";
      for (temp = unsorted_list.begin(); temp < unsorted_list.end(); temp++)
	(* temp)->show();
      cout << "\n";
    }
}

//
void SurrogateControl::store(const AugmentedPoint & ap)
{
  AugmentedPoint * temp = new AugmentedPoint(ap);
  list_of_points.push_back(temp);
}

//
void SurrogateControl::update(const AugmentedPoint * inc)
{
  store(* inc);
  if (display_factor >= 5)
    showList();
  surrogate->recalibrate(list_of_points);
}

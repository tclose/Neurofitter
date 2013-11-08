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

// mesh.cpp -- Mesh class methods. All methods of the class are called from the
// Solver object, which has direct access to Mesh.

#include <iostream>
#include <cmath>
using namespace std;
#include "mesh.h"

// Public methods.
// ---------------

// Default constructor. The Solver object creates Mesh at Start-up.
Mesh::Mesh()
{
  // These variables are set to defaults.
  initial = true;
  max_mesh_size = max_poll_size = 9.9e307;
  poll_basis = 2.0;
  mesh_size = poll_size = 1.0;
  coarsening_exponent = 1;
  refining_exponent = -1;
  display_factor = 0;
}

// Destructor.
Mesh::~Mesh()
{}

// This class interface method returns the value of 'poll_basis'.
double Mesh::getBasis()
{
  return poll_basis;
}

// This class interface method returns the value of 'mesh_size'.
double Mesh::getMeshSize()
{
  return mesh_size;
}

/* This method is called when there was a failure in the poll, i.e. all
   directions were polled without success. Then we have to refine the mesh.
   For example, for the mesh built with the default constructor, we have (after
   a failure in the poll): mesh_size = 1.0 * pow(2.0, -1) = 0.5   */
double Mesh::refine()
{
  mesh_size = mesh_size * pow(poll_basis, refining_exponent);
  return mesh_size;
}

// This method calculates the initial and maximum mesh sizes. The mesh size
// isn't equal to the poll size when the user chose MADS poll directions.
void Mesh::calculateMeshSizes(const unsigned int dimension,
			      const unsigned int choice)
{
  // In this case the user chose MADS 2*n directions.
  if (choice == 3)
    {
      // In the case where poll size is smaller than one, then the initial mesh
      // size will be even smaller.
      if ((initial) && (poll_size < 1.0))
	mesh_size = pow(poll_size, 2);
      // If (poll_size >= 1.0) the initial mesh size is equal to the poll size.

      // If 'max_poll_size' is equal to a very large number, it means that the
      // user doesn't want a maximum mesh size.
      if (max_poll_size < 9.9e307)
	{
	  // In the case where 'max_poll_size' is smaller than one, then the
	  // maximum mesh size will be even smaller.
	  if (max_poll_size < 1.0)
	    max_mesh_size = pow(max_poll_size, 2);
	  // If (max_mesh_size >= 1.0) the maximum mesh size is equal to the
	  // maximum poll size.
	}
    }
  else
    // MADS n+1 directions.
    if (choice == 4)
      {
	// The mesh size is modified for the first run only.
	if (initial)
	  {
	    if (poll_size <= dimension)
	      mesh_size = pow((poll_size / dimension), 2);
	    else
	      mesh_size = poll_size / dimension;
	  }
	// If 'max_poll_size' is equal to a very large number, it means that
	// the user doesn't want a maximum mesh size.
	if (max_poll_size < 9.9e307)
	  {
	    if (max_poll_size <= dimension)
	      max_mesh_size = pow((max_poll_size / dimension), 2);
	    else
	      max_mesh_size = max_poll_size / dimension;
	  }
      }
  // This boolean variable is put to 'false' after the first run.
  if (initial)
    initial = false;
}

/* This method is called when there is a success in the poll, to coarsen the 
   mesh. For example, for the mesh built with the default constructor, we
   have (after a success in the poll): mesh_size = 1.0 * pow(2.0, 1) = 2.0   */
void Mesh::coarsen()
{
  mesh_size = mesh_size * pow(poll_basis, coarsening_exponent);
  // The mesh size can't become larger than 'max_mesh_size'.
  if (mesh_size > max_mesh_size)
    {
      if (display_factor >= 7)
	{
	  cout << "mesh_size > max_mesh_size, ";
	  cout << mesh_size << " > " << max_mesh_size << "\n";
	}
      mesh_size = max_mesh_size;
    }
}

// The object variables are set to the user's choices.
void Mesh::set(const double delta_p, const double basis,
	       const double max_delta_p, const int coarsen, const int refine)
{
  /* If the user chose GPS poll directions, the mesh size is equal to the poll
     size. If he chose MADS directions, 'mesh_size' will be readjusted later on
     in the 'calculateMeshSize(...)' method. */
  max_mesh_size = max_poll_size = max_delta_p;
  poll_basis = basis;
  // If it's the first run the poll and mesh sizes are set. Otherwise the user
  // is continuing a previous run with the current mesh size.
  if (initial)
    mesh_size = poll_size = delta_p;
  coarsening_exponent = coarsen;
  refining_exponent = refine;
}

// The display factor has been reset.
void Mesh::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// show -- Outputs the Mesh object.
void Mesh::showInput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "\nMesh variables:\n";
  fout << "---------------\n";
  fout << "Initial poll size: " << poll_size << "\n";
  // if (poll_size != mesh_size)
    fout << "Initial mesh size: " << mesh_size << "\n";
  fout << "Maximum poll size: " << max_poll_size << "\n";
  // if (max_poll_size != max_mesh_size)
    fout << "Maximum mesh size: " << max_mesh_size << "\n";
  fout << "Poll size basis: " << poll_basis << "\n";
  fout << "Coarsening exponent: " << coarsening_exponent << "\n";
  fout << "Refining exponent: " << refining_exponent << "\n";
  fout << "\n";
}

// This method shows the final mesh size.
void Mesh::showOutput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "Final mesh size: " << mesh_size << "\n";
}

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

// mesh.h -- class definition for Mesh. This class controls the update of the
// mesh when there is a success or failure in the poll.

#ifndef _MESH_H_
#define _MESH_H_
#include <fstream>
using namespace std;

class Mesh
{
  // The Mesh object has the following variables:
  // - 'mesh_size' is the mesh size parameter.
  // - 'mesh_factor', 'coarsen_exponent' and 'refine_exponent' are
  //    used to update the mesh size in the following manner:
  //       1- success in the poll:
  //          mesh_size = mesh_size * (mesh_factor^coarsen_exponent)
  //       2- failure in the poll:
  //          mesh_size = mesh_size * (mesh_factor^refine_exponent)
  // - 'poll_size' is the initial poll size given by the user.
 private:
  bool initial;
  double max_mesh_size, max_poll_size, poll_basis, mesh_size, poll_size;
  int coarsening_exponent, refining_exponent;
  unsigned int display_factor;

// The class methods are described in "mesh.cpp".
 public:
  Mesh();
  ~Mesh();
  double getBasis();
  double getMeshSize();
  double refine();
  void calculateMeshSizes(const unsigned int dimension,
			  const unsigned int choice);
  void coarsen();
  void set(const double delta_p, const double basis, const double max_delta_p,
	   const int coarsen, const int refine);
  void setDisplay(const unsigned int display);
  void showInput(ofstream & fout);
  void showOutput(ofstream & fout);
};
#endif

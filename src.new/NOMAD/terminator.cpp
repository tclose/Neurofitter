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

/* terminator.cpp -- Terminator class methods. */

#include <iostream>
#include <cmath>
using namespace std;
#include "terminator.h"

// Default constructor. The Terminator object is created at Start-up by Solver.
Terminator::Terminator(Solver & s)
  : solver(& s)
{
  /* 'truth_evals' is set to a default value. The other variables are set to
     '-1', which means that they're not activated. */
  mesh_size = poll_size = -1.0;
  consecutive_fails = flag = iterations = new_truth_evals = -1;
  truth_evals = 50;
}

// Destructor.
Terminator::~Terminator()
{}

/* This method compares the current mesh size (delta_m), to the mesh size
   termination criterion. */
bool Terminator::compareMeshSize(const double delta_m)
{
  // if (mesh_size <= 0), it's not a criterion: we do nothing.
  if ((mesh_size > 0) && (delta_m <= mesh_size))
    /* if the current mesh size is inferior or equal than the criterion a value
       of 'true' is returned, which will alert 'Solver' to stop the run. */
    {
      flag = 0;
      return true;
    }
  else
    return false;
}

// This method returns the mesh size criterion.
double Terminator::getMeshSize()
{
  return mesh_size;
}

// This method returns the termination flag.
int Terminator::getFlag()
{
  return flag;
}

/* This method calculates the mesh size termination criterion (from the poll
   size criterion). It's called only when the user chose MADS poll directions,
   because in that case the mesh size isn't equal to the poll size. */
void Terminator::calculateMeshSize(const unsigned int dimension,
				   const unsigned int choice)
{
  /* If 'poll_size' and 'mesh_size' are less than or equal to zero, nothing is
     done. It means that the mesh size is not a termination criterion. */
  if (poll_size > 0.0)
    {
      // In this case the user chose MADS 2*n directions.
      if (choice == 3)
	{
	  /* In the case where poll size is smaller than one, then the mesh
	     size criterion will be even smaller. */
	  if (poll_size < 1.0)
	    mesh_size = pow(poll_size, 2);
	  // else the mesh size criterion is equal to the poll size criterion.
	}
      // MADS n+1 directions.
      else
	{
	  if (poll_size <= dimension)
	    mesh_size = pow((poll_size / dimension), 2);
	  else
	    mesh_size = poll_size / dimension;
	}
    }
}

/* This method compares the current number of consecutive failures to the
   consecutive failures termination criterion. */
void Terminator::compareConsecutiveFails(const int consecutive_failures)
{
  // if (consecutive_fails <= 0), it's not a criterion: we do nothing.
  if ((consecutive_fails > 0) && (consecutive_failures >= consecutive_fails))
    // If the current number is superior or equal than the criterion we stop
    // the program by calling the "endProgram()" method of the Solver object.
    {
      flag = 2;
      solver->endProgram();
    }
}

//
void Terminator::compareIterationsCount(const int iterations_count)
{
  //
  if ((iterations > 0) && (iterations_count >= iterations))
    {
      flag = 4;
      solver->endProgram();
    }
}

// This method compares the current number of new truth evaluations to the
// new truth evaluations termination criterion ('new_truth_evals').
void Terminator::compareNewTruthEvals(const int new_truth_evaluations)
{
  // if (new_truth_evals <= 0), it's not a criterion: we do nothing.
  if ((new_truth_evals > 0) && (new_truth_evaluations >= new_truth_evals))
    /* if the current number of new truth evaluations is superior or equal than
       the criterion we stop the program by calling the "endProgram" method
       of the Solver object. */
    {
      flag = 3;
      solver->endProgram();
    }
}

// This method compares the current number of truth evaluations to the truth
// evaluations termination criterion ('truth_evals').
void Terminator::compareTruthEvals(const int truth_evaluations)
{
  // if (truth_evals <= 0), it's not a criterion: we do nothing.
  if ((truth_evals > 0) && (truth_evaluations >= truth_evals))
    /* if the current number of truth evaluations is superior or equal
       than the criterion we stop the program by calling the "endProgram()"
       method of the Solver object. */
    {
      flag = 1;
      solver->endProgram();
    }
}

// The user selected non default termination criteria: they are recorded here.
void Terminator::set(const double poll_size_term,
		     const int consecutive_failures,
		     const int truth_evaluations, const int max_iter,
		     const int new_truth_evaluations)
{
  mesh_size = poll_size = poll_size_term;
  consecutive_fails = consecutive_failures;
  /* The 'flag' variable is reset to '-1'. This is needed when consecutive runs
     are made. */
  flag = -1;
  truth_evals = truth_evaluations;
  iterations = max_iter;
  new_truth_evals = new_truth_evaluations;
}

// show -- Outputs the Terminator object.
void Terminator::showInput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  fout << "Termination criteria:\n";
  fout << "---------------------\n";
  fout << "Poll size: ";
  if (poll_size <= 0)
    fout << "NA\n";
  else
    fout << poll_size << "\n";
  fout << "Mesh size: ";
  if (mesh_size <= 0)
    fout << "NA\n";
  else
    fout << mesh_size << "\n";
  fout << "Number of iterations: ";
  if (iterations <= 0)
    fout << "NA\n";
  else
    fout << iterations << "\n";
  fout << "Number of truth evaluations: ";
  if (truth_evals <= 0)
    fout << "NA\n";
  else
    fout << truth_evals << "\n";
  fout << "Number of new truth evaluations: ";
  if (new_truth_evals <= 0)
    fout << "NA\n";
  else
    fout << new_truth_evals << "\n";
  fout << "Number of consecutive failures: ";
  if (consecutive_fails <= 0)
    fout << "NA\n";
  else
    fout << consecutive_fails << "\n";
}

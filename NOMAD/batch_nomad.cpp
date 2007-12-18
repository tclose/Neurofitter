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

// batch_nomad.cpp -- making NOMAD work without the GUI.

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;
#include "defines.h"
#include "description.h"
#include "parameters.h"
#include "preferences.h"
#include "solver.h"

// Function prototypes.
void continueRun(const Description & description,
		 Parameters & parameters, Preferences & preferences,
		 Solver & solver, const char * param_file);
void solveProblem(const Description & description,
		  const Parameters & parameters, Preferences & preferences,
		  Solver & solver);

// The command line for NOMAD is this:
//   ./nomad description_file (parameters_file)
// - './nomad' is the name of the executable.
// - 'description_file' is mandatory: it contains the problem description.
// - 'parameters_file' is optional: if it's not given by the user, NOMAD will
//   solve the problem using default parameters.
int main(int argc, char * argv[])
{
  // If there's no description file nothing is done.
  if (argc == 1)
    {
      cerr << "\nA descriptor file must be included for NOMAD to run.\n";
      cerr << "The command line must be: ";
      cerr << "./nomad descriptor_file (parameters_file)\n";
      cerr << "(the parameters file is optional)\n";
      cerr << "See the program documentation in the DOC directory.\n\n";
    }
  else   // A description file was passed in the command line.
    {
      // The third argument is collected.
      char * parameters_file = 0;
      if (argc == 3)
	parameters_file = argv[2];

      // The objects are created. First the Description object.
      Description description(argv[1]);
      // The Parameters object is created.
      Parameters parameters(parameters_file);
      // The Preferences object is created.
      Preferences preferences;
      // The Solver object is created.
      Solver solver;

      // This function call checks if the description file is complete.
      if (description.readyToSolve())
	{
	  // If it is data entry can start: the problem description is sent to
	  // Solver.
	  description.capture(solver);
	  // The problem is solved.
	  solveProblem(description, parameters, preferences, solver);

	  // If the user wants to continue the current run, he just has to make
	  // this function call.
	  // char * new_parameters_file = "PROBLEMS/HS23/parameters_r2.dat";
	  // continueRun(description, parameters, preferences, solver,
	  // new_parameters_file);
	}
      else
	{
	  cerr << "The description file is incomplete!\n";
	  cerr << "Please complete it before running NOMAD.\n";
	  cerr << "See the program documentation in the 'DOC' directory.\n\n";
	}
    }
  return 0;
}

// This function continues the previous run, starting from the best point
// found.
void continueRun(const Description & description,
		 Parameters & parameters, Preferences & preferences,
		 Solver & solver, const char * param_file) {
  
	// 'Solver' is warned, so that certain variables will be reset.
  	solver.continueRun();
  
	//
  	if (param_file != 0) parameters.load(param_file);
  
	// The problem is solved.
  	solveProblem(description, parameters, preferences, solver);
}

// This function checks if the problem parameters are correct, then tells
// Solver to solve the current problem.
void solveProblem(const Description & description,
		  const Parameters & parameters, Preferences & preferences,
		  Solver & solver) {
  
	// The problem is ready to be solved if the parameters file contains all
	// the basic parameters.
  	if (parameters.readyToSolve()) {
		// The parameters are captured and sent to Solver.
      	parameters.capture(description.getGeneralConstraintsNumber(),
			 description.getSurrogateUse(), solver);
      
	  	// The preferences are sent to Solver.
      	preferences.capture(description.getName(), solver);

      	// Data entry complete: Solver evaluates the starting point.
      	if (solver.evaluateStartingPoint() > 0) {
	  		// 'fout' is the results file for the current run.
	  		ofstream fout(description.getResultsFile());
	  		
			// The problem's input is written in 'fout'.
	  		solver.showInput(fout);
	  
			// The problem is solved by the optimization module.
	  		solver.solveProblem(fout);
	  
			// The results file is closed.
	  		fout.close();
	  
			// The 'Preferences' object must be alerted that the run is over.
	  		preferences.endOfRun();
		}
		else {
	  		cerr << "The starting point is outside the bounds!\n";
	  		cerr << "Please correct before running NOMAD.\n";
	  		cerr << "See the program documentation in the 'DOC' directory.\n\n";
		}
    }
  	else {  // We alert the user that something's wrong with the parameters.
		cerr << "Something is wrong with the parameters!\n";
		cerr << "Please correct before running NOMAD.\n";
      	cerr << "See the program documentation in the 'DOC' directory.\n\n";
    }
}

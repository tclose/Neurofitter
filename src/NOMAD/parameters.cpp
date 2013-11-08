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

// parameters.cpp -- class methods for Parameters.

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "parameters.h"

// Private methods
// ****************************************************************************

// This method reads the parameters file, capturing its variables in the
// process.
void Parameters::readParameterFile(ifstream & fin)
{
  char ch;
  char line[80];
  bool check_speculative = false, check_relative = false,
    check_init_comp = false, check_iter_comp = false, check_poll_comp = false,
    check_poll_order = false, check_poll_size = false,
    check_poll_size_term = false, check_hmax = false, check_poll_basis = false,
    check_surr_tol = false, check_truth_term = false, check_iter_nb = false,
    check_last_term = false, check_refine = false,
    check_new_truth_term = false, check_coarsen = false, check_norm = false,
    check_ini_pts = false, check_ini_type = false, check_iter_pts = false,
    check_iter_type = false, check_poll_dirs = false, check_rand_seed = false,
    check_hmin = false, check_max_poll_size = false;
  // The file is read line by line in this loop.
  while (!fin.eof())
    {
      // We look at the first character, without putting it in the input buffer
      ch = fin.peek();
      fin >> line;
      // If the first character is '#', it means that we have a comment line.
      // Nothing is done in that case.
      if (ch != '#')
	{
	  // The initial poll size is extracted:
	  if (!check_poll_size && (strcmp(line, "INITIAL_POLL_SIZE") == 0))
	    {
	      fin >> poll_size;
	      check_poll_size = true;
	    }
	  // The maximum poll size is extracted:
	  if (!check_max_poll_size && (strcmp(line, "MAX_POLL_SIZE") == 0))
	    {
	      fin >> max_poll_size;
	      check_max_poll_size = true;
	    }
	  // The poll size basis is extracted:
	  else if (!check_poll_basis && (strcmp(line, "POLL_BASIS") == 0))
	    {
	      fin >> poll_basis;
	      check_poll_basis = true;
	    }
	  // The coarsening exponent is extracted:
	  else if (!check_coarsen && (strcmp(line,"COARSENING_EXPONENT") == 0))
	    {
	      fin >> coarsening_exponent;
	      check_coarsen = true;
	    }
	  // The refining exponent is extracted:
	  else if (!check_refine && (strcmp(line, "REFINING_EXPONENT") == 0))
	    {
	      fin >> refining_exponent;
	      check_refine = true;
	    }
	  // The random seed is extracted:
	  else if (!check_rand_seed && (strcmp(line, "RANDOM_SEED") == 0))
	    {
	      fin >> random_seed;
	      check_rand_seed = true;
	    }
	  // The poll order is extracted:
	  else if (!check_poll_order && (strcmp(line, "POLL_ORDER") == 0))
	    {
	      fin >> poll_order;
	      check_poll_order = true;
	    }
	  // The variable that indicates if the poll is complete is extracted:
	  else if (!check_poll_comp && (strcmp(line, "POLL_COMPLETE") == 0))
	    {
	      fin >> poll_complete;
	      check_poll_comp = true;
	    }
	  // The poll directions variable is extracted:
	  else if (!check_poll_dirs && (strcmp(line, "POLL_DIRECTIONS") == 0))
	    {
	      fin >> poll_directions;
	      check_poll_dirs = true;
	    }
	  // The initial search type is extracted:
	  else if (!check_ini_type && (strcmp(line, "INITIAL_SEARCH") == 0))
	    {
	      fin >> initial;
	      check_ini_type = true;
	    }
	  /* The variable that indicates if the initial search is complete is
	     extracted: */
	  else if (!check_init_comp && (strcmp(line,"INITIAL_COMPLETE") == 0))
	    {
	      fin >> initial_complete;
	      check_init_comp = true;
	    }
	  // The number of points of the initial search is extracted:
	  else if (!check_ini_pts && (strcmp(line, "INITIAL_POINTS") == 0))
	    {
	      fin >> initial_points;
	      check_ini_pts = true;
	    }
	  // The iterative search type is extracted:
	  else if (!check_iter_type && (strcmp(line, "ITERATIVE_SEARCH") == 0))
	    {
	      fin >> iterative;
	      check_iter_type = true;
	    }
	  /* The variable that indicates if the iterative search is complete is
	     extracted: */
	  else if (!check_iter_comp && (strcmp(line,"ITERATIVE_COMPLETE")== 0))
	    {
	      fin >> iterative_complete;
	      check_iter_comp = true;
	    }
	  // The number of points of the iterative search is extracted:
	  else if (!check_iter_pts && (strcmp(line, "ITERATIVE_POINTS") == 0))
	    {
	      fin >> iterative_points;
	      check_iter_pts = true;
	    }
	  // The presence of a speculative search is extracted:
	  else if (!check_speculative &&
		   (strcmp(line, "SPECULATIVE_SEARCH") == 0))
	    {
	      fin >> speculative_search;
	      check_speculative = true;
	    }
	  // The poll size termination criterion is extracted:
	  else if (!check_poll_size_term &&
		   (strcmp(line, "POLL_SIZE_TERM") == 0))
	    {
	      fin >> poll_size_term;
	      check_poll_size_term = true;
	    }
	  // The number of function evaluations criterion is extracted:
	  else if (!check_truth_term && (strcmp(line, "TRUTH_EVALS") == 0))
	    {
	      fin >> truth_evals;
	      check_truth_term = true;
	    }
	  // The criterion for the number of iterations is extracted.
	  else if (!check_iter_nb && (strcmp(line, "ITERATIONS") == 0))
	    {
	      fin >> iterations;
	      check_iter_nb = true;
	    }
	  // The number of new truth evaluations criterion is extracted:
	  else if (!check_new_truth_term &&
		   (strcmp(line, "NEW_TRUTH_EVALS") == 0))
	    {
	      fin >> new_truth_evals;
	      check_new_truth_term = true;
	    }
	  // The 'consecutive failures' criterion is extracted:
	  else if (!check_last_term &&
		   (strcmp(line, "CONSECUTIVE_FAILS") == 0))
	    {
	      fin >> consecutive_fails;
	      check_last_term = true;
	    }
	  /* The variable indicating if 'hmax' is relative to the starting
	     point's h(x) value is extracted: */
	  else if (!check_relative && (strcmp(line, "FILTER_RELATIVE") == 0))
	    {
	      fin >> filter_relative;
	      check_relative = true;
	    }
	  // The hmax value is extracted:
	  else if (!check_hmax && (strcmp(line, "HMAX") == 0))
	    {
	      fin >> hmax;
	      check_hmax = true;
	    }
	  // The hmin value is extracted.
	  else if (!check_hmin && (strcmp(line,"HMIN")== 0))
	    {
	      fin >> hmin;
	      check_hmin = true;
	    }
	  // The filter norm is extracted:
	  else if (!check_norm && (strcmp(line, "FILTER_NORM") == 0))
	    {
	      fin >> filter_norm;
	      check_norm = true;
	    }
	  // The surrogate tolerance is extracted:
	  else if (!check_surr_tol && (strcmp(line,"SURROGATE_TOLERANCE")== 0))
	    {
	      fin >> surrogate_tolerance;
	      check_surr_tol = true;
	    }
	}
    }
}

/* Public methods:
******************************************************************************/

// Class constructor.
Parameters::Parameters(const char * file)
{
  // All the variables are initialized to default parameters, just in case
  // the user forgets to include a given variable in the parameters file. First
  // the boolean variables are initialized.
  speculative_search = initial_complete = iterative_complete =
    poll_complete = false;
  filter_relative = poll_order = true;
  parameters_file = NULL;
  // The real variables are initialized.
  poll_size_term = 0.0, hmax = 100.0, hmin = 0.0001, poll_basis = 2.0,
    poll_size = 1.0, surrogate_tolerance = 10.0, max_poll_size = 100000;
  // The integer variables are initialized.
  consecutive_fails = -1, truth_evals = 50, iterations = -1,
    refining_exponent = -1, new_truth_evals = -1;
  // The unsigned variables are initialized.
  coarsening_exponent = 1, filter_norm = 2, initial_points = 0,
    initial = 0, iterative_points = 0, iterative = 0,
    poll_directions = 0, random_seed = 1;

  // The problem file must be loaded.
  if (file != NULL)
    load(file);
  else   // The user didn't provide a parameters file.
    {
      // A default path is given for the parameters file.
      setFile("LIBRARY/parameters.dat");
      // The parameters are written to file.
      write();
    }
}

// Destructor.
Parameters::~Parameters()
{}

bool Parameters::getFilterRelative()
{
  return filter_relative;
}

bool Parameters::getInitialComplete()
{
  return initial_complete;
}

bool Parameters::getIterativeComplete()
{
  return iterative_complete;
}

bool Parameters::getSpeculativeSearch()
{
  return speculative_search;
}

// This method checks if the problem is ready to solve.
bool Parameters::readyToSolve() const
{
  // First we check the mesh parameters.
  if (((poll_size <= 0.0) || (poll_basis <= 1.0) || (max_poll_size <= 0.0)) ||
      // The choice of poll directions is checked.	       
      (poll_directions > 4)                            ||
      // The choice of initial search is checked.	       
      (initial > 2)                                    ||
      // The choice of iterative search is checked.	       
      (iterative > 2)                                  ||
      // At least one termination criterion must be set.
      !((poll_size_term > 0.0) || (consecutive_fails > 0) ||
	(truth_evals > 0) || (iterations > 0) || (new_truth_evals > 0)))
    // Something is wrong with the parameters.
    return false;
  else
    // Everything is ok: we're ready to solve the problem.
    return true;
}

char * Parameters::getFile()
{
  return parameters_file;
}

double Parameters::getHmax()
{
  return hmax;
}

double Parameters::getHmin()
{
  return hmin;
}

double Parameters::getMaxPollSize()
{
  return max_poll_size;
}

double Parameters::getPollBasis()
{
  return poll_basis;
}

double Parameters::getPollSize()
{
  return poll_size;
}

double Parameters::getPollSizeTerm()
{
  return poll_size_term;
}

double Parameters::getSurrogateTolerance()
{
  return surrogate_tolerance;
}

int Parameters::getConsecutiveFails()
{
  return consecutive_fails;
}

int Parameters::getIterations()
{
  return iterations;
}

int Parameters::getNewTruthEvals()
{
  return new_truth_evals;
}

int Parameters::getPollDirs()
{
  return poll_directions;
}

int Parameters::getPollOrder()
{
  return poll_order;
}

int Parameters::getPollType()
{
  return poll_complete;
}

int Parameters::getRefine()
{
  return refining_exponent;
}

int Parameters::getTruthEvals()
{
  return truth_evals;
}

unsigned int Parameters::getCoarsen()
{
  return coarsening_exponent;
}

unsigned int Parameters::getFilterNorm()
{
  return filter_norm;
}

unsigned int Parameters::getInitialPoints()
{
  return initial_points;
}

unsigned int Parameters::getIterativePoints()
{
  return iterative_points;
}

unsigned int Parameters::getInitialSearch()
{
  return initial;
}

unsigned int Parameters::getIterativeSearch()
{
  return iterative;
}

unsigned int Parameters::getSeed()
{
  return random_seed;
}

// This method captures this object's attributes and passes them to Solver.
void Parameters::capture(const unsigned int gen_cons_nb,
			 const bool surrogate_use,
			 Solver & controlbox) const
{
  // The mesh parameters are sent to Solver.
  controlbox.enterMesh(poll_size, poll_basis, max_poll_size,
		       coarsening_exponent, refining_exponent);
  // The search parameters are sent.
  controlbox.enterSearch(initial, initial_complete, initial_points,
			 iterative, iterative_complete, iterative_points,
			 speculative_search);
  // The termination criteria are sent.
  controlbox.enterTerminationCriteria(poll_size_term, consecutive_fails,
				      truth_evals, iterations,
				      new_truth_evals);
  // The poll parameters are sent.
  controlbox.enterPoll(poll_order, poll_complete, poll_directions);
  // The filter parameters are sent.
  if (gen_cons_nb > 0)
    controlbox.enterFilter(filter_relative, hmax, hmin, filter_norm);
  // The surrogate parameters are sent.
  if (surrogate_use)
    controlbox.enterSurrogate(surrogate_tolerance);
  // The random seed is sent to Solver.
  controlbox.enterRandomSeed(random_seed);
}

void Parameters::enterFilter(const bool choice, const double h_max,
			     const double h_min, const unsigned int norm)
{
  filter_relative = choice;
  hmax = h_max;
  hmin = h_min;
  filter_norm = norm;
}

void Parameters::enterMesh(const double delta_p, const double basis,
			   const double max_delta_p,
			   const unsigned int coarsen, const int refine)
{
  poll_size = delta_p;
  poll_basis = basis;
  max_poll_size = max_delta_p;
  coarsening_exponent = coarsen;
  refining_exponent = refine;
}

void Parameters::enterPoll(bool fixed, const bool complete,
			   unsigned int choice)
{
  poll_order = fixed;
  poll_complete = complete;
  poll_directions = choice;
}

void Parameters::enterRandomSeed(const unsigned int seed)
{
  random_seed = seed;
}

void Parameters::enterSearch(const unsigned int ini_type,
			     const bool ini_complete,
			     const unsigned int ini_pts,
			     const unsigned int iter_type,
			     const bool iter_complete,
			     const unsigned int iter_pts,
			     const bool speculative)
{
  initial = ini_type;
  initial_complete = ini_complete;
  initial_points = ini_pts;
  iterative = iter_type;
  iterative_complete = iter_complete;
  iterative_points = iter_pts;
  speculative_search = speculative;
}

void Parameters::enterSurrogate(const double surr_tol)
{
  surrogate_tolerance = surr_tol;
}

void Parameters::enterTerminationCriteria(const double poll_size_criterion,
					  const int consecutive_failures,
					  const int truth_evaluations,
					  const int iter_number,
					  const int new_truth_evaluations)
{
  poll_size_term = poll_size_criterion;
  consecutive_fails = consecutive_failures;
  truth_evals = truth_evaluations;
  iterations = iter_number;
  new_truth_evals = new_truth_evaluations;
}

// This method opens the parameters file and loads them in memory.
void Parameters::load(const char * file)
{
  // First the 'problem_file' variable is reset.
  setFile(file);
  // We open the problem data file.
  ifstream fin(file);
  // If the file has been opened ('file' is a correct path)...
  if (fin.is_open())
    {
      // ...we read the file's data with this method.
      readParameterFile(fin);
      // The file is closed.
      fin.close();
    }
  else
    {
      // We alert the user that something is wrong with the path.
      cerr << "Couldn't open parameter file '" << file << "'\n\n";
      exit(0);
    }
}

// This method resets the 'parameters_file' variable to 'file'.
void Parameters::setFile(const char * file)
{
  if (parameters_file != NULL)
    delete [] parameters_file;
  // The memory is reserved for 'parameters_file'.
  parameters_file = new char[strlen(file) + 1];
  // 'file' is copied into 'parameters_file'.
  strcpy(parameters_file, file);
}

// This method prints the object's attributes to standard output.
void Parameters::show(const bool filter_use, const bool surrogate_use) const
{
  cout << "\n**************************************************************\n";
  cout << "Parameters file\n---------------\n\n";

  // The mesh parameters are shown.
  cout << "Mesh parameters\n---------------\n";
  cout << "Initial poll size: " << poll_size;
  cout << "\nMaximum poll size: " << max_poll_size;
  cout << "\nPoll size basis: " << poll_basis;
  cout << "\nCoarsening exponent: " << coarsening_exponent;
  cout << "\nRefining exponent: " << refining_exponent << "\n\n";

  // The random seed is shown.
  cout << "Random seed: " << random_seed << "\n";
  cout << "------------\n\n";

  // The poll parameters are shown.
  cout << "Poll parameters\n---------------\n";
  cout << "Poll order: " << poll_order;
  (poll_order == true) ? cout << " (Fixed)\n" : cout << " (Dynamic)\n";
  cout << "Complete poll? " << poll_complete;
  (poll_complete == true) ? cout << " (Complete)\n" : cout << " (Opportunistic)\n";
  cout << "Poll directions: ";
  switch (poll_directions)
    {
      case 0  :  cout << "Standard, 2 * n\n\n";
	         break;
      case 1  :  cout << "Standard, n + 1\n\n";
	         break;
      case 2  :  cout << "Standard, uniform\n\n";
	         break;
      case 3  :  cout << "Dense, 2 * n\n\n";
	         break;
      case 4  :  cout << "Dense, n + 1\n\n";
	         break;
      default :  cout << "That's not a choice!\n\n";
    }

  // The search parameters are shown.
  cout << "Search parameters\n-----------------\n";
  switch (initial)
    {
      case 0  : cout << "No initial search.\n";
	        break;
      case 1  : cout << "Initial search: Random, ";
	        break;
      case 2  : cout << "Initial search: Latin hypercube sampling, ";
	        break;
      default : cout << "This is not an initial search type!\n";
    }
  if (initial > 0)
    {
      (initial_complete > 0) ? cout << "complete, " : cout << "incomplete, ";
      cout << initial_points << " points.\n";
    }
  switch (iterative)
    {
      case 0  : cout << "No iterative searches.\n";
	        break;
      case 1  : cout << "Iterative searches: Random, ";
	        break;
      case 2  : cout << "Iterative searches: Latin hypercube sampling, ";
	        break;
      default : cout << "This is not an iterative search type!\n";
    }
  if (iterative > 0)
    {
      (iterative_complete > 0) ? cout << "complete, " : cout << "incomplete, ";
      cout << iterative_points << " points.\n";
    }
  if (speculative_search)
    cout << "Speculative search executed after a successful iteration.\n";

  // The termination parameters are shown.
  cout << "\nTermination parameters\n----------------------\n";
  if (poll_size_term > 0.0)
    cout << "Poll size: " << poll_size_term << "\n";
  if (iterations > 0)
    cout << "Number of iterations: " << iterations << "\n";
  if (truth_evals > 0)
    cout << "Number of truth evaluations: " << truth_evals << "\n";
  if (new_truth_evals > 0)
    cout << "Number of new truth evaluations: " << new_truth_evals << "\n";
  if (consecutive_fails > 0)
    cout << "Number of consecutive failures: " << consecutive_fails << "\n";
  cout << "\n";

  // If there are general constraints in the problem, the filter parameters are
  // shown.
  if (filter_use)
    {
      cout << "Filter parameters\n-----------------\n";
      cout << "hmax relative to starting point's h(x) value? ";
      (filter_relative == true) ? cout << "True\n" : cout << "False\n";
      if (filter_relative)
	cout << "Multiplication factor for determining hmax: ";
      else
	cout << "hmax set by the user: ";
      cout << hmax << "\n";
      cout << "hmin: " << hmin << "\n";
      cout << "Constraint violation norm: ";
      switch (filter_norm)
	{
	  case 0  : cout << "L-infinity\n\n";
	            break;
	  case 1  : cout << "L-one\n\n";
	            break;
	  case 2  : cout << "L-two squared\n\n";
	            break;
	  default : cout << "That's not a norm!\n\n";
	}
    }

  // If the problem uses a surrogate, the surrogate parameters are shown.
  if (surrogate_use)
    cout << "Surrogate tolerance factor: " << surrogate_tolerance << "\n";

  cout << "**************************************************************\n\n";
}

// This method writes the object's attributes in the parameters file.
void Parameters::write() const
{
  // First we open the parameters file.
  ofstream fout(parameters_file);
  if (!fout.is_open())
    {
      cerr << "Couldn't open parameters file " << parameters_file << "!!!\n";
      exit(0);
    }
  else   // The file is open; the attributes are written in the file.
    {
      /********************/
      /* Mesh parameters. */
      /********************/
      fout << "#MESH PARAMETERS\n#---------------\n";
      fout << "INITIAL_POLL_SIZE\t" << poll_size << "\n";
      fout << "MAX_POLL_SIZE\t\t" << max_poll_size << "\n";
      fout << "POLL_BASIS\t\t" << poll_basis << "\n";
      fout << "COARSENING_EXPONENT\t" << coarsening_exponent << "\n";
      fout << "REFINING_EXPONENT\t" << refining_exponent << "\n";

      /****************/
      /* Random seed. */
      /****************/
      fout << "#\n#RANDOM SEED PARAMETER\n#---------------------\n";
      fout << "RANDOM_SEED\t\t" << random_seed << "\n";

      /********************/
      /* Poll parameters. */
      /********************/
      fout << "#\n#POLL PARAMETERS\n#---------------\n";
      fout << "POLL_ORDER\t\t" << poll_order << "\n";
      fout << "POLL_COMPLETE\t\t" << poll_complete << "\n";
      fout << "POLL_DIRECTIONS\t\t" << poll_directions << "\n";

      /**********************/
      /* Search parameters. */
      /**********************/
      fout << "#\n#SEARCH PARAMETERS\n#-----------------\n";
      fout << "INITIAL_SEARCH\t\t" << initial << "\n";
      fout << "INITIAL_COMPLETE\t" << initial_complete << "\n";
      fout << "INITIAL_POINTS\t\t" << initial_points << "\n";
      fout << "ITERATIVE_SEARCH\t" << iterative << "\n";
      fout << "ITERATIVE_COMPLETE\t" << iterative_complete << "\n";
      fout << "ITERATIVE_POINTS\t" << iterative_points << "\n";
      fout << "SPECULATIVE_SEARCH\t" << speculative_search << "\n";

      /***************************/
      /* Termination parameters. */
      /***************************/
      fout << "#\n#TERMINATION PARAMETERS\n#----------------------\n";
      fout << "POLL_SIZE_TERM\t\t" << poll_size_term << "\n";
      fout << "CONSECUTIVE_FAILS\t" << consecutive_fails << "\n";
      fout << "TRUTH_EVALS\t\t" << truth_evals << "\n";
      fout << "ITERATIONS\t\t" << iterations << "\n";
      fout << "NEW_TRUTH_EVALS\t\t" << new_truth_evals << "\n";

      /**********************/
      /* Filter parameters. */
      /**********************/
      fout << "#\n#FILTER PARAMETERS\n#-----------------\n";
      fout << "FILTER_RELATIVE\t\t" << filter_relative << "\n";
      fout << "HMAX\t\t\t" << hmax << "\n";
      fout << "HMIN\t\t\t" << hmin << "\n";
      fout << "FILTER_NORM\t\t" << filter_norm << "\n";

      /*************************/
      /* Surrogate parameters. */
      /*************************/
      fout << "#\n#SURROGATE PARAMETERS\n#--------------------\n";
      fout << "SURROGATE_TOLERANCE\t" << surrogate_tolerance << "\n";

      // We close the parameters file.
      fout.close();
    }
}

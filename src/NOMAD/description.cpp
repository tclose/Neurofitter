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

// description.cpp -- class methods for Description.

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
#include "description.h"

/* Private methods
******************************************************************************/

/* This method sends the fundamental problem variables to Solver. */
void Description::captureArrays(Solver & controlbox)
{
  double * lower_bounds = NULL;
  double * upper_bounds = NULL;
  double * scaling_vector = NULL;
  // We open the starting point file.
  ifstream fin1(starting_point);
  // The starting point array is created.
  double * coord = new double[dimension];
  // We fill up the 'coord' array.
  fillArray(fin1, coord);
  // The starting point file is closed.
  fin1.close();

  // If there are bound constraints...
  if (constraints != NULL)
    {
      // ... we open the the constraints file.
      ifstream fin2(constraints);
      // We create arrays for the lower and upper bounds.
      lower_bounds = new double[dimension];
      upper_bounds = new double[dimension];
      // We fill up the bounds' arrays.
      fillArray(fin2, lower_bounds);
      fillArray(fin2, upper_bounds);
      // The constraints file is closed.
      fin2.close();
    }

  // If the user supplied a scaling vector...
  if (scaling == 2)
    {
      // ... we open the the scales file.
      ifstream fin3(scales);
      // The scaling vector array is created.
      scaling_vector = new double[dimension];
      // It's filled up with the values extracted from the file.
      fillArray(fin3, scaling_vector);
      // The scales file is closed.
      fin3.close();
    }

  // The variables and arrays collected are sent to Solver.
  controlbox.enterProblem(dimension, coord, lower_bounds, upper_bounds,
			  scaling, scaling_vector, lower, upper);

  // The arrays' memory are freed.
  delete [] coord;
  if (constraints != NULL)
    {
      delete [] lower_bounds;
      delete [] upper_bounds;
    }
  if (scaling == 2)
    delete [] scaling_vector;
}

// This method sends the black boxes to the control box (Solver object).
void Description::captureBlackBoxes(Solver & controlbox)
{
  // First we capture the general constraints' black boxes.
  std::vector<const char *> list_of_executables;
  if (general_constraints_number > 0)
    {
      ifstream fin(general_constraints);
      char line[80];
      for (unsigned int i = 0; i < general_constraints_number; i++)
	{
	  char * temp = new char[80];
	  fin.getline(line, 80);
	  strcpy(temp, line);
	  list_of_executables.push_back(temp);
	}
      fin.close();
    }
  /* The objective, surrogate and general constraint black boxes are sent to
     Solver. */
  controlbox.enterBlackBoxes(directory, input, truth, list_of_executables,
			     surrogate);
  // The 'temp' strings are deleted.
  if (general_constraints_number > 0)
    {
      std::vector<const char *>::iterator current;
      for (current = list_of_executables.begin();
	   current != list_of_executables.end(); current++)
	delete (* current);
      list_of_executables.clear();
    }
}

// This method fills the array 'coord' with numbers read from the file 'fin'.
void Description::fillArray(ifstream & fin, double * coord)
{
  char ch;
  char line[80];
  unsigned int i = 0;
  // We go in the loop: the next character is extracted from the file.
  while ((i < dimension) && (fin.get(ch)))
    {
      // '%' marks a comment line.
      if (ch == '%')
	// The line is read.
	fin.getline(line, 80);
      else   // If the end of line is reached we do nothing.
	if (ch == '\n')
	  {}
	else   // If it's not a comment line or an end of line, it's a number.
	  {
	    // The character is reinserted in the file.
	    fin.putback(ch);
	    // The number is read and put in the 'coord' array.
	    fin >> coord[i];
	    // The counter 'i' is incremented.
	    i++;
	  }
    }
}

// This method reads the problem file, capturing its variables in the process.
void Description::readProblemFile(ifstream & fin)
{
  char ch;
  char line[80], temp[80];
  bool check_name = false, check_dimension = false, check_gen_cons_nb = false,
    check_surrogate = false, check_black_box = false, check_directory = false,
    check_start_pt = false, check_input = false, check_results = false,
    check_truth_exe = false, check_bounds_use = false, check_bounds = false,
    check_gen_cons = false, check_cache_use = false, check_cache = false,
    check_surr_exe = false, check_scaling = false, check_scales = false,
    check_lower = false, check_upper = false;
  // The file is read line by line in this loop.
  while (!fin.eof())
    {
      // We look at the first character, without putting it in the input buffer
      ch = fin.peek();
      fin >> line;
      /* If the first character is '#', it means that we have a comment line.
	 Nothing is done in that case. */
      if (ch != '#')
	{
	  // The problem name is extracted:
	  if (!check_name && (strcmp(line, "PROBLEM_NAME") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'name'.
	      if (name != NULL)
		delete [] name;
	      name = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'name'.
	      strcpy(name, temp);
	      check_name = true;
	    }
	  // The problem dimension (number of variables) is extracted:
	  else if (!check_dimension && (strcmp(line, "DIMENSION") == 0))
	    {
	      fin >> dimension;
	      check_dimension = true;
	    }
	  // The number of general constraints is extracted:
	  else if (!check_gen_cons_nb && (strcmp(line, "GEN_CONS_NB") == 0))
	    {
	      fin >> general_constraints_number;
	      check_gen_cons_nb = true;
	    }
	  // The scaling choice is extracted:
	  else if (!check_scaling && (strcmp(line, "SCALING_CHOICE") == 0))
	    {
	      fin >> scaling;
	      check_scaling = true;
	    }
	  // The lower scaling bound is extracted:
	  else if (!check_lower && (strcmp(line, "LOWER_SCALE") == 0))
	    {
	      fin >> lower;
	      check_lower = true;
	    }
	  // The upper scaling bound is extracted:
	  else if (!check_upper && (strcmp(line, "UPPER_SCALE") == 0))
	    {
	      fin >> upper;
	      check_upper = true;
	    }
	  // The use of a surrogate is extracted:
	  else if (!check_surrogate && (strcmp(line, "USE_SURROGATE") == 0))
	    {
	      fin >> surrogate_use;
	      check_surrogate = true;
	    }
	  // The use of black boxes is extracted:
	  else if (!check_black_box && (strcmp(line, "USE_BLACK_BOXES") == 0))
	    {
	      fin >> black_box_use;
	      check_black_box = true;
	    }
	  // The use of bound constraints is extracted:
	  else if (!check_bounds_use && (strcmp(line, "USE_BOUNDS") == 0))
	    {
	      fin >> bounds_use;
	      check_bounds_use = true;
	    }
	  // The use of the caches is extracted:
	  else if (!check_cache_use && (strcmp(line, "USE_CACHES") == 0))
	    {
	      fin >> cache_use;
	      check_cache_use = true;
	    }
	  // The problem directory is extracted:
	  else if (!check_directory && (strcmp(line, "DIRECTORY") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'directory'.
	      if (directory != NULL)
		delete [] directory;
	      directory = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'directory'.
	      strcpy(directory, temp);
	      /* We make sure that the directory variable ends with a '/'
		 character. */
	      if (directory[strlen(directory) - 1] != '/')
		strcat (directory, "/");
	      check_directory = true;
	    }
	  // The starting point file is extracted:
	  else if (!check_start_pt && (strcmp(line, "START_PT_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'starting_point'.
	      if (starting_point != NULL)
		delete [] starting_point;
	      starting_point = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'starting_point'.
	      strcpy(starting_point, temp);
	      check_start_pt = true;
	    }
	  // The input file is extracted:
	  else if (!check_input && (strcmp(line, "INPUT_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'input'.
	      if (input != NULL)
		delete [] input;
	      input = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'input'.
	      strcpy(input, temp);
	      check_input = true;
	    }
	  // The results file is extracted:
	  else if (!check_results && (strcmp(line, "RESULTS_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'results'.
	      if (results != NULL)
		delete [] results;
	      results = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'results'.
	      strcpy(results, temp);
	      check_results = true;
	    }
	  // The path of the truth executable is extracted:
	  else if (!check_truth_exe && (strcmp(line, "TRUTH_EXE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'truth'.
	      if (truth != NULL)
		delete [] truth;
	      truth = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'truth'.
	      strcpy(truth, temp);
	      check_truth_exe = true;
	    }
	  // The scales file is extracted.
	  else if (!check_scales && (strcmp(line, "SCALES_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed and reallocated for 'scales'.
	      if (scales != NULL)
		delete [] scales;
	      scales = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'scales'.
	      strcpy(scales, temp);
	      check_scales = true;
	    }
	  // The bounds file is extracted:
	  else if (!check_bounds && (strcmp(line, "BOUNDS_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'constraints'.
	      if (constraints != NULL)
		delete [] constraints;
	      constraints = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'constraints'.
	      strcpy(constraints, temp);
	      check_bounds = true;
	    }
	  // The general constraints file is extracted:
	  else if (!check_gen_cons && (strcmp(line, "GEN_CONS_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'general_constraints'.
	      if (general_constraints != NULL)
		delete [] general_constraints;
	      general_constraints = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'general_constraints'.
	      strcpy(general_constraints, temp);
	      check_gen_cons = true;
	    }
	  // The cache file is extracted:
	  else if (!check_cache && (strcmp(line, "CACHE_FILE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'cache'.
	      if (cache != NULL)
		delete [] cache;
	      cache = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'cache'.
	      strcpy(cache, temp);
	      check_cache = true;
	    }
	  // The path of the surrogate executable is extracted:
	  else if (!check_surr_exe && (strcmp(line, "SURROGATE_EXE") == 0))
	    {
	      fin >> temp;
	      // The memory is freed, then allocated for 'surrogate'.
	      if (surrogate != NULL)
		delete [] surrogate;
	      surrogate = new char[strlen(temp) + 1];
	      // 'temp' is copied into 'surrogate'.
	      strcpy(surrogate, temp);
	      check_surr_exe = true;
	    }
	}
    }
}

/* Public methods:
******************************************************************************/

// Class constructor.
Description::Description(const char * file)
{
  // The boolean variables are initialized.
  black_box_use = bounds_use = cache_use = surrogate_use = false;
  // The strings are initialized to NULL.
  cache = constraints = directory = general_constraints = input = name =
    problem_file = results = starting_point = surrogate = scales = truth =
    NULL;
  // The real variables are initialized.
  lower = upper = 0.0;
  // The integer variable is initialized.
  scaling = 0;
  // The unsigned integer variables are initialized.
  dimension = general_constraints_number = 0;
  // The problem file must be loaded.
  if (file != NULL)
    loadProblemFile(file);
}

// Destructor.
Description::~Description()
{
  // All the strings are destroyed.
  if (cache != NULL)
    delete [] cache;
  if (constraints != NULL)
    delete [] constraints;
  if (directory != NULL)
    delete [] directory;
  if (general_constraints != NULL)
    delete [] general_constraints;
  if (input != NULL)
    delete [] input;
  if (name != NULL)
    delete [] name;
  if (problem_file != NULL)
    delete [] problem_file;
  if (results != NULL)
    delete [] results;
  if (scales != NULL)
    delete [] scales;
  if (starting_point != NULL)
    delete [] starting_point;
  if (surrogate != NULL)
    delete [] surrogate;
  if (truth!= NULL)
     delete [] truth;
}

// get...() methods.
// ****************************************************************************

// This method returns the boolean variable 'black_box_use'.
bool Description::getBlackBoxUse()
{
  return black_box_use;
}

// This method returns the boolean variable 'bounds_use'.
bool Description::getBoundsUse()
{
  return bounds_use;
}

// This method returns the boolean variable 'cache_use'.
bool Description::getCacheUse()
{
  return cache_use;
}

// This method returns the boolean variable 'surrogate_use'.
bool Description::getSurrogateUse() const
{
  return surrogate_use;
}

// This method checks if the problem is ready to solve.
bool Description::readyToSolve()
{
  /* If any of these strings are null, something is wrong and we can't solve
     the problem (we return 'false'). */
  if ((name == NULL)                                                         ||
      (directory == NULL)                                                    ||
      (starting_point == NULL)                                               ||
      (input == NULL)                                                        ||
      (results == NULL)                                                      ||
      // The problem must have a truth black box.
      ((black_box_use) && (truth == NULL))                                   ||
      // If a surrogate is used, it must have a surrogate black box.
      ((black_box_use) && (surrogate_use) && (surrogate == NULL))            ||
      /* If the problem has general constraints, it must have a general
	 constraints file containing paths for the black boxes. */
      ((black_box_use) &&
       (general_constraints_number > 0) && (general_constraints == NULL))    ||
      // If the problem has bound constraints, the bounds file must be set.
      ((bounds_use) && (constraints == NULL))                                ||
      /* If the problem uses the caches, the path of the permanent cache must
	 be set. */
      ((cache_use) && (cache == NULL))                                       ||
      // If the scaling is user-defined, the scales file must be set.
      ((scaling == 2) && (scales == NULL))                                   ||
      // The problem dimension must be set.
      (dimension <= 0))
    return false;
  else   // The Description object is ok.
    return true;
}

// This method returns the path of the permanent cache.
char * Description::getCache()
{
  return cache;
}

// This method returns the path of the constraints file.
char * Description::getConstraints()
{
  return constraints;
}

// This method returns the problem directory.
char * Description::getDirectory()
{
  return directory;
}

// This method returns the paths of the general constraints' black boxes.
char * Description::getGeneralConstraints()
{
  return general_constraints;
}

// This method returns the path of the input file.
char * Description::getInput()
{
  return input;
}

// This method returns the problem name.
char * Description::getName() const
{
  return name;
}

// This method returns the path of the problem file.
char * Description::getProblemFile()
{
  return problem_file;
}

// This method returns the path of the results file.
char * Description::getResultsFile() const
{
  return results;
}

// This method returns the path of the scales file.
char * Description::getScales()
{
  return scales;
}

// This method returns the path of the starting point file.
char * Description::getStartingPoint()
{
  return starting_point;
}

// This method returns the path of the surrogate black box.
char * Description::getSurrogate()
{
  return surrogate;
}

// This method returns the path of the objective function (truth) black box.
char * Description::getTruth()
{
  return truth;
}

// This method returns the lower scaling bound.
double Description::getLowerScale()
{
  return lower;
}

// This method returns the upper scaling bound.
double Description::getUpperScale()
{
  return upper;
}

// This method returns the scaling choice.
int Description::getScalingChoice()
{
  return scaling;
}

// This method returns the number of general constraints.
unsigned int Description::getGeneralConstraintsNumber() const
{
  return general_constraints_number;
}

// This method returns the problem dimension.
unsigned int Description::getDimension()
{
  return dimension;
}

/* This method captures this object's attributes and passes them to
   'controlbox'. */
void Description::capture(Solver & controlbox)
{
  if (black_box_use)
    // The problem black boxes are sent.
    captureBlackBoxes(controlbox);
  else   // There are no black boxes.
    // The output types are sent.
    controlbox.enterOutputTypes(surrogate_use, general_constraints_number);
  // The problem dimension, starting point and constraints are sent.
  captureArrays(controlbox);
  if (cache_use)
    // The cache usage is sent.
    controlbox.enterCache(cache);
}

/* The attributes related to the black boxes are set to the received
   parameters. */
void Description::enterBlackBoxes(const bool black_box,
				  const char * truth_exec,
				  const char * general, const char * surr_exec)
{
  black_box_use = black_box;
  // The old strings are destroyed (just a precaution).
  if (truth != NULL)
    {
      delete [] truth;
      truth = NULL;
    }
  if (general_constraints != NULL)
    {
      delete [] general_constraints;
      general_constraints = NULL;
    }
  if (surrogate != NULL)
    {
      delete [] surrogate;
      surrogate = NULL;
    }
  // If the problem uses black boxes, the received strings are copied.
  if (black_box_use)
    {
      // The memory is reserved for 'truth'.
      truth = new char[strlen(truth_exec) + 1];
      // 'truth_exec' is copied into 'truth'.
      strcpy(truth, truth_exec);
      // Maybe there's no general constraints.
      if (general_constraints_number > 0)
	{
	  // The memory is reserved for 'general_constraints'.
	  general_constraints = new char[strlen(general) + 1];
	  // 'general' is copied into 'general_constraints'.
	  strcpy(general_constraints, general);
	}
      // Maybe there's no surrogate.
      if (surrogate_use)
	{
	  // The memory is reserved for 'surrogate'.
	  surrogate = new char[strlen(surr_exec) + 1];
	  // 'surr_exec' is copied into 'surrogate'.
	  strcpy(surrogate, surr_exec);
	}
    }
}

/* The attributes related to the constraints are set to the received
   parameters. */
void Description::enterConstraints(const bool bounds, const char * bounds_file,
				   const unsigned int gen_cons_nbr)
{
  bounds_use = bounds;
  general_constraints_number = gen_cons_nbr;
  // The old string is destroyed (just a precaution).
  if (constraints != NULL)
    {
      delete [] constraints;
      constraints = NULL;
    }
  // If the problem has bound constraints, the received string is copied.
  if (bounds_use)
    {
      // The memory is reserved for 'constraints'.
      constraints = new char[strlen(bounds_file) + 1];
      // 'bounds_file' is copied into 'constraints'.
      strcpy(constraints, bounds_file);
    }
}

/* The attributes related to the problem files are set to the received
   parameters. */
void Description::enterFiles(const char * dir, const char * in,
			     const char * res)
{
  // The old strings are destroyed (just a precaution).
  if (directory != NULL)
    {
      delete [] directory;
      directory = NULL;
    }
  if (input != NULL)
    {
      delete [] input;
      input = NULL;
    }
  if (results != NULL)
    {
      delete [] results;
      results = NULL;
    }
  // The memory is reserved for 'directory'.
  directory = new char[strlen(dir) + 1];
  // 'dir' is copied into 'directory'.
  strcpy(directory, dir);
  // The memory is reserved for 'input'.
  input = new char[strlen(in) + 1];
  // 'in' is copied into 'input'.
  strcpy(input, in);
  // The memory is reserved for 'results'.
  results = new char[strlen(res) + 1];
  // 'res' is copied into 'results'.
  strcpy(results, res);
}

/* The attributes related to the problem options are set to the received
   parameters. */
void Description::enterOptions(const bool cache_usage, const char * cache_file,
			       const bool surr_use)
{
  cache_use = cache_usage;
  surrogate_use = surr_use;
  // The old cache string is destroyed.
  if (cache != NULL)
    {
      delete [] cache;
      cache = NULL;
    }
  // If the problem uses the caches, the received parameter is copied.
  if (cache_use)
    {
      // The memory is reserved for 'cache'.
      cache = new char[strlen(cache_file) + 1];
      // 'cache_file' is copied into 'cache'.
      strcpy(cache, cache_file);
    }
}

/* The attributes related to the problem definition are set to the received
   parameters. */
void Description::enterProblem(const char * prob_name, const unsigned int dim,
			       const char * start_pt)
{
  dimension = dim;
  // The old strings are destroyed (just a precaution).
  if (name != NULL)
    {
      delete [] name;
      name = NULL;
    }
  if (starting_point != NULL)
    {
      delete [] starting_point;
      starting_point = NULL;
    }
  // The memory is reserved for 'name'.
  name = new char[strlen(prob_name) + 1];
  // 'prob_name' is copied into 'name'.
  strcpy(name, prob_name);
  // The memory is reserved for 'starting_point'.
  starting_point = new char[strlen(start_pt) + 1];
  // 'start_pt' is copied into 'starting_point'.
  strcpy(starting_point, start_pt);
}

/* The attributes related to the problem options are set to the received
   parameters. */
void Description::enterScales(const int index, const char * scales_file,
			      const double low, const double up)
{
  scaling = index;
  // The old 'scales' string is destroyed.
  if (scales != NULL)
    {
      delete [] scales;
      scales = NULL;
    }
  /* If the user gave us a vector for the scaling, the 'scales' string is
     recreated. */
  if (scaling == 2)
    {
      // The memory is reserved.
      scales = new char[strlen(scales_file) + 1];
      // 'scales_file' is copied into 'scales'.
      strcpy(scales, scales_file);
    }
  else
    // The user gave bounds for the scaling.
    if (scaling == 3)
      {
	lower = low;
	upper = up;
      }
}

/* This method opens the problem file and loads the problem parameters in
   memory. */
void Description::loadProblemFile(const char * file)
{
  // First the 'problem_file' variable is reset.
  setProblemFile(file);
  // We open the problem data file.
  ifstream fin(file);
  // If the file has been opened ('file' is a correct path)...
  if (fin.is_open())
    {
      // ...we read the file's data with this method.
      readProblemFile(fin);
      // The file is closed.
      fin.close();
    }
  else
    {
      // We alert the user that something is wrong with the path.
      cerr << "Couldn't open problem file " << file << "!!!\n\n";
      exit(0);
    }
}

// This method resets the 'problem_file' variable to 'file'.
void Description::setProblemFile(const char * file)
{
  if (problem_file != NULL)
    delete [] problem_file;
  // The memory is reserved for 'problem_file'.
  problem_file = new char[strlen(file) + 1];
  // 'file' is copied into 'problem_file'.
  strcpy(problem_file, file);
}

void Description::show() const
{
  cout << "\n**************************************************************\n";
  cout << "Problem description: " << name << "\n";
  cout << "Problem directory: " << directory << "\n";
  cout << "Input file: " << input << "\n";
  cout << "Results file: " << results << "\n";
  cout << "Starting point file: " << starting_point << "\n";
  cout << "Scaling choice: ";
  switch(scaling)
    {
      case 0  : cout << "No scaling.\n";
	        break;
      case 1  : cout << "Automatic scaling.\n";
	        break;
      case 2  : cout << "User-provided scaling vector: " << scales << "\n";
	        break;
      case 3  : cout << "User-provided bounds. \nLower bound = " << lower;
	        cout << ", upper bound = " << upper << "\n";
		break;
      default : cerr << "This is not a scaling choice!\n";
    }
  cout << "Use black boxes? ";
  (black_box_use == true) ? cout << "True\n" : cout << "False\n";
  if (black_box_use)
    {
      if (surrogate_use)
	cout << "Surrogate executable: " << surrogate << "\n";
      cout << "Truth executable: " << truth << "\n";
    }
  cout << "Use bound constraints? ";
  (bounds_use == true) ? cout << "True\n" : cout << "False\n";
  if (bounds_use)
    cout << "Constraints path: " << constraints << "\n";
  cout << "Use caches? ";
  (cache_use == true) ? cout << "True\n" : cout << "False\n";
  if (cache_use)
    cout << "Cache path: " << cache << "\n";
  cout << "Use a surrogate? ";
  (surrogate_use == true) ? cout << "True\n" : cout << "False\n";
  cout << "Problem dimension: " << dimension << "\n";
  cout << "Number of general constraints: " << general_constraints_number;
  if (general_constraints_number > 0)
    cout << ", path: " << general_constraints;
  cout << "\n";
  cout << "**************************************************************\n";
}

// This method writes the object's attributes in the problem file.
void Description::write() const
{
  // First we open the problem file.
  ofstream fout(problem_file);
  if (!fout.is_open())
    {
      cerr << "Couldn't open problem file " << problem_file << "!!!\n";
      exit(0);
    }
  else   // The file is open; the attributes are written in the problem file.
    {
      // The problem name:
      fout << "PROBLEM_NAME\t" << name << "\n";
      // The problem dimension:
      fout << "DIMENSION\t" << dimension << "\n";
      // The scaling choice:
      fout << "SCALING_CHOICE\t" << scaling << "\n";
      // If the user provided a scaling vector, its path is shown:
      if (scaling == 2)
	fout << "SCALES_FILE\t" << scales << "\n";
      // If the user provided scaling bounds, they're shown:
      if (scaling == 3)
	{
	  fout << "LOWER_SCALE\t" << lower << "\n";
	  fout << "UPPER_SCALE\t" << upper << "\n";
	}
      // The number of general constraints:
      fout << "GEN_CONS_NB\t" << general_constraints_number << "\n";
      // The use of a surrogate:
      fout << "USE_SURROGATE\t" << surrogate_use << "\n";
      // The use of black boxes:
      fout << "USE_BLACK_BOXES\t" << black_box_use << "\n";
      // The use of bound constraints:
      fout << "USE_BOUNDS\t" << bounds_use << "\n";
      // The use of the caches:
      fout << "USE_CACHES\t" << cache_use << "\n";
      // The problem directory:
      fout << "DIRECTORY\t" << directory << "\n";
      // The starting point file:
      fout << "START_PT_FILE\t" << starting_point << "\n";
      // The input file:
      fout << "INPUT_FILE\t" << input << "\n";
      // The results file:
      fout << "RESULTS_FILE\t" << results << "\n";
      // The path of the truth executable:
      if (black_box_use)
	{
	  fout << "TRUTH_EXE\t" << truth << "\n";
	  // The general constraints file:
	  if (general_constraints_number > 0)
	    fout << "GEN_CONS_FILE\t" << general_constraints << "\n";
	  // The path of the surrogate executable:
	  if (surrogate_use)
	    fout << "SURROGATE_EXE\t" << surrogate << "\n";
	}
      // The bounds file:
      if (bounds_use)
	fout << "BOUNDS_FILE\t" << constraints << "\n";
      // The cache file:
      if (cache_use)
	fout << "CACHE_FILE\t" << cache << "\n";
      // We close the problem file.
      fout.close();
    }
}

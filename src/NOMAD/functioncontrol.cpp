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

/* functioncontrol.cpp -- FunctionControl class methods. All methods of the
   class are called from Evaluator, which is the only class with direct access
   to FunctionControl. */

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;
#include "cachecontrol.h"
#include "defines.h"
#include "functioncontrol.h"
#include "generalconstraints.h"
#include "surrfunction.h"
#include "truthfunction.h"

// Private methods.
// ----------------

//
char * FunctionControl::createBlackBoxString(char * file, const char * dir,
					    const char * box, const char * txt)
{ 
  const char * str1 = "cd ";
  const char * str2 = "; ";
  const char * str3 = " > ";
  file = new char[8 + strlen(dir) + strlen(box) + strlen(txt) + 1];
  strcpy(file, str1);
  strcat(file, dir);
  strcat(file, str2);
  strcat(file, box);
  strcat(file, str3);
  strcat(file, txt);
  return file;
}

//
char * FunctionControl::createFileString(char * file, const char * dir,
					 const char * txt)
{
  file = new char[strlen(dir) + strlen(txt) + 1];
  strcpy(file, dir);
  strcat(file, txt);
  return file;
}

// This method evaluates the objective function value for a given point 'ap'.
double FunctionControl::giveMeTruth(AugmentedPoint & ap)
{
  double val = 0.0;
  if (truth != NULL)
    // The truth function is inside Nomad.
    val = truth->evaluate(ap.getPoint());
  else   // We call the truth function black box with a system call.
    {
      system(truth_box);
      ifstream output(output_file, ios::in);
      if ((output >> val) == false)
	val = INFINIT;
      output.close();
    }
  /* The "new_truth_evaluations" counter is incremented. We ask the Terminator
     object if the new truth evaluations criterion is exceeded. */
  term->compareNewTruthEvals(++new_truth_evaluations);
  // The truth value is stored in 'ap'.
  ap.setOutput(0, val);
  return val;
}

/* This method builds the 'scales_mult' array for unbounded problems. The other
   array, 'scales_add' isn't needed. */
void FunctionControl::automaticScaling(const unsigned int dim, double * coord)
{
  double scale_exponent;
  for(unsigned int i = 0; i < dim; i++)
    {
      scale_exponent = coord[i] != 0.0 ? log(abs(coord[i])) / log(2.0) : 0.0;
      // We build the 'scales_mult' array.
      scales_mult[i] = pow(2.0, floor(scale_exponent - 1.0));
    }
}

/* This method builds the 'scales_mult' array for bounded problems. The other
   array, 'scales_add' isn't needed. */
void FunctionControl::automaticScaling(const unsigned int dim, double * coord,
				       const double * lower,
				       const double * upper)
{
  double scale_exponent;
  unsigned int finite;
  for(unsigned int i = 0; i < dim; i++)
    {
      finite = ((lower[i] != 0.0) && (lower[i] != -INFINIT)) ? 4 : 0;
      finite += ((upper[i] != 0.0) && (upper[i] != INFINIT)) ? 2 : 0;
      if (finite == 0)
	finite += (coord[i] != 0.0) ? 1 : 0;
      switch(finite)
	{
	  case 6  : scale_exponent =
		      ((log(abs(lower[i])) / log(2.0)) +
		       (log(abs(upper[i])) / log(2.0))) / 2.0;
	            if (display_factor >= 5)
		      cout << "Case 6: lower[i] and upper[i] finite.\n";
		    break;
	  case 4  : scale_exponent = log(abs(lower[i])) / log(2.0);
	            if (display_factor >= 5)
		      cout << "Case 4: lower[i] finite.\n";
		    break;
	  case 2  : scale_exponent = log(abs(upper[i])) / log(2.0);
	            if (display_factor >= 5)
		      cout << "Case 2: upper[i] finite.\n";
		    break;
	  case 1  : scale_exponent = log(abs(coord[i])) / log(2.0);
	            if (display_factor >= 5)
		      cout << "Case 1: x[i] finite.\n";
		    break;
	  case 0  : scale_exponent = 1.0;
	            if (display_factor >= 5)
		      cout << "Case 0: Really bad!\n";
		    break;
	  default : scale_exponent = 1.0;
	            cout << "This is not a possible case!\n";
	}
      // We build the 'scales_mult' array.
      scales_mult[i] = pow(2.0, floor(scale_exponent - 1.0));
    }
}

/* This method is called when the user provided bounds for the scaling; i.e all
   variables will be scaled between two real numbers, 'low' and 'up'. */
void FunctionControl::boundScaling(const unsigned int dim,
				   const double * lower, const double * upper,
				   const double low, const double up)
{
  // The two scaling arrays are built.
  for (unsigned int i = 0; i < dim; i++)
    {
      scales_mult[i] = (upper[i] - lower[i]) / (up - low);
      scales_add[i] = low - (lower[i] / scales_mult[i]);
    }
}

// The point coordinates are written in the input file.
void FunctionControl::openInputFile(const unsigned int dim,
				    const double * coord)
{
  // The file is opened.
  ofstream input(input_file);
  if (input.fail())
    {
      cerr << "\nError while opening " << input_file << endl;
      exit(0);
    }
  input.precision(64);
  // Each coordinate is written on a separate line.
  for (unsigned int i = 0; i < dim; i++)
    input << coord[i] << "\n";
  // The file is closed.
  input.close();
}

// This method outputs the scaling arrays to a file.
void FunctionControl::showScales(const unsigned int dim, ofstream & fout)
{
  unsigned int i;
  // First 'scales_mult' is written.
  fout << "Multiplication array: (";
  for (i = 0; i < dim - 1; i++)
    fout << scales_mult[i] << ", ";
  fout << scales_mult[i] << ")\n";
  // Then 'scales_add' is written.
  fout << "Addition array: (";
  for (i = 0; i < dim - 1; i++)
    fout << scales_add[i] << ", ";
  fout << scales_add[i] << ")\n";
}

// Public methods.
// ---------------

// Default constructor, called from Evaluator at Start-up.
FunctionControl::FunctionControl(Terminator & t)
  : term(& t)
{
  // The variables are initialized to 0.
  low = 0.0;
  up = 0.0;
  display_factor = 0;
  truth_evaluations = 0;
  new_truth_evaluations = 0;
  use_scaling = 0;
  // The scaling vectors are initialized to NULL.
  scales_add = NULL;
  scales_mult = NULL;
  // The strings are initialized to 0 (NULL).
  input_file = 0;
  surrogate_box = 0;
  truth_box = 0;
  output_file = 0;
  // The pointers are set to NULL.
  cache_control = NULL;
  gen_cons = NULL;
  surr_f = NULL;
  surr_control = NULL;
  truth = NULL;
}

// Destructor.
FunctionControl::~FunctionControl()
{
  if (cache_control != NULL)
    delete cache_control;
  if (gen_cons != NULL)
    delete gen_cons;
  if (truth != NULL)
    delete truth;
  if (surr_f != NULL)
    delete surr_f;
  // The strings are destroyed.
  if (input_file != 0)
    delete [] input_file;
  if (truth_box != 0)
    delete [] truth_box;
  if (surrogate_box != 0)
    delete [] surrogate_box;
  if (output_file != 0)
    delete [] output_file;
  if (scales_add != NULL)
    delete [] scales_add;
  if (scales_mult != NULL)
    delete [] scales_mult;
}

// This method checks if the caches are used in a run.
bool FunctionControl::cachesUsed()
{
  if (cache_control != NULL)
    return true;
  else
    return false;
}

// This method evaluates the objective function value for a given point 'ap'.
bool FunctionControl::evaluateTruth(AugmentedPoint & ap, bool startingPoint)
{
  double val = 0.0;
  unsigned int cache_hit = NO_CACHE_HIT;
  unsigned int dim = ap.getDimension();
  unsigned int i;
  /* We have to unscale the point's coordinates before we can call the
     objective function. First we get ap's coordinates... */
  double * copy = ap.getPoint();
  // The unscaled coordinates will be put into 'apoint'.
  double * apoint = new double[dim];
  // The scaled coordinates are unscaled by this formula:
  for (i = 0; i < dim; i++)
    apoint[i] = (copy[i] - scales_add[i]) * scales_mult[i];

  // The point is shown, scaled and unscaled (for debugging).
  if (display_factor >= 10)
    {
      cout << "\nScaled point: (";
      for (i = 0; i < dim - 1; i++)
	cout << copy[i] << ", ";
      cout << copy[dim - 1] << ")\n";
      cout << "Unscaled point: (";
      for (i = 0; i < dim - 1; i++)
	cout << apoint[i] << ", ";
      cout << apoint[dim - 1] << ")\n";
    }

  // apoint's coordinates are written in the input file.
  if (truth == NULL)
    openInputFile(ap.getDimension(), apoint);
  // We check if the point is already in the caches.
  if (cache_control != NULL)
    cache_hit = cache_control->contains(apoint, ap.getTruthValues());
  /* If the point is in the temporary cache, the 'truth_evaluations' counter
     is not incremented. In all other cases the counter is incremented
     {point not in the caches, point in the permanent cache}. */
  if (cache_hit != TEMP_CACHE_HIT)
    {
      // The "truth_evaluations" counter is incremented. We ask the
      // Terminator object if the truth evaluations criterion is exceeded.
      term->compareTruthEvals(++truth_evaluations);
      // If there are general constraints, we evaluate the Gi(x).
      if (gen_cons != NULL)
	gen_cons->evaluate(ap, startingPoint);
      /* If the point wasn't in the caches, its objective function value is
	 evaluated. */
      if (cache_hit == NO_CACHE_HIT)
	val = giveMeTruth(ap);
    }
  if (cache_control != NULL)
    // The point is inserted in the caches.
    cache_control->insert(apoint, ap.getTruthValues());
  // We give the augmented point to the surrogate manager.
  if (surr_control != NULL)
    surr_control->store(ap);
  // apoint's memory is freed.
  delete [] apoint;
  /* The method returns 'false' in these cases:
     1- The point was in the temporary cache.
     2- The truth value is infinite. */
  if ((cache_hit == TEMP_CACHE_HIT) || (val == -INFINIT) || (val == INFINIT))
    return false;
  else
    return true;
}

/// WVG added for parallelization
// This method evaluates the objective function value for a given point 'ap'.
#ifdef PARANOMAD
vector< int > FunctionControl::parallelEvaluateTruth(vector< AugmentedPoint * > points) {

	/// WVG todo check the scaling mess
	vector < double * > unScaledPoints(points.size());
	vector < double > fitnessValues(points.size());  
	vector < int > returnValues(points.size());  


	for (unsigned int nPoint = 0; nPoint < points.size(); nPoint++) {

		unsigned int dim = points[nPoint]->getDimension();
		unsigned int i;

		/* We have to unscale the point's coordinates before we can call the
		objective function. First we get ap's coordinates... */
  
		double * copy = points[nPoint]->getPoint();
  		// The unscaled coordinates will be put into 'apoint'.
  		unScaledPoints[nPoint] = new double[dim];
  		// The scaled coordinates are unscaled by this formula:
  		for (i = 0; i < dim; i++) {
    		unScaledPoints[nPoint][i] = (copy[i] - scales_add[i]) * scales_mult[i];
		}

  		// The point is shown, scaled and unscaled (for debugging).
  		if (display_factor >= 10) {
			cout << "\nScaled point: (";
      		for (i = 0; i < dim - 1; i++) {
				cout << copy[i] << ", ";
	  		}
      		cout << copy[dim - 1] << ")\n";
      		cout << "Unscaled point: (";
      		for (i = 0; i < dim - 1; i++) {
				cout << unScaledPoints[nPoint][i] << ", ";
			}
      		cout << unScaledPoints[nPoint][dim - 1] << ")\n";
    	}

		// If there are general constraints, we evaluate the Gi(x).
      	if (gen_cons != NULL) {
			gen_cons->evaluate(*(points[i]), false);
		}

	}

	fitnessValues = truth->parallelEvaluate(unScaledPoints);

	truth_evaluations = truth_evaluations + points.size();
	new_truth_evaluations = new_truth_evaluations + points.size();

    term->compareTruthEvals(truth_evaluations);
	term->compareNewTruthEvals(new_truth_evaluations);

	for (unsigned int nPoint = 0; nPoint < points.size(); nPoint++) {
		points[nPoint]->setOutput(0, fitnessValues[nPoint]);

		if (surr_control != NULL) {
    		surr_control->store(*(points[nPoint]));
		}
  	
		// unscaled point's memory is freed.
  		delete [] unScaledPoints[nPoint];
  
		/* The method returns 'false' in these cases:
     	1- The truth value is infinite. */
  		if ((fitnessValues[nPoint] == -INFINIT) || (fitnessValues[nPoint] == INFINIT)) {
			 returnValues[nPoint] = false;
		}
  		else {
    		returnValues[nPoint] = true;
		}
	}
		
	return returnValues;  
}
#endif

// This method returns the scales addition array.
double * FunctionControl::getScalesAdd()
{
  return scales_add;
}

// This method returns the scales multiplication array.
double * FunctionControl::getScalesMult()
{
  return scales_mult;
}

// Returns the number of hits in the permanent cache.
unsigned int FunctionControl::getPermCacheHits()
{
  if (cache_control != NULL)
    return cache_control->getPermCacheHits();
  else
    return 0;
}

// Returns the number of hits in the temporary cache.
unsigned int FunctionControl::getTempCacheHits()
{
  if (cache_control != NULL)
    return cache_control->getTempCacheHits();
  else
    return 0;
}

// This method builds the scaling arrays for the current problem.
void FunctionControl::buildScales(const unsigned int dim, double * coord,
				  const double * lower, const double * upper,
				  const unsigned int scaling,
				  const double * scale, const double a,
				  const double b)
{
  low = a;
  up = b;
  use_scaling = scaling;
  unsigned int i;
  // The scales arrays  are created for the design variables and the bounds.
  scales_add = new double[dim];
  scales_mult = new double[dim];
  /* The arrays are initialized to default values. The scales multiplier values
     are '1.0' and the scales adder values are '0.0', which is the unscaled
     case. */
  for(i = 0; i < dim; i++)
    {
      scales_add[i] = 0.0;
      scales_mult[i] = 1.0;
    }
  // If the user requested scaling...
  if (scaling > 0)
    {
      // First case: The user provided his own scaling vector.
      if ((scaling == 2) && (scale != NULL))
	// The 'scales_add' array stays unchanged.
	for(i = 0; i < dim; i++)
	  scales_mult[i] = scale[i];
      else
	/* The user provided bounds for scaling: the lower and upper bounds
	   must exist for this method to work. */
	if ((scaling == 3) && (lower != NULL) &&
	    (upper != NULL) && (low != up))
	  boundScaling(dim, lower, upper, low, up);
	else
	  {
	    /* In this case, either the user requested automatic scaling
	       ('scaling' equals 1), or he requested some other kind of
	       scaling but failed to provide the correct parameters. */
	    if ((lower == NULL) && (upper == NULL))
	      automaticScaling(dim, coord);
	    else
	      automaticScaling(dim, coord, lower, upper);
	  }
    }
}

// The caches are closed at the end of the run.
void FunctionControl::closeCaches()
{
  if (cache_control != NULL)
    cache_control->closeCaches();
}

// The user wants to continue the run.
void FunctionControl::continueRun()
{
  truth_evaluations = 0;
  new_truth_evaluations = 0;
  if (cache_control != NULL)
    cache_control->continueRun();
}

/* The directory and executable names for the truth and surrogate functions are
   sent to FunctionControl. */
void FunctionControl::enterBlackBoxes(const char * dir, const char * input,
				      const char * truthF,
				   std::vector<const char *> & gen_constraints,
				      const char * surrF)
{
  // The input string is created.
  input_file = new char[strlen(input) + 1];
  strcpy(input_file, input);
  // The truth box string is created.
  truth_box = createBlackBoxString(truth_box, dir, truthF, "output.txt");
  output_file = createFileString(output_file, dir, "output.txt");
  // If required, the surrogate box string is created.
  if (surrF != NULL)
    surrogate_box = createBlackBoxString(surrogate_box, dir, surrF,
					 "output.txt");
  // If required, the GeneralConstraints object is created.
  if ((gen_constraints.size() > 0) && (gen_cons == NULL))
    gen_cons = new GeneralConstraints(dir, input, output_file,
				      gen_constraints);
  // These displays help debug the program.
  if (display_factor >= 9)
    {
      cout << "In 'FunctionControl::enterBlackBoxes(...)'\n";
      cout << "input_file = " << input_file << "\n";
      cout << "output_file = " << output_file << "\n";
      cout << "truth_box = " << truth_box << "\n";
      if (surrF != NULL)
	cout << "surrogate_box = " << surrogate_box << "\n";
      cout << "\n";
    }
}

// The filter parameters are received and passed on to GeneralConstraints.
void FunctionControl::enterFilter(const bool choice, const double h_max,
				  const double h_min, const unsigned int norm)
{
  if (gen_cons != NULL)
    gen_cons->enterFilter(choice, h_max, h_min, norm);
}

/* The variables for the number of general constraints, and the use of a
   surrogate are received. (It means that the user doesn't use black boxes.) */
void FunctionControl::enterOutputTypes(const bool surr_used,
				       const unsigned int gen_cons_nb)
{
  if ((gen_cons_nb > 0) && (gen_cons == NULL))
    gen_cons = new GeneralConstraints(gen_cons_nb);
  truth = new TruthFunction();
  if (surr_used)
    surr_f = new SurrFunction();
}

/* This method calculates the surrogate function value for a given augmented
   point, 'ap'. */
void FunctionControl::evaluateFlow(AugmentedPoint & ap)
{
  /* We have to unscale the point's coordinates before we can call the
     objective function. First we get ap's coordinates... */
  double * copy = ap.getPoint();
  double * truth_val = ap.getTruthValues();
  unsigned int dim = ap.getDimension();
  // The unscaled coordinates will be put into 'apoint'.
  double * apoint = new double[dim];
  // The scaled coordinates are multiplied by the scale.
  for (unsigned int i = 0; i < dim; i++)
    apoint[i] = (copy[i] - scales_add[i]) * scales_mult[i];
  // We check if the point is already in the caches.
  if ((cache_control != NULL) &&
      (cache_control->contains(apoint, truth_val) > 0))
    {
      // The point is already in the caches.
      if (display_factor >= 7)
	cout << "Cache hit for evaluateFlow!\n";
    }
  else   // The point wasn't in the cache (maybe the cache is not used).
    {
      // The point's surrogate function value is evaluated.
      double val;
      if (surr_f != NULL)
	val = surr_f->evaluate(apoint);
      else
	{
	  openInputFile(ap.getDimension(), apoint);
	  system(surrogate_box);
	  ifstream output(output_file);
	  if ((output >> val) == false)
	    val = INFINIT;
	  output.close();
	}
      ap.setLow(val);
    }
  // apoint's memory is freed.
  delete [] apoint;
}

// This method fills the array 'coord' with the unscaled incumbent.
void FunctionControl::fillWithBestPoint(const unsigned int dim, double * coord,
					const double * incumbent)
{
  for (unsigned int i = 0; i < dim; i++)
    coord[i] = (incumbent[i] - scales_add[i]) * scales_mult[i];
}

// This method creates a link to the surrogate manager.
void FunctionControl::linkWithSurrogate(SurrogateControl & surr)
{
  surr_control = & surr;
}

// This method is called when the user wants to use the caches.
void FunctionControl::makeDatabases(const unsigned int dim,
				    const unsigned int out, const char * perm)
{
  // The 'CacheManager' object is created.
  if (perm != NULL)
    cache_control = new CacheControl(dim, out, perm);
}

// The display factor is reset.
void FunctionControl::setDisplay(const unsigned int display)
{
  display_factor = display;
  // These objects' display factors are also reset.
  if (gen_cons != NULL)
    gen_cons->setDisplay(display_factor);
  if (cache_control != NULL)
    cache_control->setDisplay(display_factor);
}

// showInput -- Writes FunctionControl's variables in a file.
void FunctionControl::showInput(const unsigned int dim, ofstream & fout)
{
  if (!fout.is_open())
    return;
  fout << "\nScaling:\n--------\n";
  switch(use_scaling)
    {
      case 0  : fout << "No scaling\n";
	        break;
      case 1  : fout << "Automatic\n";
	        break;
      case 2  : fout << "User-provided multiplication vector\n";
	        break;
      case 3  : fout << "User-provided bounds, a = " << low;
	        fout << ", b = " << up << "\n";
		break;
      default : cout << "This is not a scaling choice!\n";
    }
  if (use_scaling > 0)
    showScales(dim, fout);
  if (gen_cons != NULL)
    gen_cons->showInput(fout);
}

// showOutput -- Writes FunctionControl's outputs in a file.
void FunctionControl::showOutput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  fout << "Truth evaluations: " << truth_evaluations << "\n";
  fout << "New truth evaluations: " << new_truth_evaluations << "\n";
  if (cache_control != NULL)
    cache_control->showOutput(fout);
}

// This method writes the history legend, at the top of the history file.
void FunctionControl::writeHistoryLegend(ofstream & history)
{
  // The item for the surrogate value is written, if needed.
  if (surr_control != NULL)
    history << "surr(x)" << "\t";

  // The items for the general constraints values are written, if needed.
  if (gen_cons != NULL)
    {
      unsigned int nb = gen_cons->getGenConsNb();
      for (unsigned int i = 1; i <= nb; i++)
	history << "g" << i << "(x)" << "\t";
      // The item for h(x) is written.
      history << "h(x)" << "\n\n";
    }
}

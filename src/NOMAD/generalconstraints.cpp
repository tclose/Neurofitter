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

// generalconstraints.cpp -- GeneralConstraints class methods.

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdlib.h>
using namespace std;
#include "defines.h"
#include "generalconstraints.h"

// Private methods.
// ----------------

/* This method creates the string that will be used as a system call to a
   general constraint black box. */
char * GeneralConstraints::createBlackBoxString(char * file, const char * dir,
						const char * box,
						const char * txt)
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

/* This method computes h(x), the constraint violation function which is an
   aggregate of the general constraint functions gi(x). The constraint
   violation function has these properties:
   - h(x) = 0 if x is feasible.
   - h(x) > 0 if x is unfeasible. */
double GeneralConstraints::calculateHx(const double coord[],
				       bool startingPoint, unsigned int pos)
{
  unsigned int i;
  double Hx = 0.0;
  double temp, val;
  for (i = 0; i < gen_constraints_nb; i++)
    {
      val = coord[pos++];
      temp = 0.0 > val ? 0.0 : val;
      switch(filter_norm)
	{
	  case 0  : Hx = Hx < temp ? temp : Hx;
                    break;
	  case 1  : Hx += temp;
                    break;
	  case 2  : Hx += pow(temp, 2);
	            break;
	  default : cerr << "This is not a norm!\n";
	}
    }
  /* h(x) is computed. Its value is changed in these cases:
     - h(x) is set to '0' if its value is less than 'hmin', a user-defined
       parameter.
     - h(x) is set to 'infinity' if it is over 'hmax', another user-defined
       parameter. */
  if ((Hx != 0.0) && (Hx < hmin))
    {
      if (display_factor >= 7)
	{
	  cout << "h(x) < hmin: " << Hx << " < " << hmin << "\n";
	  cout << "h(x) is reset to 0.0\n";
	}
      Hx = 0.0;
    }
  /* There's an 'else' here because hmin must always be strictly inferior than
     hmax. Thus the two cases can't both be true, it's one or the other. */
  else
    if ((!startingPoint) && (Hx > hmax) )
      Hx = INFINIT;
  return Hx;
}

/* This method calculates the general constraints function values for a given
   point 'ap'. */
void GeneralConstraints::blackBoxRoutine(AugmentedPoint & ap, unsigned int pos)
{
  double val;
  for (current = gen_constraints_boxes.begin();
       current != gen_constraints_boxes.end(); current++)
    {
      val = ap.getOutput(pos);
      if (val == INFINIT)
	{
	  system(* current);
	  ifstream output(output_file, ios::in);
	  output >> val;
	  output.close();
	  ap.setOutput(pos, val);
	}
      pos++;
    }
}

//
void GeneralConstraints::initialize(const unsigned int gen_cons_nb)
{
  relative = false;
  display_factor = 0;
  gen_constraints_nb = gen_cons_nb;
  hmax = 100.0;
  hmin = 0.0001;
  // Default filter norm.
  filter_norm = 2;
}

/* The user enters his general constraints functions here.
   - 'ap' is the current iterate.
   - 'pos' indicates where the first ci(x) is located in the output array. */
void GeneralConstraints::userRoutine(AugmentedPoint & ap, unsigned int pos)
{
  /* In this example we have 3 general constraints functions. We'll calculate
     their values sequentially and put them in the output array of 'ap'. */
  double val;
  // We need the point's coordinates for the calculations.
  double * coord = ap.getPoint();

  // We check if the general constraint function value was calculated.
  val = ap.getOutput(pos);
  if (val == INFINIT)   // It wasn't calculated.
    {
      // First general constraint function.
      val = -coord[0] - coord[1] + 1.0;
      /* Each result must be stored in the output array of 'ap', at the correct
	 position. The output array 'coord' is built like this:

	 coord[0]: the point's truth value.
	 coord[1]: the point's surrogate function value, if a surrogate is
	           used. If not, the first general constraint function value.
	 coord[2]: first or second general constraint function value (depends
	           on what's in position 1).
	 ........
	 coord[n]: last general constraint function value. */
      ap.setOutput(pos, val);
    }
  pos++;

  // The process is repeated for the other general constraint functions.
  val = ap.getOutput(pos);
  if (val == INFINIT)
    {
      // Second general constraint function.
      val = -pow(coord[0], 2) + coord[1];
      ap.setOutput(pos, val);
    }
  pos++;

  val = ap.getOutput(pos);
  if (val == INFINIT)
    {
      // Third general constraint function.
      val = -pow(coord[1], 2) + coord[0];
      ap.setOutput(pos, val);
    }
}

// Public methods.
// ---------------

// This constructor is used with black boxes.
GeneralConstraints::GeneralConstraints(const char * dir, const char * input,
				       const char * output,
				   std::vector<const char *> & gen_constraints)
  : Subject()
{
  // The variables are initialized.
  initialize(gen_constraints.size());
  black_box_used = true;
  // The strings are created.
  input_file = new char[strlen(input) + 1];
  strcpy(input_file, input);
  output_file = new char[strlen(output) + 1];
  strcpy(output_file, output);
  for (std::vector<const char *>::iterator str = gen_constraints.begin();
       str != gen_constraints.end(); str++)
    {
      char * black_box = createBlackBoxString(black_box, dir, * str,
					      "output.txt");
      gen_constraints_boxes.push_back(black_box);
    }
  if (display_factor >= 9)
    {
      cout << "In GeneralConstraints' constructor\n";
      cout << "input_file = " << input_file << "\n";
      cout << "output_file = " << output_file << "\n";
      for (current = gen_constraints_boxes.begin();
	   current != gen_constraints_boxes.end(); current++)
	cout << * current << "\n";
      cout << "\n";
    }
}

// This constructor is used when there are no black boxes.
GeneralConstraints::GeneralConstraints(const unsigned int gen_cons_nb)
{
  // The variables are initialized.
  initialize(gen_cons_nb);
  input_file = 0;
  output_file = 0;
  black_box_used = false;
}

// Destructor.
GeneralConstraints::~GeneralConstraints()
{
  if (black_box_used)
    {
      delete [] input_file;
      delete [] output_file;
      for (current = gen_constraints_boxes.begin();
	   current != gen_constraints_boxes.end(); current++)
	delete * current;
      gen_constraints_boxes.clear();
    }
}

// This method returns hmax.
double GeneralConstraints::getHmax()
{
  return hmax;
}

// The filter parameters are received, the variables are set.
void GeneralConstraints::enterFilter(const bool choice, const double h_max,
				   const double h_min, const unsigned int norm)
{
  relative = choice;
  hmax = h_max;
  hmin = h_min;
  filter_norm = norm;
  notify();
}

// This method evaluates the general constraints function values and h(x).
void GeneralConstraints::evaluate(AugmentedPoint & ap, bool startingPoint)
{
  /* The integer variable 'pos' tells us at which position is stored the first
     general constraint function value in the point's output array. All the
     other general constraint values are stored sequentially after that. */
  unsigned int pos = ap.getPosition(2);
  // The proper general constraints evaluation routine is called.
  if (black_box_used)
    blackBoxRoutine(ap, pos);
  else
    userRoutine(ap, pos);
  // The general constraints values are calculated. It's time to calculate h(x)
  double Hx = calculateHx(ap.getTruthValues(), startingPoint, pos);
  // 'hmax' can be relative to the starting point's h(x).
  if ((startingPoint) && (relative) && (Hx > 0.0) && (Hx != INFINIT) )
    hmax = hmax * Hx;
  // The point's h(x) value is stored.
  ap.setHx(Hx);
}

// The display factor is reset.
void GeneralConstraints::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// show -- Outputs the GeneralConstraints object.
void GeneralConstraints::showInput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "\nNumber of general constraints: " << gen_constraints_nb << "\n";
    fout << "Constraint violation norm: ";
  switch (filter_norm)
    {
      case 1  : fout << "L1\n";
	        break;
      case 2  : fout << "L2 squared\n";
	        break;
      case 0  : fout << "L infinity\n";
	        break;
      default : fout << "That's not a norm!\n";
    }
  if (relative)
    fout << "hmax relative to starting point's h(x): ";
  else
    fout << "hmax set by the user: ";
  fout  << hmax << "\n";
  fout << "hmin: " << hmin << "\n";
}

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

// directionset.cpp -- DirectionSet class methods. Public methods of the class
// are called from the PollControl object, which has direct access to
// DirectionSet.

#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std;
#include "defines.h"
#include "directionset.h"

// Private methods.
// ----------------

// This method receives a real number and rounds it to the 4th decimal before
// returning it.
double DirectionSet::roundNumber(double temp)
{
  temp = floor(temp * 10000 + 0.5);
  return temp / 10000;
}

// This method checks if a permanent direction is already built for 'index'. If
// the direction is found, its position in 'permanent_dirs' is returned. If
// it's not found '-1' is returned.
int DirectionSet::findIndex(const unsigned int max_dim, const double index)
{
  double perm_dir_index;
  int position = 0;
  vector<vector<double> >::iterator perm_dir;
  for (perm_dir = permanent_dirs.begin(); perm_dir != permanent_dirs.end();
       perm_dir++)
    {
      // In each vector of the 'permanent_dirs' structure, the index is stored
      // at position 'max_dim + 1'.
      perm_dir_index = (* perm_dir)[max_dim + 1];
      // In this case we know that there's no direction containing 'index' in
      // 'permanent_dirs'.
      if (perm_dir_index > index)
	return -1;
      else
	// The direction is found: its position is returned.
	if (perm_dir_index == index)
	  {
	    if (index == 0)
	      {
		// The permanent poll direction for index '0' will be
		// regenerated: The first direction is erased.
		if (display_factor >= 7)
		  cout << "Erasing first direction...\n";
		permanent_dirs.erase(permanent_dirs.begin());
	      }
	    return position;
	  }
      // 'position' is incremented.
      position++;
    }
  // The position wasn't found.
  return -1;
}

// This method builds a permanent dense poll direction.
int DirectionSet::makePermanentDir(const unsigned int max_dim,
				   const double index,
				   const double index_factor)
{
  // The vector 'b_l' is built. It will contain a vector of random numbers,
  // followed by 'i_l' in the last position.
  double a, p;
  std::vector<double> b_l;
  std::vector<double>::iterator temp;
  b_l.reserve(max_dim + 2);

  // The 'rand()' function generates a random number between 0 and RAND_MAX.
  // 'i_l' is a real number between 0 and 'max_dim'.
  double i_l = floor(rand() / r_max * max_dim);
  if (display_factor >= 7)
    cout << "Building new permanent direction, i_l = " << i_l << "\n";

  // In this loop 'b_l' is filled with random numbers.
  for (p = 0.0; p < max_dim; p++)
    {
      // 'a' is either -1.0 or 1.0, with equal probability.
      a = (rand() / r_max < 0.5) ? -1.0 : 1.0;
      // We have this case when 'p' is equal to 'i_l'.
      if (p == i_l)
	b_l.push_back(floor(a * index_factor));
      else
	// In all other cases a random number is inserted at the end of 'b_l'.
	b_l.push_back(floor(a * ((rand() / r_max) * index_factor - 1.0)));
    }

  // The 'i_l' value is inserted at the end of 'b_l, at position 'max_dim'. The
  // index value is inserted right after, at position 'max_dim + 1'.
  b_l.push_back(i_l);
  b_l.push_back(index);

  // The vector is shown.
  if (display_factor >= 7)
    {
      cout << "b_l = ";
      for (temp = b_l.begin(); temp != b_l.end(); temp++)
	cout << * temp << " ";
      cout << "\n";
    }

  // In the next section the 'b_l' vector is inserted at the correct position
  // in the 'permanent_dirs' structure.

  // This variable will indicate the position in 'permanent_dirs' where 'b_l'
  // is inserted.
  int position = 0;

  // If 'permanent_dirs' is empty, 'b_l' is inserted.
  if (permanent_dirs.empty())
    {
      if (display_factor >= 7)
	cout << "'permanent_dirs' empty! Insertion at the start.\n";
      permanent_dirs.push_back(b_l);
    }
  else   // 'permanent_dirs' is not empty.
    // If 'index' equals zero, 'b_l' is inserted at the start.
    if (index == 0)
      {
	if (display_factor >= 7)
	  cout << "'index' = 0! Insertion at the start.\n";
	// The permanent poll direction for index '0' was regenerated: The new
	// 'b_l' vector is inserted in its place.
	permanent_dirs.insert(permanent_dirs.begin(), b_l);
      }
    else
      // 'index' is greater than zero. 'b_l' is inserted at the correct
      // position.
      {
	double perm_dir_index;
	vector<vector<double> >::iterator perm_dir;
	for (perm_dir = permanent_dirs.begin();
	     perm_dir != permanent_dirs.end(); perm_dir++)
	  {
	    // In each vector of the 'permanent_dirs' structure, the index is
	    // stored at position 'max_dim + 1'.
	    perm_dir_index = (* perm_dir)[max_dim + 1];
	    if (perm_dir_index > index)
	      break;
	    // 'position is incremented.
	    position++;
	  }
	// 'b_l' is inserted before 'perm_dir'.
	permanent_dirs.insert(perm_dir, b_l);
	if (display_factor >= 7)
	  cout << "'index' > 0, insertion at position " << position << "\n";
      }

  // The 'b_l' vector is deleted.
  b_l.clear();

  // 'position' is returned.
  return position;
}

// This method builds a specific kind of poll directions. All numbers in the
// array are 0.0 except one, which has a value of "val" and is at position
// "pos" in the array.
void DirectionSet::buildDirection(const unsigned int dim,
				  const unsigned int pos, const int val)
{
  std::vector<double> v;
  for (unsigned int i = 0; i < dim; i++)
    {
      if (i == pos)
	// We put "val" at the end of the vector.
	v.push_back(val);
      else
	// We put 0.0 at the end of the vector.
	v.push_back(0.0);
    }
  // The direction is added to the set.
  makePollDir(v, polldirs);
  v.clear();
}

// This method builds hybrid poll directions, constructed by mixing the user's
// original polldirs choice (n+1, uniform n+1) with 0.0 values in the
// dimensions where the boundaries were approached. Let's continue the example
// that we began in "build_orthogonalPollDirs(...)" above. This method would
// build these 3 vectors, adding them to "polldirs":
//           (-1.0, 0.0, -1.0)
//	     (1.0, 0.0, 0.0)
//	     (0.0, 0.0, 1.0)
// The second element of each vector is 0.0, because the proximity vector is
// (0, 1, 0). Then a temporary polldir set would be built, with the call
// 'make_nplus1PollDirs(3 - 1, temp_polldirs)', resulting in this set:
//           (-1.0, -1.0)
//	     (1.0, 0.0)
//	     (0.0, 1.0)
// The first and third columns of "polldirs" are the first and second columns
// of the temporary set.
void DirectionSet::buildHybridPollDirs(int * prox, const unsigned int dim,
				       const unsigned int count)
{
  // First we build a set of temporary directions, with a dimension of
  // 'dim - count', which is the problem dimension minus the number of bounds
  // near the trial point (represented by the 'count' variable).
  std::deque<Direction> temp_polldirs;
  std::deque<Direction>::iterator temp_dir;
  if (choice == 1)   // This choice is n+1 directions.
    makeNPlus1PollDirs(dim - count, temp_polldirs);
  else
    if (choice == 2)   // This choice is uniform n+1 directions.
      makeUniformPollDirs(dim - count, temp_polldirs);

  std::vector<double> v, temp;
  std::vector<double>::iterator iter;
  // The temporary set is built: we make "temp_dir" point its first element.
  temp_dir = temp_polldirs.begin();
  // The hybrid directions are built in this loop.
  for (unsigned int i = 0; i < (dim - count + 1); i++)
    {
      // We put the current vector into "temp".
      temp = (* temp_dir).getDirection();
      iter = temp.begin();
      // A complete direction is built in this loop.
      for (unsigned int j = 0; j < dim; j++)
	{
	  // If we're near this boundary ('prox[j] == 1'), we add 0.0 at the
	  // vector's end.
	  if (prox[j] == 1)
	    v.push_back(0.0);
	  else
	    {
	      // The current element pointed by "*iter" is added at the
	      // vector's end.
	      v.push_back(*iter);
	      // We advance "iter" to the next element of "temp".
	      *iter++;
	    }
	}
      // We advance "temp_dir" to the next direction.
      temp_dir++;
      // The direction is added to the set.
      makePollDir(v, polldirs);
      // "v" is erased.
      v.clear();
    }
  // The temporary set of directions is erased.
  temp_polldirs.clear();
}

// At first we build the orthogonal poll directions, i.e. those perpendicular
// to the nearby boundary. As an example, let's say we're in R^3 (dim = 3),
// the user chose n+1 polldirs (choice = 1), the proximity vector is (0, 1, 0)
// so count = 1. This method would build the following directions (with the 
// 'build_direction(...)' calls:
//           (0.0, 1.0, 0.0)
//	     (0.0, -1.0, 0.0)
void DirectionSet::buildOrthogonalPollDirs(int * prox, const unsigned int dim,
					   const unsigned int count)
{
  unsigned int i;
  unsigned int position = 0;
  bool another_pass = true;
  // In this loop we build two directions (one with 1.0, one with -1.0) for
  // each bound that the trial point approaches.
  for (i = 0; i < count; i++)
    {
      // In this loop we find the next position in the "prox" array where
      // 'prox[position] == 1'.
      while ((another_pass) && (position < dim))
	{
	  if (prox[position] == 1)
	    {
	      // The value in the array is changed (we don't want to repeat
	      // the position next time we're in this loop).
	      prox[position] = 10;
	      // We'll exit this loop.
	      another_pass = false;
	    }
	  else
	    // Else we move to the next value of the "prox" array.
	    position++;
	}
      // We build the two directions.
      buildDirection(dim, position, 1);
      buildDirection(dim, position, -1);
      // We'll re-enter this loop, unless 'position >= dim'.
      another_pass = true;
    }
  // We rearrange the "prox" array like it was at the start, changing 10 values
  // back to 1.
  for (i = 0; i < dim; i++)
    if (prox[i] == 10)
      prox[i] = 1;
  // Now we build the hybrid directions.
  buildHybridPollDirs(prox, dim, count);
}

// This method creates the correct number of directions for MADS; the
// directions are initialized to '0.0' values.
void DirectionSet::makeDensePollDirs(const unsigned int dimension)
{
  unsigned int how_many = (choice == 3) ? dimension * 2 : dimension + 1;
  // An empty vector is created, called "v".
  std::vector<double> v;
  // Each of the 'how_many' passes in the loop creates a poll direction.
  for (unsigned int i = 0; i < how_many; i++)
    {
      // Vector building loop. "v" starts empty, and each "v.push_back(...)"
      // call adds an element at the end.
      for (unsigned int j = 0; j < dimension; j++)
	// All directions are initialized with '0.0' values.
	v.push_back(0.0);
      // The vector is built: we add it to the set of poll directions. Then
      // the vector elements are erased by the "v.clear" call.
      makePollDir(v, polldirs);
      v.clear();
    }
  if (display_factor >= 9)
    {
      cout << "Initializing dense poll direction set...\n";
      showDirections();
    }
}

// This method builds "n+1" poll directions, n being the problem dimension.
// Each poll direction is a C++ STL vector. For example, if "n" equals 3, the
// following poll directions will be created:
//     -1.0 -1.0 -1.0
//      1.0  0.0  0.0
//      0.0  1.0  0.0
//      0.0  0.0  1.0
void DirectionSet::makeNPlus1PollDirs(const unsigned int dimension,
				      std::deque<Direction> & p)
{
  // An empty vector is created, called "v".
  std::vector<double> v;
  // Each of the (dimension + 1) iterations creates a poll direction.
  for (unsigned int i = 0; i < (dimension + 1); i++)
    {
      // Vector building loop. "v" starts empty, and each "v.push_back( )" 
      // call adds an element at the end.
      for (unsigned int j = 0; j < dimension; j++)
	{
	  // The last vector is composed entirely of "-1.0" values.
	  if (i == 0)
	    v.push_back(-1.0);
	  else
	    {
	      // The diagonal is composed of "1.0" values.
	      if (i == j + 1)
		v.push_back(1.0);
	      // "0.0" values are put in the other positions.
	      else
		v.push_back(0.0);
	    }
	}
      // The vector is built: we add it to the set of poll directions, "p"
      // (received as parameter). Then the vector elements are erased by the
      // "v.clear" call.
      makePollDir(v, p);
      v.clear();
    }
}

// This method builds a poll direction.
void DirectionSet::makePollDir(const std::vector<double> v,
			       std::deque<Direction> & p)
{
  // The poll direction is created with "v", the vector of doubles received as
  // parameter.
  Direction * pdir = new Direction(v);
  // "pdir" is added to the set of poll directions, "p".
  p.push_back(* pdir);
  // The pointer is destroyed.
  delete pdir;
}

// This method calls one of the poll directions building methods, according to
// the parameter "choice". It passes to it "dimension" as an argument.
void DirectionSet::makePollDirSet(const unsigned int dimension)
{
  switch (choice)
    {
      case 0  :  make2NPollDirs(dimension);
                 break;
      case 1  :  makeNPlus1PollDirs(dimension, polldirs);
                 break;
      case 2  :  makeUniformPollDirs(dimension, polldirs);
                 break;
      case 3  :  break;
      case 4  :  break;
      default :  cout << "That's not a choice!\n";
    }
}

// This method builds "2n" poll directions, n being the problem dimension. 
// Each poll direction is a C++ STL vector. For example, if "n" equals 3, the
// following poll directions will be created:
//      1.0  0.0  0.0
//      0.0  1.0  0.0
//      0.0  0.0  1.0
//     -1.0  0.0  0.0
//      0.0 -1.0  0.0
//      0.0  0.0 -1.0
void DirectionSet::make2NPollDirs(const unsigned int dimension)
{
  // An empty vector is created, called "v".
  std::vector<double> v;
  // Each of the (dimension * 2) iterations creates a poll direction.
  for (unsigned int i = 0; i < (dimension * 2); i++)
    {
      // Vector building loop. "v" starts empty, and each "v.push_back( )" 
      // call adds an element at the end.
      for (unsigned int j = 0; j < dimension; j++)
	{
	  // if (j equals (i modulo dimension)) : case when we don't add "0.0"
	  // at the end of the vector.
	  if (j == (i % dimension))
	    {
	      // for vectors 0, 1, ..., n we add "1.0" at the end.
	      if (i < dimension)
		v.push_back(1.0);
	      // for vectors n+1, n+2, ..., n*2 we add "-1.0" at the end.
	      else
		v.push_back(-1.0);
	    }
	  // in all other cases we add "0.0" at the end.
	  else
	    v.push_back(0.0);
	}
      // The vector is built: we add it to the set of poll directions.
      // Then the vector elements are erased by the 'v.clear()' call.
      makePollDir(v, polldirs);
      v.clear();
    }
}

// This method builds "n+1" poll directions with uniform angles, n being the
// problem dimension. Each poll direction is a C++ STL vector. For example, if
// "n" equals 3, the following poll directions will be created:
//      0.7071  0.4082  0.2887
//     -0.7071  0.4082  0.2887
//      0      -0.8165  0.2887
//      0       0      -0.8660 
// To learn more about how the uniform angles are generated, see:
// "P. Alberto, F. Nogueira, H. Rocha, and L. N. Vicente, Pattern Search 
// Methods for Molecular Geometry Problems, September 2000", especially
// Section 3.1, "Positive bases with uniform angles".
// The article can be downloaded at:
// http://www.mat.uc.pt/~lvicente/papers/reports.html
void DirectionSet::makeUniformPollDirs(const unsigned int dimension,
				       std::deque<Direction> & p)
{
  // An empty vector is created, called "v".
  std::vector<double> v;
  double temp;
  double n = dimension;
  // Each of the (dimension + 1) iterations creates a poll direction.
  for (double i = 1.0; i < (dimension + 2); i++)
    {
      // Vector building loop. "v" starts empty, and each "v.push_back( )" 
      // call adds an element at the end.
      for (double j = 1.0; j < (dimension + 1); j++)
	{
	  if (i > (j + 1))
	    temp = 0.0;
	  else
	    if (i == (j + 1))
	      temp = - sqrt(j / (j + 1));
	    else
	      temp = 1 / (sqrt(j * (j + 1)));
	  // We multiply 'temp' by "sqrt((n + 1) / n", so that the final vector
	  // will have a lenght of one.
	  temp = temp * sqrt((n + 1) / n);
	  // We add 'temp' to 'v'.
	  v.push_back(roundNumber(temp));
	}
      // The vector is built: we add it to the set of poll directions, "p"
      // (received as parameter). Then the vector elements are erased by the
      // "v.clear" call.
      makePollDir(v, p);
      v.clear();
    }
}

// This method outputs the set of poll directions.
void DirectionSet::showDirections()
{
  unsigned int n = 0;
  for (direction = polldirs.begin(); direction != polldirs.end(); direction++)
    {
      cout << "Poll Direction " << n++ << ": ";
      (* direction).show();
    }
  cout << "\n";
}

// This method writes the set of permanent poll directions to standard output.
void DirectionSet::showPermanentDirs()
{
  unsigned int n = 0;
  std::vector<vector<double> >::iterator perm_dir;
  std::vector<double>::iterator temp;
  for (perm_dir = permanent_dirs.begin(); perm_dir != permanent_dirs.end();
       perm_dir++)
    {
      cout << "Permanent direction " << n++ << ": ";
      for (temp = (* perm_dir).begin(); temp != (* perm_dir).end(); temp++)
	cout << * temp << " ";
      cout << "\n";
    }
  cout << "\n";
}

// Public methods.
// ---------------

// Default constructor. DirectionSet is created at start-up by PollControl.
DirectionSet::DirectionSet(const unsigned int ch, const unsigned int dimension)
{
  display_factor = 0;
  // The variable 'choice' is set to 'ch'.
  choice = ch;
  // 'r_max' contains the largest possible random number generated.
  r_max = RAND_MAX + 1.0;
  // A default set of directions (GPS 2 * n) is built.
  direction = polldirs.begin();
  makePollDirSet(dimension);
}

// Destructor.
DirectionSet::~DirectionSet()
{
  // The poll directions are erased.
  if (!polldirs.empty())
    polldirs.clear();
  // The permanent directions are erased.
  if (!permanent_dirs.empty())
    permanent_dirs.clear();
}

// This method returns the value of the variable 'choice'.
int DirectionSet::getChoice()
{
  return choice;
}

// This method sends the address of 'polldirs' to PollControl.
std::deque<Direction> & DirectionSet::getPollDirs()
{
  return polldirs;
}

// This method builds a new set of dense poll directions at every iteration.
// It's called if the user chose a MADS set of poll directions.
void DirectionSet::makeDenseSet(const unsigned int max_dim, const double delta,
				const double factor)
{
  // This variable will be used to multiply the poll directions when the mesh
  // size is greater than one.
  double poll_size_correction = 1.0;

  // The poll directions are erased and rebuilt.
  polldirs.clear();
  makeDensePollDirs(max_dim);

  // The 'index' variable is calculated. It has these values:
  // - index < 0 : when delta > 1
  // - index = 0 : when delta = 1
  // - index > 0 : when delta < 1
  double index = -floor((log(delta) / log(factor)) + 0.5);

  // When the mesh size is greater than one, 'index' is negative: we have to
  // put it back to zero, since it's used to index the 'permanent_dirs' array.
  // Furthermore a poll size correction factor is calculated; the poll
  // directions will be multiplied by this factor later on.
  if (index < 0.0)
    {
      poll_size_correction = pow(factor, index / 2.0);
      index = 0.0;
    }

  if (display_factor >= 7)
    cout << "Building dense set, index = " << index << "\n";

  // We check if a direction is already built for the current index.
  int position = findIndex(max_dim, index);
  if (display_factor >= 7)
    (position == -1) ? cout << "Direction not found!\n"
      : cout << "Direction found!\n";
  // This variable will be used to fill the dense poll directions.
  double index_factor = pow(sqrt(factor), index);
  // If 'index' is equal to zero (delta is greater or equal than one), or the
  // direction wasn't found ('position' equals -1) a permanent direction must
  // be built.
  if ((index == 0.0) || (position == -1))
    {
      position = makePermanentDir(max_dim, index, index_factor);
      if (display_factor >= 7)
	cout << "position = " << position << "\n";
    }
  else
    if (display_factor >= 7)
      cout << "\nPermanent direction already exists!\n";

  if (display_factor >= 5)
    showPermanentDirs();

  // The indice vectors are built.
  std::vector<unsigned int> rand_p, rand_q;
  std::vector<unsigned int>::iterator temp;
  rand_p.reserve(max_dim - 1);
  rand_q.reserve(max_dim);

  // The indice vectors are filled with '0, 1, ..., max_dim - 1'.
  unsigned int p;
  for (p = 0; p < max_dim; p++)
    {
      // 'rand_p' will not have 'i_l' (a random number between
      // '0, max_dim - 1') as one of its elements.
      if (p != permanent_dirs[position][max_dim])
	rand_p.push_back(p);
      rand_q.push_back(p);
    }

  // The vectors are randomly shuffled.
  std::random_shuffle(rand_p.begin(), rand_p.end());
  std::random_shuffle(rand_q.begin(), rand_q.end());

  // The vectors are shown.
  if (display_factor >= 9)
    {
      cout << "rand_p = ";
      for (temp = rand_p.begin(); temp != rand_p.end(); temp++)
	cout << * temp << " ";
      cout << "\nrand_q = ";
      for (temp = rand_q.begin(); temp != rand_q.end(); temp++)
	cout << * temp << " ";
      cout << "\n";
    }

  // From this point onward, the dense set of poll directions is built.
  double a, b;
  unsigned int i, j;
  polldirs[rand_q[max_dim - 1]].set(max_dim - 1,
		 permanent_dirs[position][max_dim - 1] * poll_size_correction);
  for (j = 0; j < max_dim - 1; j++)
    {
      polldirs[rand_q[max_dim - 1]].set
	(j, permanent_dirs[position][j] * poll_size_correction);
      polldirs[rand_q[j]].set((int)permanent_dirs[position][max_dim], 0.0);
      a = (rand() / r_max < 0.5) ? -1.0 : 1.0;
      polldirs[rand_q[j]].set
	(rand_p[j], a * index_factor * poll_size_correction);
      for (i = 0; i < j; i++)
	polldirs[rand_q[j]].set(rand_p[i], 0.0);
      for (i = j + 1; i < max_dim - 1; i++)
	{
	  a = (rand() / r_max < 0.5) ? -1.0 : 1.0;
	  polldirs[rand_q[j]].set
	    (rand_p[i],
	     floor(a * ((rand() / r_max) * index_factor - 1.0))
	     * poll_size_correction);
	}
    }

  // The directions n+1, ..., 2*n are built (the user chose MADS 2*n
  // directions). They are necessary so that the set of poll directions forms
  // a positive basis.
  if (choice == 3)
    {
      // Each direction from n+1 to 2*n is the negative of its related
      // direction from 1 to n.
      for (j = 0; j < max_dim; j++)
	for (i = 0; i < max_dim; i++)
	  {
	    a = polldirs[j].get(i);
	    a = (a == 0.0) ? 0.0 : -a;
	    polldirs[max_dim + j].set(i, a);
	  }
    }
  else
    // The directions n+1 is  built (the user chose MADS n+1 directions). It is
    // necessary so that the set of poll directions forms a positive basis.
    {
      // The direction n+1 is the negative of the sum of vectors '1, ..., n'.
      for (j = 0; j < max_dim; j++)
	for (i = 0; i < max_dim; i++)
	  {
	    a = polldirs[max_dim].get(i);
	    b = polldirs[j].get(i);
	    polldirs[max_dim].set(i, a - b);
	  }
    }

  if (display_factor >= 5)
    showDirections();
}

// This method is called when the trial point returned from the poll is near a
// boundary: we have to build a new set of poll directions.
void DirectionSet::rebuildPollDirSet(int * prox, const unsigned int dim)
{
  if (display_factor >= 5)
    cout << "Rebuilding polldirs set...";
  unsigned int count = 0;
  // We count how many bounds are near the trial point.
  for (unsigned int i = 0; i < dim; i++)
    // The trial point is near a boundary when prox[i] == 1.
    if (prox[i] == 1)
      count++;
  // The former set of poll directions is erased.
  polldirs.clear();
  // If trial is not near a boundary, we build the original polldir set.
  if (count == 0)
    makePollDirSet(dim);
  else
    // If trial is near at least (dim - 1) bounds, we build 2n polldirs.
    if (count >= dim - 1)
      make2NPollDirs(dim);
    else
      // We have to build an hybrid polldirs set, mixing directions of the
      // user's choice with orthogonal directions.
      buildOrthogonalPollDirs(prox, dim, count);
  if (display_factor >= 5)
    {
      cout << "New polldirs set:\n";
      showDirections();
    }
}

// The display factor is reset.
void DirectionSet::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// showInput -- Outputs the user's poll directions choice.
void DirectionSet::showInput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  switch (choice)
    {
      case 0  :  fout << "Standard, 2 * n ";
	         break;
      case 1  :  fout << "Standard, n + 1 ";
	         break;
      case 2  :  fout << "Standard, uniform ";
	         break;
      case 3  :  fout << "Dense, 2 * n ";
	         break;
      case 4  :  fout << "Dense, n + 1 ";
	         break;
      default :  cout << "That's not a choice!\n";
    }
  fout << "directions.\n\n";
}

// This method is called when there is a success in the poll , i.e. when the
// point returned by the poll has a better truth function value than the 
// incumbent.
void DirectionSet::success(const unsigned int index)
{
  // The poll directions are ordered by success. The current poll direction
  // (pointed to by 'direction') is inserted at the start of the list, then
  // erased at his current position. The iterator 'direction' is moved to the
  // start of the list.
  if (display_factor >= 5)
    cout << "Poll direction #" << index << " is pushed to the front: ";

  // The direction at position 'index' is pushed to the front of the polldirs
  // container.
  polldirs.push_front(polldirs[index]);

  // The 'direction' iterator is advanced until it points the copied direction.
  direction = polldirs.begin();
  for (unsigned int i = 0; i <= index; i++)
    direction++;

  // The copied direction is shown.
  if (display_factor >= 5)
    (* direction).show();

  // The copied direction is erased (it's been safely copied so it's safe).
  polldirs.erase(direction);

  // The directions set is shown.
  if (display_factor >= 5)
    showDirections();
}

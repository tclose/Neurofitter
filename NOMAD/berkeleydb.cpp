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

/* berkeleydb.cpp -- BerkeleyDB class methods. For all things related to DbEnv,
   Db or Dbt objects, see the Berkeley DB online documentation at
   "http://www.sleepycat.com/docs/index.html". */

#include <cstring>
#include <iostream>
using namespace std;
#include "berkeleydb.h"

/* Database ordering function. We give it two points to order, "a" and "b". The
   points are stored in DBT objects in key/data pairs. */
extern "C" {
  static int order(DB *, const DBT * a, const DBT * b)
  {
    // First we build two arrays, ax and ay.
    unsigned int sz = a->size / sizeof(double);
    double * ax = new double[sz];   memcpy(ax, a->data, a->size);
    double * ay = new double[sz];   memcpy(ay, b->data, b->size);
    /* The points are compared, number by number. We exit the loop as soon as
       an inequality is found. */
    int ans = 0;
    for (unsigned int i = 0; i < sz; i++)
      {
	if (ax[i] < ay[i])
	  {
	    ans = -1;
	    break;
	  }
	if (ax[i] > ay[i])
	  {
	    ans = 1;
	    break;
	  }
      }
    // We free the memory allocated to the arrays.
    delete [] ax;
    delete [] ay;
    /* We return an integer, equal to -1, 0 or 1:
       -1: a < b
       0: a = b
       1: a > b */
    return ans;
  }
}

// Public methods.
// ****************************************************************************

/* Constructor. The database is created if the user chooses to use a cache,
   during data entry (in the user interface). */
BerkeleyDB::BerkeleyDB(const char * name, unsigned keySize, unsigned datSize)
  : Database(name, keySize, datSize),     // explained in "database.h".
    env(new DbEnv(DB_CXX_NO_EXCEPTIONS)), /* the database environment is built,
					     with a flag related to error
					     handling. */
    dbase(new Db(0,0))   /* The DB object is created as a stand-alone database
			   (1st parameter), and no flag (2nd parameter). */
{
  int ret = 0;
  bool file_exists = false;
  // The cache evaluations counter is initialized to zero.
  cache_hits = 0;

  // TEST
  Db * test_db = new Db(0, 0);

  // We set the Btree comparison function to "order", described above.
  dbase->set_bt_compare(order);
  test_db->set_bt_compare(order);

  ifstream fin(filename);
  if (fin.is_open())
    file_exists = true;
  fin.clear();
  fin.close();

  // We verify that the database is not corrupted.
  if ((file_exists) && ((ret = test_db->verify(filename, NULL, NULL, 0)) != 0))
    {
      cerr << "Database corrupted!!! Run terminated.\n";
      exit(1);
    }
  else
    {
      /* We open the databse, with these parameters:
	 - filename: name of the physical file on disk.
	 - NULL: indicates that we have a single logical file in the physical
	   file.
	 - DB_BTREE: the database is of type BTREE, it uses a Btree to store
	   data.
	 - DB_CREATE: create the database if it doesn't already exist.
	 - 0664: Unix mode -rw-rw-r-- . */
      if ((ret = dbase->open(NULL, filename, NULL, DB_BTREE, DB_CREATE, 0664))
	  != 0)
	cerr << "Open failed: " << db_strerror(ret) << "\n";
    }

  // The 'test_db' handle is deleted.
  delete test_db;
  test_db = NULL;
}

// Destructor. The database is closed, the pointers are destroyed.
BerkeleyDB::~BerkeleyDB()
{
  dbase->close(0);
  env->close(0);
  delete dbase;
  delete env;
}

// Returns the cache evaluations counter.
unsigned int BerkeleyDB::getHits()
{
  return cache_hits;
}

// This method checks if the database contains a point.
bool BerkeleyDB::contains(void * inp, void * utp)
{
  // First two Dbt objects are created, "key" and "dat".
  Dbt key(inp, inSize);
  Dbt dat(utp, utSize);
  /* The flag "DB_DBT_USERMEM" indicates that "dat" must reference memory that
     is at least "ulen" bytes in lenght. */
  dat.set_flags(DB_DBT_USERMEM);
  // The "ulen" value is set.
  dat.set_ulen(utSize);
  /* The "Db::get(...)" function will try to find the point in the cache, by
     ordering keys. It returns '0' if the point was found, '1' otherwise.*/
  int ret;
  if ((ret = dbase->get(0, & key, & dat, 0)) != 0)
    {
      if (ret != DB_NOTFOUND)
	cerr << "Get failed: " << db_strerror(ret) << "\n";
      return (1);
    }
  else
    {
      // The point was found: the counter of hits is incremented.
      cache_hits++;
      return (0);
    }
}

// This method removes a point from the cache.
bool BerkeleyDB::remove(void * inp)
{
  Dbt key(inp, inSize);
  int ret;
  if ((ret = dbase->del(0, & key, 0)) != 0)
    {
      cerr << "Del failed: " << db_strerror(ret) << "\n";
      return  (1);
    }
  else
    return (0);
}

// This method inserts a point in the cache.
bool BerkeleyDB::insert(void * inp, void * utp)
{
  // First two Dbt objects are created, "key" and "dat".
  Dbt key(inp, inSize);
  Dbt dat(utp, utSize);
  // The "Db::put(...)" function will insert the point in the cache.
  int ret;
  if ((ret = dbase->put(0, & key, & dat, 0)) != 0)
    {
      cerr << "Put failed: " << db_strerror(ret) << "\n";
      return (1);
    }
  else
    return (0);
}

//
int BerkeleyDB::sync()
{
  int ret = dbase->sync(0);
  if (ret != 0)
    {
      cerr << "Sync failed: " << db_strerror(ret) << "\n";
      return(1);
    }
  else
    return(0);
}

//
int BerkeleyDB::truncate()
{
  u_int32_t countp = 0;
  int ret = dbase->truncate(NULL, & countp, 0);
  if (ret != 0)
    {
      cerr << "Truncate failed: " << db_strerror(ret) << "\n";
      return(1);
    }
  else
    return(0);
}

// show -- Outputs the BerkeleyDB object.
void BerkeleyDB::showOutput(ofstream & fout)
{
  if (!fout.is_open())
    return;
  fout << cache_hits << "\n";
}

//
void BerkeleyDB::continueRun()
{
  cache_hits = 0;
}

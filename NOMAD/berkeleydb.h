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

/* berkeleydb.h -- class definition for BerkeleyDB. The methods of this class
   are called from FunctionControl. This class manages the NOMAD database. It
   uses the Open Source embedded database system Berkeley DB, by Sleepycat
   Software. Berkeley DB must be downloaded from "http://www.sleepycat.com/"
   and installed on your system for this software to work. */

#ifndef _BERKELEYDB_H
#define _BERKELEYDB_H

#include <db_cxx.h>
#include <fstream>
using namespace std;
#include "database.h"

// BerkeleyDB has public inheritance from the Database class.
class BerkeleyDB: public Database
{
/* This class has the following data:
   - a counter of cache hits.
   - a pointer to a database environment object, "env".
   - a pointer to the Berkeley DB database, "dbase". */
private:
  unsigned int cache_hits;
  DbEnv * env;
  Db * dbase, * test_db;

  // Public methods: described in "berkeleydb.cpp" and "database.h"
public:
  BerkeleyDB(const char * name, unsigned keySize, unsigned datSize);
  ~BerkeleyDB();
  unsigned int getHits();
  bool contains(void * inp, void * utp = 0);
  bool remove(void * inp);
  bool insert(void * inp, void * utp);
  int sync();
  int truncate();
  void showOutput(ofstream & fout);
  void continueRun();
};
#endif

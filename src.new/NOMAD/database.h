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

/* database.h -- class definition for Database, which is a parent class to
   BerkeleyDB. */
#ifndef _DATABASE_H
#define _DATABASE_H

#define Real double

enum DatabaseOpState {FOUND, NOTFOUND, ERROR};

class Database
{
  /* The class methods are described in "berkeleydb.C", except for the class
     constructor described further below. */
public:
  Database(const char * name, unsigned keySize, unsigned datSize);
  virtual ~Database() {}
  virtual bool contains(void * inp, void * utp = 0) = 0;
  virtual bool remove(void * inp) = 0;
  virtual bool insert(void * inp, void* utp) = 0;

private:

/* DATABASE has the following data:
   - a string variable, the database file name.
   - "inSize" is the size in bytes of the point's coordinates. It is equal to 
   'sizeof(double) * dimension'.
   - "utSize" is the size in bytes of the point's outputs. It is equal to
   'sizeof(double) * number of outputs'. */
protected:
  char const * filename;
  unsigned const inSize;
  unsigned const utSize;
};

/* Constructor. The parameters received are the name of the database file,
   "keySize" is the problem dimension (also the number of real numbers in a
   point's coordinates), "datSize" is the number of outputs. */
inline Database::Database(const char * name, unsigned keySize,unsigned datSize)
  : filename(name), inSize(keySize * sizeof(double) ),
    utSize(datSize * sizeof(double) )
{}

#endif

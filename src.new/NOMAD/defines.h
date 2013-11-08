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

// defines.h -- Constant definitions for the NOMAD software.

#ifndef _DEFINES_H_
#define _DEFINES_H_

// These boolean constants represent flag values. When a flag is raised (set
// to true) it means there's a problem.
const bool RAISED = true;
const bool LOWERED = false;

// This number is used in NOMAD to represent infinity: it's the largest
// floating point number for some computers.
const double INFINIT = 9.9e307;
// An approximation of pi.
const double PI = 3.141592654;

// These integer constants indicate if a point was found in the caches, and in
// which cache it was found (temporary or permanent).
const unsigned int NO_CACHE_HIT = 0;
const unsigned int TEMP_CACHE_HIT = 1;
const unsigned int PERM_CACHE_HIT = 2;

// These integer constants indicate which object produced the new best iterate.
// They're used for the run statistics.
const int POLL = 0;
const int INITIAL = 1;
const int ITERATIVE = 2;
const int SPECULATIVE = 3;
const int SPECULATIVE_AND_ITERATIVE = 6;

#endif

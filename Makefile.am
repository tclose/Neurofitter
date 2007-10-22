## Process this file with automake to produce Makefile.in

CLEANFILES = *~
DISTCLEANFILES = 
MAINTAINERCLEANFILES  = aclocal.m4 configure config.guess config.sub Makefile.in
MAINTAINERCLEANDIRS   = autom4te.cache

if BUILD_EO_SRC
   EO_DIR = EO
endif
if BUILD_NOMAD_SRC
   NOMAD_DIR = NOMAD
endif
SUBDIRS = $(EO_DIR) $(NOMAD_DIR) bin tests
DIST_SUBDIRS = $(EO_DIR) $(NOMAD_DIR) bin tests


INCLUDES = -I$(top_srcdir) 
LIBS = 

ACLOCAL_AMFLAGS ?= -I $(top_srcdir)/m4

NF_CXXFLAGS = -ansi -pedantic 
NF_CFLAGS   = -ansi -pedantic 

if NEUROFITTER_DEBUG
  NF_DEBUG_CXXFLAGS = -Wall -g -O3 
  NF_DEBUG_CPPFLAGS = -DDEBUG 
else
  NF_DEBUG_CXXFLAGS = -O3 
endif

if USE_MPI
  NF_MPI_CXXFLAGS = -Wno-deprecated -Wno-long-long 
  NF_MPI_CPPFLAGS = -Wno-deprecated -Wno-long-long 
endif

AM_CXXFLAGS = $(NF_CXXFLAGS) $(NF_DEBUG_CXXFLAGS) $(NF_MPI_CXXFLAGS) 
AM_CPPFLAGS = $(NF_CPPFLAGS) $(NF_DEBUG_CPPFLAGS) $(NF_MPI_CPPFLAGS) 


## Include files that are NOT going to be a part of the distribution.
noinst_HEADERS = 

## Additional files to include in the distribution
ACLOCAL_DIST = m4/ac_caolan_search_package.m4 \
	       m4/acx_mpi.m4 \
	       m4/ax_prog_doxygen.m4 \
               m4/libxml2.m4

SCRIPTS_DIST =  

EXTRA_DIST  = autogen.sh TODO $(ACLOCAL_DIST) $(SCRIPTS_DIST)

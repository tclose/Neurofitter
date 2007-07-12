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
SUBDIRS = $(EO_DIR) $(NOMAD_DIR) tests
DIST_SUBDIRS = tests


INCLUDES = -I$(top_srcdir) 
LIBS = -rdynamic

ACLOCAL_AMFLAGS ?= -I $(top_srcdir)/m4

AM_CPPFLAGS = 
AM_CXXFLAGS = -ansi -pedantic 
AM_LDFLAGS  = 
AM_CFLAGS   = -ansi -pedantic 

if NEUROFITTER_DEBUG
  AM_CXXFLAGS += -Wall -g -O3
  AM_CPPFLAGS += -DDEBUG
else
  AM_CXXFLAGS += -O3
endif

SRCDIR=$(top_srcdir)/implementation
INCLUDEDIR=$(top_srcdir)

## Programs to be built
if USE_MPI
MPINEUROFITTER=mpineurofitter
endif

bin_PROGRAMS = neurofitter $(MPINEUROFITTER)

## Add necessary flags for EO and NOMAD libraries
if USE_EO
   EO_SRC=$(SRCDIR)/EOFitterInterface.cpp
   eo_CFLAGS+= -Wno-deprecated -Wno-long-long -DWITH_EO 
   eo_LDFLAGS+= -les -leoutils -leo 
endif

if USE_NOMAD
   NOMAD_SRC=$(SRCDIR)/NOMADFitterInterface.cpp $(SRCDIR)/truthfunction.cpp 
   nomad_CFLAGS+= -DWITH_NOMAD -DPARANOMAD 
   nomad_LDFLAGS+= -lnomad 
if USE_EO
   EONOMAD_SRC=$(SRCDIR)/EONOMADFitterInterface.cpp
endif
endif

## Sources for the neurofitter program -- will be compiled with the
## flags below
neurofitter_SOURCES =  $(SRCDIR)/Neurofitter.cpp \
 $(SRCDIR)/DataTrace.cpp \
 $(SRCDIR)/FixedParameters.cpp \
 $(SRCDIR)/NormalTracesReader.cpp \
 $(SRCDIR)/XMLString.cpp \
 $(SRCDIR)/ModelResults.cpp \
 $(SRCDIR)/GenesisModelInterface.cpp \
 $(SRCDIR)/NeuronModelInterface.cpp \
 $(SRCDIR)/ExecutableModelInterface.cpp \
 $(SRCDIR)/ModelTuningParameters.cpp \
 $(SRCDIR)/DirectVdVdtMatrix.cpp \
 $(SRCDIR)/MapVdVdtMatrix.cpp \
 $(SRCDIR)/DistanceVdVdtMatrix.cpp \
 $(SRCDIR)/MatrixFitnessCalculator.cpp \
 $(SRCDIR)/FakeExperimentInterface.cpp \
 $(SRCDIR)/FileExperimentInterface.cpp \
 $(SRCDIR)/FileFitterInterface.cpp \
 $(SRCDIR)/FitterResults.cpp \
 $(SRCDIR)/MeshFitterInterface.cpp \
 $(SRCDIR)/EasyFitterInterface.cpp \
 $(SRCDIR)/RandomFitterInterface.cpp \
 $(SRCDIR)/SwarmFitterInterface.cpp \
 $(SRCDIR)/SwarmFly.cpp \
 $(EO_SRC) \
 $(NOMAD_SRC) \
 $(EONOMAD_SRC) 

## Compile flags for the neurofitter program
neurofitter_LDFLAGS  = $(XML_LIBS) $(eo_LDFLAGS) $(nomad_LDFLAGS) $(AM_LDFLAGS)
neurofitter_CFLAGS   = $(XML_CPPFLAGS) $(eo_CFLAGS) $(nomad_CFLAGS) $(AM_CFLAGS)
neurofitter_CXXFLAGS = $(neurofitter_CFLAGS) $(AM_CXXFLAGS)

## Sources for the mpineurofitter program -- will be compiled with the
## flags below
mpineurofitter_SOURCES = $(neurofitter_SOURCES) \
 $(SRCDIR)/MPIModelInterface.cpp \
 $(SRCDIR)/MPIFitnessCalculator.cpp \
 $(SRCDIR)/MPIStream.cpp

## Compile flags for the mpineurofitter program
mpineurofitter_LDFLAGS=$(neurofitter_LDFLAGS) $(MPILIBS)
mpineurofitter_CFLAGS=$(neurofitter_CFLAGS)
mpineurofitter_CXXFLAGS=$(neurofitter_CXXFLAGS)

## Include files that are going to be a part of the distribution.  The
## `pkginclude_' prefix means that these files will be installed in
## ${prefix}/include/${package}. Use the `include_' prefix to install
## header files in ${prefix}/include.
pkginclude_HEADERS = $(INCLUDEDIR)/Neurofitter.h \
  $(INCLUDEDIR)/DataTrace.h \
  $(INCLUDEDIR)/FileExperimentInterface.h \
  $(INCLUDEDIR)/MersenneTwister.h \
  $(INCLUDEDIR)/NOMADFitterInterface.h \
  $(INCLUDEDIR)/DirectVdVdtMatrix.h \
  $(INCLUDEDIR)/FileFitterInterface.h \
  $(INCLUDEDIR)/MeshFitterInterface.h \
  $(INCLUDEDIR)/NormalTracesReader.h \
  $(INCLUDEDIR)/DistanceVdVdtMatrix.h \
  $(INCLUDEDIR)/FitnessCalculator.h \
  $(INCLUDEDIR)/ModelInterface.h \
  $(INCLUDEDIR)/OutputChannel.h \
  $(INCLUDEDIR)/EasyFitterInterface.h \
  $(INCLUDEDIR)/FitterInterface.h \
  $(INCLUDEDIR)/ModelResults.h \
  $(INCLUDEDIR)/RandomFitterInterface.h \
  $(INCLUDEDIR)/EODistFitness.h	\
  $(INCLUDEDIR)/FitterResults.h	\
  $(INCLUDEDIR)/ModelTuningParameters.h	\
  $(INCLUDEDIR)/SwarmFitterInterface.h \
  $(INCLUDEDIR)/EOFitnessCalculator.h \
  $(INCLUDEDIR)/FixedParameters.h \
  $(INCLUDEDIR)/MPIFitnessCalculator.h \
  $(INCLUDEDIR)/SwarmFly.h \
  $(INCLUDEDIR)/EOFitterInterface.h \
  $(INCLUDEDIR)/FixedParamObject.h \
  $(INCLUDEDIR)/MPIModelInterface.h \
  $(INCLUDEDIR)/Tools.h \
  $(INCLUDEDIR)/EONOMADFitterInterface.h \
  $(INCLUDEDIR)/GenesisModelInterface.h  \
  $(INCLUDEDIR)/MPIStream.h \
  $(INCLUDEDIR)/TracesReader.h \
  $(INCLUDEDIR)/ExecutableModelInterface.h  \
  $(INCLUDEDIR)/InputChannel.h	\
  $(INCLUDEDIR)/ExperimentInterface.h \
  $(INCLUDEDIR)/MapVdVdtMatrix.h \
  $(INCLUDEDIR)/VdVdtMatrix.h \
  $(INCLUDEDIR)/FakeExperimentInterface.h \
  $(INCLUDEDIR)/MatrixFitnessCalculator.h \
  $(INCLUDEDIR)/NeuronModelInterface.h	\
  $(INCLUDEDIR)/XMLString.h \
  $(INCLUDEDIR)/truthfunction.h

## Include files that are NOT going to be a part of the distribution.
noinst_HEADERS = 

## Additional files to include in the distribution
ACLOCAL_DIST = m4/ac_caolan_search_package.m4 \
	       m4/acx_mpi.m4 \
	       m4/ax_prog_doxygen.m4 \
               m4/libxml2.m4

SCRIPTS_DIST =  

EXTRA_DIST  = autogen.sh TODO $(ACLOCAL_DIST) $(SCRIPTS_DIST)

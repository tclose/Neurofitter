MAINTAINERCLEANFILES = Makefile.in

SRCDIR=$(top_srcdir)/implementation
INCLUDEDIR=$(top_srcdir)

## Programs to be built
if USE_MPI
MPINEUROFITTER=mpineurofitter
endif

bin_PROGRAMS = neurofitter $(MPINEUROFITTER)

## Add necessary flags for EO and NOMAD libraries
if BUILD_EO_SRC
   EO_SRC=$(SRCDIR)/EOFitterInterface.cpp
   eo_src_CFL = @eo_src_CFLAGS_include@ -Wno-deprecated -Wno-long-long -DWITH_EO 
   eo_src_LDFL = @eo_src_LDFLAGS_include@ -les -leoutils -leo 
else
if USE_EO
   EO_SRC=$(SRCDIR)/EOFitterInterface.cpp
   eo_CFL = @eo_CFLAGS_include@ -Wno-deprecated -Wno-long-long -DWITH_EO 
   eo_LDFL = @eo_LDFLAGS_include@ -les -leoutils -leo 
endif
endif

if BUILD_NOMAD_SRC
   NOMAD_SRC=$(SRCDIR)/NOMADFitterInterface.cpp $(SRCDIR)/truthfunction.cpp 
   nomad_src_CFL = @nomad_src_CFLAGS_include@ -DWITH_NOMAD -DPARANOMAD 
   nomad_src_LDFL = @nomad_src_LDFLAGS_include@ -lnomad 
if BUILD_EO_SRC
   EONOMAD_SRC=$(SRCDIR)/EONOMADFitterInterface.cpp
endif
else
if USE_NOMAD
   NOMAD_SRC=$(SRCDIR)/NOMADFitterInterface.cpp $(SRCDIR)/truthfunction.cpp 
   nomad_CFL = @nomad_CFLAGS_include@ -DWITH_NOMAD -DPARANOMAD 
   nomad_LDFL = @nomad_LDFLAGS_include@ -lnomad 
if USE_EO
   EONOMAD_SRC=$(SRCDIR)/EONOMADFitterInterface.cpp
endif
endif
endif

NF_CXXFLAGS = -ansi -pedantic -Wall -O3 
NF_CFLAGS   = -ansi -pedantic -Wall -O3 

if NEUROFITTER_DEBUG
  NF_DEBUG_CXXFLAGS = -Wall -g 
  NF_DEBUG_CPPFLAGS = -DDEBUG 
else
  NF_DEBUG_CXXFLAGS = 
endif

if USE_MPI
  NF_MPI_CXXFLAGS = -Wno-deprecated -Wno-long-long 
  NF_MPI_CPPFLAGS = -Wno-deprecated -Wno-long-long 
endif

AM_CXXFLAGS = $(NF_CXXFLAGS) $(NF_DEBUG_CXXFLAGS) $(NF_MPI_CXXFLAGS) 
AM_CPPFLAGS = $(NF_CPPFLAGS) $(NF_DEBUG_CPPFLAGS) $(NF_MPI_CPPFLAGS) 

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
 $(SRCDIR)/MeshVdVdtMatrix.cpp \
 $(SRCDIR)/MapVdVdtMatrix.cpp \
 $(SRCDIR)/DistanceVdVdtMatrix.cpp \
 $(SRCDIR)/ROI.cpp \
 $(SRCDIR)/ROIStringParser.cpp \
 $(SRCDIR)/ROIVdVdtMatrix.cpp \
 $(SRCDIR)/MatrixErrorValueCalculator.cpp \
 $(SRCDIR)/FakeExperimentInterface.cpp \
 $(SRCDIR)/FileExperimentInterface.cpp \
 $(SRCDIR)/FileListExperimentInterface.cpp \
 $(SRCDIR)/FileFitterInterface.cpp \
 $(SRCDIR)/FitterResults.cpp \
 $(SRCDIR)/MeshFitterInterface.cpp \
 $(SRCDIR)/EasyFitterInterface.cpp \
 $(SRCDIR)/SimpleGradientFitterInterface.cpp \
 $(SRCDIR)/RandomFitterInterface.cpp \
 $(SRCDIR)/SwarmFitterInterface.cpp \
 $(SRCDIR)/SwarmFly.cpp \
 $(SRCDIR)/NSGA2FitterInterface.cpp \
 $(SRCDIR)/NSGA2Individual.cpp \
 $(SRCDIR)/NSGA2Population.cpp \
 $(EO_SRC) \
 $(NOMAD_SRC) \
 $(EONOMAD_SRC) 

## Compile flags for the neurofitter program
neurofitter_LDFLAGS  = $(XML_LIBS) $(eo_LDFL) $(nomad_LDFL) $(eo_src_LDFL) $(nomad_src_LDFL) $(AM_LDFLAGS)
neurofitter_CFLAGS   = $(XML_CPPFLAGS) $(eo_src_CFL) $(nomad_src_CFL) $(AM_CFLAGS)
neurofitter_CXXFLAGS = $(neurofitter_CFLAGS) $(AM_CXXFLAGS)

## Sources for the mpineurofitter program -- will be compiled with the
## flags below
mpineurofitter_SOURCES = $(neurofitter_SOURCES) \
 $(SRCDIR)/MPIModelInterface.cpp \
 $(SRCDIR)/MPIErrorValueCalculator.cpp \
 $(SRCDIR)/MPIStream.cpp

## Compile flags for the mpineurofitter program
mpineurofitter_LDFLAGS=$(neurofitter_LDFLAGS) $(MPILIBS)
mpineurofitter_CFLAGS=$(neurofitter_CFLAGS)  -DWITH_MPI
mpineurofitter_CXXFLAGS=$(neurofitter_CXXFLAGS)  -DWITH_MPI

## Include files that are going to be a part of the distribution.  The
## `pkginclude_' prefix means that these files will be installed in
## ${prefix}/include/${package}. Use the `include_' prefix to install
## header files in ${prefix}/include.
pkginclude_HEADERS = $(INCLUDEDIR)/Neurofitter.h \
  $(INCLUDEDIR)/DataTrace.h \
  $(INCLUDEDIR)/FileExperimentInterface.h \
  $(INCLUDEDIR)/FileListExperimentInterface.h \
  $(INCLUDEDIR)/MersenneTwister.h \
  $(INCLUDEDIR)/NOMADFitterInterface.h \
  $(INCLUDEDIR)/DirectVdVdtMatrix.h \
  $(INCLUDEDIR)/ROIVdVdtMatrix.h \
  $(INCLUDEDIR)/ROIStringParser.h \
  $(INCLUDEDIR)/ROI.h \
  $(INCLUDEDIR)/SimpleGradientFitterInterface.h \
  $(INCLUDEDIR)/FileFitterInterface.h \
  $(INCLUDEDIR)/MeshFitterInterface.h \
  $(INCLUDEDIR)/NormalTracesReader.h \
  $(INCLUDEDIR)/DistanceVdVdtMatrix.h \
  $(INCLUDEDIR)/ErrorValueCalculator.h \
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
  $(INCLUDEDIR)/NSGA2FitterInterface.h \
  $(INCLUDEDIR)/NSGA2Population.h \
  $(INCLUDEDIR)/NSGA2Individual.h \
  $(INCLUDEDIR)/EOErrorValueCalculator.h \
  $(INCLUDEDIR)/FixedParameters.h \
  $(INCLUDEDIR)/MPIErrorValueCalculator.h \
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
  $(INCLUDEDIR)/MeshVdVdtMatrix.h \
  $(INCLUDEDIR)/MapVdVdtMatrix.h \
  $(INCLUDEDIR)/VdVdtMatrix.h \
  $(INCLUDEDIR)/FakeExperimentInterface.h \
  $(INCLUDEDIR)/MatrixErrorValueCalculator.h \
  $(INCLUDEDIR)/NeuronModelInterface.h	\
  $(INCLUDEDIR)/XMLString.h \
  $(INCLUDEDIR)/truthfunction.h

MAINTAINERCLEANFILES = Makefile.in

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
mpineurofitter_CFLAGS=$(neurofitter_CFLAGS)  -DWITH_MPI
mpineurofitter_CXXFLAGS=$(neurofitter_CXXFLAGS)  -DWITH_MPI

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

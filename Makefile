########################
# Neurofitter Makefile #
########################

#############################################################
# Changed these values if they are different on your system #
#############################################################
DIR_EO = ../EO/src
DIR_NOMAD = ../NOMAD
DIR_LIBXML2 = /usr/include/libxml2

SHELL = /bin/sh
RM = /bin/rm
#############################################################

LIBDIRS=-L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -L$(DIR_NOMAD) #-L$(DIR_LIB_MPI)
LIBS= $(LIBDIRS) -les -leoutils -leo -lnomad -lxml2
 
CXXLIBS = $(LIBS) 

#Added Wno-long-long added for MPI
CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -Wno-long-long -g -I$(DIR_LIBXML2)

MPICXX=mpicxx

DOXYGEN = /opt/local/bin/doxygen

BINDIR = bin
CPPDIR = implementation

.SUFFIXES: .cpp .o

# The object files related to general functionality			
NEUROFITTEROBJS=	$(CPPDIR)/DataTrace.o \
			$(CPPDIR)/FixedParameters.o \
			$(CPPDIR)/NormalTracesReader.o \
			$(CPPDIR)/XMLString.o 

# The object files related to model interfaces			
MODOBJS=	$(CPPDIR)/ModelResults.o \
			$(CPPDIR)/GenesisModelInterface.o \
			$(CPPDIR)/NeuronModelInterface.o \
			$(CPPDIR)/ModelTuningParameters.o

# The object files related to fitness calculator interfaces			
FITOBJS=	$(CPPDIR)/NormalVdVdtMatrix.o \
			$(CPPDIR)/MatrixFitnessCalculator.o

# The object files related to experiment interfaces			
EXPOBJS=	$(CPPDIR)/FakeExperimentInterface.o \
			$(CPPDIR)/FileExperimentInterface.o

# The object files related to fitter interfaces
FITTEROBJS=	$(CPPDIR)/NOMADFitterInterface.o \
			$(CPPDIR)/FitterResults.o \
			$(CPPDIR)/truthfunction.o \
			$(CPPDIR)/EOFitterInterface.o \
			$(CPPDIR)/EONOMADFitterInterface.o \
			$(CPPDIR)/MeshFitterInterface.o \
			$(CPPDIR)/RandomFitterInterface.o \
			$(CPPDIR)/SwarmFitterInterface.o \
			$(CPPDIR)/SwarmFly.o

# The object files related Neurofitter without MPI
NORMALOBJS = $(CPPDIR)/Neurofitter.o

# The object files related Neurofitter with MPI
MPIOBJS = 	$(CPPDIR)/MPIModelInterface.o \
			$(CPPDIR)/MPIFitnessCalculator.o \
			$(CPPDIR)/MPINeurofitter.o \
			$(CPPDIR)/MPIStream.o

# All object files except NORMALOBJS and MPIOBJS
OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(NEUROFITTEROBJS) 

all : $(BINDIR)/Neurofitter

mpi : $(BINDIR)/MPINeurofitter

$(BINDIR)/Neurofitter : $(OBJS) $(NORMALOBJS) ; 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(NORMALOBJS) $(CXXLIBS)

$(BINDIR)/MPINeurofitter : $(OBJS) $(MPIOBJS) ; 
	$(MPICXX) $(CXXFLAGS) -o $@ $(OBJS) $(MPIOBJS) $(CXXLIBS)

$(CPPDIR)/EOFitterInterface.o : $(CPPDIR)/EOFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -c -I$(DIR_EO) -o $@ $<

$(CPPDIR)/EOFitness.o : $(CPPDIR)/EOFitness.cpp ;
	$(CXX) $(CXXFLAGS) -c -I$(DIR_EO) -o $@ $<

$(CPPDIR)/NOMADFitterInterface.o : $(CPPDIR)/NOMADFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_NOMAD) -o $@ $<

$(CPPDIR)/EONOMADFitterInterface.o : $(CPPDIR)/EONOMADFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<

$(CPPDIR)/truthfunction.o : $(CPPDIR)/truthfunction.cpp ;
	$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_NOMAD) -o $@ $<

$(CPPDIR)/Neurofitter.o : $(CPPDIR)/Neurofitter.cpp ;
	$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<

$(CPPDIR)/MPIStream.o : $(CPPDIR)/MPIStream.cpp ;
	$(MPICXX) $(CXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIModelInterface.o : $(CPPDIR)/MPIModelInterface.cpp ;
	$(MPICXX) $(CXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIFitnessCalculator.o : $(CPPDIR)/MPIFitnessCalculator.cpp ;
	$(MPICXX) $(CXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPINeurofitter.o : $(CPPDIR)/MPINeurofitter.cpp ;
	$(MPICXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<

clean : 
	$(RM) -f $(ALL) *.o implementation/*.o

doc : *.h implementation/*.cpp doxygen.config ; 
	$(DOXYGEN) doxygen.config

docclean :
	$(RM) -rf doc

test :
	@run

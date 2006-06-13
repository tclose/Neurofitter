SHELL = /bin/sh

# .SUFFIXES:
.SUFFIXES: .cpp .o

ALL = $(BINDIR)/Neurofitter

BINDIR = bin
CPPDIR = implementation

DIR_EO = ../EO/src
DIR_NOMAD = ../NOMAD
DIR_LIBXML2 = /usr/include/libxml2

LIBDIRS=-L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -L$(DIR_NOMAD) #-L$(DIR_LIB_MPI)
LIBS= $(LIBDIRS) -les -leoutils -leo -lnomad -lxml2

#LIBS= $(LIBDIRS) -les -leoutils -leo -lnomad -lxml2 -lz -lpthread -liconv -lm
 
CXXLIBS = $(LIBS) 

#Added Wno-long-long added for MPI
CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -Wno-long-long -g -I$(DIR_LIBXML2)

#CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -g -fast -mcpu=7450

MPICXX=mpicxx

DOXYGEN = /opt/local/bin/doxygen

NEUROFITTEROBJS=	$(CPPDIR)/DataTrace.o \
			$(CPPDIR)/FixedParameters.o \
			$(CPPDIR)/XMLString.o 

MODOBJS=	$(CPPDIR)/ModelResults.o \
			$(CPPDIR)/GenesisModelInterface.o \
			$(CPPDIR)/ModelTuningParameters.o

FITOBJS=	$(CPPDIR)/NormalVdVdtMatrix.o \
			$(CPPDIR)/MatrixFitnessCalculator.o
			
EXPOBJS=	$(CPPDIR)/FakeExperimentInterface.o

FITTEROBJS=	$(CPPDIR)/NOMADFitterInterface.o \
			$(CPPDIR)/FitterResults.o \
			$(CPPDIR)/truthfunction.o \
			$(CPPDIR)/EOFitterInterface.o \
			$(CPPDIR)/EONOMADFitterInterface.o \
			$(CPPDIR)/MeshFitterInterface.o \
			$(CPPDIR)/RandomFitterInterface.o \
			$(CPPDIR)/SwarmFitterInterface.o \
			$(CPPDIR)/SwarmFly.o

NORMALOBJS = $(CPPDIR)/Neurofitter.o
MPIOBJS = 	$(CPPDIR)/MPIModelInterface.o \
			$(CPPDIR)/MPIFitnessCalculator.o \
			$(CPPDIR)/MPINeurofitter.o \
			$(CPPDIR)/MPIStream.o

OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(NEUROFITTEROBJS) 

all : $(ALL)

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
	@/bin/rm -rf $(ALL) *.gch *.o implementation/*.o

doc : *.h implementation/*.cpp doxygen.config ; 
	@$(DOXYGEN) doxygen.config

docclean :
	@/bin/rm -rf doc

test :
	@run

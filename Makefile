########################
# Neurofitter Makefile #
########################

#############################################################
# Changed these values if they are different on your system #
#############################################################
DIR_LIBXML2 = /usr/include/libxml2

SHELL = /bin/sh
RM = /bin/rm

DIR_EO = ../EO/src #Directory that contains the EO source, if you don't want EO support don't define this variable
DIR_NOMAD = ../NOMAD #Directory that contains the NOMAD source, if you don't want NOMAD support don't define this variable
#############################################################

#Added Wno-long-long added for MPI
CXXFLAGS = -ansi -pedantic -O3 -Wall -g -I$(DIR_LIBXML2)
EOCXXFLAGS = -Wno-deprecated -Wno-long-long
MPICXXFLAGS = -Wno-long-long -Wnon-virtual-dtor

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
			$(CPPDIR)/ExecutableModelInterface.o \
			$(CPPDIR)/ModelTuningParameters.o

# The object files related to fitness calculator interfaces			
FITOBJS=	$(CPPDIR)/DirectVdVdtMatrix.o \
			$(CPPDIR)/MapVdVdtMatrix.o \
			$(CPPDIR)/DistanceVdVdtMatrix.o \
			$(CPPDIR)/MatrixFitnessCalculator.o

# The object files related to experiment interfaces			
EXPOBJS=	$(CPPDIR)/FakeExperimentInterface.o \
			$(CPPDIR)/FileExperimentInterface.o

# The object files related to fitter interfaces
FITTEROBJS=	$(CPPDIR)/FitterResults.o \
			$(CPPDIR)/MeshFitterInterface.o \
			$(CPPDIR)/EasyFitterInterface.o \
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


ifdef DIR_EO
			CXXFLAGS = $(CXXFLAGS) -DWITH_EO
			FITTEROBJS = $(FITTEROBJS) $(CPPDIR)/EOFitterInterface.o
			LIBDIRS=$(LIBDIRS) -L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -les -leoutils -leo
endif

ifdef DIR_NOMAD
			CXXFLAGS = $(CXXFLAGS) -DWITH_NOMAD
			FITTEROBJS = $(FITTEROBJS) $(CPPDIR)/NOMADFitterInterface.o $(CPPDIR)/truthfunction.o
			LIBDIRS=$(LIBDIRS) -L$(DIR_NOMAD) -lnomad
endif

ifdef DIR_EO
	ifdef DIR_NOMAD
			FITTEROBJS = $(FITTEROBJS) $(CPPDIR)/EONOMADFitterInterface.o
	endif
endif

LIBS= $(LIBDIRS) -lxml2 
CXXLIBS = $(LIBS) 


# All object files except NORMALOBJS and MPIOBJS
OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(NEUROFITTEROBJS) 

all : $(BINDIR)/Neurofitter

mpi : $(BINDIR)/MPINeurofitter

$(BINDIR)/Neurofitter : $(OBJS) $(NORMALOBJS) ; 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(NORMALOBJS) $(CXXLIBS)

$(BINDIR)/MPINeurofitter : $(OBJS) $(MPIOBJS) ; 
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -o $@ $(OBJS) $(MPIOBJS) $(CXXLIBS)


$(CPPDIR)/Neurofitter.o : $(CPPDIR)/Neurofitter.cpp ;
	$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<

$(CPPDIR)/MPIStream.o : $(CPPDIR)/MPIStream.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIModelInterface.o : $(CPPDIR)/MPIModelInterface.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIFitnessCalculator.o : $(CPPDIR)/MPIFitnessCalculator.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPINeurofitter.o : $(CPPDIR)/MPINeurofitter.cpp ;
	$(MPICXX) $(CXXFLAGS) $(EOCXXFLAGS) $(MPICXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<


#ifdef DIR_EO
	$(CPPDIR)/EOFitterInterface.o : $(CPPDIR)/EOFitterInterface.cpp ;
		$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) -c -I$(DIR_EO) -o $@ $<
	
	$(CPPDIR)/EOFitness.o : $(CPPDIR)/EOFitness.cpp ;
		$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) -c -I$(DIR_EO) -o $@ $<
#else
#	$(CPPDIR)/EOFitterInterface.o :
#	$(CPPDIR)/EOFitness.o :
#endif

#ifdef DIR_NOMAD
	$(CPPDIR)/NOMADFitterInterface.o : $(CPPDIR)/NOMADFitterInterface.cpp ;
		$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_NOMAD) -o $@ $<
	$(CPPDIR)/truthfunction.o : $(CPPDIR)/truthfunction.cpp ;
		$(CXX) $(CXXFLAGS) -c -DPARANOMAD -I$(DIR_NOMAD) -o $@ $<
#else
#	$(CPPDIR)/NOMADFitterInterface.o :
#	$(CPPDIR)/truthfunction.o :
#endif

#ifdef DIR_EO
#	ifdef DIR_NOMAD
		$(CPPDIR)/EONOMADFitterInterface.o : $(CPPDIR)/EONOMADFitterInterface.cpp ;
			$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) -c -DPARANOMAD -I$(DIR_EO) -I$(DIR_NOMAD) -o $@ $<		
#	else 
#		$(CPPDIR)/EONOMADFitterInterface.o ;
#	endif
#else
#	$(CPPDIR)/EONOMADFitterInterface.o ;
#endif


clean : 
	$(RM) -f $(ALL) *.o implementation/*.o

doc : *.h implementation/*.cpp doxygen.config ; 
	$(DOXYGEN) doxygen.config

docclean :
	$(RM) -rf doc

test :
	@run

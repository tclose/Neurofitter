SHELL = /bin/sh

# .SUFFIXES:
.SUFFIXES: .cpp .o

ALL = $(BINDIR)/Evolufit
BINDIR = bin
OBJDIR = ./build
CPPDIR = implementation

DIR_EO = ../eo/src
DIR_NOMAD = ../nomad
DIR_LIBXML2	= /usr/include/libxml2

BERKELEY_DIR      = /opt/local
BERKELEY_INCLUDE  = $(BERKELEY_DIR)/include/db4
BERKELEY_LIBDIR	  = $(BERKELEY_DIR)/lib
BERKELEY_LIB      = -ldb_cxx-4.3 -ldb-4.3 -lpthread

LIBDIRS=-L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -L$(DIR_NOMAD) -L$(BERKELEY_LIBDIR) 
LIBS= $(LIBDIRS) -les -leoutils -leo -lnomad $(BERKELEY_LIB) -lxml2 -lz -lpthread -liconv -lm
 
CXXLIBS = $(LIBS) 

CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -g -I$(DIR_LIBXML2)
#CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -g -fast -mcpu=7450


DOXYGEN = /opt/local/bin/doxygen

EVOOBJS=	$(CPPDIR)/Evolufit.o \
			$(CPPDIR)/EvolufitParameters.o \
			$(CPPDIR)/EvolufitState.o \
			$(CPPDIR)/DataTrace.o \
			$(CPPDIR)/EvolufitResults.o \
			$(CPPDIR)/FixedParameters.o \
			$(CPPDIR)/XMLString.o 

MODOBJS=	$(CPPDIR)/ModelResults.o \
			$(CPPDIR)/WernerModelInterface.o \
			$(CPPDIR)/GenesisModelInterface.o \
			$(CPPDIR)/PabloModelInterface.o \
			$(CPPDIR)/ModelTuningParameters.o

FITOBJS=	$(CPPDIR)/PabloVdVdtMatrix.o \
			$(CPPDIR)/PabloFitnessCalculator.o
			
EXPOBJS=	$(CPPDIR)/PabloExperimentInterface.o \
			$(CPPDIR)/WernerExperimentInterface.o

FITTEROBJS=	$(CPPDIR)/NOMADFitterInterface.o \
			$(CPPDIR)/FitterResults.o \
			$(CPPDIR)/truthfunction.o \
			$(CPPDIR)/EOFitterInterface.o \
			$(CPPDIR)/PabloFitterInterface.o \
			$(CPPDIR)/MeshFitterInterface.o \
			$(CPPDIR)/SwarmFitterInterface.o \
			$(CPPDIR)/SwarmFly.o

OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(EVOOBJS) 

all : $(ALL)

$(BINDIR)/Evolufit : $(OBJS) ; 
	$(CXX) -o $@ $(OBJS) $(CXXLIBS)

$(CPPDIR)/EOFitterInterface.o : $(CPPDIR)/EOFitterInterface.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_EO) -o $@ $<

$(CPPDIR)/EOFitnessCalculator.o : $(CPPDIR)/EOFitnessCalculator.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_EO) -o $@ $<

$(CPPDIR)/NOMADFitterInterface.o : $(CPPDIR)/NOMADFitterInterface.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ $<

$(CPPDIR)/PabloFitterInterface.o : $(CPPDIR)/PabloFitterInterface.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_EO) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ $<

$(CPPDIR)/truthfunction.o : $(CPPDIR)/truthfunction.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ $<

$(CPPDIR)/Evolufit.o : $(CPPDIR)/Evolufit.cpp ;
	$(CXX) -c $(CXXFLAGS) -I$(DIR_EO) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ $<

clean : 
	@/bin/rm -rf $(ALL) *.gch *.o implementation/*.o

doc : *.h implementation/*.cpp doxygen.config ; 
	@$(DOXYGEN) doxygen.config

docclean :
	@/bin/rm -rf doc

test :
	@run

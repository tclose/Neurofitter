SHELL = /bin/sh

# .SUFFIXES:
# .SUFFIXES: .cpp .o

ALL = $(BINDIR)/Evolufit
BINDIR = bin
OBJDIR = ./build
CPPDIR = implementation
CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -g
#CXXFLAGS = -ansi -pedantic -O3 -Wall -Wno-deprecated -g -fast -mcpu=7450
DIR_EO = ../eo/src
DIR_NOMAD = ../nomad

LIBXML2_DIR	= /usr/include/libxml2

BERKELEY_DIR      = /opt/local
BERKELEY_INCLUDE  = $(BERKELEY_DIR)/include/db4
BERKELEY_LIBDIR	  = $(BERKELEY_DIR)/lib
BERKELEY_LIB      = -ldb_cxx-4.3 -ldb-4.3 -lpthread

INCLUDES=-I. -I$(DIR_EO) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) 
LIBDIRS=-L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -L$(DIR_NOMAD) -L$(BERKELEY_LIBDIR) 
LIBS= -les -leoutils -leo -lnomad $(BERKELEY_LIB) -lxml2 -lz -lpthread -liconv -lm

DOXYGEN = /opt/local/bin/doxygen

EVOOBJS=	$(CPPDIR)/Evolufit.o \
			$(CPPDIR)/EvolufitParameters.o \
			$(CPPDIR)/EvolufitState.o \
			$(CPPDIR)/DataTrace.o \
			$(CPPDIR)/EvolufitResults.o 

MODOBJS=	$(CPPDIR)/ModelResults.o \
			$(CPPDIR)/WernerModelInterface.o \
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
			$(CPPDIR)/PabloFitterInterface.o

OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(EVOOBJS) 

OPTIONS = $(CXXFLAGS) $(INCLUDES) $(LIBDIRS)

all : $(ALL)

$(BINDIR)/Evolufit : *.h $(OBJS) ; 
	$(CXX) $(CXXFLAGS) $(LIBDIRS) -o $@ $(OBJS) $(LIBS)

$(CPPDIR)/EOFitterInterface.o : $(CPPDIR)/EOFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_EO) -o $@ -c $<

$(CPPDIR)/EOFitnessCalculator.o : $(CPPDIR)/EOFitnessCalculator.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_EO) -o $@ -c $<

$(CPPDIR)/NOMADFitterInterface.o : $(CPPDIR)/NOMADFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ -c $<

$(CPPDIR)/PabloFitterInterface.o : $(CPPDIR)/PabloFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_EO) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ -c $<


$(CPPDIR)/truthfunction.o : $(CPPDIR)/truthfunction.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -o $@ -c $<

$(CPPDIR)/Evolufit.o : $(CPPDIR)/Evolufit.cpp ;
	$(CXX) $(CXXFLAGS) -I$(DIR_EO) -I$(DIR_NOMAD) -I$(BERKELEY_INCLUDE) -I$(LIBXML2_DIR) -o $@ -c $<

clean : 
	@/bin/rm -rf $(ALL) *.gch *.o implementation/*.o

doc : *.h implementation/*.cpp doxygen.config ; 
	@$(DOXYGEN) doxygen.config

docclean :
	@/bin/rm -rf doc

test :
	@run

########################
# Neurofitter Makefile #
########################

#############################################################
# Changed these values if they are different on your system #
#############################################################
DIR_LIBXML2 = /usr/include/libxml2

SHELL = /bin/sh
RM = /bin/rm

#Directory that contains the EO source, if you don't want EO support don't define this variable
#DIR_EO = "../EO/srct"
#Directory that contains the NOMAD source, if you don't want NOMAD support don't define this variable
#DIR_NOMAD = "../NOMAD" 


DOXYGEN = /opt/local/bin/doxygen #Only necessary for make doc
#############################################################

CXXFLAGS = -ansi -pedantic -O3 -Wall -g -I$(DIR_LIBXML2)
MPICXXFLAGS = -Wno-long-long -Wnon-virtual-dtor

MPICXX=mpicxx

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
			EOCXXFLAGS = -Wno-deprecated -Wno-long-long -DWITH_EO
			EOINCL = -I$(DIR_EO)
			FITTEROBJS += $(CPPDIR)/EOFitterInterface.o
			LIBDIRS += -L$(DIR_EO) -L$(DIR_EO)/es -L$(DIR_EO)/utils -les -leoutils -leo
endif

ifdef DIR_NOMAD
			NOMADCXXFLAGS += -DWITH_NOMAD -DPARANOMAD
			NOMADINCL = -I$(DIR_NOMAD)
			FITTEROBJS += $(CPPDIR)/NOMADFitterInterface.o $(CPPDIR)/truthfunction.o
			LIBDIRS += -L$(DIR_NOMAD) -lnomad
endif

ifdef DIR_EO
	ifdef DIR_NOMAD
			FITTEROBJS += $(CPPDIR)/EONOMADFitterInterface.o
	endif
endif

LIBS = $(LIBDIRS) -lxml2 
CXXLIBS = $(LIBS) 


# All object files except NORMALOBJS and MPIOBJS
OBJS = $(MODOBJS) $(FITOBJS) $(EXPOBJS) $(FITTEROBJS) $(NEUROFITTEROBJS) 

all : $(BINDIR)/Neurofitter

mpi : $(BINDIR)/MPINeurofitter

########
# Main #
########

$(BINDIR)/Neurofitter : $(OBJS) $(NORMALOBJS) ; 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(NORMALOBJS) $(CXXLIBS)

$(CPPDIR)/Neurofitter.o : $(CPPDIR)/Neurofitter.cpp ;
	$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) $(NOMADCXXFLAGS) $(EOINCL) $(NOMADINCL) -c -o $@ $<


#######
# MPI #
#######
$(BINDIR)/MPINeurofitter : $(OBJS) $(MPIOBJS) ; 
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -o $@ $(OBJS) $(MPIOBJS) $(CXXLIBS)

$(CPPDIR)/MPINeurofitter.o : $(CPPDIR)/Neurofitter.cpp ;
	$(MPICXX) $(CXXFLAGS) $(EOCXXFLAGS) $(NOMADCXXFLAGS) $(MPICXXFLAGS) $(EOINCL) $(NOMADINCL) -DWITH_MPI -c -o $@ $<

$(CPPDIR)/MPIStream.o : $(CPPDIR)/MPIStream.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIModelInterface.o : $(CPPDIR)/MPIModelInterface.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

$(CPPDIR)/MPIFitnessCalculator.o : $(CPPDIR)/MPIFitnessCalculator.cpp ;
	$(MPICXX) $(CXXFLAGS) $(MPICXXFLAGS) -c -o $@ $<

######
# EO #
######
$(CPPDIR)/EOFitterInterface.o : $(CPPDIR)/EOFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) $(EOINCL) -c -o $@ $<
	
$(CPPDIR)/EOFitness.o : $(CPPDIR)/EOFitness.cpp ;
	$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) $(EOINCL) -c -o $@ $<

#########
# NOMAD #
#########
$(CPPDIR)/NOMADFitterInterface.o : $(CPPDIR)/NOMADFitterInterface.cpp ;
	$(CXX) $(CXXFLAGS) $(NOMADCXXFLAGS) $(NOMADINCL) -c -o $@ $<

$(CPPDIR)/truthfunction.o : $(CPPDIR)/truthfunction.cpp ;
	$(CXX) $(CXXFLAGS) $(NOMADCXXFLAGS) $(NOMADINCL) -c -o $@ $<

###########
# EONOMAD #
###########
$(CPPDIR)/EONOMADFitterInterface.o : $(CPPDIR)/EONOMADFitterInterface.cpp ;
		$(CXX) $(CXXFLAGS) $(EOCXXFLAGS) $(NOMADCXXFLAGS) $(EOINCL) $(NOMADINCL) -c -o $@ $<		

#########
# CLEAN #
#########
clean : 
	$(RM) -f $(BINDIR)/Neurofitter $(BINDIR)/MPINeurofitter *.o implementation/*.o

#######
# DOC #
#######
doc : *.h implementation/*.cpp doxygen.config ; 
	$(DOXYGEN) doxygen.config

############
# DOCCLEAN #
############
docclean :
	$(RM) -rf doc

#######
# RUN #
#######
test :
	@run

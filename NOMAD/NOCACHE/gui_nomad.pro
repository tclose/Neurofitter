CONFIG		+= qt warn_on release

HEADERS		+= augmentedpoint.h \
	  	   cachecontrol.h \
		   constraints.h \
		   continuouspollster.h \
		   database.h \
		   defines.h \
		   description.h \
		   descriptionwindow.h \
		   direction.h \
		   directionset.h \
		   evaluator.h \
		   filter.h \
		   filtergraph.h \
		   functioncontrol.h \
		   generalconstraints.h \
		   graph.h \
		   latinhypercubesearcher.h \
		   mainapplication.h \
		   mainwindow.h \
		   mesh.h \
		   observer.h \
		   parameters.h \
		   parameterswindow.h \
		   point.h \
		   pollcontrol.h \
		   pollster.h \
		   preferences.h \
		   preferenceswindow.h \
		   randomsearcher.h \
		   searchcontrol.h \
		   searcher.h \
		   solver.h \
		   speculativesearcher.h \
		   statistics.h \
		   subject.h \
		   surrfunction.h \
		   surrogate.h \
		   surrogatecontrol.h \
		   terminator.h \
		   texteditor.h \
		   truthfunction.h

SOURCES		+= augmentedpoint.cpp \
		   cachecontrol.cpp \
		   constraints.cpp \
		   continuouspollster.cpp \
		   description.cpp \
		   descriptionwindow.cpp \
		   direction.cpp \
		   directionset.cpp \
		   evaluator.cpp \
		   filter.cpp \
		   filtergraph.cpp \
		   functioncontrol.cpp \
		   generalconstraints.cpp \
		   graph.cpp \
		   latinhypercubesearcher.cpp \
                   main.cpp \
		   mainapplication.cpp \
		   mainwindow.cpp \
		   mesh.cpp \
		   parameters.cpp \
	  	   parameterswindow.cpp \
		   point.cpp \
		   pollcontrol.cpp \
		   preferences.cpp \
		   preferenceswindow.cpp \
		   randomsearcher.cpp \
		   searchcontrol.cpp \
		   solver.cpp \
		   speculativesearcher.cpp \
		   statistics.cpp \
		   subject.cpp \
		   surrfunction.cpp \
		   surrogate.cpp \
		   surrogatecontrol.cpp \
		   terminator.cpp \
		   texteditor.cpp \
		   truthfunction.cpp

TARGET		= gui_nomad

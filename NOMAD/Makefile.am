
# Proxy makefile that invokes the real NOMAD makefile

SUBDIR=NOMAD

all-local:
	     $(MAKE) -f Lib_Makefile_parallel $(AM_MAKEFLAGS) libnomad.a
check-local:

clean-local:
	     $(MAKE) -f Lib_Makefile_parallel $(AM_MAKEFLAGS) clean


install-data-local:

install-exec-local:

uninstall-local:


# Distribute files from here.
EXTRA_DIST = Lib_Makefile_parallel $(shell find $(SUBDIR) -iname *.cpp -iname *.h) 

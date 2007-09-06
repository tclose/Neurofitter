MAINTAINERCLEANFILES = Makefile.in

## Process this file with automake to produce Makefile.in

pkgdata_DATA = wforms.css websetup.sh websetup-form.html

EXTRA_DIST = websetup-form.scm websetup-template.scm 

websetup-form.html: 
	@echo "Generating websetup-form.html..."
	./websetup-form.scm > websetup-form.html

DISTCLEANFILES = websetup-form.html

CLEANFILES = *~ 


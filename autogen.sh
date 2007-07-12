#!/bin/sh

# This script generates the files necessary for configure and
# automake. It needs to be run before the software is packaged, to
# ensure that configure can run on systems where autoconf and friends
# are not installed.

aclocal -I m4
glibtoolize -f -c
autoheader 
autoconf 
automake  -a -c

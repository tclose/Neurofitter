#!/bin/sh

# This script generates the files necessary for configure and
# automake. It needs to be run before the software is packaged, to
# ensure that configure can run on systems where autoconf and friends
# are not installed.

# Echo commands after command-line processing.
set -x

if test x"$LIBTOOLIZE" = x; then
# Libtoolize. On Darwin, libtoolize is called glibtoolize.
if glibtoolize --version > /dev/null 2> /dev/null; then 
LIBTOOLIZE="glibtoolize"
elif libtoolize --version > /dev/null 2> /dev/null; then
LIBTOOLIZE="libtoolize"
fi
fi
"$LIBTOOLIZE" --copy --force

# Gather macros required for autoconf.
aclocal

#
autoheader

#
#
automake --foreign --add-missing --copy

#
autoconf

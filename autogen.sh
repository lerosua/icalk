#!/bin/sh
# Run this to generate all the initial makefiles, etc.

: ${AUTOCONF=autoconf}
: ${AUTOHEADER=autoheader}
: ${AUTOMAKE=automake-1.9}
: ${ACLOCAL=aclocal-1.9}
: ${LIBTOOLIZE=libtoolize}
: ${INTLTOOLIZE=intltoolize}
: ${LIBTOOL=libtool}
: ${GNOME_DOC_PREPARE=gnome-doc-prepare}

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd $srcdir
PROJECT="icalk"
TEST_TYPE=-f
FILE=src/about.h
CONFIGURE=configure.in

DIE=0

($AUTOCONF --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have autoconf installed to compile $PROJECT."
	echo "Download the appropriate package for your distribution,"
	echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
	DIE=1
}

(grep "^AC_PROG_INTLTOOL" $srcdir/$CONFIGURE >/dev/null) && {
  ($INTLTOOLIZE --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have \`intltoolize' installed to compile $PROJECT."
    echo "Get ftp://ftp.gnome.org/pub/GNOME/stable/sources/intltool/intltool-0.35.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

(grep "^GNOME_DOC_INIT" $srcdir/$CONFIGURE >/dev/null) && {
  ($GNOME_DOC_PREPARE --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have \`gnome-doc-prepare' installed to compile $PROJECT."
    DIE=1
  }
}

($AUTOMAKE --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have automake 1.9 installed to compile $PROJECT."
	echo "Get ftp://ftp.gnu.org/pub/gnu/automake/automake-1.9.6.tar.gz"
	echo "(or a newer version of 1.9.x if it is available)"
	DIE=1
}

(grep "^AM_PROG_LIBTOOL" $CONFIGURE >/dev/null) && {
  ($LIBTOOL --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`libtool' installed to compile $PROJECT."
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool/libtool-1.5.22.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

if grep "^AM_[A-Z0-9_]\{1,\}_GETTEXT" "$CONFIGURE" >/dev/null; then
  if grep "sed.*POTFILES" "$CONFIGURE" >/dev/null; then
    GETTEXTIZE=""
  else
    if grep "^AM_GLIB_GNU_GETTEXT" "$CONFIGURE" >/dev/null; then
      GETTEXTIZE="glib-gettextize"
      GETTEXTIZE_URL="ftp://ftp.gtk.org/pub/gtk/v2.0/glib-2.0.0.tar.gz"
    else
      GETTEXTIZE="gettextize"
      GETTEXTIZE_URL="ftp://alpha.gnu.org/gnu/gettext-0.10.35.tar.gz"
    fi
                                                                                                          
    $GETTEXTIZE --version < /dev/null > /dev/null 2>&1
    if test $? -ne 0; then
      echo
      echo "**Error**: You must have \`$GETTEXTIZE' installed to compile $PKG_NAME."
      echo "Get $GETTEXTIZE_URL"
      echo "(or a newer version if it is available)"
      DIE=1
    fi
  fi
fi

if test "$DIE" -eq 1; then
	exit 1
fi

test $TEST_TYPE $FILE || {
	echo "You must run this script in the top-level $PROJECT directory"
	exit 1
}

#if test -z "$*"; then
#	echo "I am going to run ./configure with no arguments - if you wish "
#        echo "to pass any to it, please specify them on the $0 command line."
#fi

case $CC in
*xlc | *xlc\ * | *lcc | *lcc\ *) am_opt=--include-deps;;
esac

for coin in .
do 
  dr=`dirname $coin`
  if test -f $dr/NO-AUTO-GEN; then
    echo skipping $dr -- flagged as no auto-gen
  else
    echo processing $dr
    macrodirs= #`sed -n -e 's,AM_ACLOCAL_INCLUDE(\(.*\)),\1,gp' < $coin`
    ( cd $dr
      aclocalinclude="$ACLOCAL_FLAGS"
      for k in $macrodirs; do
  	if test -d $k; then
          aclocalinclude="$aclocalinclude -I $k"
  	##else 
	##  echo "**Warning**: No such directory \`$k'.  Ignored."
        fi
      done
      if grep "^AM_GLIB_GNU_GETTEXT" $CONFIGURE >/dev/null; then
	if grep "sed.*POTFILES" $CONFIGURE >/dev/null; then
	  : do nothing -- we still have an old unmodified $CONFIGURE
	else
	  echo "Creating $dr/aclocal.m4 ..."
	  test -r $dr/aclocal.m4 || touch $dr/aclocal.m4
	  echo "Running glib-gettextize...  Ignore non-fatal messages."
	  echo "no" | glib-gettextize --force --copy
	  echo "Making $dr/aclocal.m4 writable ..."
	  test -r $dr/aclocal.m4 && chmod u+w $dr/aclocal.m4
        fi
      fi
      if grep "^IT_PROG_INTLTOOL" $CONFIGURE >/dev/null; then
        echo "Running intltoolize..."
	intltoolize --copy --force --automake
      fi
      if grep "^GNOME_DOC_INIT" $CONFIGURE >/dev/null; then
        echo "Running $GNOME_DOC_PREPARE..."
	$GNOME_DOC_PREPARE --force --copy || exit 1
      fi
      if grep "^AM_PROG_LIBTOOL" $CONFIGURE >/dev/null; then
	echo "Running $LIBTOOLIZE..."
	$LIBTOOLIZE --force --copy
      fi
      echo "Running $ACLOCAL $aclocalinclude ..."
      $ACLOCAL $aclocalinclude
      if grep "^AM_CONFIG_HEADER" $CONFIGURE >/dev/null; then
	echo "Running $AUTOHEADER..."
	$AUTOHEADER
      fi
      echo "Running $AUTOMAKE --copy --gnu $am_opt ..."
      $AUTOMAKE --add-missing --copy --gnu $am_opt
      echo "Running $AUTOCONF ..."
      $AUTOCONF
    )
  fi
done

conf_flags="--enable-maintainer-mode"

cd "$ORIGDIR"

if test x$NOCONFIGURE = x; then
  echo Running $srcdir/configure $conf_flags "$@" ...
  $srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PROJECT  || exit 1
else
  echo Skipping configure process.
fi

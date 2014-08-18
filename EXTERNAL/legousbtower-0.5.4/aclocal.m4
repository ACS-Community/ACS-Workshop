dnl AX_MSG_RESULT_YN(a)
dnl results "yes" iff a==1, "no" else
AC_DEFUN(AX_MSG_RESULT_YN, [x=no
test "x$1" = "x1" && x=yes
AC_MSG_RESULT($x)])

dnl AX_EGREP(expr, file, action-if-found, action-if-not-found)
dnl egrep for expr in file
AC_DEFUN(AX_EGREP, [dnl
changequote(, )dnl
  if egrep "$1" $2 >/dev/null 2>&1; then
changequote([, ])dnl
  ifelse([$3], , :, [$3])
ifelse([$4], , , [else
  $4
])dnl
fi
])

dnl AX_MACRO_STR(macro, file)
dnl grep string #define out of header
dnl result value in $ax_result
AC_DEFUN(AX_MACRO_STR,
[AC_REQUIRE_CPP()dnl
cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
#include <$2>
[$1]
EOF
(eval "$ac_cpp conftest.$ac_ext") 2>&AC_FD_CC |\
 sed -n -e 's/^ *"\(.*\)".*$/\1/p' >conftest
changequote(, )dnl
set "X" "`cat conftest`"
changequote([, ])dnl
ax_result=[$]2
rm -f conftest*
])

AC_DEFUN(AX_SAVE, [eval $1_AX_[$ax_stack]=[\"]$$1[\"]])
AC_DEFUN(AX_RESTORE, [eval $1=[\"\$]{$1_AX_[$ax_stack]}[\"]])

dnl AX_PUSH
dnl save the contents of CFLAGS etc.
AC_DEFUN(AX_PUSH, [dnl
ax_stack=`expr 0$ax_stack + 1`
AX_SAVE(CFLAGS)
AX_SAVE(CPPFLAGS)
AX_SAVE(LDFLAGS)
AX_SAVE(LIBS)
])

dnl AX_POP
dnl restore the contents of CFLAGS etc.
AC_DEFUN(AX_POP, [dnl
AX_RESTORE(CFLAGS)
AX_RESTORE(CPPFLAGS)
AX_RESTORE(LDFLAGS)
AX_RESTORE(LIBS)
ax_stack=`expr 0$ax_stack - 1`
])

dnl AX_GCC_NOALIAS
dnl adjust CFLAGS to include -fno-strict-aliasing if available.
AC_DEFUN(AX_GCC_NOALIAS, [dnl
AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK([whether $CC needs -fno-strict-aliasing], ax_cv_prog_gcc_noalias, [
  ax_cv_prog_gcc_noalias=no
  if test "$GCC" ; then
    AX_PUSH
    CFLAGS="-fno-strict-aliasing $CFLAGS"
    AC_TRY_COMPILE([], [return;], ax_cv_prog_gcc_noalias=yes)
    AX_POP
  fi
])
test "$ax_cv_prog_gcc_noalias" = "yes" && CFLAGS="-fno-strict-aliasing $CFLAGS"
])

dnl AX_FMTSTRING(N, V)
dnl do AC_DEFINE_UNQUOTED if V is a proper format string
AC_DEFUN(AX_FMTSTRING, [dnl
if test `echo "$2" | wc -c` -gt 120 ; then
  AC_MSG_WARN(format string too long - ignoring);
else
  changequote(, )dnl why do I have to use [[]] despite this???
  dnl change %[^s] to %% and %s to %
  dnl the blank after $2 is necessary!
  ax_ps=`echo "$2 " | sed 's/%[[^s]]/%%/g;s/%s/%/g;s/[[^%]]//g'`
  changequote([, ])dnl
  if test "$ax_ps" = "%" ; then 
    AC_DEFINE_UNQUOTED($1, $2)
  else 
    AC_MSG_WARN($2 is not a proper format string - ignoring); 
  fi
fi])

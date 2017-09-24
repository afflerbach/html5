PHP_ARG_ENABLE(
    html5,
    [whether to enable HTML5 support],
    [  --enable-html5          Enable HTML5 support],
    [yes],
    [no]
)

if test "$PHP_HTML5" != "no" ; then
    #AC_CHECK_HEADER(libxml/tree.h, ,   [ AC_MSG_ERROR([Couldn't find libxml2 headers]) ])
    AC_CHECK_HEADER(myhtml/myhtml.h, , [ AC_MSG_ERROR([Couldn't find myhtml headers]) ])

    PHP_ADD_LIBPATH(/usr/local/lib64)
    PHP_ADD_LIBPATH(/usr/local/lib)
    PHP_ADD_LIBPATH(/usr/lib64)
    PHP_ADD_LIBPATH(/usr/lib)

    CFLAGS+=' -std=c99 -Wall -pedantic -I/usr/local/include/libxml2 -I/usr/include/libxml2 -Wl,-lmyhtml -Wl,-lxml2'

    PHP_ARG_ANALYZE_EX([$PHP_HTML5])
    PHP_NEW_EXTENSION(html5, php_html5.c html5.c dump.c, $ext_shared)
fi

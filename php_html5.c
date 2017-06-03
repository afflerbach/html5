#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <ext/dom/xml_common.h>

#include <php_html5.h>
#include <html5.h>

/* ======================================================================== */

ZEND_BEGIN_ARG_INFO_EX(arginfo_html5_parse, 0, 0, 1)
    ZEND_ARG_INFO(0, "html")
    ZEND_ARG_TYPE_INFO(0, html, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* ======================================================================== */

const zend_function_entry PHP_HTML5_EXTENSION_FUNCTIONS[] = {
    PHP_FE(html5_parse, arginfo_html5_parse)
    PHP_FE_END
};

zend_module_entry html5_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_HTML5_EXTENSION_NAME,
    PHP_HTML5_EXTENSION_FUNCTIONS,
    NULL, /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    PHP_HTML5_EXTENSION_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HTML5
ZEND_GET_MODULE(html5)
#endif

/* ========================================================================= */

PHP_FUNCTION(html5_parse) {
    char *html, *encoding;
    size_t html_len, encoding_len;
    int ret;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s", &html, &html_len, &encoding, &encoding_len) == FAILURE) {
        return;
    }

    xmlDocPtr document = parseHTML5((const char*) html, html_len, (const char*) encoding);

    DOM_RET_OBJ((xmlNodePtr) document, &ret, NULL);
}

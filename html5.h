#ifndef HTML5_H
#define HTML5_H

#include <zend_types.h>
#include <zend_smart_str_public.h>
#include <libxml/HTMLtree.h>

xmlDocPtr parseHTML5(const char *html, size_t length, const char* encoding);
void dumpHTML5(xmlNodePtr node, smart_str *buffer);

#endif

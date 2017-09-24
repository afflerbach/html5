#include <zend_smart_str.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlstring.h>

#define SVG_NS "http://www.w3.org/2000/svg"

// https://www.w3.org/TR/html5/syntax.html#elements-0
static const char *voidElements[26][2] = {
    ['a' - 97] = { "area", "" },
    ['b' - 97] = { "br", "base" },
    ['c' - 97] = { "col", "" },
    ['e' - 97] = { "embed" "" },
    ['h' - 97] = { "hr", "" },
    ['i' - 97] = { "img", "input" },
    ['k' - 97] = { "keygen", "" },
    ['l' - 97] = { "link", "" },
    ['m' - 97] = { "meta", "" },
    ['p' - 97] = { "param" "" },
    ['s' - 97] = { "source" "" },
    ['t' - 97] = { "track" "" },
    ['w' - 97] = { "wbr" "" },
};

const char *selfClosingSVGElements[1] = {
    "image"
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int isVoidElement(xmlNodePtr element) {
    const char *elementName = (const char*) element->name;

    if (elementName[0]) {
        char **elementNames = (char**) voidElements[elementName[0] - 97];

        if (*elementNames) {
            for (int i = 0; i < 2; i++) {
                if (elementNames[i] && strcmp(elementName, elementNames[i]) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

 int isSelfClosingForeignElement(xmlNodePtr element) {
    xmlNsPtr namespace = element->ns;
     if (namespace && xmlStrcmp(namespace->href, BAD_CAST SVG_NS) == 0) {

        const char *elementName = (const char*) element->name;

        for (int i = 0; i < 1; i++) {
            if (strcmp(elementName, selfClosingSVGElements[i]) == 0) {
                return 1;
            }
        }
    }

    return 0;
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpNode(xmlNodePtr node, smart_str *buffer);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpNodes(xmlNodePtr node, smart_str *buffer) {
    while (node) {
        dumpNode(node, buffer);
        node = node->next;
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpAttribute(xmlAttrPtr attribute, smart_str *buffer) {
    if (attribute == NULL) {
        return;
    }

    xmlNsPtr namespace = attribute->ns;
    if (namespace != NULL && namespace->prefix != NULL) {
        smart_str_appends(buffer, (const char*) namespace->prefix);
        smart_str_appendc(buffer, ':');
    }

    smart_str_appends(buffer, (const char*) attribute->name);
    if (attribute->children == NULL) {
        return;
    }

    xmlChar *value = xmlNodeListGetString(attribute->doc, attribute->children, 0);

    smart_str_appends(buffer, "=\"");
    smart_str_appends(buffer, (const char*) value);
    smart_str_appendc(buffer, '"');
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpAttributes(xmlAttrPtr attribute, smart_str *buffer) {
    while (attribute) {
        smart_str_appendc(buffer, ' ');
        dumpAttribute(attribute, buffer);
        attribute = attribute->next;
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpDocument(xmlDocPtr document, smart_str *buffer) {
    if (document->intSubset) {
        smart_str_appends(buffer, "<!DOCTYPE html>\n");
    }

    dumpNodes(document->children, buffer);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpElement(xmlNodePtr element, smart_str *buffer) {
    smart_str_appendc(buffer, '<');
    smart_str_appends(buffer, (const char*) element->name);

    dumpAttributes(element->properties, buffer);

    if (isSelfClosingForeignElement(element)) {
        smart_str_appends(buffer, "/>");
        return;
    }

    smart_str_appendc(buffer, '>');

    if (isVoidElement(element)) {
        return;
    }

    dumpNodes(element->children, buffer);

    smart_str_appends(buffer, "</");
    smart_str_appends(buffer, (const char*) element->name);
    smart_str_appendc(buffer, '>');
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpComment(xmlNodePtr comment, smart_str *buffer) {
    smart_str_appends(buffer, "<!--");
    smart_str_appends(buffer, (const char*) comment->content);
    smart_str_appends(buffer, "-->");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpText(xmlNodePtr text, smart_str *buffer) {
    // FIXME CDATA
    smart_str_appends(buffer, (const char*) text->content);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpNode(xmlNodePtr node, smart_str *buffer) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
            dumpDocument((xmlDocPtr) node, buffer);
            break;

        case XML_ELEMENT_NODE:
            dumpElement(node, buffer);
            break;

        case XML_ATTRIBUTE_NODE:
            dumpAttribute((xmlAttrPtr) node, buffer);
            break;

        case HTML_TEXT_NODE:
            dumpText(node, buffer);
            break;

        case HTML_COMMENT_NODE:
            dumpComment(node, buffer);
            break;

        case HTML_ENTITY_REF_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
        case XML_NAMESPACE_DECL:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
        case XML_DOCB_DOCUMENT_NODE:
            break;
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void dumpHTML5(xmlNodePtr node, smart_str *buffer) {
    dumpNode(node, buffer);
}

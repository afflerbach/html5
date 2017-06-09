#include <ctype.h>

#include <libxml/HTMLtree.h>
#include <myhtml/myhtml.h>
#include <myhtml/mynamespace.h>

#include <html5.h>

static const char *attributeNamespacePrefixes[MyHTML_NAMESPACE_LAST_ENTRY][2] = {
    [MyHTML_NAMESPACE_XMLNS] = { "xmlns", "http://www.w3.org/2000/xmlns/" },
    [MyHTML_NAMESPACE_XLINK] = { "xlink", "http://www.w3.org/1999/xlink" }
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

char *trim(char *str) {
    while (isspace((unsigned char) *str)) str++;

    if (*str) {
        char *end = str;
        while (*end) end++;
        while (isspace((unsigned char) *(--end)));
        end[1] = '\0';
    }

    return str;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addChildren(const xmlNodePtr parentNode, const myhtml_tree_t *tree, myhtml_tree_node_t *children);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addMetaCharsetElement(xmlNodePtr headElement, const char* encoding) {
    xmlNodePtr meta = xmlNewNode(NULL, BAD_CAST "meta");
    xmlChar *content = xmlStrncatNew(BAD_CAST "text/html; charset=", BAD_CAST encoding, -1);

    xmlNewProp(meta, BAD_CAST "http-equiv", BAD_CAST "Content-Type");
    xmlNewProp(meta, BAD_CAST "content",    content);
    xmlAddChild(headElement, meta);

    xmlFree(content);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

xmlNodePtr createDOMElement(const myhtml_tree_t *tree, myhtml_tree_node_t *element) {
    xmlNsPtr namespace = NULL;
    myhtml_namespace_t ns;
    myhtml_tag_id_t nodeType = myhtml_node_tag_id(element);

    const char *elementName = myhtml_tag_name_by_id((myhtml_tree_t*)tree, nodeType, NULL);

    ns = myhtml_node_namespace(element);
    if (ns != MyHTML_NAMESPACE_UNDEF && ns != MyHTML_NAMESPACE_HTML) {
        const char *namespaceURI = myhtml_namespace_url_by_id(ns, NULL);
        namespace = xmlNewNs(NULL, BAD_CAST namespaceURI, NULL);
    }

    return xmlNewNode(namespace, BAD_CAST elementName);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

xmlNsPtr createAttributeNamespace(myhtml_tree_attr_t *attribute) {

    myhtml_namespace_t ns = myhtml_attribute_namespace(attribute);
    if (ns == MyHTML_NAMESPACE_UNDEF || ns == MyHTML_NAMESPACE_HTML) {
        return NULL;
    }

    const char *namespaceURI = myhtml_namespace_url_by_id(ns, NULL);
    const char *namespacePrefix = NULL;

    if (ns != MyHTML_NAMESPACE_XMLNS || strcmp(myhtml_attribute_key(attribute, NULL), "xmlns") != 0) {
        namespacePrefix = attributeNamespacePrefixes[ns][0];
    }

    return xmlNewNs(NULL, BAD_CAST namespaceURI, BAD_CAST namespacePrefix);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addAttributes(xmlNodePtr domElement, myhtml_tree_attr_t *attributes) {
    myhtml_tree_attr_t *attribute = attributes;
    while (attribute) {
        xmlNsPtr namespace = createAttributeNamespace(attribute);

        const char *name = myhtml_attribute_key(attribute, NULL);
        const char *value = myhtml_attribute_value(attribute, NULL);
        xmlAttrPtr domAttr = xmlNewNsProp(domElement, namespace, BAD_CAST name, BAD_CAST value);

        if (strcmp(name, "id") == 0) {
            xmlAddID(NULL, domElement->doc, BAD_CAST value, domAttr);
        }

        attribute = myhtml_attribute_next(attribute);
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addElement(xmlNodePtr parentNode, const myhtml_tree_t *tree, myhtml_tree_node_t *element) {
    xmlDocPtr document = parentNode->doc;
    xmlNodePtr domElement = createDOMElement(tree, element);

    xmlAddChild(parentNode, domElement);

    if (parentNode == (xmlNodePtr) document) {
        xmlDocSetRootElement(document, domElement);
    }

    addAttributes(domElement, myhtml_node_attribute_first(element));

    myhtml_tag_id_t nodeType = myhtml_node_tag_id(element);

    if (nodeType == MyHTML_TAG_HEAD) {
        addMetaCharsetElement(domElement, (const char*) document->encoding);
    }

    addChildren(domElement, tree, myhtml_node_child(element));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addChildren(const xmlNodePtr parentNode, const myhtml_tree_t *tree, myhtml_tree_node_t *children) {
    myhtml_tree_node_t *child = children;
    while (child) {
        switch (myhtml_node_tag_id(child)) {
            case MyHTML_TAG__TEXT:
                xmlAddChild(parentNode, xmlNewText(BAD_CAST myhtml_node_text(child, NULL)));
                break;
            case MyHTML_TAG__COMMENT:
                xmlAddChild(parentNode, xmlNewComment(BAD_CAST myhtml_node_text(child, NULL)));
                break;
            case MyHTML_TAG__END_OF_FILE:
                break;
            case MyHTML_TAG__DOCTYPE:
            case MyHTML_TAG__UNDEF:
                break;
            default:
                addElement(parentNode, tree, child);
                break;
        }

        child = myhtml_node_next(child);
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

myencoding_t determineEncoding(const char *encoding, const char *html, size_t length) {
    myencoding_t result = MyENCODING_NOT_DETERMINED;

    if (encoding && *encoding) {
        const char *trimmedEncoding = trim((char*) encoding);
        myencoding_by_name(trimmedEncoding, strlen(trimmedEncoding), &result);
    }

    if (result == MyENCODING_NOT_DETERMINED) {
        result = myencoding_prescan_stream_to_determine_encoding(html, length);
    }

    return result == MyENCODING_NOT_DETERMINED ? MyENCODING_UTF_8 : result;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void setEncoding(xmlDocPtr document, const char *encoding) {
    if (document->encoding != NULL) {
        xmlFree((xmlChar *) document->encoding);
    }

    document->encoding = xmlStrdup((const xmlChar *) encoding);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

xmlDocPtr parseHTML5(const char *html, size_t length, const char* encoding) {
    myhtml_t *myhtml;
    myhtml_tree_t *tree;
    myhtml_tree_node_t *rootNode;
    xmlDocPtr document;
    myencoding_t treeEncoding;

    myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    treeEncoding = determineEncoding(encoding, html, length);
    myhtml_parse(tree, treeEncoding, html, length);

    rootNode = myhtml_node_child(myhtml_tree_get_document(tree));

    document = xmlNewDoc(BAD_CAST "1.0");
    setEncoding(document, myencoding_name_by_id(treeEncoding, NULL));
    xmlCreateIntSubset(document, BAD_CAST "html", NULL, NULL);
    addChildren((xmlNodePtr) document, tree, rootNode);

    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return document;
}

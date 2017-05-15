#include <libxml2/libxml/HTMLtree.h>
#include <myhtml/myhtml.h>
#include <myhtml/mynamespace.h>

#include <html5.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void addChildren(const xmlNodePtr parentNode, const myhtml_tree_t *tree, myhtml_tree_node_t *children);

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

void addAttributes(xmlNodePtr domElement, myhtml_tree_attr_t *attributes) {
    myhtml_tree_attr_t *attribute = attributes;
    while (attribute) {
        const char *name = myhtml_attribute_key(attribute, NULL);
        const char *value = myhtml_attribute_value(attribute, NULL);
        xmlAttrPtr domAttr = xmlNewProp(domElement, BAD_CAST name, BAD_CAST value);

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

xmlDocPtr parseHTML5(const char *html, size_t length) {
    myhtml_t *myhtml;
    myhtml_tree_t *tree;
    myhtml_tree_node_t *rootNode;
    xmlDocPtr document;

    myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    myhtml_parse(tree, MyENCODING_UTF_8, html, length);

    rootNode = myhtml_node_child(myhtml_tree_get_document(tree));

    document = xmlNewDoc(BAD_CAST "1.0");
    xmlCreateIntSubset(document, BAD_CAST "html", NULL, NULL);
    addChildren((xmlNodePtr) document, tree, rootNode);

    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return document;
}

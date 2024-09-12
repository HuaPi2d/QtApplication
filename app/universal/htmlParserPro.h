#ifndef HTMLPARSERPRO_H
#define HTMLPARSERPRO_H


#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <iostream>
#include <string>

// 初始化并解析 HTML
xmlDocPtr get_html_doc(const std::string &html_content) {
    return htmlReadMemory(html_content.c_str(), html_content.size(), NULL, "UTF-8", HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
}

// 执行 XPath 查询
xmlXPathObjectPtr get_xpath_nodeset(xmlDocPtr doc, const xmlChar *xpath) {
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        std::cerr << "Error in xmlXPathNewContext\n";
        return nullptr;
    }
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (result == NULL) {
        std::cerr << "Error in xmlXPathEvalExpression\n";
        return nullptr;
    }
    return result;
}

#endif // HTMLPARSERPRO_H

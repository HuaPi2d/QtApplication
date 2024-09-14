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

// 函数接口：传入节点和 XPath 表达式，返回匹配的内容或属性
std::string get_node_attribute(xmlNodePtr node, const xmlChar* xpathExpr) {
    std::string result;

    if (node == NULL || xpathExpr == NULL) {
        std::cerr << "Invalid node or XPath expression" << std::endl;
        return result;
    }

    // 获取文档指针，node 所属的文档
    xmlDocPtr doc = node->doc;

    // 创建 XPath 上下文
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL) {
        std::cerr << "Failed to create XPath context" << std::endl;
        return result;
    }

    // 设置上下文节点
    xpathCtx->node = node;

    // 评估 XPath 表达式
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        return result;
    }

    // 获取结果节点集
    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    if (nodes != NULL && nodes->nodeNr > 0) {
        xmlNodePtr resultNode = nodes->nodeTab[0];
        if (resultNode->type == XML_ATTRIBUTE_NODE || resultNode->type == XML_ELEMENT_NODE) {
            // 获取节点内容或属性
            xmlChar* content = xmlNodeGetContent(resultNode);
            if (content != NULL) {
                result += (const char*)content;
                xmlFree(content);
            }
        }
    } else {
        std::cerr << "No nodes found for the given XPath expression" << std::endl;
    }

    // 释放 XPath 对象和上下文
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    return result;
}

#endif // HTMLPARSERPRO_H

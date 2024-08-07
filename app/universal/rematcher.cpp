#include "rematcher.h"

QRegularExpressionMatchIterator findFitStruct(QString pattern, QString content)
{
    QRegularExpression regex(pattern);
    QRegularExpressionMatchIterator i = regex.globalMatch(content);
    return i;
}



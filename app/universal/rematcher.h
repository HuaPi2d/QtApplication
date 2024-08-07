#ifndef REMATCHER_H
#define REMATCHER_H

#include <QObject>
#include <QRegularExpression>


QRegularExpressionMatchIterator findFitStruct(QString pattern, QString content);

#endif // REMATCHER_H

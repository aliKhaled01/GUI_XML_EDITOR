#ifndef CHECKCONSISTENCY_H
#define CHECKCONSISTENCY_H
#include <QMainWindow>
#include "QStack"
#include "QMap"
bool checkConsistency(QVector<QString> xmlVec);
QMap<qint32, QString> detectErrors(QVector<QString> xml);
#endif

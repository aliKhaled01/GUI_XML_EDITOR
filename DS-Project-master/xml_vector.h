#ifndef XML_VECTOR_H
#define XML_VECTOR_H
#include <QMainWindow>
#include <QString>
using namespace std;
QString removeSpaces(QString line);
QVector<QString> xmlToVec(QString str);
vector<string> xmlToVecN(string str);
string removeSpacesN(string line);
#endif // XML_VECTOR_H

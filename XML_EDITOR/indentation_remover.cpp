#include <indentation_remover.h>

QString indentation_remover(QString str, QString indentation){
    QString tmp = "";
    qint32 indLength = indentation.length();
    for(int i=0; i<str.length()-indLength;i++){
        tmp += str[i];
    }
    return tmp;
}

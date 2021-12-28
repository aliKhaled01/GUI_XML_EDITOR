#include "checkConsistency.h"


// XML Consistency Check. Returns a boolean value whether the file is XML-consistent or not
bool checkConsistency(QVector<QString> xmlVec){
    QStack<QString> tag;
        for (int i = 0; i < xmlVec.size(); i++) {
            QString line = xmlVec[i];
            if (line[0] == '<') {
                if (line[1] == '/') {
                    if(!tag.empty()){
                        if (line != ("</" + tag.top())) {
                            // tags mismatch
                            return false;
                        }
                        else {
                            tag.pop();
                        }
                    }
                    else{
                        //no opening tag
                        return false;
                    }
                }
                else if(line[1] != '!' && line[1] != '?') {
                    if(!(line[line.length()-2] == '/' && line[line.length()-1] == '>')){
                        QString temp = "";
                        for(int j=1; j<line.length(); j++){
                            if(line[j] != ' ' && line[j] != '>'){
                             temp += line[j];
                            }
                            else{
                                temp += '>';
                                break;
                            }
                        }
                        tag.push(temp);
                    }
                }
            }
        }
    return (tag.empty());
}


QMap<qint32, QString> detectErrors(QVector<QString> xml){
    QMap<qint32, QString> result;
    QStack<QString> tag;
    QStack<int> lineNum;
        int i = 0;
        for (i = 0; i < xml.size(); i++) {
            QString l = xml[i];
            if (l[0] == '<') {
                if (l[1] == '/') {
                    if(!tag.empty()){
                        if (l != ("</" + tag.top())) {
                            // tags mismatch : make a closing tag that fits the opening one
                            result[i] = "</" + tag.top();
                            tag.pop();
                            lineNum.pop();
                        }
                        else {
                            tag.pop();
                            lineNum.pop();
                        }
                    }
                    else{
                        // closing tag without opening tag    --->   remove it
                        result[i] = "D";
                    }
                }
                else if(l[1] != '!' && l[1] != '?') {
                    if(!(l[l.length()-2] == '/' && l[l.length()-1] == '>')){
                        QString tmp = "";
                        for(int j=1; j<l.length(); j++){
                            if(l[j] != ' ' && l[j] != '>'){
                             tmp += l[j];
                            }
                            else{
                                tmp += '>';
                                break;
                            }
                        }
                        tag.push(tmp);
                        lineNum.push(i);
                    }
                }
            }
            else{
                // no closing tag : add it
                if(i+1 < xml.length()){
                    QString closingTag = xml[i+1]; //must be closing tag
                    if(!(closingTag[0] == '<' && closingTag[1] == '/')){
                        if(!tag.empty()){
                            QString correct = "</" + tag.top();
                            tag.pop();
                            lineNum.pop();
                            result[i] = "U" + correct;
                        }
                    }
                }
            }
        }
        if(!tag.empty()){
            // opening tag without closing tag    --->   add the missing closing tag
            int size = tag.size();
            for(int j = 0; j<size; j++){
                QString extra = tag.top();
                result[lineNum.top()] = "A" + extra;
                tag.pop();
                lineNum.pop();
            }
        }
        return result;
}


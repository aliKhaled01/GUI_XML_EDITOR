#include "xml_vector.h"

//xml code to vector of strings

QString removeSpaces(QString line){
    int start = 0;
    while(line[start] == ' ' || line[start] == '\t' || line[start] == '\n'){
        start += 1;
    }
    int end = line.length() - 1;
    while(line[end] == ' ' || line[end] == '\t' || line[end] == '\n') {
        end -= 1;
    }
    QString result = "";
    for(int i = start ; i <= end; i++){
        result += line[i];
    }
    return result;
}

QVector<QString> xmlToVec(QString str){
    QVector<QString> resultVec;
    QString tmp = "";
    int i = 0;
    while(i < str.length()){
        if(str[i] == '<'){
            while(i < str.length()){
                if(str[i] != '>'){
                    tmp += str[i];
                }
                else{
                    break;
                }
                i++;
            }
            tmp += '>';
            tmp = removeSpaces(tmp);
            i++;
            resultVec.push_back(tmp);
            tmp = "";
        }
        else{
            while(i < str.length()) {
                if(str[i] != '<'){
                    tmp += str[i];
                }
                else{
                    break;
                }
                i++;
            }
            bool a = false;
            for(int j=0; j<tmp.length(); j++){
                if(tmp[j] != ' ' && tmp[j] != '\n' && tmp[j] != '\t'){
                    a = true;
                    break;
                }
            }
            if(a == true){
                tmp = removeSpaces(tmp);
                resultVec.push_back(tmp);
            }
            tmp = "";
        }
    }
    return resultVec;
}


string removeSpacesN(string line){
    int start = 0;
    while(line[start] == ' ' || line[start] == '\t' || line[start] == '\n'){
        start += 1;
    }
    int end = line.length() - 1;
    while(line[end] == ' ' || line[end] == '\t' || line[end] == '\n') {
        end -= 1;
    }
    string result = "";
    for(int i = start ; i <= end; i++){
        result += line[i];
    }
    return result;
}

vector<string> xmlToVecN(string str){
    vector<string> resultVec;
    string tmp = "";
    int i = 0;
    while(i < str.length()){
        if(str[i] == '<'){
            while(i < str.length()){
                if(str[i] != '>'){
                    tmp += str[i];
                }
                else{
                    break;
                }
                i++;
            }
            tmp += '>';
            tmp = removeSpacesN(tmp);
            i++;
            resultVec.push_back(tmp);
            tmp = "";
        }
        else{
            while(i < str.length()) {
                if(str[i] != '<'){
                    tmp += str[i];
                }
                else{
                    break;
                }
                i++;
            }
            bool a = false;
            for(int j=0; j<tmp.length(); j++){
                if(tmp[j] != ' ' && tmp[j] != '\n' && tmp[j] != '\t'){
                    a = true;
                    break;
                }
            }
            if(a == true){
                tmp = removeSpacesN(tmp);
                resultVec.push_back(tmp);
            }
            tmp = "";
        }
    }
    return resultVec;
}

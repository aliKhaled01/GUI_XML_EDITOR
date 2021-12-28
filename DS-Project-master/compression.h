#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <iostream>
#include<stdio.h>
#include<queue>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<cstdlib>
#include<fstream>
#include <sstream>
#include "QMainWindow"
#include "QMessageBox"
#include "QFile"
#include "QTextStream"
#include "QDir"
#define setBit(reg,bit) reg|=(1<<bit)
#define clearBit(reg,bit) reg&=~(1<<bit)
#define readBit(reg,bit) ((reg>>bit)&1)

using namespace std;

string substr(string s,int index,int len,int stringlen);
string binaryToString(string z);
void writefile(string filename,int len, queue<unsigned char> &kj);

class treeNode{
private:
    char ch;
    int frequency;
    bool z; //a leaf or not
    treeNode* left;
    treeNode* right;
public:
    treeNode(char ch,int frequency);
    treeNode();
    treeNode* addParentNode(treeNode*left,treeNode*right);
    bool leaf_checker(treeNode*n);
    int getFrequency();
    char getChar();
    treeNode* getLeft();
    treeNode* getRight();
    treeNode* setNode(char ch,int frequency,treeNode*left,treeNode*right);
    friend class tree;
};

class compare{
public:
    bool operator()(treeNode* n,treeNode* m);
};

class tree{
private:
    treeNode*root;
public:
    void encode(treeNode* n,string c, map<char,string>&newcode);
    queue<unsigned char> assembleTree(string c, map<char,string> &codes,int &sizee);
};

#endif // COMPRESSION_H

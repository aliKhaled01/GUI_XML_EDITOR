#ifndef XML_TO_JSON_H
#define XML_TO_JSON_H
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <string>

using namespace std;

class attribute{
    public:
        string key="";
        string value="";
};

class node{
public:
        string tag;
        string code;
        node* last = nullptr;
        std::list<attribute> attribute;
        std::list<node*>nodePtrList;
        bool isDuplicated=0;
        bool noAttributes();
        int parents_number();
};

string tag_print(string s);
int attribute_check(string s);
vector<string> attribute_print(string s, int index);
int duplication_check(node *child);
node* tree_creator(vector<string> strVec, int size);
string json_print(node* root);
string iteration(node* root, string& result);


#endif

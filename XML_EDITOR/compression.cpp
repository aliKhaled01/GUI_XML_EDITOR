#include "compression.h"

string substr(string s,int index,int len,int stringlen){
    string substr="";
    for (int i=index;i<(index+len);i++){
        substr+=s[i];
        if (i==stringlen) break;
    }
    return substr;
}

string binaryToString(string bstr){//from binary to string
    unsigned char bc=0;
    string result="";
    for(unsigned int i=0;i<bstr.length();i++){
        bc=bstr[i];
        for(int j=0;j<8;j++){
        if (readBit(bc,7-j)==1) result+="1";
           else result+="0";
      }
    }
    return result;
}

treeNode::treeNode(char ch,int frequency){
    this->ch=ch;
    this->frequency=frequency;
    z=1;    //it is a leaf
    left=NULL;
    right=NULL;
}

treeNode::treeNode(){}

treeNode* treeNode::addParentNode(treeNode*left,treeNode*right){
    treeNode*p=new treeNode();
    p->frequency=left->frequency+right->frequency;
    p->left=left;
    p->right=right;
    z=0;    //it is not a leaf
    p->ch='\0';
    return p;
}

bool treeNode::leaf_checker(treeNode*n){
    return n->left==NULL && n->right==NULL;
}

int treeNode::getFrequency(){
    return frequency;
}

char treeNode::getChar(){
    return ch;
}

treeNode* treeNode::getLeft(){
    return left;
}

treeNode* treeNode::getRight(){
    return right;
}

treeNode* treeNode::setNode(char ch,int frequency,treeNode*left,treeNode*right){
    treeNode*ptr=new treeNode();
    ptr->ch=ch;
    ptr->frequency=frequency;
    ptr->left=left;
    ptr->right=right;
    return ptr;
}

bool::compare::operator()(treeNode* n,treeNode* m){
    return n->getFrequency() > m->getFrequency();
}

void tree::encode(treeNode* node,string str, map<char,string>&newCode){
    if(node==NULL) return;
       //to check if it is leaf or not
       if(node->leaf_checker(node)) {
           if (str!="") newCode[node->ch] = str;
           else newCode[node->ch] = "1";
       }
       encode(node->left,str+"0",newCode);
       encode(node->right,str+"1",newCode);
}

queue<unsigned char> tree::assembleTree(string str, map<char,string> &code,int &size){
    queue<unsigned char> resultQ;
    if (str=="") return resultQ;
    //finding the frequency of characters
    map<char,int>charAndFreq; //store chars and frequencies
    int freq=0;
    for (unsigned int i=0;i<str.length();i++){
        freq= count(str.begin(),str.end(),str[i]);
        if ((freq!=0) && (charAndFreq.count(str[i])==0)) charAndFreq[str[i]]=freq;
    }
    //pushing all nodes first
    treeNode n;
    priority_queue<treeNode*,vector<treeNode*>,compare>que;   //saving nodes
    for(auto i=charAndFreq.begin();i!=charAndFreq.end();i++) {
        que.push(n.setNode(i->first,i->second,NULL,NULL));
    }

    //making parent nodes
    while (que.size()!=1){
        treeNode* left=que.top();
        que.pop();
        treeNode* right=que.top();
        que.pop();
        que.push(n.addParentNode(left,right));
    }
    root=que.top(); //root of huffman tree


    map<char,string> newCode;   //store characters and their codes
    tree tr;
    tr.encode(root,"",newCode);

    //now the text after comp
    string s1="";
    for(unsigned int i=0;i<str.length();i++){
        s1+=newCode[str[i]];
    }

    //coverting from string to bits
    //pushing binary no. in a queue
    unsigned char bin=0;
    string s2;
    string s3;
    size = s1.length()%8;
    size = 8-size;
    for(unsigned int i=0;i<s1.length();i+=8){
        s2=substr(s1,i,8,s1.length());
        for(int j=0;j<8;j++){
            s3=s2[j];
            if(s3=="1") setBit(bin,7-j);
            else clearBit(bin,7-j);
      }
       resultQ.push(bin);
    }
    code = newCode;
    return resultQ;
}

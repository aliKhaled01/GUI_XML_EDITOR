#include "xml_to_json.h"
int global=1;
bool node::noAttributes(){
    if(attribute.empty()== 1)
            return 1;
        return 0;
}

int node::parents_number()
{
    int no=0;
    node *tmp=new node;
    tmp=last;
    while(tmp!=nullptr)
    {
        no++;
        tmp=tmp->last;
    }
    return no;
}

string tag_print(string str)
{
    int j =1;
    string tag;
    if(str[1] == '/')
    {
        j=2;
    }
    while((str[j] !='>') && (str[j] !=' ') && (j != str.length()-1))
    {
        if(str[j] == '/')
        {
            break;
        }
        tag += str[j];
        j++;
    }
    return tag;
}

int attribute_check(string s) //this function searching for attribute if found return its starting index else return 0
{
    for(int i=1; i<s.length(); i++)
    {
        if(s[i] == ' ')
            return i+1;
    }
    return 0;
}

vector<string> attribute_print(string str, int index)
{
    int i=index;
        string key;
        string value;
        vector<string>resultVec;
         while(i<str.length())
        {
            if(str[i] == '=')
            {
                resultVec.push_back(key);
                key.clear();
                value.clear();
            }
            else if(str[i] == ' ' || str[i] == '>')
            {
                resultVec.push_back(value);
                key.clear();
                value.clear();
            }
            else
            {
                if(i == str.length()-2 && str[i] =='/')
                {

                }

                else
                {
                    key += str[i];
                    value += str[i];
                }

            }

            i++;

        }
        return resultVec;
}

int duplication_check(node *child)
{
    node *parent=new node;
    parent = child->last;
    int count=0;

    std::list<node*>::iterator j;

    if(parent != nullptr)
    {
        for(j=parent->nodePtrList.begin(); j!=parent->nodePtrList.end(); j++)
        {
            if(*j == child)
            {
                j++;
                while(j!=parent->nodePtrList.end())
                {

                    if(child->tag == (*j)->tag)
                    {
                        count++;
                        (*j)->isDuplicated =1;
                    }
                    else
                    {
                        break;
                    }

                    j++;
                }
                break;
            }

        }
    }
    return count;
}

node* tree_creator(vector<string> strVec, int size)
{

        int i=0;
        vector<string>vec;
        stack<string>openingTags;
        stack<node*>nodes; //pointers to nodes
        int index;
        node *root=new node;

        while(i<size)
        {
                if(strVec[i][0]=='<' && strVec[i][1] != '/' && i==0 &&(strVec[i][1] != '!' && strVec[i][1] != '?'))
                {
                    root->tag=tag_print(strVec[0]);
                    openingTags.push(tag_print(strVec[i]));
                    nodes.push(root);
                    index= attribute_check(strVec[i]);
                    if(index != 0)
                    {
                        vec=attribute_print(strVec[i],index);
                        for(int i=0; i<vec.size(); i+=2)
                        {
                            attribute *node= new attribute;
                            node->key=vec[i];
                            node->value=vec[i+1];
                            root->attribute.push_back(*node);

                        }
                    }

                }
                else if(strVec[i][0]=='<' && strVec[i][1] != '/' &&(strVec[i][1] != '!' && strVec[i][1] != '?'))
                    if(strVec[i][strVec[i].length()-2] =='/')
                    {
                    node *branch= new node;
                    nodes.top()->nodePtrList.push_back(branch);

                    branch->last=(nodes.top()); //children and parents know each other
                    branch->tag=tag_print(strVec[i]);
                    index= attribute_check(strVec[i]);
                    if(index != 0)
                    {
                        vec=attribute_print(strVec[i],index);
                        for(int i=0; i<vec.size(); i+=2)
                        {
                            attribute *aNode= new attribute;
                            aNode->key=vec[i];
                            aNode->value=vec[i+1];
                            branch->attribute.push_back(*aNode);

                        }
                    }

                    }
                    else{
                    {
                    node *branch= new node;
                    nodes.top()->nodePtrList.push_back(branch);

                    branch->last=(nodes.top()); //children and parents know each other
                    branch->tag=tag_print(strVec[i]);

                    openingTags.push(tag_print(strVec[i]));
                    nodes.push(branch);
                    index= attribute_check(strVec[i]);

                    if(index != 0)
                    {
                        vec=attribute_print(strVec[i],index);
                        for(int i=0; i<vec.size(); i+=2)
                        {
                            attribute *anode= new attribute;
                            anode->key=vec[i];
                            anode->value=vec[i+1];
                            branch->attribute.push_back(*anode);

                        }
                    }

                    }



                }
                else if(strVec[i][0]=='<' && strVec[i][1] == '/') //closing tag
                {
                    if(openingTags.top()==tag_print(strVec[i]))
                    {
                        openingTags.pop();
                        nodes.pop();

                    }
                }
                else if(strVec[i][0] != '<')    //the string inside last tag
                {
                    if(nodes.top()->code=="")
                    {
                        nodes.top()->code=strVec[i];
                    }
                    else
                    {
                        nodes.top()->code+=" "+ strVec[i];
                    }
                }
         i++;
        }
        return root;
}

string json_print(node* root)
{
        string sp1;
        string sp2; //for spaces
        for(int i=0 ;i<root->parents_number();i++)
        {
            sp1+="    ";
        }
        sp2=sp1+"    ";
        string resultStr;

            if(root->isDuplicated == 0 )
            {
                resultStr+="\n"+ sp1+ "\""+root->tag + "\":";
                if(duplication_check(root)!=0)
                {
                    resultStr+=" [";
                }
            }
            if(root->last==nullptr && root->nodePtrList.size()==0)
            {
                if(root->code=="")
                {
                        resultStr+=" \"\"";
                }
                else
                {
                    resultStr+=" \""+ root->code +"\"";
                }
            }

            else if(root->attribute.empty() && root->nodePtrList.empty())
            {
                resultStr += '"' + root->code + '"';
                if(root->last->nodePtrList.size()>1)
                {
                    if(!duplication_check(root)==0 || !root->isDuplicated==0)
                    {
                        if(duplication_check!=0)
                        {
                            resultStr+=",";
                        }
                    }
                    else
                        resultStr+=",";
                }
            }


            else if(root->attribute.size() != 0 || root->code != "")
            {
                resultStr += "{\n    ";

                if(root->attribute.size() != 0)
                {
                    for(auto i=root->attribute.begin(); i!= root->attribute.end(); i++)
                    {
                        resultStr += sp1+"\"@" + i->key + "\":"+ i->value +",\n    ";
                    }

                }
                if(root->code != "")
                {
                resultStr += sp1+ "\"#text\":\"" + root->code + "\"";
                if(!root->nodePtrList.empty())
                {
                    resultStr+=",";
                }
                }


            }
            else if(root->nodePtrList.size() !=0)
            {
                resultStr +="{\n    ";
            }

        return resultStr;
}

string iteration(node* root, string& result)
{
        result += json_print(root);
        string spc;
        std::list<node*>::iterator i;
        for (i = root->nodePtrList.begin(); i != root->nodePtrList.end(); i++)
        {
            global++;
            iteration(*i, result);

        }
        for(int i=0 ;i<root->parents_number()+1;i++)
        {
            spc+="    ";
        }
        if (!root->attribute.empty() || !root->nodePtrList.empty())
        {
            result+="\n"+spc+"}";
            if(root->isDuplicated==1 || duplication_check(root)!=0)
            {
                result+=",";
            }

        }
            if(root->isDuplicated==1 && duplication_check(root)==0)
            {
                result+="\n"+spc+"]";
            }

        return result;
}


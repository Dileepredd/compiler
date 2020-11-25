// symbol tabel
#include<bits/stdc++.h>
using namespace std;
enum kind_of{ARGUMENT,FIELD,LOCAL,STATIC_DEC};
enum INTO{CLASS_T,METHOD};
struct record{
    string name;
    kind_of kind;
    string type;
    int position;
};
string getSegement(kind_of kind)
{
    switch(kind)
    {
        case ARGUMENT: return "argument";
        case FIELD: return "this";
        case LOCAL: return "local";
        default: return "static";
    }
}
class symboltabel
{
    unordered_map<string,struct record> classtable;
    unordered_map<string,struct record> methodtable;
    unordered_set<string> allowedtypes;
    int lclcount;
    int argcount;
    int fieldcount;
    int staticcount;
    int labelcount;
    public:
    symboltabel()
    {
        lclcount = 0;
        argcount = 0;
        fieldcount = 0;
        staticcount = 0;
        labelcount = 0;
    }
    bool istype(string type)
    {
        if(allowedtypes.find(type) == allowedtypes.end())return false;
        return true;
    }
    void inserttype(string type)
    {
        allowedtypes.insert(type);
    }
    void clear(void)
    {
        methodtable.clear();
        lclcount = 0;
        argcount = 0;
    }
    void insert(INTO into,string name,kind_of kind,string type)
    {
        int pos = 0;
        if(kind == ARGUMENT)
        {
            pos = argcount;
            argcount++;
        }
        else if(kind == LOCAL)
        {
            pos = lclcount;
            lclcount++;
        }
        else if(kind == FIELD)
        {
            pos = fieldcount;
            fieldcount++;
        }
        else
        {
            pos = staticcount;
            staticcount++;
        }
        if(into == CLASS_T)
        {
            classtable.insert(make_pair(name,record {name,kind,type,pos}));
        }
        else
        {
            methodtable.insert(make_pair(name,record {name,kind,type,pos}));
        }
    }
    struct record getrecord(string name)
    {
        if(methodtable.find(name) != methodtable.end())
        {
            return methodtable[name];
        }
        return classtable[name];
    }
    bool isinscope(string name)
    {
        if(methodtable.find(name) == methodtable.end())
        {
            if(classtable.find(name) == classtable.end())
            {
                return false;
            }
        }
        return true;
    }
    void print(void)
    {
        cout<<"classtable:"<<endl;
        for(auto ele : classtable)
        {
            cout<<ele.second.name<<"    "<<ele.second.type<<"   "<<ele.second.position<<"   "<<ele.second.kind<<endl;
        }
        cout<<"methodtable:"<<endl;
        for(auto ele : methodtable)
        {
            cout<<ele.second.name<<"    "<<ele.second.type<<"   "<<ele.second.position<<"   "<<ele.second.kind<<endl;
        }
    }
    string getLabel(void)
    {
        labelcount++;
        return "L"+to_string(labelcount);
    }
    int getClassSize(void )
    {
        return fieldcount;
    }
};
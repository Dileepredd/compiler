#include<bits/stdc++.h>
#include "ast.h"
#include "codewriter.h"
#include "../y.tab.h"
#include "symboltabel.h"
using namespace std;
void compileexpr(nodeType* root,string &code);
void compilestatements(nodeType* root,string &code,int &count);
string classname;
symboltabel st;
extern ofstream fout;
int compileVL(INTO& context,kind_of& seg,string& type,nodeType* root)
{
    if(root == nullptr)return 0;
    int pc = compileVL(context,seg,type,root->opr.op[0]);
    string idname = root->opr.op[1]->id.name;
    st.insert(context,idname,seg,type);
    return pc+1;
}
int compileVD(kind_of seg,INTO context,nodeType* root)
{
    string type = root->opr.op[0]->id.name;
    int c = compileVL(context,seg,type,root->opr.op[1]);
    return c;
}
void compileEL(nodeType* root,string &code,int &count)
{
    if(root==nullptr)return;
    compileEL(root->opr.op[0],code,count);
    compileexpr(root->opr.op[1],code);
    count++;
}
void compileexpr(nodeType* root,string &code)
{
    if(root == nullptr)return;
    if(root->type == typeId)
    {
        struct record temp = st.getrecord(root->id.name);
        string kind = getSegement(temp.kind); 
        code+="push "+kind+" "+to_string(temp.position)+"\n";
    }
    else if(root->type == typeCon)
    {
        code+="push constant "+to_string(root->con.value)+"\n";
    }
    else
    {
        switch(root->opr.oper)
        {
            case '+':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="add\n";
            }
            break;
            case '-':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="sub\n";
            }
            break;
            case '*':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="mul\n";
            }
            break;
            case '/':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="div\n";
            }
            break;
            case OR:{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="or\n";
            }
            break;
            case AND:{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="and\n";
            }
            break;
            case '<':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="lt\n";
            }
            break;
            case '>':{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="gt\n";
            }
            break;
            case EQ:{
                compileexpr(root->opr.op[0],code);
                compileexpr(root->opr.op[1],code);
                code+="eq\n";
            }
            break;
            case UMINUS:{
                compileexpr(root->opr.op[0],code);
                code+="neg\n";
            }
            break;
            case '!':{
                compileexpr(root->opr.op[0],code);
                code+="not\n";
            }
            break;
            case ARRAY:{
                string idname = root->opr.op[0]->id.name;
                struct record temp = st.getrecord(idname);
                string kind = getSegement(temp.kind); 
                code+="push "+kind+" "+to_string(temp.position)+"\n";
                compileexpr(root->opr.op[1],code);
                code += "add\n";
                code += "pop pointer 1\n";
                code += "push that 0\n";
            }
            break;
            case True:{
                code+="push constant 1\nneg\n";
            }
            break;
            case False:{
                code+="push constant 0\n";
            }
            break;
            case Null:{
                code+="push constant 0\n";
            }
            break;
            case FUNCTIONCALL:{
                string funcname = root->opr.op[0]->id.name;
                int c = 0;
                compileEL(root->opr.op[1],code,c);
                code+="call "+classname+"."+funcname+" "+to_string(c)+"\n";
            }
            break;
            case CONSTRUCTORCALL:{
                string funcname = root->opr.op[0]->id.name;
                int c = 0;
                compileEL(root->opr.op[1],code,c);
                code+="call "+funcname+".new "+to_string(c)+"\n";
            }
            break;
            case THIS:{
                code += "push pointer 0\n";
            }
            break;
            case METHODCALL:{
                int c = 0;
                string objname = root->opr.op[0]->id.name;
                if(st.isinscope(objname))
                {
                    struct record temp = st.getrecord(objname);
                    string kind = getSegement(temp.kind); 
                    code+="push "+kind+" "+to_string(temp.position)+"\n";
                    c++;
                    string funcname = root->opr.op[1]->id.name;
                    compileEL(root->opr.op[2],code,c);
                    code+="call "+temp.type+"."+funcname+" "+to_string(c)+"\n";
                }
                else
                {
                    string funcname = root->opr.op[1]->id.name;
                    compileEL(root->opr.op[2],code,c);
                    code+="call "+objname+"."+funcname+" "+to_string(c)+"\n";
                }
            }
            break;
            default:;
        }
    }
    
}
void compileass(nodeType* root,string &code)
{
    // compilelhs
    compileexpr(root->opr.op[1],code);
    if(root->opr.op[0]->type == typeId)
    {
        struct record temp = st.getrecord(root->opr.op[0]->id.name);
        string kind = getSegement(temp.kind); 
        code+="pop "+kind+" "+to_string(temp.position)+"\n";
    }
    else
    {
        string idname = root->opr.op[0]->opr.op[0]->id.name;
        struct record temp = st.getrecord(idname);
        string kind = getSegement(temp.kind); 
        code+="push "+kind+" "+to_string(temp.position)+"\n";
        compileexpr(root->opr.op[0]->opr.op[1],code);
        code += "add\n";
        code += "pop pointer 1\n";
        code += "pop that 0\n";
    }
}
void compilestatement(nodeType* root,string &code,int &count)
{
    if(root->opr.oper == VARDEC)
    {
        count += compileVD(LOCAL,METHOD,root);
    }
    else if (root->opr.oper == ASS)
    {
        compileass(root,code);
    }
    else if(root->opr.oper == IF)
    {
        nodeType* exp = root->opr.op[0];
        nodeType* ifsts = root->opr.op[1];
        nodeType* elsests = root->opr.op[2];
        if(elsests == nullptr)
        {
            string label1 = st.getLabel();
            compileexpr(exp,code);
            code+="not\n";
            code += "if-goto "+label1+"\n";
            compilestatements(ifsts,code,count);
            code+= "label "+label1+"\n";
        }
        else
        {
            string label1 = st.getLabel();
            string label2 = st.getLabel();
            compileexpr(exp,code);
            code+="not\n";
            code += "if-goto "+label1+"\n";
            compilestatements(ifsts,code,count);
            code += "goto "+label2+"\n";
            code+= "label "+label1+"\n";
            compilestatements(elsests,code,count);
            code += "label "+label2+"\n";
        }
    }
    else if(root->opr.oper == WHILE)
    {
        nodeType* exp = root->opr.op[0];
        nodeType* wsts = root->opr.op[1];
        string label1 = st.getLabel();
        string label2 = st.getLabel();
        code += "label "+label1+"\n";
        compileexpr(exp,code);
        code += "not\n";
        code += "if-goto "+label2+"\n";
        compilestatements(wsts,code,count);
        code += "goto "+label1+"\n";
        code += "label "+label2+"\n";
    }
    else if(root->opr.oper == RETURN)
    {
        if(root->opr.op[0] == nullptr)
        {
            code += "push constant 0\nreturn\n";
        }
        else
        {
            compileexpr(root->opr.op[0],code);
            code += "return\n";
        }
    }
    else
    {
        compileexpr(root,code);
        code += "pop temp 0\n";
    }
}
void compilestatements(nodeType* root,string &code,int &count)
{
    if(root == nullptr)return;
    compilestatements(root->opr.op[0],code,count);
    compilestatement(root->opr.op[1],code,count);
}
void compilePL(nodeType* root)
{
    if(root == nullptr)return;
    compilePL(root->opr.op[0]);
    string type = root->opr.op[1]->id.name;
    string name = root->opr.op[2]->id.name;
    st.insert(METHOD,name,ARGUMENT,type);
}
void compileRD(kind_of seg,nodeType* root)
{
    st.clear();
    if(root->opr.oper == ROTINE)
    {
        // rotinedec
        int count = 0;
        if(seg != STATIC_DEC)
        {
            st.insert(METHOD,"this",ARGUMENT,classname);
            count++;
        }
        string funcname = root->opr.op[1]->id.name;
        compilePL(root->opr.op[2]);
        string code;
        compilestatements(root->opr.op[3],code,count);
        fout<<"function"<<" "<<classname<<"."<<funcname<<" "<<count<<endl;
        if(seg != STATIC_DEC)
        {
            fout<<"push argument 0\npop pointer 0"<<endl;
        }
        fout<<code<<endl;
    }
    else
    {
        // constructordec
        int blocksize = st.getClassSize();
        string funcname = root->opr.op[0]->id.name;
        compilePL(root->opr.op[1]);
        string code;
        int count = 0;
        compilestatements(root->opr.op[1],code,count);
        fout<<"function "<<classname<<".new "<<count<<endl;
        fout<<"push constant "<<blocksize<<endl;
        fout<<"call Memory.alloc 1"<<endl;
        fout<<"pop pointer 0"<<endl;
        fout<<code<<endl;
    }
}
void compileClassDec(nodeType* root)
{
    if(root == nullptr)return;
    compileClassDec(root->opr.op[0]);
    if(root->opr.op[1]->opr.oper == STATIC_ROTINE)
    {
        nodeType* funcdec = root->opr.op[1]->opr.op[0];
        compileRD(STATIC_DEC,funcdec);
    }
    else if(root->opr.op[1]->opr.oper == STATIC_VARDEC)
    {
        nodeType* vardec = root->opr.op[1]->opr.op[0];
        compileVD(STATIC_DEC,CLASS_T,vardec);
    }
    else if(root->opr.op[1]->opr.oper == VARDEC)
    {
        nodeType* vardec = root->opr.op[1];
        compileVD(FIELD,CLASS_T,vardec);
    }
    else
    {
        nodeType* funcdec = root->opr.op[1];
        compileRD(FIELD,funcdec);
    }
}
void compile(nodeType* root)
{
    if(root->type == typeOpr)
    {
        classname = root->opr.op[0]->id.name;
        compileClassDec(root->opr.op[1]);
    }
    // st.print();
}
void compileimports(nodeType* root)
{
    if(root == nullptr)return;
    compileimports(root->opr.op[0]);
    st.inserttype(root->opr.op[1]->id.name);
}
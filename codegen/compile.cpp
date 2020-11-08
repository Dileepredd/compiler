#include<bits/stdc++.h>
#include "ast.h"
#include "../y.tab.h"
using namespace std;
void computeexprlist(nodeType* ptr,int& nargs)
{
    if(ptr == nullptr)return;
    if(ptr->opr.oper == EXPRLIST)
    {
        computeexprlist(ptr->opr.op[0],nargs);
        compile(ptr->opr.op[1]);
        nargs++; 
    }
    else
    {
        nargs++;
        compile(ptr);   
    }
}
void compile(nodeType* root)
{
    if (root == nullptr)return;
    switch(root->type)
    {
        case typeId: {
            cout<<"typeId: "<<root->id.name<<endl;
        }
        break;
        case typeCon: {
            cout<<"push constant "<<root->con.value<<endl;
        }
        break;
        case typeOpr: {
            switch(root->opr.oper){
                case FUNCTIONCALL: {
                    // function name
                    int nargs = 0;
                    computeexprlist(root->opr.op[1],nargs);
                    cout<<root->opr.op[0]->id.name<<" "<<nargs<<endl;
                    // arguments
                }
                break;
                case '+':
                {
                    compile(root->opr.op[0]);
                    compile(root->opr.op[1]);
                    cout<<"add"<<endl;
                }
                break;
                case '-':
                {
                    compile(root->opr.op[0]);
                    compile(root->opr.op[1]);
                    cout<<"sub"<<endl;
                }
                break;
                case '*':
                {
                    compile(root->opr.op[0]);
                    compile(root->opr.op[1]);
                    cout<<"mul"<<endl;
                }
                break;
            default:
            cout<<"operator: "<<root->opr.oper<<" "<<root->opr.nops<<endl;
            for(int i=0;i<root->opr.nops;i++)
            {
                compile(root->opr.op[i]);
            }
            }
        }
        break;
        default:
        ;
    }
}
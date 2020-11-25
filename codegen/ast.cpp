#include <bits/stdc++.h>
#include "ast.h"
int yyerror(char* );
using namespace std;
nodeType *constint(int value) {
 nodeType *p;
 /* allocate node */
 if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeCon;
 p->con.value = value;
 return p;
}
nodeType *constchar(int value) {
 nodeType *p;
 /* allocate node */
 if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeCon;
 p->con.value = value;
 return p;
}
nodeType *id(char* name) {
 nodeType *p;
 /* allocate node */
 if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeId;
 strncpy(p->id.name,name,31);
 
 p->id.name[31] = '\0';
 return p;
} 
nodeType *opr(int oper, int nops, ...) {
 va_list ap;
 nodeType *p;
 int i;
 /* allocate node */
 if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 if ((p->opr.op = (nodeType**)malloc(nops * sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeOpr;
 p->opr.oper = oper;
 p->opr.nops = nops;
 va_start(ap, nops);
 for (i = 0; i < nops; i++)
 p->opr.op[i] = va_arg(ap, nodeType*);
 va_end(ap);
 return p;
}
void freeNode(nodeType *p) {
 int i;
 if (!p) return;
 if (p->type == typeOpr) {
 for (i = 0; i < p->opr.nops; i++)
 freeNode(p->opr.op[i]);
 free(p->opr.op);
 }
 free (p);
}

nodeType* conststr(char* str)
{
    nodeType *p;
    if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
    yyerror("out of memory");
    p->type = typeStr;
    int count = 0;
    while(str[count]!='\0')
    {
        count++;
    }
    p->str.count = count;
    p->str.str = str;
    return p;
}

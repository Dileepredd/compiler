
// abstract syntax tree construction
#ifndef AST_H
#define AST_H
typedef enum { typeCon, typeId, typeOpr } nodeEnum;

typedef struct {
 int value; 
} conNodeType;

typedef struct {
    char name[32];
} idNodeType; 

typedef struct {
 int oper; 
 int nops; 
 struct nodeTypeTag **op; 
} oprNodeType;

typedef struct nodeTypeTag {
 nodeEnum type; 
 union {
 conNodeType con;
 idNodeType id; 
 oprNodeType opr; 
 };
} nodeType;

/* prototype declerations */
nodeType* opr(int , int , ...);
nodeType* id(char *);
nodeType* constint(int );
nodeType* constchar(int );
#endif
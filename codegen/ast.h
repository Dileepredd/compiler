
// symbol tabel
struct record
{
    int type;
    int memseg;
    int location;
    char name[32];
};

typedef struct record symboltable;

extern symboltable global[];
extern symboltable current[]; 

int append(int context,int type,int memseg,char* ptr);
void clear(int context);

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
void compile(nodeType* );
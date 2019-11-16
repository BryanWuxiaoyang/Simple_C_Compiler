%{
    #include <stdio.h>
    #include <string.h>
    #include "lex.yy.c"
    #include "SemanticAnalysis.h"
    #include "IntermediateLanguage.h"
    void yyerror(char* msg);
    Node nodeBuffer[8];
    void errorTypeB(int lineno,int errorId){
        ok=0;
        if(lex_err == lineno){
            //lex_err=0;
            return;
        }
        printf("Error type B at Line %d: ",lineno);
        switch(errorId){
            case 1:printf("Error difine list\n");break;
            case 2:printf("Statement missing \";\"\n");break;
            case 3:printf("Statement missing \";\"\n");break;
            case 4:printf("Additional \"else\"\n");break;
            case 5:printf("Error use of \"[\"\n");break;
            case 6:printf("Statement missing \";\"\n");break;
            case 7:printf("Statement missing \";\"\n");break;
            case 20:printf("expression error\n");break;
            case 21:printf("brackets not matching\n"); break;
            default: printf("Syntax Error\n"); break;
        }
        lex_err = lineno;
    }
%}

%define api.value.type {struct _Node_*}
%locations

%token INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE_INT TYPE_FLOAT LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE NEG ERROR

%right  ASSIGNOP
%left   OR
%left   AND
%left   RELOP
%left   PLUS MINUS          //TODO: 加上负号和减号的处理
%left   STAR DIV
%right  NOT NEG
%left   LP RP LB RB DOT


%%
Program:ExtDefList{
        nodeBuffer[0]=$1;  
        $$=createNode(1,nodeBuffer,1,Program,@$.first_line);     
        nodeBuffer[0]=$$; 
    };
ExtDefList:ExtDef ExtDefList{
        nodeBuffer[0]=$1;   
        nodeBuffer[1]=$2;  
        $$=createNode(2,nodeBuffer,1,ExtDefList,@$.first_line);
    }|   {
        $$=createNode(0,NULL,2,ExtDefList,@$.first_line); 
    };
ExtDef:Specifier ExtDecList SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,1,ExtDef,@$.first_line); 
    }|  Specifier SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,2,ExtDef,@$.first_line); 
    }|  Specifier FunDec CompSt {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,3,ExtDef,@$.first_line);
    }|  Specifier FunDec SEMI{
        nodeBuffer[0]=$1;
        nodeBuffer[1]=$2;
        nodeBuffer[2]=$3;
        $$=createNode(3,nodeBuffer,4,ExtDef,@$.first_line);
    }|  error RC{
        errorTypeB(@$.first_line, 12);
    }|  Specifier ExtDecList error {
        errorTypeB(@$.first_line, 2);
    }|  Specifier error {
        errorTypeB(@$.first_line, 2);
    }|  error SEMI{
        errorTypeB(@$.first_line, 1);
    }|  error RB{
        errorTypeB(@$.first_line, 1);
    };
ExtDecList:VarDec{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,ExtDecList,@$.first_line); 
    }|  VarDec COMMA ExtDecList {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,2,ExtDecList,@$.first_line);
    };
Specifier:TYPE_FLOAT{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,Specifier,@$.first_line); 
    }|  TYPE_INT{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,2,Specifier,@$.first_line); 
    }|  StructSpecifier {
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,3,Specifier,@$.first_line); 
    };
StructSpecifier:STRUCT OptTag LC DefList RC {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        nodeBuffer[4]=$5; 
        $$=createNode(5,nodeBuffer,1,StructSpecifier,@$.first_line); 
    }|  STRUCT Tag  {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,2,StructSpecifier,@$.first_line); 
    };
OptTag:ID{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,OptTag,@$.first_line);
    }|  {
        $$=createNode(0,NULL,2,OptTag,@$.first_line);
    };
Tag:ID{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,Tag,@$.first_line); 
    };
VarDec:ID{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,VarDec,@$.first_line); 
    }|  VarDec LB INT RB{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        $$=createNode(4,nodeBuffer,2,VarDec,@$.first_line); 
    }|  VarDec LB RB {
        $$ = createNode(0, NULL,3,VarDec,@$.first_line); 
        errorTypeB(@$.first_line, 5);
    };
FunDec:ID LP VarList RP {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        $$=createNode(4,nodeBuffer,1,FunDec,@$.first_line); 
    }|  ID LP RP{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,2,FunDec,@$.first_line); 
    }|  ID LP error{
        errorTypeB(@$.first_line, 21);
    }|  ID LP error RP{
        errorTypeB(@$.first_line, 21);
    };
VarList:ParamDec COMMA VarList{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,1,VarList,@$.first_line); 
    }|  ParamDec{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,2,VarList,@$.first_line); 
    };
ParamDec:Specifier VarDec{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,1,ParamDec,@$.first_line); 
    };
CompSt:LC DefList StmtList RC{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        $$=createNode(4,nodeBuffer,1,CompSt,@$.first_line); 
    }|  error RC{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,2,CompSt,@$.first_line); 
        errorTypeB(@$.first_line, 0);
    };
StmtList:Stmt StmtList{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,1,StmtList,@$.first_line); 
    }|  {
        $$=createNode(0,NULL,2,StmtList,@$.first_line);
    };
Stmt:   Exp SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,1,Stmt,@$.first_line); 
    }|  CompSt{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,2,Stmt,@$.first_line); 
    }|  RETURN Exp SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,3,Stmt,@$.first_line); 
    }|  IF LP Exp RP Stmt{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        nodeBuffer[4]=$5; 
        $$=createNode(5,nodeBuffer,4,Stmt,@$.first_line); 
    }|  IF LP Exp RP Stmt ELSE Stmt{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        nodeBuffer[4]=$5; 
        nodeBuffer[5]=$6; 
        nodeBuffer[6]=$7; 
        $$=createNode(7,nodeBuffer,5,Stmt,@$.first_line);
    }|  WHILE LP Exp RP Stmt{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        nodeBuffer[4]=$5; 
        $$=createNode(5,nodeBuffer,6,Stmt,@$.first_line); 
    }|  Exp error{ 
        errorTypeB(@$.first_line, 2);
    }|  RETURN Exp error{
        errorTypeB(@$.first_line, 3);
    }|  error ELSE Stmt{
        errorTypeB(@$.first_line, 4);
    }|  error SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,7,Stmt,@$.first_line); 
        errorTypeB(@$.first_line, 0);
        yyerrok;
    };
DefList:Def DefList {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,1,DefList,@$.first_line); 
    }|  {
        $$=createNode(0,NULL,2,DefList,@$.first_line); 
    };
Def:Specifier DecList SEMI{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,1,Def,@$.first_line); 
    }|  error SEMI{
        errorTypeB(@$.first_line, 1);
    }|  Specifier DecList error{
        errorTypeB(@$.first_line, 9);
    };
DecList:Dec {
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,DecList,@$.first_line); 
    }|  Dec COMMA DecList{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,2,DecList,@$.first_line); 
    };
Dec:VarDec{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,1,Dec,@$.first_line); 
    }|  VarDec ASSIGNOP Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,2,Dec,@$.first_line); 
    };
Exp:Exp ASSIGNOP Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,1,Exp,@$.first_line); 
    }|  Exp AND Exp {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,2,Exp,@$.first_line); 
    }|  Exp OR Exp  {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,3,Exp,@$.first_line); 
    }|  Exp RELOP Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,4,Exp,@$.first_line); 
    }|  Exp PLUS Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,5,Exp,@$.first_line); 
    }|  Exp MINUS Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,6,Exp,@$.first_line); 
    }|  Exp STAR Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,7,Exp,@$.first_line); 
    }|  Exp DIV Exp {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,8,Exp,@$.first_line); 
    }|  LP Exp RP{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,9,Exp,@$.first_line);
    }|  NEG Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,10,Exp,@$.first_line); 
    }|  NOT Exp{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        $$=createNode(2,nodeBuffer,11,Exp,@$.first_line); 
    }|  ID LP Args RP{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        $$=createNode(4,nodeBuffer,12,Exp,@$.first_line); 
    }|  ID LP RP{
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,13,Exp,@$.first_line); 
    }|  Exp LB Exp RB {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        nodeBuffer[3]=$4; 
        $$=createNode(4,nodeBuffer,14,Exp,@$.first_line); 
    }|  Exp DOT ID  {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,15,Exp,@$.first_line); 
    }|  ID  {
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,16,Exp,@$.first_line); 
    }|  INT {
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,17,Exp,@$.first_line); 
    }|  FLOAT{
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,18,Exp,@$.first_line); 
    }|  Exp LB RB{
        $$ = createNode(0, nodeBuffer,19,Exp,@$.first_line); 
        errorTypeB(@$.first_line, 5);
    }|  Exp LB error RB{
        errorTypeB(@$.first_line, 21);
    }|  Exp LB Exp error{
        errorTypeB(@$.first_line, 21);
    };
Args:Exp COMMA Args {
        nodeBuffer[0]=$1; 
        nodeBuffer[1]=$2; 
        nodeBuffer[2]=$3; 
        $$=createNode(3,nodeBuffer,1,Args,@$.first_line); 
    }|  Exp {
        nodeBuffer[0]=$1; 
        $$=createNode(1,nodeBuffer,2,Args,@$.first_line); 
    };
%%

void yyerror(char* msg){
        ok=0;
}

int main(int argc,char** argv){
    if(argc<=1)
        return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f)
    {
        printf("Error Open File!\n");
        return 1;
    }
    yyrestart(f);
    yyparse();
    if(ok){
        semAnalysis(nodeBuffer[0]);
        printInterCodeList(NULL, NULL);
    }
    return 0;
}
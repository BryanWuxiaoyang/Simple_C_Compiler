%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>
    #include <malloc.h>
    #include "syntax.tab.h"
    #include "SymTable.h"
    #include "SemanticAnalysis.h"

    /*INT,FLOAT,ID,SEMI,COMMA,ASSIGNOP,RELOP,PLUS,
    MINUS,STAR,DIV,AND,OR,DOT,NOT,TYPE,LP,RP,LB,
    RB,LC,RC,STRUCT,RETURN,IF,ELSE,WHILE,ERROR,TYPE_INT,TYPE_FLOAT*/

    int yycolumn = 1;
    int last_unit = 0;
    int lex_err=0;
    int ok=1;

    #define YY_USER_ACTION {\
        yylloc.first_line = yylloc.last_line = yylineno;\
        yylloc.first_column = yycolumn;\
        yylloc.last_column = yycolumn + yyleng - 1;\
        yycolumn += yyleng;\
    }

    Node createNode(int childNum,Node child[],int no,int code,int line){
        Node newNode=(Node)malloc(sizeof(struct _Node_));
        newNode->childNum=childNum;
        for(int i=0;i<childNum;i++)
            newNode->child[i]=child[i];
        newNode->expandNo=no;
        newNode->symCode=code;
        newNode->lineno=line;
        return newNode;
    }

    int installINT(){
        int len=strlen(yytext),pos=0,base,value=0,temp;
        if(yytext[0]=='0'){
            if(len>1&&yytext[1]!='X'&&yytext[1]!='x'){
                base=8;
                pos=1;
            }
            else if(len>1){
                base=16;
                pos=2;
            }
            else{
                return 0;
            }
        }
        else{
            base = 10;
            pos = 0;
        }

        for(int i = pos; i < len; i++){
            if(yytext[i]>='0'&&yytext[i]<='9')
                temp = yytext[i] - '0';
            else if(yytext[i] >= 'a' && yytext[i] <= 'f')
                temp = yytext[i] - 'a'+10;
            else if(yytext[i] >= 'A' && yytext[i] <= 'Z')
                temp = yytext[i] - 'A'+10;
            else assert(0);

            value = value * base + temp;
        }
        return value;
    }

    float installFLOAT(){
        return atof(yytext);
    }

    void errorTypeA(){
        lex_err=yylineno;
        ok=0;
        printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
    }
%}

%option yylineno


delim   [ \t]
ws      {delim}+
letter  [_A-Za-z]
digit   [0-9]
hex     [0-9A-Fa-f]
oct     [0-7]
dec     [1-9]
id      {letter}({letter}|{digit})*
int     ((0x|0X){hex}+)|({dec}{digit}*)|(0{oct}*)
float   {digit}+(\.{digit}+) 

%%

{ws}    {/*No action*/}

\n      {yycolumn = 1; }

int     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=TYPE_INT;                                    return(TYPE_INT);       }
float   {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=TYPE_FLOAT;                                   return(TYPE_FLOAT);       }
struct  {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=STRUCT;                                   return(STRUCT);     }
return  {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=RETURN;                                  return(RETURN);     }
if      {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=IF;                                     return(IF);         }
else    {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=ELSE;                                       return(ELSE);       }
while   {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=WHILE;                                  return(WHILE);      }
{id}    {
    yylval=createNode(0,NULL,0,SYN_ID,yylineno); 
    yylval->str_val = (char*)malloc(sizeof(char) * (strlen(yytext) + 2));
    strcpy(yylval->str_val, yytext);  
    last_unit=ID;        
    return(ID);      
    }
{int}   {yylval=createNode(0,NULL,0,SYN_INT,yylineno); yylval->int_val=installINT();  last_unit=INT;       return(INT);        }
{float} {yylval=createNode(0,NULL,0,SYN_FLOAT,yylineno); yylval->float_val=installFLOAT(); last_unit=FLOAT;    return(FLOAT);      }
";"     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=SEMI;                                      return(SEMI);       }
","     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=COMMA;                                     return(COMMA);      }
"="     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=ASSIGNOP;                              return(ASSIGNOP);   }
">"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_G; last_unit=RELOP;                               return(RELOP);      }
">="    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_GE; last_unit=RELOP;                                 return(RELOP);      }
"<"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_L; last_unit=RELOP;                                      return(RELOP);      }
"<="    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_LE; last_unit=RELOP;                                  return(RELOP);      }
"=="    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_E; last_unit=RELOP;                                   return(RELOP);      }
"!="    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_NE; last_unit=RELOP;                                    return(RELOP);      }
"+"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_PLUS; last_unit=PLUS;                                  return(PLUS);       }
"-"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); if(last_unit==INT||last_unit==FLOAT||last_unit==RP||last_unit==RB||last_unit==ID)  {yylval->op = OP_MINUS; last_unit=MINUS;   return(MINUS);}   else{yylval->op = OP_NEG; last_unit=NEG;  return(NEG); }     }
"*"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_STAR; last_unit=STAR;                                 return(STAR);       }
"/"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_DIV; last_unit=DIV;                                    return(DIV);        }
"&&"    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_AND; last_unit=AND;                                        return(AND);        }
"||"    {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_OR; last_unit=OR;                                         return(OR);         }
"."     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=DOT;                                       return(DOT);        }
"!"     {yylval=createNode(0,NULL,0,SYN_OP,yylineno); yylval->op = OP_NOT; last_unit=NOT;                                         return(NOT);        }
"("     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=LP;                                         return(LP);         }
")"     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=RP;                                           return(RP);         }
"["     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=LB;                                      return(LB);         }
"]"     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=RB;                                     return(RB);         }
"{"     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=LC;                                           return(LC);         }
"}"     {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=RC;                                     return(RC);         }
.       {yylval=createNode(0,NULL,0,Other,yylineno); yylval->int_val=last_unit=ERROR;              errorTypeA();       return(ERROR);      }

%%




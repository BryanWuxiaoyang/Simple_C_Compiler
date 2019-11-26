#include "IntermediateLanguage.h"
#include <stdlib.h>
#include <string.h>

#define INF 0x3fffffff

struct UniNode{
    void* value;
    struct UniNode* next;
};
typedef struct UniNode UniNode;

UniNode* createUniNode(void* value,UniNode* next){
    UniNode* newUniNode=malloc(sizeof(UniNode));
    newUniNode->value=value;
    newUniNode->next=next;
    return newUniNode;
}

struct UniNodeList{
    UniNode* head;
    UniNode* rear;
};
typedef struct UniNodeList UniNodeList;

UniNodeList* createUniNodeList(){
    UniNodeList* newUniNodeList=malloc(sizeof(UniNodeList));
    newUniNodeList->head=createUniNode(NULL,NULL);
    newUniNodeList->rear=newUniNodeList->head;
    return newUniNodeList;
}

void addUniNode(UniNodeList* list, void* value){
    //在末尾加入一个节点，并将rear指向加入元素的下一个元素
    UniNode* cur;
    for(cur=list->head;cur!=list->rear;cur=cur->next);
    cur->value=value;
    UniNode* newUnNode=createUniNodeList(NULL,NULL);
    cur->next=newUnNode;
    list->rear=newUnNode;
}

struct SymChecker{
    char symName[32];
    UniNodeList* assignTable;
    UniNodeList* useTable;
};
typedef struct SymChecker SymChecker;

SymChecker* createSymChecker(char* name){
    SymChecker* newSymChecker=malloc(sizeof(SymChecker));
    strcpy(newSymChecker->symName,name);
    newSymChecker->assignTable=createUniNodeList();
    newSymChecker->useTable=createUniNodeList();
    return newSymChecker;
}

enum TableType{
    USETAB,
    ASSIGNTAB
};
typedef enum TableType TableType;

int searchLine(SymChecker* sym,int lineno, TableType tableType){
    //返回第一个第lineno及其之后引用/赋值该变量的行数，如果没有，返回正无穷
    UniNodeList* list;
    if(tableType==ASSIGNTAB)
        list=sym->assignTable;
    else
        list=sym->useTable;
    for(UniNode* cur=list->head;cur!=list->rear;cur=cur->next)
        if(*(int*)(cur->value)>=lineno)
            return *(int*)(cur->value);
    return INF;
}

int isSameSym(char* name,SymChecker* sym){
    return strcmp(name,sym->symName)==0;
}

SymChecker* searchSymList(char* name,UniNodeList* list){
    for(UniNode* cur=list->head;cur!=list->rear;cur=cur->next)
        if(isSameSym(name,cur->value))
            return cur->value;
    return NULL;
}

enum CodePart{
    ARG,
    TARGET
};
typedef enum CodePart CodePart;

void processCodePart(char* name,UniNodeList* symList,int* lineno,CodePart codePart){
    //TODO：还缺少指针，标签以及引用
    if(name[0]!='#')
        return;//立即数不参与优化处理
    SymChecker* symChecker=searchSymList(name,symList);
    if(symChecker==NULL){
        symChecker=createSymChecker(name);
        addUniNode(symList,symChecker);
    }
    if(codePart==ARG)//引用
        addUniNode(symChecker->useTable,lineno);
    else if(codePart==TARGET)//赋值
        addUniNode(symChecker->assignTable,lineno);
}

void processCode(InterCode code,int lineno,UniNodeList* symList){
    //装载各种引用表和赋值表 
    //注：没有处理标签
    //TODO：还缺少指针，标签以及引用
    char* arg1 = code->arg1;
	char* arg2 = code->arg2;
	char* target = code->target;
    int *p=malloc(sizeof(int));
    *p=lineno;
    switch(code->op){
        //下面这些要处理两个参数引用和一个目标赋值
        case ILOP_PLUS:
        case ILOP_MINUS:
        case ILOP_MUL:
        case ILOP_DIV:
            processCodePart(arg1,symList,p,ARG);
            processCodePart(arg2,symList,p,ARG);
            processCodePart(target,symList,p,TARGET);
            break;
        //下面这些要处理一个参数引用和一个目标赋值
        case ILOP_ASSIGN:
        case ILOP_ADDR:
            processCodePart(arg1,symList,p,ARG);
            processCodePart(target,symList,p,TARGET);
            break;
        //下面这些要处理两个参数引用
        case ILOP_IF_G:
        case ILOP_IF_GE:
        case ILOP_IF_E:
        case ILOP_IF_NE:
        case ILOP_IF_L:
        case ILOP_IF_LE:
            processCodePart(arg1,symList,p,ARG);
            processCodePart(arg2,symList,p,ARG);
            break;
        //下面这些要处理一个目标赋值
        case ILOP_DEC:
        case ILOP_CALL:
            processCodePart(target,symList,p,TARGET);
            break;
        //下面这些要处理一个参数引用
        case ILOP_ARG://注意对于ARG指令，引用的其实是target，不是arg
        case ILOP_RETURN://注意对于RETURN指令，引用的其实是target，不是arg
        case ILOP_PARAM://注意对于PARAM指令，引用的其实是target，不是arg
        case ILOP_WRITE://注意对于WRITE指令，引用的其实是target，不是arg
            processCodePart(target,symList,p,ARG);
            break;
        //下面这些暂且啥也不处理
        case ILOP_LABEL:
        case ILOP_FUNCTION:
        case ILOP_GOTO:
        case ILOP_READ://READ函数就不优化了，要不可能读文件读错位
            break;
    }
}

void deleteUselessCode(UniNodeList* symList,ListIterator it){
    //情况1：无效代码，给了个变量赋值或计算，后面压根没用它
    UniNodeList* uselessLine=createUniNodeList();
    for(UniNode* cur=symList->head;cur!=symList->rear;cur=cur->next){
            int *p;
            SymChecker* curSymChecker=(SymChecker*)(cur->value);
            int curAssignOp,nextUseOp,nextAssignOp;
            for(UniNode* assign=curSymChecker->assignTable->head;assign->next!=curSymChecker->assignTable->rear;assign=assign->next){
                curAssignOp=*(int*)(assign->value);
                nextUseOp=searchLine(curSymChecker,curAssignOp,USETAB);
                nextAssignOp=*(int*)(assign->next->value);
                if(nextUseOp>nextAssignOp){
                    p=malloc(sizeof(int));
                    *p=curAssignOp;
                    addUniNode(uselessLine,p);
                }
            }
            nextUseOp=searchLine(curSymChecker,nextAssignOp,USETAB);//处理最后一个赋值
            if(nextUseOp>=INF){
                p=malloc(sizeof(int));
                *p=nextAssignOp;
                addUniNode(uselessLine,p);
            }
    }
    int cur=0;
    UniNode* nextDeleteLineNode=uselessLine->head;
	while (MyList_hasNext(it)) {
        if(cur==*(int*)(nextDeleteLineNode->value)){
            MyList_removeNext(it);
            nextDeleteLineNode=nextDeleteLineNode->next;
        }   
		MyList_getNext(it);
        cur++;
	}
}

void processUselessCode(UniNodeList* symList,ListIterator it){
    //情况2：无效中间变量，给变量t赋值为a，a在后面没被修改，用t参与计算或赋值
    //情况3：类似情况2，给变量t计算为一个东西，参与计算的变量在后面没被修改，又把t赋值给另一个变量
    int lineno=0;
    while(MyList_hasNext(it)){
        InterCode code=(InterCode)MyList_getNext(it);
        InterCode curCode;
         char* arg1 = code->arg1;
	    char* arg2 = code->arg2;
	    char* target = code->target;
         int changeLineArg,changeLineTarget,changeLine;
         int i;
         SymChecker* arg1Sym;
         SymChecker* targetSym;
         ListIterator runIt=it;
        switch(code->op){
            case ILOP_ASSIGN:
                targetSym=searchSymList(target,symList);
                arg1Sym=searchSymList(arg1,symList);
                changeLineArg=searchLine(arg1Sym,lineno,ASSIGNTAB);
                changeLineTarget=searchLine(targetSym,lineno,ASSIGNTAB);
                changeLine=changeLineTarget<changeLineArg?changeLineTarget:changeLineArg;
                if(changeLine>=INF)
                    changeLine=-1;
                for(i=lineno+1;i<=changeLine;i++){
                    curCode=(InterCode)MyList_getNext(runIt);
                    if(strcmp(curCode->arg1,target)==0)
                        strcpy(curCode->arg1,target);
                    if(strcmp(curCode->arg2,target)==0)
                        strcpy(curCode->arg2,target);
                }
        }
        lineno++;
    }
}

void processUselessCode2(ListIterator it){
    while(MyList_hasNext(it)){
        InterCode code=(InterCode)MyList_getNext(it);
        InterCode curCode;
	    char* target = code->target;
        char* arg1=code->arg1;
        if(curCode==ILOP_CALL){
            InterCode nextCode=(InterCode)MyList_getNext(it);
            char* nextArg1=nextCode->arg1;
            if(nextCode->op==ILOP_ASSIGN&&strcmp(target,nextArg1)==0){
                nextCode->op=ILOP_CALL;
                strcpy(nextArg1,arg1);
            }
        }
    }
}

UniNodeList* getCodeList(ListIterator it){
    UniNodeList* symList=createUniNodeList();
    int cur=0;
	while (MyList_hasNext(it)) {
		InterCode code = (InterCode)MyList_getNext(it);
        processCode(code,cur,symList);
        cur++;
	}
}

void optimizeInterCodeLinear() {
	ListIterator it = MyList_createIterator(interCodeList);
    UniNodeList* symList=getCodeList(it);
    it=MyList_createIterator(interCodeList);
    //processUselessCode(symList,it);//处理情况2和3，过程中不会删除代码，只会修改，处理之后会产生一些无效的赋值或计算；但是这样没有考虑循环和分支，有不少问题
    processUselessCode2(it);
    it=MyList_createIterator(interCodeList);
    UniNodeList* newSymList=getCodeList(it);
    it=MyList_createIterator(interCodeList);
    deleteUselessCode(newSymList,it);//此时统一删除所有无效代码
	MyList_destroyIterator(it);
}
////////////newer////////////////
#include "er.h"
#include <time.h>
#include <stdio.h>
#include "myqueue.h"
#include <iostream>
using namespace std;
/******************************************************/

clause_list clauses;//ER处理的子句集合
clause_list iiclauses=new(clause_list_elt);//IER处理的子句集
char junk[100];//缓冲池
int var_number;//文件给定的变量个数
int clause_number;//文件给定的子句的个数
int new_clause_number=0;//用完化简规则后的子句的个数
int new_var_number = 0;//用完化简规则后的变量的个数
//int pre_var_number = 0;//预处理后的变量个数（当前不算1，2两个变量）
int flag1[MAX];
int pFlag = 1;
int sFlag = 1;
int * pointer;//临时变量，当前处理的子句

long seed;
myqueue my_queue;//保存不含互补对的子句
long double totalnumber;//极大项的个数
long double number;//可扩展出的极大项的个数
int* hasliteral;//临时变量，用于求子句集的并集
//int hasliteral[MAX];
int musknumber;//用来选择C，C＝1，2……musknumber
int maxVarNum=0;//子句集中最大的子句的标号
FILE *in, *out;
int numberofnzero;
char * error = "ERROR";
void do_error(char *s);
int* varArray;

int calltimes;
int endtimes;

/***********主函数*************************************/
int main(int argc, char *argv[])
{

	int  clanum = 100;
    int varnum = 30;
    int maxl = 3;
    seed = 2234;
	in = fopen( "e://in.txt", "w" );
	randomGeneration( in,clanum,varnum,maxl);
	fclose( in );
	/*******开始记时*************************/
	FILE *fp;/*fp为指向FILE文件结构体的指针变量*/
	/************************open SAT file***************************/
	fp = fopen("e://in.txt","r");
	load_clauses(fp, &clauses);
	normal(&clauses);
	new_clause_number = clause_number;
	new_var_number	= var_number;
	varArray = new int[new_var_number];
	endtimes = pow(2,new_var_number);
	clock_t tm1,tm2;
	int interval;
	tm1 = clock();
	if(Binsearch(0) == true)
		cout<<"SAT"<<endl;
	else
		cout<<"UNSAT"<<endl;
	tm2 = clock();
	interval=tm2-tm1;
	printf("Seconds Elasped: ");
	printf("%f",interval/1000.0);
	cout<<"calltimes is "<<calltimes<<endl;
	FILE * fp1 = fopen("time_newER.txt","a");
	FILE * fp2 = fopen("total_newER.txt","a");
	fprintf(fp1,"%f\n",interval/1000.0);
	fprintf(fp2,"%d\n",calltimes);
	return 0;
}
bool CanBeExpand()
{
	clause_list curr = clauses;
	while(curr != NULL){
		INCLLIST* current = (curr->item);
		while(current != NULL){
			int lit = current->prop;
			int index = lit;
			if(index < 0)
				index = 0 - index;
			if( !((varArray[index - 1] == 1 && lit > 0) || (varArray[index - 1] == 0 && lit < 0)))
				break;
			current = current->next;
		}
		if(current == NULL)
			return true;
		curr = curr->next;
	}
	return false;
}




bool Binsearch(int varid1)
{
	if(varid1 == new_var_number){
		calltimes++;
		if(CanBeExpand() == false){
			cout<<"++++++++++++++++++++++++++++"<<endl;
			return true;
		}else
			return false;
	}else{
		for(int i = 0; i < 2; i++){
			varArray[varid1] = i;
			int varid = varid1 + 1;
			if(Binsearch(varid) == true)
				return true;
		}
		return false;
	}
}
/*******************************************************************************
 *
 *         load_clauses      装载文件
 *
********************************************************************************/
int load_clauses(FILE *fp,clause_list *cptr)
{
	int c;

	int pp = read_item(fp, junk);
	while( pp != 5)
		pp = read_item(fp, junk);

	read_item(fp, junk);
	var_number = atoi(junk);
	read_item(fp, junk);
	clause_number = atoi(junk);
	c = load_clause_recursive(fp, cptr);
	return c;
}
/*******************************************************************************
 *
 *  make_clause_list    将新事实组成事实列表的一个元素
 *
********************************************************************************/
clause_list make_clause_list(incl_list tlist, clause_list ptr)
{
  clause_list cptr = (clause_list) malloc(sizeof(clause_list_elt));
  cptr->item=tlist; /*不需再分配空间  只需将指针赋值*/
  cptr->next=ptr;
  return cptr;
}

int load_clause_recursive(FILE *fp, clause_list *cptr)
{
	incl_list ilist;
	int currentRead = read_item(fp, junk);
	if (currentRead == EOF )
	{
		*cptr = NULL;
		return 0;
	}


	int i = 0;
	while((junk[i] == '-')||isdigit(junk[i]))
	{
		i++;//测试当前命题长度，－3为2， －36为3，36为2
	}
	for (;i>=0;i--)
		ungetc(junk[i],fp);

	ilist = load_tokens(fp);//断点到这，调用load_takens，上面全对



	*cptr = make_clause_list(ilist, NULL);
	return 1 + load_clause_recursive(fp,&((*cptr)->next));


}

/*******************************************************************************
 *
 *incl_list_constructor         将一个命题序列构成一个字符序列链表的一个元素
 *
********************************************************************************/
incl_list incl_list_constructor(int prop)
{
	incl_list ret=(incl_list) malloc(sizeof(incl_list_elt));
	ret->prop = prop;
	ret->next=NULL;
	return ret;
}
/*******************************************************************************
 *
 *   load_tokens   读一个命题序列的链表 直到遇到0结束，
 *  其间假如tautology规则，若有tautology队则直接返回NULL
 *
 *
********************************************************************************/

incl_list load_tokens(FILE *fp)
{
	char temp_token[MAX_TOKEN_LENGTH];
	int result;
	incl_list_elt *current;

	result = read_item(fp, temp_token);

	if (result == ZERO)
		return NULL;
	int prop = atoi(temp_token);
	if(abs(prop)>maxVarNum)
		maxVarNum=abs(prop);



	current = incl_list_constructor(prop);

	current->next = load_tokens(fp);/*将下一个字符序列添加到该字符序列链表中*/

	return current;
}

/*****************************************************************************
 *
 *            is_stopper        读到空格,')','('返回值为1
 *
 ****************************************************************************/
int is_stopper(int c)
{
	return (isspace(c) );
}

int pushed=0;/*= next empty spot in stack.栈中结点为空*/
char buf[PUSH_HEIGHT][MAX_TOKEN_LENGTH];

void push_back(char bufin[MAX_TOKEN_LENGTH])
{
	if (pushed==PUSH_HEIGHT)
		printf("What th'! you are trying to push too much!\n");
	strcpy(buf[pushed],bufin);
	pushed++;
}
/*****************************************************************************
 *
 *             read_item    读 EOF , '(',')',概率,字符序列
 *
 ****************************************************************************/
int read_item(FILE *fp, char *str)
{
	char *strbegin;
	if (pushed)
	{/*栈中结点不空  从栈中读记录*/
		strbegin=str;
		strcpy(str,buf[--pushed]);
	}
	else
	{
		strbegin=str;
		/* first read in any blankspace and check for EOF */
		while(isspace(str[0] = getc(fp)));/*读空格*/
		if (str[0] != EOF && !is_stopper(str[0]))
		{
			str++;
			while(!is_stopper(*str++ = getc(fp)));    /* 读不含空格,'(',')'的单词 */
			// if (*(str-1) == CONNECTOR) *(str-1) = REPLACEMENT;/*将'_'转成'.'*/

			ungetc(*--str, fp);                      /*文件指针退回一格 */
			*str = '\0';                             /* 结束符 */
		}
	}
	if (strbegin[0] == EOF) return EOF;/*文件结束*/

	if (strbegin[0] == '0') return ZERO;/*读0*/

	if (strbegin[0] == '%') return EOF;/*读%*/ //为了benchmark，读入%也结束。

	if ((strbegin[0] == 'c')&&(strbegin[1] == 'n')&&(strbegin[2]=='f')) return BEGIN;

	if (isdigit(strbegin[0])|| (strbegin[0]=='-' && isdigit(strbegin[1])) )
		return PROP;/*读命题值*/
	else
		return UNUSEFULE;/*读字符序列*/
}

/***************************************************************************
 *
 *  normal     处理文件函数，将重复的原子删掉
 *
 **************************************************************************/

void normal(clause_list *clauses)
{
	int flag;
	clause_list *current;
	clause_list *next;
	current = clauses;
	next = &(*current)->next;
	current = clauses;
	next = &(*current)->next;
	flag = 0;
	incl_list *props;
    props = &((*current)->item);
	for (clause_list c1 = *clauses; c1 !=NULL; c1 = c1->next)
	{
		flag =0;
	   	for (incl_list i1 = c1->item; i1 != NULL; i1 = i1->next)
		{
		   for (incl_list i2=i1->next; i2 != NULL; i2 = i2->next)
		   {
			   if (i1->prop == (i2->prop))
			   	  flag = 1;//this prop has been in the clause.
		   }
		   if (flag == 1)
		   {
			   sFlag = 1;
			   if ((*props)->next != NULL)
			   {
				   *props = (*props)->next;
			   }
		   }
		   else
		   {
			   if (((*props)->next) !=NULL)
				   props = &(*props)->next;
		   }
		   flag = 0;
		}
		if ((*current)->item == NULL)
		{
			if ((*current)->next != NULL)
			{
				*current = *next;
				next = &(*current)->next;
			}
			else
				*current =NULL;
		}
		else
		{
			current = &(*current)->next;
			if (*current != NULL&&(((*current)->next !=NULL)))
			{
				next = &(*current)->next;
			}
		}
		props = &((*current)->item);
	}
}


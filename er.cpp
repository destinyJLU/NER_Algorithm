////////////newer////////////////
#include "er.h"
#include <time.h>
#include <stdio.h>
#include "myqueue.h"
#include <iostream>
using namespace std;
/******************************************************/

clause_list clauses;//ER������Ӿ伯��
clause_list iiclauses=new(clause_list_elt);//IER������Ӿ伯
char junk[100];//�����
int var_number;//�ļ������ı�������
int clause_number;//�ļ��������Ӿ�ĸ���
int new_clause_number=0;//���껯��������Ӿ�ĸ���
int new_var_number = 0;//���껯������ı����ĸ���
//int pre_var_number = 0;//Ԥ�����ı�����������ǰ����1��2����������
int flag1[MAX];
int pFlag = 1;
int sFlag = 1;
int * pointer;//��ʱ��������ǰ������Ӿ�

long seed;
myqueue my_queue;//���治�������Ե��Ӿ�
long double totalnumber;//������ĸ���
long double number;//����չ���ļ�����ĸ���
int* hasliteral;//��ʱ�������������Ӿ伯�Ĳ���
//int hasliteral[MAX];
int musknumber;//����ѡ��C��C��1��2����musknumber
int maxVarNum=0;//�Ӿ伯�������Ӿ�ı��
FILE *in, *out;
int numberofnzero;
char * error = "ERROR";
void do_error(char *s);
int* varArray;

int calltimes;
int endtimes;

/***********������*************************************/
int main(int argc, char *argv[])
{

	int  clanum = 100;
    int varnum = 30;
    int maxl = 3;
    seed = 2234;
	in = fopen( "e://in.txt", "w" );
	randomGeneration( in,clanum,varnum,maxl);
	fclose( in );
	/*******��ʼ��ʱ*************************/
	FILE *fp;/*fpΪָ��FILE�ļ��ṹ���ָ�����*/
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
 *         load_clauses      װ���ļ�
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
 *  make_clause_list    ������ʵ�����ʵ�б��һ��Ԫ��
 *
********************************************************************************/
clause_list make_clause_list(incl_list tlist, clause_list ptr)
{
  clause_list cptr = (clause_list) malloc(sizeof(clause_list_elt));
  cptr->item=tlist; /*�����ٷ���ռ�  ֻ�轫ָ�븳ֵ*/
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
		i++;//���Ե�ǰ���ⳤ�ȣ���3Ϊ2�� ��36Ϊ3��36Ϊ2
	}
	for (;i>=0;i--)
		ungetc(junk[i],fp);

	ilist = load_tokens(fp);//�ϵ㵽�⣬����load_takens������ȫ��



	*cptr = make_clause_list(ilist, NULL);
	return 1 + load_clause_recursive(fp,&((*cptr)->next));


}

/*******************************************************************************
 *
 *incl_list_constructor         ��һ���������й���һ���ַ����������һ��Ԫ��
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
 *   load_tokens   ��һ���������е����� ֱ������0������
 *  ������tautology��������tautology����ֱ�ӷ���NULL
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

	current->next = load_tokens(fp);/*����һ���ַ�������ӵ����ַ�����������*/

	return current;
}

/*****************************************************************************
 *
 *            is_stopper        �����ո�,')','('����ֵΪ1
 *
 ****************************************************************************/
int is_stopper(int c)
{
	return (isspace(c) );
}

int pushed=0;/*= next empty spot in stack.ջ�н��Ϊ��*/
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
 *             read_item    �� EOF , '(',')',����,�ַ�����
 *
 ****************************************************************************/
int read_item(FILE *fp, char *str)
{
	char *strbegin;
	if (pushed)
	{/*ջ�н�㲻��  ��ջ�ж���¼*/
		strbegin=str;
		strcpy(str,buf[--pushed]);
	}
	else
	{
		strbegin=str;
		/* first read in any blankspace and check for EOF */
		while(isspace(str[0] = getc(fp)));/*���ո�*/
		if (str[0] != EOF && !is_stopper(str[0]))
		{
			str++;
			while(!is_stopper(*str++ = getc(fp)));    /* �������ո�,'(',')'�ĵ��� */
			// if (*(str-1) == CONNECTOR) *(str-1) = REPLACEMENT;/*��'_'ת��'.'*/

			ungetc(*--str, fp);                      /*�ļ�ָ���˻�һ�� */
			*str = '\0';                             /* ������ */
		}
	}
	if (strbegin[0] == EOF) return EOF;/*�ļ�����*/

	if (strbegin[0] == '0') return ZERO;/*��0*/

	if (strbegin[0] == '%') return EOF;/*��%*/ //Ϊ��benchmark������%Ҳ������

	if ((strbegin[0] == 'c')&&(strbegin[1] == 'n')&&(strbegin[2]=='f')) return BEGIN;

	if (isdigit(strbegin[0])|| (strbegin[0]=='-' && isdigit(strbegin[1])) )
		return PROP;/*������ֵ*/
	else
		return UNUSEFULE;/*���ַ�����*/
}

/***************************************************************************
 *
 *  normal     �����ļ����������ظ���ԭ��ɾ��
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


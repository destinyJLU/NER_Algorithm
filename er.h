#include <stdlib.h>/*������̬�洢����ĺ���*/
#include <malloc.h>/*������̬�洢����ĺ���*/
#include <stdio.h>
#include <ctype.h>/*�����ַ�����*/
#include <string.h>
#include <assert.h>
#include <math.h>


#define MAX_TOKEN_LENGTH 100
#define PUSH_HEIGHT 2
#define ZERO 0
#define PROP 1
#define UNUSEFULE 2
#define EQUAL 3
#define ENDCLAUSE 4
#define BEGIN 5
#define MAX 50000


extern long seed;


/************************************data structure*************/
typedef struct INCLLIST{
	  int prop;
	  struct INCLLIST *next;
	  INCLLIST(int pprop=0,INCLLIST *nnext=NULL)
	  {
		  prop=pprop;
		  next=nnext;
	  }
}*incl_list,incl_list_elt,*incl_ptr;/*******���ڴ洢һ���Ӿ䣬ÿ��propΪһ������******/

typedef struct CLAUSELIST{
	incl_list item;
	struct CLAUSELIST *next;
	CLAUSELIST(incl_list iitem=0,CLAUSELIST *nnext=NULL)
	{
		item=iitem;
		next=nnext;
	}
}clause_list_elt,*clause_list;/************���ڴ洢һ���Ӿ�����******************/


/*************************utility functions********************/
/**************************load files**********************/
int load_clauses(FILE *fp, clause_list *cptr);
clause_list make_clause_list(incl_list tlist, clause_list ptr);
int load_clause_recursive(FILE *fp, clause_list *cptr);/*װ�ز����ļ�*/
void decidetest(clause_list * clause);
int read_item(FILE *fp, char *str);
incl_list load_tokens(FILE *fp);
incl_list incl_list_constructor(int prop );
/**************************handle files************************/
//void Tautology(clause_list *clauses);
void normal(clause_list *clauses);

void PureLit(clause_list *clause);
int SingLit(clause_list *clause);
long double computeexp(clause_list *clauses,int k);

int decide1();

long double computeexp_in(clause_list *clauses);
int decide_in1(clause_list *clause);
void preprocess(clause_list *clause,int * clauseNumber);
double complefact(clause_list *clause);
void randomGeneration( FILE* in, int clauseNumber, int valueNumber, 
					   int clauseLength );
void inclusion(clause_list *clause);


 bool Binsearch(int varid);
 bool CanBeExpand();
#include "myqueue.h"

myqueue::myqueue(long maxsize)
{
	m_maxsize=maxsize;
	m_queue=new int[m_maxsize+1];
	front=rear=1;
	for(int i=0;i<=m_maxsize;i++)
		m_queue[i]=-1;
}

myqueue::myqueue()
{
	m_maxsize=5000000;
	m_queue=new int[5000000+1];
	front=rear=1;
//	for(int i=0;i<=5000000;i++)
//	{
//		m_queue[i]=-1;
//	}
	queuenumber=0;
}

myqueue::~myqueue()
{
	delete m_queue;
}

void myqueue::reset()
{
	front=rear=1;
}
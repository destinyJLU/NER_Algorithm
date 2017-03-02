//#include "StdAfx.h"

#ifndef myqueue_h
#define myqueue_h

class myqueue
{
public:
	myqueue(long maxsize);
	myqueue();
	~myqueue();
	void reset();
	inline bool isitempty() {return front==rear;}
	inline bool isitoverflow() {return (rear+1)%m_maxsize==front;}
	inline bool pushitem(int item)
	{
		if(!isitoverflow())
		{
			m_queue[rear]=item;
			rear=(rear+1)%m_maxsize;
			queuenumber++;
			return 1;
		}
		else
			return 0;
	}
	inline bool popitem()
	{
		if(!isitempty())
		{
			front=(front+1)%m_maxsize;
			queuenumber--;
			return 1;
		}
		else
			return 0;
	}
	int* m_queue;
	long m_maxsize;
	long front;
	long rear;
	int queuenumber;
};

#endif
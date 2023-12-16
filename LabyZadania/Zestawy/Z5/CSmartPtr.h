#pragma once


class CRefCounter
{
public:
	CRefCounter() { i_count = 0; }
	int iAdd() { return(++i_count); }
	int iDec() { return(--i_count); };
	int iGet() { return(i_count); }
private:
	int i_count;
};


template <class T>
class CSmartPtr
{
public:
	CSmartPtr<T>(T* pcPointer)
	{
		pc_pointer = pcPointer;
		pc_counter = new CRefCounter();
		pc_counter->iAdd();
	}

	CSmartPtr<T> operator=(const CSmartPtr<T> cOther) {
		if (pc_counter->iDec() == 0)
		{
			delete pc_pointer;
			delete pc_counter;
		}
		pc_pointer = cOther.pc_pointer;
		pc_counter = cOther.pc_counter;
		pc_counter->iAdd();
	}

	CSmartPtr<T>(const CSmartPtr& pcOther)
	{
		pc_pointer = pcOther.pc_pointer;
		pc_counter = pcOther.pc_counter;
		pc_counter->iAdd();
	}


	~CSmartPtr<T>()
	{
		if (pc_counter->iDec() == 0)
		{
			delete pc_pointer;
			delete pc_counter;
		}//if (pc_counter->iDec())
	}

	T& operator*() { return(*pc_pointer); }
	T* operator->() { return(pc_pointer); }
private:
	CRefCounter* pc_counter;
	T* pc_pointer;
};


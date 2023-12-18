#pragma once
#include <set>

template <class T>
class CSmartPtr;


template<class T>
class CRefCounter
{
public:
	CRefCounter<T>() { i_count = 0; }
	int iAdd(CSmartPtr<T>* pPtr) { 
		vPointers.insert(pPtr);
		return(++i_count); }
	int iDec(CSmartPtr<T>* pPtr) {
		vPointers.erase(pPtr);
		return(--i_count); };
	int iGet() { return(i_count); }
private:
	int i_count;
	std::set<CSmartPtr<T>*> vPointers;
};


template <class T>
class CSmartPtr
{
public:
	CSmartPtr<T>(T* pcPointer)
	{
		pc_pointer = pcPointer;
		pc_counter = new CRefCounter<T>();
		pc_counter->iAdd(this);

	}

	CSmartPtr<T> operator=(const CSmartPtr<T>& cOther) {
		if (pc_counter->iDec(this) == 0)
		{
			delete pc_pointer;
			delete pc_counter;
		}
		pc_pointer = cOther.pc_pointer;
		pc_counter = cOther.pc_counter;
		pc_counter->iAdd(this);

		return *this;
	}

	CSmartPtr<T>(const CSmartPtr& pcOther)
	{
		pc_pointer = pcOther.pc_pointer;
		pc_counter = pcOther.pc_counter;
		pc_counter->iAdd(this);
	}


	~CSmartPtr<T>()
	{
		if (pc_counter->iDec(this) == 0)
		{
			delete pc_pointer;
			delete pc_counter;
		}//if (pc_counter->iDec())
	}

	T& operator*() const { return(*pc_pointer); }
	T* operator->() const { return(pc_pointer); }
private:
	CRefCounter<T>* pc_counter;
	T* pc_pointer;
};


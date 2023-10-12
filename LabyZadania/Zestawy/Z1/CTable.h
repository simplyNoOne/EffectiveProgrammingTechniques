#pragma once
#include <string>
#include <iostream>

const std::string defaultName = "none";
const int defaultTabLen = 1;

class CTable {
public:
	//
	CTable();
	CTable(std::string sName, int iTableLen);
	CTable(CTable& pcOther);
	
	//
	~CTable();

	//
	inline void vSetName(std::string sName) { this->sName = sName; }
	bool bSetNewSize(int iTableLen);
	CTable* pcClone();

private:
	std::string sName;
	int* piTable;	
	int iTableLen;


private:
	//helper function for constructors so I don't have to type the same thing 3times
	void vCTableInit(std::string sName, int iTableLen, std::string message);
};
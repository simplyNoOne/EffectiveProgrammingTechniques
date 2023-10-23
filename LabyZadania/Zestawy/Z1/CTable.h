#pragma once
#include <string>
#include <iostream>

const std::string sDefaultName = "none";
const int iDefaultTabLen = 1;
const std::string sCopySuffix = "_copy";
const std::string sDoubleSuffix = "_double";

class CTable {
public:
	//
	CTable();
	CTable(bool setCustom);
	CTable(std::string sName, int iTableLen);
	CTable(const CTable& pcOther);
	
	//
	~CTable();

	//
	inline void vSetName(std::string sName) { this->sName = sName; }
	bool bSetNewSize(int iTableLen);
	CTable* pcClone();
	std::string sToString();
	void vDoubleSize(CTable **other);

private:
	std::string sName;
	int* piTable;	
	int iTableLen;


private:
	//helper function for constructors so I don't have to type the same thing 3times
	void vCTableInit(std::string sName, int iTableLen, std::string message);
};
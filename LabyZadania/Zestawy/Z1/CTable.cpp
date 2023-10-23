#include "CTable.h"
#include <sstream>

CTable::CTable(){  
	vCTableInit(sDefaultName, iDefaultTabLen, "bezp");
	
}

CTable::CTable(bool setCustom)
{
	vCTableInit("custom", 4, "custom");
	for (int i = 0; i < this->iTableLen; i++) {
		this->piTable[i] = i;
	}
}

CTable::CTable(std::string sName, int iTableLen){
	vCTableInit(sName, iTableLen, "parametr");
	
}

CTable::CTable(const CTable& pcOther){
	vCTableInit(pcOther.sName + sCopySuffix, pcOther.iTableLen, "kopiuj");
	
	for (int i = 0; i < this->iTableLen; i++) {
		this->piTable[i] = pcOther.piTable[i];
	}
}

CTable::~CTable() {
	std::cout << "usuwam " << this->sName << std::endl;
	delete[] piTable;

}


bool CTable::bSetNewSize(int iTableLen){
	if (iTableLen > 0) {
		int* piTemp = new int[iTableLen];
		this->iTableLen = iTableLen;
		std::copy(this->piTable, this->piTable + std::min(iTableLen, this->iTableLen), piTemp);
		delete this->piTable;
		this->piTable = piTemp;
	}
	else {
		return false;
	}
	return true;
}

CTable* CTable::pcClone()
{
	CTable* pcTemp = new CTable(*this);
	//make sure that the name doesn't have the stupid _copy
	pcTemp->sName = this->sName;
	return pcTemp;
}

std::string CTable::sToString()
{
	std::ostringstream ossResult;
	ossResult << "name: " << this->sName;
	ossResult << "[ ";
	for (int i = 0; i < this->iTableLen; i++) {
		ossResult << this->piTable[i]<< " ";
	}
	ossResult << "]\n";
	return ossResult.str();
}

void CTable::vDoubleSize(CTable** other)
{
	if (other == NULL) {
		return;
	}
	*other = new CTable(this->sName + sDoubleSuffix, this->iTableLen * 2);
	std::copy(this->piTable, this->piTable + this->iTableLen, (*other)->piTable);
	std::copy(this->piTable, this->piTable + this->iTableLen, (*other)->piTable + this->iTableLen);
	
}

void CTable::vCTableInit(std::string sName, int iTableLen, std::string message) {
	this->sName = sName;
	std::cout << message << ": '" << this->sName << "'\n";
	this->piTable = new int[iTableLen];
	this->iTableLen = iTableLen;
}

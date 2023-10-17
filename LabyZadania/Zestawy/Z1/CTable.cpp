#include "CTable.h"

CTable::CTable(){  
	vCTableInit(defaultName, defaultTabLen, "bezp");
	
}

CTable::CTable(std::string sName, int iTableLen){
	vCTableInit(sName, iTableLen, "parametr");
	
}

CTable::CTable(CTable& pcOther){
	vCTableInit(pcOther.sName + "_copy", pcOther.iTableLen, "kopiuj");
	
	for (int i = 0; i < iTableLen; i++) {
		piTable[i] = pcOther.piTable[i];
	}
}

CTable::~CTable() {
	std::cout << "usuwam " << sName << std::endl;
	delete[] piTable;
}


bool CTable::bSetNewSize(int iTableLen){
	if (iTableLen > 0) {
		int* piTemp = new int[iTableLen];
		int iLenToCopy = this->iTableLen > iTableLen ? iTableLen : this->iTableLen;
		this->iTableLen = iTableLen;
		for (int i = 0; i < iLenToCopy; i++) {
			piTemp[i] = piTable[i];
		}
		delete[] piTable;
		piTable = piTemp;
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

void CTable::vCTableInit(std::string sName, int iTableLen, std::string message) {
	this->sName = sName;
	std::cout << message << ": '" << this->sName << "'\n";
	piTable = new int[iTableLen];
	this->iTableLen = iTableLen;
}

#include "z1Tools.h"

void vAllocTableFill34(int iSize) {

	int* piTable;
	if (iSize > 0) {
		piTable = new int[iSize];
		for (int i = 0; i < iSize; i++) {
			piTable[i] = valToFillTab;
		}
		std::cout << "[ ";
		for (int i = 0; i < iSize; i++) {
			std::cout << piTable[i] << " ";
		}
		std::cout << "]\n";
		delete piTable;
	}
	else {
		std::cout << "table size must be greater than 0!\n";
	}

}

bool bAllocTable2Dim(int*** piTable, int iSizeX, int iSizeY) {
	if (iSizeX <= 0 || iSizeY <= 0 || piTable == NULL) {
		return false;
	}
	*piTable = new int* [iSizeX];
	for (int i = 0; i < iSizeX; i++) {
		(*piTable)[i] = new int[iSizeY];
	}

	return true;
}

//technically, you could just pass this one by value
// ...cuz the tables will get destroyed either way
// ...but with passing by value, our original pointer will point to invalid data
//passing only iSizeX is enough, iSizeY is redundant
bool bDeallocTable2Dim(int*** piTable, int iSizeX, int iSizeY) {
	if (iSizeX <= 0 || iSizeY <= 0 || piTable == NULL || *piTable == NULL) {
		return false;
	}
	for (int i = 0; i < iSizeX; i++) {
		if ((*piTable)[i] != NULL) {
			delete (*piTable)[i];
		}
	}
	delete* piTable;

	*piTable = NULL;
	return true;
}


//the address of the original is passed, and so, modification works, no copy is created
void vModTable(CTable* pcTab, int iNewSize) {
	pcTab->bSetNewSize(iNewSize);
}

//here a copy constructor is called and a copy is created, no mod of the original
void vModTable(CTable cTab, int iNewSize) {
	cTab.bSetNewSize(iNewSize);
}


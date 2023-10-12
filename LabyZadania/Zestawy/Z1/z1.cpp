#include "z1.h"


void test();

void vZ1start() {
	//1
	//vAllocTableFill34(6);

	//2 //no way there is a way to do it without reference????
	int** piTable;
	std::cout << bAllocTable2Dim(&piTable, 5, 3);

	//3
	//std::cout << bDeallocTable2Dim(&piTable, 5, 3);

	//4
	/*CTable cTab1;
	cTab1.vSetName("one");
	CTable* pcTab2 = new CTable(cTab1);
	CTable* pcTab3 = new CTable("three", 7);
	CTable* pcTabClone = pcTab3->pcClone();
	cTab1.bSetNewSize(3);
	vModTable(cTab1, 7);
	vModTable(pcTab3, 9);

	delete pcTab2;
	delete pcTab3;
	delete pcTabClone;*/
	test();
}

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
		delete[] piTable;
	}
	else {
		std::cout << "table size must be greater than 0!\n";
	}

}

bool bAllocTable2Dim(int*** piTable, int iSizeX, int iSizeY) {
	if (iSizeX <= 0 || iSizeY <= 0) {
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
	if (iSizeX <= 0 || iSizeY <= 0 || *piTable == NULL) {
		return false;
	}
	for (int i = 0; i < iSizeX; i++) {
		if ((*piTable)[i] != NULL) {
			delete[] (*piTable)[i];
		}		
	}
	delete[] *piTable;
	
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


void test() {

	int ourInt = 6;

	int* ptrToOurInt = &ourInt;

	int** ptrToPtrToOurInt = &ptrToOurInt;

	int* tab = new int[4];
	int** ptr2Tab = &tab;

	tab[1] = 3;
	std::cout << *(tab + 1);

}
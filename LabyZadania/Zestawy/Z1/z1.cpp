#include "z1.h"
#include "z1Tools.h"



void vZ1start() {
	//1
	vAllocTableFill34(6);

	vAllocTableFill34(-1);

	vAllocTableFill34(0);

	//2 
	int** piTable1;
	std::cout << bAllocTable2Dim(&piTable1, 5, 3);

	int** piTable2;
	std::cout << bAllocTable2Dim(&piTable2, 4, 0);

	int** piTable3;
	std::cout << bAllocTable2Dim(&piTable3, -2, 0);

	int** piTable4 = NULL;
	std::cout << bAllocTable2Dim(&piTable4, 4, 9);

	std::cout << bAllocTable2Dim(NULL, 4, 9);



	//3
	std::cout << bDeallocTable2Dim(&piTable1, 5, 3);

	std::cout<< bDeallocTable2Dim(&piTable2, 4, 0);

	piTable3 = NULL;
	std::cout<< bDeallocTable2Dim(&piTable3, 4, 1);


	//4
	CTable cTab1;
	cTab1.vSetName("one");
	CTable* pcTab2 = new CTable(cTab1);
	CTable* pcTab3 = new CTable("three", 7);
	CTable* pcTabClone = pcTab3->pcClone();
	cTab1.bSetNewSize(3);

	vModTable(cTab1, 7);
	vModTable(pcTab3, 9);
	vModTable(pcTab3, -1);


	delete pcTab2;
	delete pcTab3;
	delete pcTabClone;
}

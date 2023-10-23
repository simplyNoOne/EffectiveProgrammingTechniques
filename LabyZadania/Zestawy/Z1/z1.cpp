#include "z1.h"
#include "z1Tools.h"
#include <vector>


void vZ1start() {
	//1
	vAllocTableFill34(6);

	vAllocTableFill34(-1);

	vAllocTableFill34(0);

	//2 
	int** piTable1;
	std::cout << bAllocTable2Dim(&piTable1, 5, 3)<<std::endl;

	int** piTable2;
	std::cout << bAllocTable2Dim(&piTable2, 4, 0) << std::endl;

	int** piTable3;
	std::cout << bAllocTable2Dim(&piTable3, -2, 0) << std::endl;

	int** piTable4 = NULL;
	std::cout << bAllocTable2Dim(&piTable4, 4, 9) << std::endl;

	std::cout << bAllocTable2Dim(NULL, 4, 9) << std::endl;


	//3
	std::cout << bDeallocTable2Dim(&piTable1, 5, 3) << std::endl;

	std::cout<< bDeallocTable2Dim(&piTable2, 4, 0) << std::endl;

	piTable3 = NULL;
	std::cout<< bDeallocTable2Dim(&piTable3, 4, 1) << std::endl;


	//4
	CTable cTab1;
	cTab1.vSetName("one");
	CTable* pcTab2 = new CTable(cTab1);
	CTable* pcTab3 = new CTable("three", 7);
	CTable* pcTabClone = pcTab3->pcClone();
	std::cout << cTab1.bSetNewSize(3)<<std::endl;

	std::cout << pcTab3->bSetNewSize(-3)<<std::endl;


	CTable ctatata = CTable("onefdf", 6 );

	std::cout << cTab1.sToString();
	vModTable(cTab1, 7);
	std::cout << cTab1.sToString();
	std::cout<<pcTab3->sToString();
	vModTable(pcTab3, 9);
	std::cout << pcTab3->sToString();
	vModTable(pcTab3, -1);


	delete pcTab2;
	delete pcTab3;
	delete pcTabClone;

	CTable* arr = new CTable[3];
	delete[] arr;


	//
	CTable cTab(true);
	std::cout<<cTab.sToString();

	CTable* pcDoubleTab;
	cTab.vDoubleSize(&pcDoubleTab);
	std::cout << pcDoubleTab->sToString();

	delete pcDoubleTab;


}

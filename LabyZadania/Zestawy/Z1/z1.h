#pragma once
#include "CTable.h"

const int valToFillTab = 34;


void vZ1start();

//1
void vAllocTableFill34(int iSize);

//2
bool bAllocTable2Dim(int*** piTable, int iSizeX, int iSizeY);

//3
bool bDeallocTable2Dim(int*** piTable, int iSizeX, int iSizeY);

//4
void vModTable(CTable* pcTab, int iNewSize);

//this one is just bull, it doesn't modify anything, merely creates a copy
void vModTable(CTable cTab, int iNewSize);


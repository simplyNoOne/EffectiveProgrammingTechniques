#include "tests.h"
#include "CNumber.h"
#include <iostream>


void Tests::vRunTests(){
	//test addition
	testAddition(5, 3, 8, 4);
	testAddition(-2, 7, 5, 10);
	testAddition(5000, 2000, 7000, 17);
	testAddition(0, 0, 0, 10);
	testAddition(-5, -8, -13, 10);
	testAddition(999, -999, 0, 10);
	testAddition(123456, 987654, 1111110, 12);
	testAddition(999999, 1, 1000000, 10);
	testAddition(123456, -987654, -864198, 3);
	testAddition(123456, 987654, 1111110, 10);

	// Test subtraction
	testSubtraction(10, 3, 7, 10);
	testSubtraction(-7, -2, -5, 10);
	testSubtraction(3000, 2000, 1000, 12);
	testSubtraction(0, 0, 0, 10);
	testSubtraction(5, 8, -3, 10);
	testSubtraction(999, -999, 1998, 15);
	testSubtraction(98765, 12345, 86420, 10);
	testSubtraction(100000, 1, 99999, 10);
	testSubtraction(-12345, -98765, 86420, 7);
	testSubtraction(100000, 98765, 1235, 10);

	// Test multiplication
	testMultiplication(2, 3, 6, 4);
	testMultiplication(-7, -2, 14, 10);
	testMultiplication(3000, 2000, 6000000, 5);
	testMultiplication(0, 0, 0, 10);
	testMultiplication(5, 8, 40, 10);
	testMultiplication(999, -999, -998001, 7);
	testMultiplication(98765, 12345, 1219253925, 10);
	testMultiplication(100000, 1, 100000, 10);
	testMultiplication(-12345, -98765, 1219253925, 10);

	// Test division
	testDivision(10, 2, 5, 10);
	testDivision(-14, -2, 7, 14);
	testDivision(6000, 2000, 3, 10);
	testDivision(1, 5, 0, 11);
	testDivision(40, 8, 5, 10);
	testDivision(43, 8768, 0, 10);
	testDivision(-998001, -999, 999, 10);
	testDivision(1219253925, 12345, 98765, 10);
	testDivision(100000000, 1, 100000000, 9);
	testDivision(1219253925, -12345, -98765, 10);

	vShowStats();

}

int Tests::testAddition(int iLeft, int iRight, int iResult, int iBase){	
	printf("%d + %d = %d (base: %d)\n", iLeft, iRight, iResult, iBase);
	CNumber::vSetSystemBase(iBase);
	int iTestPassed = 0;
	CNumber cLeft = iLeft;
	CNumber cRight = iRight;
	CNumber cResult = cLeft + cRight;
	printf("CNumber + CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = cLeft + iRight;
	printf("CNumber + int -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = iLeft + cRight;
	printf("int + CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	std::cout << std::endl;
	vIncreasePassed(iTestPassed);
	vIncreaseTotal(3);
	return iTestPassed;
}

int Tests::testSubtraction(int iLeft, int iRight, int iResult, int iBase){
	printf("%d - %d = %d (base: %d)\n", iLeft, iRight, iResult, iBase);
	CNumber::vSetSystemBase(iBase);
	int iTestPassed = 0;
	CNumber cLeft = iLeft;
	CNumber cRight = iRight;
	CNumber cResult = cLeft - cRight;
	printf("CNumber - CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = cLeft - iRight;
	printf("CNumber - int -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = iLeft - cRight;
	printf("int - CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	std::cout << std::endl;
	vIncreasePassed(iTestPassed);
	vIncreaseTotal(3);
	return iTestPassed;
}

int Tests::testMultiplication(int iLeft, int iRight, int iResult, int iBase){
	printf("%d * %d = %d (base: %d)\n", iLeft, iRight, iResult, iBase);
	CNumber::vSetSystemBase(iBase);
	int iTestPassed = 0;
	CNumber cLeft = iLeft;
	CNumber cRight = iRight;
	CNumber cResult = cLeft * cRight;
	printf("CNumber * CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = cLeft * iRight;
	printf("CNumber * int -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = iLeft * cRight;
	printf("int * CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	std::cout << std::endl;
	vIncreasePassed(iTestPassed);
	vIncreaseTotal(3);
	return iTestPassed;
}

int Tests::testDivision(int iLeft, int iRight, int iResult, int iBase){
	printf("%d / %d = %d (base: %d)\n", iLeft, iRight, iResult, iBase);
	CNumber::vSetSystemBase(iBase);
	int iTestPassed = 0;
	CNumber cLeft = iLeft;
	CNumber cRight = iRight;
	CNumber cResult = cLeft / cRight;
	printf("CNumber / CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = cLeft / iRight;
	printf("CNumber / int -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	cResult = iLeft / cRight;
	printf("int / CNumber -> %s\n", (cResult == iResult) ? "true" : "false");
	iTestPassed += (cResult == iResult) ? 1 : 0;
	std::cout << std::endl;
	vIncreasePassed(iTestPassed);
	vIncreaseTotal(3);
	return iTestPassed;
}

void Tests::vShowStats(){
	std::cout<< "passed " << iPassedTests << " out of " << iTotalTests<<std::endl;
}


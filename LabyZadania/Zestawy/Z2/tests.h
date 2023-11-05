#pragma once


class CNumber;
namespace Tests {

	static int iTotalTests;
	static int iPassedTests;

	void vRunTests();

	int testAddition(int iLeft, int iRight, int iResult, int iBase = 10);
	int testSubtraction(int iLeft, int iRight, int iResult, int iBase = 10);
	int testMultiplication(int iLeft, int iRight, int iResult, int iBase = 10);
	int testDivision(int iLeft, int iRight, int iResult, int iBase = 10);

	inline void vIncreasePassed(int amount) { iPassedTests += amount; }
	inline void vIncreaseTotal(int amount) { iTotalTests += amount;  }
	void vShowStats();
}
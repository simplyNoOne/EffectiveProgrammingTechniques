#include "CNumber.h"

#include <cmath>
#include <algorithm>


CNumber::CNumber()
{
	iLength = iDefualtNumberLength;
	piNumber = new int[iLength];
};

CNumber::CNumber(const int iValue) {
	CNumber();
	*this = iValue;
}

CNumber::CNumber(const CNumber& cValue)
{
	CNumber();
	*this = cValue;
}


void CNumber::operator=(const int iValue)
{
	delete piNumber;
	int iNumLen = 1;
	while(iValue >= std::pow(iDecimalSystemBase, iNumLen)){
		iNumLen++;
	}
	iLength = iNumLen;
	piNumber = new int[iLength];
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = (iValue / (int)std::pow(iDecimalSystemBase, i)) % iDecimalSystemBase;
	}
}

void CNumber::operator=(const CNumber& cValue)
{
	delete piNumber;
	iLength = cValue.iLength;
	piNumber = new int[iLength];
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = cValue.piNumber[i];
	}
}

CNumber CNumber::operator+(CNumber& cValue)
{
	int iLenToTraverse = std::min(iLength, cValue.iLength);
	int iMinNumLen = std::max(iLength, cValue.iLength);

	CNumber cResult = CNumber();
	cResult.iLength = iMinNumLen;
	cResult.piNumber = new int[iMinNumLen];

	int iCarry = 0;
	int iDigitResult = 0;
	for (int i = 0; i < iLenToTraverse; i++) {
		iDigitResult = (iCarry + piNumber[i] + cValue.piNumber[i]);
		cResult.piNumber[i] = iDigitResult % iDecimalSystemBase;
		iCarry = iDigitResult / iDecimalSystemBase;
	}
	int* piLongerNum = (iLength > cValue.iLength) ? piNumber : cValue.piNumber;

	for (int i = iLenToTraverse; i < iMinNumLen; i++) {
		iDigitResult = iCarry + piLongerNum[i];
		cResult.piNumber[i] = iDigitResult % iDecimalSystemBase;
		iCarry = iDigitResult / iDecimalSystemBase;
	}

	if (iCarry != 0) {
		int* piTemp = cResult.piNumber;
		cResult.piNumber = new int[++cResult.iLength];
		for (int i = 0; i < iMinNumLen; i++) {
			cResult.piNumber[i] = piTemp[i];
		}
		cResult.piNumber[iMinNumLen] = iCarry;
		delete piTemp;
	}

	return cResult;
}

CNumber CNumber::operator*(CNumber& cValue)
{
	return CNumber();
}

CNumber CNumber::operator-(CNumber& cValue)
{
	int iLenToTraverse = std::min(iLength, cValue.iLength);
	int iMaxNumLen = std::max(iLength, cValue.iLength);

	int* piLhsSubRhs = new int[iMaxNumLen];
	int* piRhsSubLhs = new int[iMaxNumLen];
	int iCarryL = 0;
	int iCarryR = 0;

	bExecuteSubtractionLoop(0, iLenToTraverse, piNumber, cValue.piNumber, piLhsSubRhs, piRhsSubLhs, &iCarryL, &iCarryR);

	CNumber cResult = CNumber();
	cResult.iLength = iMaxNumLen;
	cResult.piNumber = (iCarryL == 0) ? piLhsSubRhs : piRhsSubLhs;


	if (bExecuteSubtractionLoop(iLenToTraverse, cValue.iLength, NULL, cValue.piNumber, NULL, piRhsSubLhs, NULL, &iCarryR)){
		cResult.piNumber = piRhsSubLhs;
	}

	if(bExecuteSubtractionLoop(iLenToTraverse, iLength, piNumber, NULL, piLhsSubRhs, NULL, &iCarryL, NULL)) {
		cResult.piNumber = piLhsSubRhs;
	}

	if (cResult.piNumber == piRhsSubLhs) {
		delete piLhsSubRhs;
	}
	else {
		delete piRhsSubLhs;
	}
	
	return cResult;
}

CNumber CNumber::operator/(CNumber& cValue)
{
	return CNumber();
}

CNumber CNumber::operator+(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this + cConvertedValue;
}

CNumber CNumber::operator*(int iValue)
{
	return CNumber();
}

CNumber CNumber::operator-(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this - cConvertedValue;
}

CNumber CNumber::operator/(int iValue)
{
	return CNumber();
}

std::string CNumber::sToString()
{
	std::ostringstream ossResult;
	for (int i = iLength - 1; i >= 0; i--) {
		ossResult<<piNumber[i];
	}
	return ossResult.str();
}

bool CNumber::bExecuteSubtractionLoop(int iStart, int iStop, const int* piLhs, const int* piRhs, int* piLtR, int* piRtL, int *iCarryL, int *iCarryR) {
	int iEnteredLoop = false;

	for (int i = iStart; i < iStop; i++) {
		if (!iEnteredLoop) {
			iEnteredLoop = true;
		}
		if (piLtR != NULL) {
			piLtR[i] = getResultingDigit(i, piLhs, piRhs, iCarryL);
		}

		if (piRtL != NULL) {
			piRtL[i] = getResultingDigit(i, piRhs, piLhs, iCarryR);
		}
	
	}
	return iEnteredLoop;
}

int CNumber::getResultingDigit(int iIndex, const int* piLhs, const int* piRhs, int* iCarry) {
	int iResult = 0;

	if (piRhs == NULL) {
		iResult = (piLhs[iIndex] - *iCarry);
	}
	else {
		iResult = (piLhs[iIndex] - *iCarry - piRhs[iIndex]);
	}

	if (iResult < 0) {
		*iCarry = 1;
		iResult = iDecimalSystemBase + iResult;
	}
	else {
		*iCarry = 0;
	}

	return iResult;
}
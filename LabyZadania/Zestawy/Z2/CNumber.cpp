#include "CNumber.h"

#include <cmath>
#include <algorithm>


CNumber::CNumber()
{
	iLength = iDefualtNumberLength;
	piNumber = new int[iLength];
	piNumber[0] = 0;
	bPositive = true;
};

CNumber::CNumber(const int iValue) {
	piNumber = new int[iDefualtNumberLength];
	*this = iValue;
	//*this = cIntToNumber(iValue);
	
}

CNumber::CNumber(const CNumber& cValue)
{
	piNumber = new int[iDefualtNumberLength];
	*this = cValue;
}


void CNumber::operator=(const int iValue)
{
	delete piNumber;
	bPositive = (iValue >= 0);
	int iAbs = std::abs(iValue);
	int iNumLen = 0;
	while (iAbs > 0) {
		iAbs /= iDecimalBase;
		iNumLen++;
	}
	if (iNumLen == 0) {
		iNumLen++;
	}
	iLength = iNumLen;
	piNumber = new int[iLength];

	iAbs = std::abs(iValue);
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = (iAbs / (int)std::pow(iDecimalBase, i)) % iDecimalBase;
	}
	/*
	delete piNumber;
	bPositive = (iValue >= 0);
	int iAbs = std::abs(iValue);
	int iDecimal = iToDecimal(iAbs);
	int iNumLen = 1;
	while(iDecimal >= std::pow(iSystemBase, iNumLen)){
		iNumLen++;
	}
	iLength = iNumLen;
	piNumber = new int[iLength];
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = (iAbs / (int)std::pow(iDecimalBase, i)) % iDecimalBase;
	}*/
	//* this = cIntToNumber(iValue);
}

void CNumber::operator=(const CNumber& cValue)
{
	delete piNumber;
	bPositive = cValue.bPositive;
	iLength = cValue.iLength;
	piNumber = new int[iLength];
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = cValue.piNumber[i];
	}
}

CNumber CNumber::operator+(CNumber& cValue)
{
	CNumber* cResult;
	if (bPositive && cValue.bPositive) {
		cResult = cUnsignedAddition(*this, cValue);
	}
	else if(!bPositive && !cValue.bPositive){
		cResult = cUnsignedAddition(*this, cValue);
		cResult->bPositive = false;
	}
	else if(!bPositive){
		cResult = cUnsignedSubtraction(cValue, *this);
	}
	else {
		cResult = cUnsignedSubtraction(*this, cValue);
	}
	CNumber cCopy = *cResult;
	delete cResult;
	return cCopy;
}

CNumber CNumber::operator*(CNumber& cValue)
{
	int iMaxNumLen = iLength + cValue.iLength;
	CNumber cResult;
	cResult = 0;

	int** ppiResultMatrix = new int* [iLength];
	

	for (int i = 0; i < iLength; i++) {
		ppiResultMatrix[i] = new int[i + cValue.iLength + 1];
		for (int j = 0; j < i; j++) {
			ppiResultMatrix[i][j] = 0;
		}
		int iCarry = 0;
		int iSingleResult;
		for (int j = 0; j < cValue.iLength; j++) {
			iSingleResult = cValue.piNumber[j] * piNumber[i] + iCarry;
			iCarry = iSingleResult / 10;
			ppiResultMatrix[i][j + i] = iSingleResult % 10;
		}
		ppiResultMatrix[i][i + cValue.iLength] = iCarry;
		CNumber cRow = CNumber(ppiResultMatrix[i], i + cValue.iLength + 1);
		cResult = cResult + cRow;
	}
	
	cResult.bPositive = ((bPositive && cValue.bPositive) || (!bPositive && !cValue.bPositive));

	vRemoveLeadingZeros(cResult);

	return cResult;
}

CNumber CNumber::operator-(CNumber& cValue)
{
	CNumber* cResult;
	if (bPositive && cValue.bPositive) {
		cResult = cUnsignedSubtraction(*this, cValue);
	}
	else if (!bPositive && !cValue.bPositive) {
		cResult = cUnsignedSubtraction(*this, cValue);
		cResult->bPositive = false;
	}
	else if (!bPositive) {
		cResult = cUnsignedAddition(*this, cValue);
		cResult->bPositive = false;
	}
	else {
		cResult = cUnsignedAddition(*this, cValue);
	}
	CNumber cCopy = *cResult;
	delete cResult;
	return cCopy;
}

CNumber CNumber::operator/(CNumber& cValue)
{
	CNumber* cPartial = new CNumber();
	CNumber cAbs = abs(*this);
	CNumber* cResult = new CNumber();
	while (*cPartial <= cAbs) {
		cPartial = cUnsignedAddition(*cPartial, cValue);
		*cResult = *cResult + 1;
	}
	*cResult = *cResult - 1;
	cResult->bPositive = ((bPositive && cValue.bPositive) || (!bPositive && !cValue.bPositive));
	CNumber cReturn = *cResult;
	delete cResult;
	return cReturn;
}

CNumber CNumber::operator+=(int iValue)
{
	return *this + iValue;
}

CNumber CNumber::operator-()
{
	CNumber cResult = *this;
	cResult.bPositive = !cResult.bPositive;
	return cResult;
}

bool CNumber::operator<=(CNumber& cValue)
{
	
	return (*this < cValue) || (*this == cValue);
}

bool CNumber::operator<(CNumber& cValue)
{
	bool bResult = false;
	if (!this->bPositive && cValue.bPositive) {
		bResult = true;
	}
	else if (this->bPositive && !cValue.bPositive) {
		bResult = false;
	}
	else {
		if (this->iLength < cValue.iLength) {
			bResult = true;
		}
		else if (this->iLength > cValue.iLength) {
			bResult = false;
		}
		else {
			bool bDifferenceFound = false;
			int it = 1;
			int iLhs, iRhs;
			while (!bDifferenceFound && it <= this->iLength) {
				iLhs = this->piNumber[this->iLength - it];
				iRhs = cValue.piNumber[cValue.iLength - it];
				if (iLhs != iRhs) {
					bDifferenceFound = true;
					bResult = iLhs < iRhs;
				}
				else {
					it++;
				}
			}
		}
		if (!this->bPositive && !cValue.bPositive) {
			bResult = !bResult;
		}
	}
	return bResult;
}

bool CNumber::operator==(CNumber& cValue)
{
	bool bResult;
	if (this->iLength != cValue.iLength) {
		bResult = false;
	}else if(this->bPositive + cValue.bPositive == 1){
		bResult = false;
	}
	else {
		bool bDifferenceFound = false;
		int it = 1;
		int iLhs, iRhs;
		while (!bDifferenceFound && it <= this->iLength) {
			iLhs = this->piNumber[this->iLength - it];
			iRhs = cValue.piNumber[cValue.iLength - it];
			if (iLhs != iRhs) {
				bDifferenceFound = true;
				bResult = false;
			}
			else {
				it++;
			}
		}
		if (!bDifferenceFound) {
			bResult = true;
		}
	}
	return bResult;
}



CNumber CNumber::operator+(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this + cConvertedValue;
}

CNumber CNumber::operator*(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this * cConvertedValue;
}

CNumber CNumber::operator-(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this - cConvertedValue;
}

CNumber CNumber::operator/(int iValue)
{
	CNumber cConvertedValue;
	cConvertedValue = iValue;
	return *this / cConvertedValue;
}

CNumber CNumber::abs(CNumber& cValue)
{
	CNumber cReturn = cValue;
	cReturn.bPositive = true;
	return cReturn;
}

std::string CNumber::sToString()
{
	std::ostringstream ossResult;
	if (!bPositive) {
		ossResult << "- ";
	}
	for (int i = iLength - 1; i >= 0; i--) {
		ossResult<<piNumber[i];
	}
	return ossResult.str();
}

CNumber::CNumber(int* piNumber, int iLength)
{
	this->bPositive = true;
	this->piNumber = piNumber;
	this->iLength = iLength;
}

CNumber* CNumber::cUnsignedAddition(CNumber& cLValue, CNumber& cRValue)
{
	int iLenToTraverse = std::min(cLValue.iLength, cRValue.iLength);
	int iMinNumLen = std::max(cLValue.iLength, cRValue.iLength);

	CNumber* cResult = new CNumber();
	cResult->iLength = iMinNumLen;
	delete cResult->piNumber;
	cResult->piNumber = new int[iMinNumLen];

	int iCarry = 0;
	int iDigitResult = 0;
	for (int i = 0; i < iLenToTraverse; i++) {
		iDigitResult = (iCarry + cLValue.piNumber[i] + cRValue.piNumber[i]);
		cResult->piNumber[i] = iDigitResult % iSystemBase;
		iCarry = iDigitResult / iSystemBase;
	}
	int* piLongerNum = (cLValue.iLength > cRValue.iLength) ? cLValue.piNumber : cRValue.piNumber;

	for (int i = iLenToTraverse; i < iMinNumLen; i++) {
		iDigitResult = iCarry + piLongerNum[i];
		cResult->piNumber[i] = iDigitResult % iSystemBase;
		iCarry = iDigitResult / iSystemBase;
	}

	if (iCarry != 0) {
		int* piTemp = cResult->piNumber;
		cResult->piNumber = new int[++cResult->iLength];
		for (int i = 0; i < iMinNumLen; i++) {
			cResult->piNumber[i] = piTemp[i];
		}
		cResult->piNumber[iMinNumLen] = iCarry;
		delete piTemp;
	}

	return cResult;
}

CNumber* CNumber::cUnsignedSubtraction(CNumber& cLValue, CNumber& cRValue)
{
	int iLenToTraverse = std::min(cLValue.iLength, cRValue.iLength);
	int iMaxNumLen = std::max(cLValue.iLength, cRValue.iLength);

	int* piLhsSubRhs = new int[iMaxNumLen];
	int* piRhsSubLhs = new int[iMaxNumLen];
	int iCarryL = 0;
	int iCarryR = 0;

	bExecuteSubtractionLoop(0, iLenToTraverse, cLValue.piNumber, cRValue.piNumber, piLhsSubRhs, piRhsSubLhs, &iCarryL, &iCarryR);

	CNumber* cResult = new CNumber();
	cResult->iLength = iMaxNumLen;
	delete cResult->piNumber;
	cResult->piNumber = (iCarryL == 0) ? piLhsSubRhs : piRhsSubLhs;


	if (bExecuteSubtractionLoop(iLenToTraverse, cRValue.iLength, NULL, cRValue.piNumber, NULL, piRhsSubLhs, NULL, &iCarryR)) {
		cResult->piNumber = piRhsSubLhs;
	}

	if (bExecuteSubtractionLoop(iLenToTraverse, cLValue.iLength, cLValue.piNumber, NULL, piLhsSubRhs, NULL, &iCarryL, NULL)) {
		cResult->piNumber = piLhsSubRhs;
	}

	if (cResult->piNumber == piRhsSubLhs) {
		delete piLhsSubRhs;
		cResult->bPositive = false;
	}
	else {
		delete piRhsSubLhs;
	}

	vRemoveLeadingZeros(*cResult);

	return cResult;
}

bool CNumber::bExecuteSubtractionLoop(int iStart, int iStop, const int* piLhs, const int* piRhs, int* piLtR, int* piRtL, int *iCarryL, int *iCarryR) {
	int iEnteredLoop = false;

	for (int i = iStart; i < iStop; i++) {
		if (!iEnteredLoop) {
			iEnteredLoop = true;
		}
		if (piLtR != NULL) {
			piLtR[i] = iGetResultingDigit(i, piLhs, piRhs, iCarryL);
		}

		if (piRtL != NULL) {
			piRtL[i] = iGetResultingDigit(i, piRhs, piLhs, iCarryR);
		}
	
	}
	return iEnteredLoop;
}

 void CNumber::vRemoveLeadingZeros(CNumber& cValue)
{

	bool bNonZero = false;
	int iZeroCounter = 0;
	while (!bNonZero && iZeroCounter < cValue.iLength - 1) {
		if (cValue.piNumber[cValue.iLength - iZeroCounter - 1] != 0) {
			bNonZero = true;
		}
		else {
			iZeroCounter++;
		}
	}
	if (iZeroCounter > 0) {
		int* piWithoutZeros = new int[cValue.iLength - iZeroCounter];
		std::copy(cValue.piNumber, cValue.piNumber + (cValue.iLength - iZeroCounter), piWithoutZeros);
		delete cValue.piNumber;
		cValue.piNumber = piWithoutZeros;
		cValue.iLength -= iZeroCounter;
	}

}

 int CNumber::iToDecimal(int iValue)
 {
	int iTemp = iValue;
	int iResult = 0;
	int base = 1;  

	while (iTemp > 0) {
		int lastDigit = iTemp % 10;  
		iResult += lastDigit * base;  
		iTemp /= 10;  
		base *= iSystemBase;  
	}

	return iResult;
	 
 }


int CNumber::iGetResultingDigit(int iIndex, const int* piLhs, const int* piRhs, int* iCarry) {
	int iResult = 0;

	if (piRhs == NULL) {
		iResult = (piLhs[iIndex] - *iCarry);
	}
	else {
		iResult = (piLhs[iIndex] - *iCarry - piRhs[iIndex]);
	}

	if (iResult < 0) {
		*iCarry = 1;
		iResult = iSystemBase + iResult;
	}
	else {
		*iCarry = 0;
	}

	return iResult;
}
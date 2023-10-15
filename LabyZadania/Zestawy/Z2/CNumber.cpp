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
	
	CNumber cResult = CNumber();
	cResult.iLength = iMaxNumLen;
	cResult.piNumber = new int[iMaxNumLen];

	int iCarry = 0;
	int iDigitResult = 0;
	for (int i = 0; i < iLenToTraverse; i++) {
		iDigitResult = (piNumber[i] - iCarry - cValue.piNumber[i]);
		if (iDigitResult < 0) {
			iCarry = 1;
			iDigitResult = iDecimalSystemBase + iDigitResult;
		}
		else {
			iCarry = 0;
		}
		cResult.piNumber[i] = iDigitResult;
	}

	int* piLongerNum = (iLength > cValue.iLength) ? piNumber : cValue.piNumber;
	int iIter = iLenToTraverse;
	while (iCarry != 0) {
		iDigitResult = (piLongerNum[iIter] - iCarry );
		if (iDigitResult < 0) {
			iCarry = 1;
			iDigitResult = iDecimalSystemBase + iDigitResult;
		}
		else {
			iCarry = 0;
		}
		cResult.piNumber[iIter] = iDigitResult;
		iIter++;
	}

	while (iIter < iMaxNumLen) {
		cResult.piNumber[iIter] = piLongerNum[iIter];
		iIter++;
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
	return CNumber();
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

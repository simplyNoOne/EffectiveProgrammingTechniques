#include "CNumber.h"

#include <cmath>
//#include <algorithm>


CNumber::CNumber(){
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

CNumber::CNumber(const CNumber& cValue){
	piNumber = new int[iDefualtNumberLength];
	*this = cValue;

	//std::cout << "COPY\n";
}
CNumber::CNumber(const std::string& sValue)
{
	piNumber = new int[iDefualtNumberLength];
	*this = sValue;
}
/*
CNumber::CNumber(CNumber&& cValue) noexcept
{
	bPositive = cValue.bPositive;
	iLength = cValue.iLength;
	piNumber = cValue.piNumber;
	cValue.piNumber = NULL;
	std::cout << "MOVE\n";
}
*/
CNumber::CNumber(int* piNumber, int iLength){
	this->bPositive = true;
	this->piNumber = piNumber;
	this->iLength = iLength;
}

//wip
void CNumber::operator=(const std::string& sValue)
{
}

void CNumber::operator=(const int iValue){
	delete piNumber;	//as the defualt constructor already dynamically allocates an array, gotta ensure there is no memory leak

	bPositive = (iValue >= 0);
	int iAbs = std::abs(iValue);
	int iNumLen = 1;
	while (iAbs >= 10) {
		iAbs /= iDecimalBase;
		iNumLen++;
	}
	iLength = iNumLen;
	piNumber = new int[iLength];

	iAbs = std::abs(iValue);
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = (iAbs / (int)std::pow(iDecimalBase, i)) % iDecimalBase;
	}
}

void CNumber::operator=(const CNumber& cValue){
	delete piNumber;
	bPositive = cValue.bPositive;
	iLength = cValue.iLength;
	piNumber = new int[iLength];
	for (int i = 0; i < iLength; i++) {
		piNumber[i] = cValue.piNumber[i];
	}
}
/*
void CNumber::operator=(CNumber&& cValue) {
	delete piNumber;
	iLength = cValue.iLength;
	bPositive = cValue.bPositive;
	piNumber = cValue.piNumber;
	cValue.piNumber = NULL;
}*/

CNumber CNumber::operator+(const CNumber& cValue)const{
	CNumber* cResult;
	if (this->bPositive + cValue.bPositive != 1) {
		cResult = cUnsignedAddition(*this, cValue);
	}
	else {
		cResult = cUnsignedSubtraction(*this, cValue);
	}
	cResult->bPositive = this->bPositive;
	//CNumber cCopy = std::move(*cResult);
	CNumber cCopy = *cResult;
	delete cResult;
	return cCopy;
}

CNumber CNumber::operator*(const CNumber& cValue)const{
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
	
	cResult.bPositive = (bPositive + cValue.bPositive != 1);

	cResult.vRemoveLeadingZeros();

	return cResult;
}

CNumber CNumber::operator-(const CNumber& cValue)const{
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
	//CNumber cCopy = std::move(*cResult);
	delete cResult;
	return cCopy;
}
/*
CNumber CNumber::operator/(const CNumber& cValue){
	CNumber* cPartial = new CNumber();
	CNumber cAbs = abs(*this);
	CNumber cResult;
	cResult = 0;
	while (*cPartial <= cAbs) {
		cPartial = cUnsignedAddition(*cPartial, cValue);
		cResult = cResult + 1;
	}
	cResult = cResult - 1;
	cResult.bPositive = bPositive + cValue.bPositive != 1;
	return cResult;
}*/

CNumber CNumber::operator/(const CNumber& cValue) const{
	CNumber cDividend = abs(*this);
	CNumber cDiv = abs(cValue);
	if (cDividend < cDiv) {
		return CNumber();
	}
	int iLenDiff = this->iLength - cValue.iLength;
	int* piResult = new int[iLenDiff + 1];
	std::fill(piResult, piResult + iLenDiff + 1, 0);
	CNumber cCurrentDiv;
	bool bKeepDividing = true;
	while (bKeepDividing && iLenDiff >= 0) {
		if (cDividend < cDiv * std::pow(iSystemBase, iLenDiff)) {
			if (iLenDiff == 0) {
				bKeepDividing = false;
			}
			iLenDiff--;
		}
		if (bKeepDividing) {
			cCurrentDiv = cDiv * std::pow(iSystemBase, iLenDiff);
			piResult[iLenDiff] = cDividend.iGetResultingDivDigit(cCurrentDiv);
			cDividend -= cCurrentDiv * piResult[iLenDiff];
		}
	}
	CNumber cResult(piResult, this->iLength - cValue.iLength + 1);
	cResult.vRemoveLeadingZeros();
	cResult.bPositive = bPositive + cValue.bPositive != 1;
	return cResult;
}


CNumber CNumber::operator+=(const CNumber& cValue)
{
	*this = *this + cValue;
	return *this;
}

CNumber CNumber::operator-=(const CNumber& cValue)
{
	*this = *this - cValue;
	return *this;
}

CNumber CNumber::operator*=(const CNumber& cValue)
{
	*this = *this * cValue;
	return *this;
}

CNumber CNumber::operator/=(const CNumber& cValue)
{
	*this = *this / cValue;
	return *this;
}

CNumber CNumber::operator-(){
	//CNumber cResult = std::move(*this);
	CNumber cResult = *this;
	cResult.bPositive = !cResult.bPositive;
	return cResult;
}

bool CNumber::operator<=(const CNumber& cValue)const{
	return (*this < cValue) || (*this == cValue);
}

bool CNumber::operator<(const CNumber& cValue)const{
	bool bResult = false;
	if (this->bPositive + cValue.bPositive == 1) {
		bResult = !this->bPositive;
	}
	else {
		if (this->iLength != cValue.iLength) {
			bResult = this->iLength < cValue.iLength;
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
		if (!this->bPositive) {
			bResult = !bResult;
		}
	}
	return bResult;
}

bool CNumber::operator>=(const CNumber& cValue) const
{
	return cValue <= *this;
}

bool CNumber::operator>(const CNumber& cValue) const
{
	return cValue < *this;
}

bool CNumber::operator==(const CNumber& cValue)const{
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


CNumber CNumber::abs(const CNumber& cValue){
	//CNumber cReturn = std::move(cValue);
	CNumber cReturn = cValue;
	cReturn.bPositive = true;
	return cReturn;
}

std::string CNumber::sToString() const{
	std::ostringstream ossResult;
	if (!bPositive) {
		ossResult << "- ";
	}
	for (int i = iLength - 1; i >= 0; i--) {
		ossResult<<piNumber[i];
	}
	return ossResult.str();
}


CNumber* CNumber::cUnsignedAddition(const CNumber& cLValue,const CNumber& cRValue)
{
	int iMaxNumLen = std::max(cLValue.iLength, cRValue.iLength) + 1;

	CNumber* cResult = new CNumber();
	cResult->iLength = iMaxNumLen;
	delete cResult->piNumber;
	cResult->piNumber = new int[iMaxNumLen];

	int iCarry = 0;
	int iDigitResult = 0;
	int iLNum, iRNum = 0;
	for (int i = 0; i < iMaxNumLen; i++) {
		iLNum  = cLValue.iLength <= i ? 0 : cLValue.piNumber[i];
		iRNum = cRValue.iLength <= i ? 0 : cRValue.piNumber[i];
		iDigitResult = (iCarry + iLNum + iRNum);
		cResult->piNumber[i] = iDigitResult % iSystemBase;
		iCarry = iDigitResult / iSystemBase;
	}
	
	cResult->vRemoveLeadingZeros();

	return cResult;
}



CNumber* CNumber::cUnsignedSubtraction(const CNumber& cLValue, const CNumber& cRValue)
{
	int iMaxNumLen = std::max(cLValue.iLength, cRValue.iLength);
	int* piSub= new int[iMaxNumLen];

	int iBorrow = 0;
	int iDigitResult = 0;
	int iLNum, iRNum = 0;
	for (int i = 0; i < iMaxNumLen; i++) {
		iLNum = cLValue.iLength <= i ? 0 : cLValue.piNumber[i];
		iRNum = cRValue.iLength <= i ? 0 : cRValue.piNumber[i];
		iDigitResult = (iLNum -iBorrow - iRNum);
		if (iDigitResult < 0) {
			iDigitResult = iDigitResult + iSystemBase;
			iBorrow = 1;
		}
		else {
			iBorrow = 0;
		}
		piSub[i] = iDigitResult;
	}

	CNumber* cResult = new CNumber(piSub, iMaxNumLen);

	if (iBorrow != 0) {
		cResult->bPositive = false;
		cResult->vFlipToComplement();
	}

	cResult->vRemoveLeadingZeros();
	return cResult;
}

 void CNumber::vRemoveLeadingZeros(){

	bool bNonZero = false;
	int iZeroCounter = 0;
	while (!bNonZero && iZeroCounter < this->iLength - 1) {
		if (this->piNumber[this->iLength - iZeroCounter - 1] != 0) {
			bNonZero = true;
		}
		else {
			iZeroCounter++;
		}
	}
	if (iZeroCounter > 0) {
		int* piWithoutZeros = new int[this->iLength - iZeroCounter];
		std::copy(this->piNumber, this->piNumber + (this->iLength - iZeroCounter), piWithoutZeros);
		delete this->piNumber;
		this->piNumber = piWithoutZeros;
		this->iLength -= iZeroCounter;
	}
}

 int CNumber::iGetResultingDivDigit( CNumber& cDiv)
 {
	 int i = 1;
	 bool found = false;
	 while (!found) {
		 if (*this < cDiv * i) {
			 found = true;
			 i--;
		 }
		 else {
			 i++;
		 }
	 }
	 return i;
 }

 void CNumber::vFlipToComplement() {
	 int* piComplement = new int[this->iLength];
	 for (int i = 0; i < this->iLength; i++) {
		 piComplement[i] = iSystemBase - 1 - this->piNumber[i];
	 }
	 piComplement[0]++;
	 delete this->piNumber;
	 this->piNumber = piComplement;
 }

 std::ostream& operator<<(std::ostream& os, const CNumber& cValue)
 {
	 os << cValue.sToString();
	 return os;
 }

 //wip
 std::istream& operator>>(std::istream& is, const CNumber& cValue)
 {
	 std::string s;
	 std::getline(is, s);
//	 std::to_int
	 return is;
 }

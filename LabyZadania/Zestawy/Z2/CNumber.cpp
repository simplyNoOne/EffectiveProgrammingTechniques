#include "CNumber.h"

#include <cmath>

int CNumber::iSystemBase = 10;
std::vector<std::string>* CNumber::vDict = NULL;

//public section
std::ostream& operator<<(std::ostream& os, const CNumber& cValue) {
	os << cValue.sToString();
	return os;
}

CNumber::CNumber(){
	iLength = iDefualtNumberLength;
	piNumber = new int[iLength];
	piNumber[0] = 0;
	bPositive = true;
};

CNumber::CNumber(const int iValue) {
	piNumber = new int[iDefualtNumberLength];
	*this = iValue;
}

CNumber::CNumber(const std::string& sValue) {
	piNumber = new int[iDefualtNumberLength];
	*this = sValue;
}

CNumber::CNumber(const CNumber& cValue){
	piNumber = new int[iDefualtNumberLength];
	*this = cValue;
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

void CNumber::operator=(const int iValue) {
	delete piNumber;

	bPositive = (iValue >= 0);
	//int iToConvert = std::abs(iValue);
	int iToConvert = std::abs(iValue);
	//if (iToConvert < 0) {
	//	iToConvert = -iToConvert;
	//}
	this->iLength = iGetLenFromDecimal(iToConvert);
	this->piNumber = new int[this->iLength];
	int iIndex = 0;
	while (iIndex < this->iLength) {
		piNumber[iIndex] = iToConvert % iSystemBase;
		iToConvert /= iSystemBase;
		iIndex++;
	}
}

void CNumber::operator=(const std::string& sValue) {
	delete this->piNumber;

	if (iSystemBase <= iDecimalBase) {
		vNumFromStringNoSeparator(sValue);
	}
	else {
		vNumFromStringWithSeparator(sValue);
	}
	vRemoveLeadingZeros();
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

CNumber CNumber::operator-() const {
	//CNumber cResult = std::move(*this);
	if (*this == 0) {
		return *this;
	}
	CNumber cResult = *this;
	cResult.bPositive = !cResult.bPositive;
	return cResult;
}

CNumber CNumber::operator+(const CNumber& cValue)const{
	CNumber cResult;
	if (this->bPositive + cValue.bPositive != 1) {
		cResult = cUnsignedAddition(*this, cValue);
		cResult.bPositive = this->bPositive;
	}
	else {
		cResult = cUnsignedSubtraction(*this, cValue);
		if (!this->bPositive) {
			cResult.bPositive = !cResult.bPositive;
		}
		if (abs(cResult) == 0) {
			cResult.bPositive = true;
		}
	}
	return cResult;
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
			iCarry = iSingleResult / iSystemBase;
			ppiResultMatrix[i][j + i] = iSingleResult % iSystemBase;
		}
		ppiResultMatrix[i][i + cValue.iLength] = iCarry;
		CNumber cRow = CNumber(ppiResultMatrix[i], i + cValue.iLength + 1);
		cResult = cResult + cRow;
	}
	cResult.vRemoveLeadingZeros();
	cResult.bPositive = (this->bPositive + cValue.bPositive != 1);
	if (abs(cResult) == 0) {
		cResult.bPositive = true;
	}
	return cResult;
}

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
		if (cDividend < cDiv * pow(iSystemBase, iLenDiff)) {
			if (iLenDiff == 0) {
				bKeepDividing = false;
			}
			iLenDiff--;
		}
		if (bKeepDividing) {
			cCurrentDiv = cDiv * pow(iSystemBase, iLenDiff);
			piResult[iLenDiff] = cDividend.iGetResultingDivDigit(cCurrentDiv);
			cDividend -= cCurrentDiv * piResult[iLenDiff];
		}
	}
	CNumber cResult(piResult, this->iLength - cValue.iLength + 1);
	cResult.vRemoveLeadingZeros();
	cResult.bPositive = this->bPositive + cValue.bPositive != 1;
	if (abs(cResult) == 0) {
		cResult.bPositive = true;
	}
	return cResult;
}

CNumber CNumber::operator%(const CNumber& cValue) const
{
	return abs(*this) - abs((*this/cValue) * cValue);
}

CNumber CNumber::operator+=(const CNumber& cValue){
	*this = *this + cValue;
	return *this;
}

CNumber CNumber::operator-=(const CNumber& cValue){
	*this = *this - cValue;
	return *this;
}

CNumber CNumber::operator*=(const CNumber& cValue){
	*this = *this * cValue;
	return *this;
}

CNumber CNumber::operator/=(const CNumber& cValue){
	*this = *this / cValue;
	return *this;
}

bool CNumber::operator==(const CNumber& cValue)const {
	if (this->iLength != cValue.iLength) {
		return false;
	}
	else if (this->bPositive + cValue.bPositive == 1) {
		return false;
	}
	else {
		int it = 1;
		int iLhs, iRhs;
		while (it <= this->iLength) {
			iLhs = this->piNumber[this->iLength - it];
			iRhs = cValue.piNumber[cValue.iLength - it];
			if (iLhs != iRhs) {
				return false;
			}
			it++;
		}
		return true;
	}
}

bool CNumber::operator<(const CNumber& cValue)const{
	bool bResult = false;
	if (this->bPositive + cValue.bPositive == 1) {
		return !this->bPositive;
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

std::string CNumber::sToString() const {
	std::ostringstream ossResult;
	if (!bPositive) {
		ossResult << "- ";
	}
	if (iSystemBase <= iDecimalBase) {
		for (int i = iLength - 1; i >= 0; i--) {
			ossResult << piNumber[i];
		}
	}
	else {
		for (int i = iLength - 1; i >= 0; i--) {
			ossResult << vDict->at(piNumber[i]);
		}
	}
	return ossResult.str();
}

CNumber CNumber::abs(const CNumber& cValue){
	//CNumber cReturn = std::move(cValue);
	CNumber cReturn = cValue;
	cReturn.bPositive = true;
	return cReturn;
}

CNumber CNumber::pow(const CNumber& cValue, int iExp)
{
	CNumber cResult = 1;
	CNumber cBase = cValue;
	while (iExp != 0){
		if (iExp % 2 == 1) {
			cResult *= cBase;
		}
		iExp/= 2;
		cBase *= cBase;
	}
	return cResult;
}

void CNumber::vSetSystemBase(int iBase) {
	if (vDict != NULL) {
		delete vDict;
		vDict = NULL;
	}
	iSystemBase = iBase;
	if (iSystemBase > iDecimalBase) {
		vInitDictionary();
	}
}

CNumber CNumber::cDivRest(const CNumber& cOther, CNumber* pcRest)
{
	if (*this % cOther == 0) {
		pcRest = NULL;
	}
	else {
		*pcRest = *this % cOther;
	}
	return *this / cOther;
}

//private section

CNumber::CNumber(int* piNumber, int iLength) {
	this->bPositive = true;
	this->piNumber = piNumber;
	this->iLength = iLength;
}

void CNumber::vInitDictionary() {
	vDict = new std::vector<std::string>();
	for (int i = 0; i < iSystemBase; i++) {
		vDict->push_back(std::to_string(i) + " ");
	}
}

CNumber CNumber::cUnsignedAddition(const CNumber& cLValue,const CNumber& cRValue){
	int iMaxNumLen = std::max(cLValue.iLength, cRValue.iLength) + 1;

	CNumber cResult;
	cResult.iLength = iMaxNumLen;
	delete cResult.piNumber;
	cResult.piNumber = new int[iMaxNumLen];

	int iCarry = 0;
	int iDigitResult = 0;
	int iLNum, iRNum = 0;
	for (int i = 0; i < iMaxNumLen; i++) {
		iLNum  = cLValue.iLength <= i ? 0 : cLValue.piNumber[i];
		iRNum = cRValue.iLength <= i ? 0 : cRValue.piNumber[i];
		iDigitResult = (iCarry + iLNum + iRNum);
		cResult.piNumber[i] = iDigitResult % iSystemBase;
		iCarry = iDigitResult / iSystemBase;
	}
	
	cResult.vRemoveLeadingZeros();
	return cResult;
}

CNumber CNumber::cUnsignedSubtraction(const CNumber& cLValue, const CNumber& cRValue)
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
	CNumber cResult(piSub, iMaxNumLen);

	if (iBorrow != 0) {
		cResult.vFlipToComplement();
		cResult.bPositive = false;
	}
	cResult.vRemoveLeadingZeros();
	return cResult;
}

 void CNumber::vFlipToComplement() {
	 int* piComplement = new int[this->iLength];
	 for (int i = 0; i < this->iLength; i++) {
		 piComplement[i] = iSystemBase - 1 - this->piNumber[i];
	 }
	 delete this->piNumber;
	 this->piNumber = piComplement;
	 *this = cUnsignedAddition(*this, CNumber(1));
 }

 void CNumber::vRemoveLeadingZeros() {
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

 int CNumber::iGetResultingDivDigit(const CNumber& cDiv) {
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

 int CNumber::iGetLenFromDecimal(int iValue) {
	 if (iValue == 0) {
		 return 1;
	 }
	 return 1 + (int)(log2(iValue) / log2(iSystemBase));
 }

 void CNumber::vNumFromStringNoSeparator(std::string sValue) {
	 int iOffset = 0;
	 int iPos = 0;
	 if (sValue[0] == '-') {
		 iOffset = 1;
		 this->bPositive = false;
	 }
	 else {
		 this->bPositive = true;
	 }
	 this->iLength = sValue.length() - iOffset;
	 this->piNumber = new int[this->iLength];

	 while (iPos + iOffset != sValue.length()) {
		 if (!std::isdigit(sValue[iPos + iOffset])) {
			 *this = 0;
			 return;
		 }
		 this->piNumber[this->iLength - iPos - 1] = sValue[iPos + iOffset] - '0';
		 iPos++;
	 }
 }

 void CNumber::vNumFromStringWithSeparator(std::string sValue) {
	 std::stringstream sStream(sValue);
	 std::string part;
	 int count = 0;
	 this->bPositive = sValue[0] != '-';
	 while (getline(sStream, part, ' ')) {
		 count++;
	 }
	 sStream.str("");
	 sStream.clear();
	 if (!this->bPositive) {
		 sStream << sValue.substr(1);
	 }
	 else {
		 sStream << sValue;
	 }
	 this->iLength = count;
	 this->piNumber = new int[count];
	 count = 0;
	 while (count < this->iLength) {
		 getline(sStream, part, ' ');
		 try {
			 this->piNumber[this->iLength - 1 - count] = std::stoi(part);
		 }
		 catch (std::invalid_argument ex) {
			 *this = 0;
		 }
		 count++;
	 }
 }


 
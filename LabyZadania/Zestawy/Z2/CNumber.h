#pragma once
#include <sstream>	
#include <vector>

const int iDefualtNumberLength = 1;
const int iDecimalBase = 10;


class CNumber
{
public:
	friend std::ostream& operator<<(std::ostream& os, const CNumber& cValue);

	//so that the order is interchangable
	//int and CNumber
	inline friend CNumber operator+(int iValue, const CNumber& cValue) { return cValue + iValue; }
	inline friend CNumber operator-(int iValue, const CNumber& cValue) { return CNumber(iValue) - cValue; }
	inline friend CNumber operator*(int iValue, const CNumber& cValue) { return cValue * iValue; }
	inline friend CNumber operator/(int iValue, const CNumber& cValue) { return CNumber(iValue) / cValue; }
	inline friend CNumber operator%(int iValue, const CNumber& cValue) { return CNumber(iValue) % cValue; }

	inline friend bool operator==(int iValue, const CNumber& cValue) { return  cValue == iValue; }
	inline friend bool operator<=(int iValue, const CNumber& cValue) { return cValue >= iValue; }
	inline friend bool operator<(int iValue, const CNumber& cValue) { return cValue > iValue; }
	inline friend bool operator>=(int iValue, const CNumber& cValue) { return cValue <= iValue; }
	inline friend bool operator>(int iValue, const CNumber& cValue) { return cValue < iValue; }


	//Constructors 
	CNumber();
	CNumber(int iValue);
	CNumber(const std::string& sValue);
	CNumber(const CNumber& cValue);
	//CNumber(CNumber&& cValue) noexcept;
	inline ~CNumber() { if(piNumber != NULL) delete piNumber; }

	//Assignments
	void operator=(const int iValue);
	void operator=(const std::string& sValue);
	void operator=(const CNumber& cValue);
	//void operator=(CNumber&& cValue);

	//Negation
	CNumber operator-()const;

	//CNumber and CNumber
	CNumber operator+(const CNumber& cValue)const;
	inline CNumber operator-(const CNumber& cValue)const { return *this + (-cValue); }
	CNumber operator*(const CNumber& cValue)const;
	CNumber operator/(const CNumber& cValue)const;
	CNumber operator%(const CNumber& cValue)const;


	CNumber operator+=(const CNumber& cValue);
	CNumber operator-=(const CNumber& cValue);
	CNumber operator*=(const CNumber& cValue);
	CNumber operator/=(const CNumber& cValue);

	bool operator==(const CNumber& cValue)const;
	inline bool operator<=(const CNumber& cValue)const { return (*this < cValue) || (*this == cValue); }
	bool operator<(const CNumber& cValue)const;
	inline bool operator>=(const CNumber& cValue)const { return cValue <= *this; }
	inline bool operator>(const CNumber& cValue)const { return cValue < *this; }

	//CNumber and int
	inline CNumber operator+(int iValue) const { return *this + CNumber(iValue); }
	inline CNumber operator-(int iValue) const { return *this - CNumber(iValue); }
	inline CNumber operator*(int iValue) const { return *this * CNumber(iValue); }
	inline CNumber operator/(int iValue) const { return *this / CNumber(iValue); }
	inline CNumber operator%(int iValue) const { return *this % CNumber(iValue); }

	inline CNumber operator+=(int iValue) { return *this += CNumber(iValue); }
	inline CNumber operator-=(int iValue) { return *this -= CNumber(iValue); }
	inline CNumber operator*=(int iValue) { return *this *= CNumber(iValue); }
	inline CNumber operator/=(int iValue) { return *this /= CNumber(iValue); }

	inline bool operator==(int iValue)const { return *this == CNumber(iValue); }
	inline bool operator<=(int iValue)const { return *this <= CNumber(iValue); }
	inline bool operator<(int iValue)const { return *this < CNumber(iValue); }
	inline bool operator>=(int iValue)const { return *this >= CNumber(iValue); }
	inline bool operator>(int iValue)const { return *this > CNumber(iValue); }

	std::string sToString() const;

	static CNumber abs(const CNumber& cValue);
	static CNumber pow(const CNumber& cValue, int iExp);
	static void vSetSystemBase(int iBase);

	CNumber cDivRest(const CNumber& cOther, CNumber* pcRest);

private:
	int* piNumber;
	int iLength;
	bool bPositive;

	//statics
	static int iSystemBase;
	static std::vector<std::string>* vDict;
	

private:

	CNumber(int* piNumber, int iLength);

	static void vInitDictionary();

	/* Performs addition as if on two positive numbers*/
	static CNumber cUnsignedAddition(const CNumber &cLValue, const CNumber &cRValue);
	/* Performs subtraction as if on two positive numbers*/
	static CNumber cUnsignedSubtraction(const CNumber& cLValue,const CNumber& cRValue);

	void vFlipToComplement();
	void vRemoveLeadingZeros();
	int iGetResultingDivDigit(const CNumber &cDiv);
	int iGetLenFromDecimal(int iValue);
	void vNumFromStringNoSeparator(std::string sValue);
	void vNumFromStringWithSeparator(std::string sValue);


};
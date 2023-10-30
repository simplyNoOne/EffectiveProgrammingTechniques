#pragma once
#include <sstream>	//gotta get rid of this and create custom string, will be able to forward declare instead
#include <vector>
#include <string>
const int iDefualtNumberLength = 1;
static int iSystemBase = 13;
const int iDecimalBase = 10;


class CNumber
{
public:
	friend std::ostream& operator<<(std::ostream& os, const CNumber& cValue);
	//so that the order is interchangable
	//int and CNumber
	inline friend CNumber operator+(int iValue, const CNumber& cValue) { return cValue + iValue; }
	inline friend CNumber operator-(int iValue, const CNumber& cValue) { return cValue - iValue; }
	inline friend CNumber operator*(int iValue, const CNumber& cValue) { return cValue * iValue; }
	inline friend CNumber operator/(int iValue, const CNumber& cValue) { return cValue / iValue; }

public:
	CNumber();
	CNumber(const int iValue);
	CNumber(const CNumber& cValue);
	CNumber(const std::string& sValue);
	//CNumber(CNumber&& cValue) noexcept;
	inline ~CNumber() { if(piNumber != NULL) delete piNumber; }

	CNumber operator-();

	//CNumber and CNumber
	//void operator=(CNumber&& cValue);
	void operator=(const CNumber& cValue);
	CNumber operator+(const CNumber& cValue)const;
	CNumber operator-(const CNumber& cValue)const;
	CNumber operator*(const CNumber& cValue)const;
	CNumber operator/(const CNumber& cValue)const;

	CNumber operator+=(const CNumber& cValue);
	CNumber operator-=(const CNumber& cValue);
	CNumber operator*=(const CNumber& cValue);
	CNumber operator/=(const CNumber& cValue);

	bool operator==(const CNumber& cValue)const;
	bool operator<=(const CNumber& cValue)const;
	bool operator<(const CNumber& cValue)const;
	bool operator>=(const CNumber& cValue)const;
	bool operator>(const CNumber& cValue)const;

	//CNumber and int

	void operator=(const std::string& sValue);
	void operator=(const int iValue);
	inline CNumber operator+(int iValue) const { return *this + CNumber(iValue); }
	inline CNumber operator-(int iValue) const { return *this - CNumber(iValue); }
	inline CNumber operator*(int iValue) const { return *this * CNumber(iValue); }
	inline CNumber operator/(int iValue) const { return *this / CNumber(iValue); }

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
	static void vSetSystemBase(int iBase);

private:
	int* piNumber;
	int iLength;
	bool bPositive;
	static std::vector<std::string> vDict;
	

private:

	CNumber(int* piNumber, int iLength);

	/* Performs addition as if on two positive numbers*/
	static CNumber* cUnsignedAddition(const CNumber &cLValue, const CNumber &cRValue);
	/* Performs subtraction as if on two positive numbers*/
	static CNumber* cUnsignedSubtraction(const CNumber& cLValue,const CNumber& cRValue);

	void vFlipToComplement();
	void vRemoveLeadingZeros();
	int iGetResultingDivDigit( CNumber &cDiv);
	static void vInitDictionary();

	int iGetLenFromDecimal(int iValue);
	void vFromStringNoSeparator(std::string sValue);
	void vFromStringWithSeparator(std::string sValue);


	


};
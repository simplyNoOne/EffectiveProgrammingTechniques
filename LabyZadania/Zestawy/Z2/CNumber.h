#pragma once
#include <sstream>	//gotta get rid of this and create custom string, will be able to forward declare instead

const int iDefualtNumberLength = 1;
const int iSystemBase = 10;
const int iDecimalBase = 10;


class CNumber
{
public:
	CNumber();
	CNumber(const int iValue);
	CNumber(const CNumber& cValue);
	CNumber(CNumber&& cValue) noexcept;
	inline ~CNumber() {
		if(piNumber != NULL)
		delete piNumber; 
	}

	
	void operator=(const CNumber& cValue);
	CNumber operator+(const CNumber& cValue);
	CNumber operator*(const CNumber& cValue);
	CNumber operator-(const CNumber& cValue);
	CNumber operator/(const CNumber& cValue);

	CNumber operator+=(int iValue);

	CNumber operator-();

	bool operator<=(CNumber& cValue);
	bool operator<(CNumber& cValue);
	bool operator==(CNumber& cValue);

	void operator=(const int iValue);
	void operator=(CNumber&& cValue);
	CNumber operator+(int iValue);
	CNumber operator*(int iValue);
	CNumber operator-(int iValue);
	CNumber operator/(int iValue);


	static CNumber abs(CNumber& cValue);

	std::string sToString();

private:
	int* piNumber;
	int iLength;
	bool bPositive;

private:

	CNumber(int* piNumber, int iLength);

	/* Performs addition as if on two positive numbers*/
	static CNumber* cUnsignedAddition(const CNumber &cLValue, const CNumber &cRValue);
	/* Performs subtraction as if on two positive numbers*/
	static CNumber* cUnsignedSubtraction(const CNumber& cLValue,const CNumber& cRValue);

	void vRemoveLeadingZeros();

	void vFlipToComplement();


};
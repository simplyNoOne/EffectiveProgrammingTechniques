#pragma once
#include <sstream>	//gotta get rid of this and create custom string, will be able to forward declare instead

const int iDefualtNumberLength = 1;
const int iSystemBase = 3;
const int iDecimalBase = 10;


class CNumber
{
public:
	CNumber();
	CNumber(const int iValue);
	CNumber(const CNumber& cValue);
	inline ~CNumber() { delete piNumber; }

	
	void operator=(const CNumber& cValue);
	CNumber operator+(CNumber& cValue);
	CNumber operator*(CNumber& cValue);
	CNumber operator-(CNumber& cValue);
	CNumber operator/(CNumber& cValue);

	CNumber operator+=(int iValue);

	CNumber operator-();

	bool operator<=(CNumber& cValue);
	bool operator<(CNumber& cValue);
	bool operator==(CNumber& cValue);

	void operator=(const int iValue);
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
	static CNumber* cUnsignedAddition(CNumber &cLValue, CNumber &cRValue);
	/* Performs subtraction as if on two positive numbers*/
	static CNumber* cUnsignedSubtraction(CNumber& cLValue, CNumber& cRValue);

	/** Subtracts corresponding digits from 2 arrays; Returns true when loop was executed, false otherwise*/
	static bool  bExecuteSubtractionLoop(int iStart, int iStop, const int* piLhs, const int* piRhs, int* piLtR, int* piRtL, int* iCarryL, int* iCarryR);

	static int iGetResultingDigit(int iIndex, const int* piLhs, const int* piRhs, int* iCarry);

	static void vRemoveLeadingZeros(CNumber& cValue);

	static int iToDecimal(int iValue);


};
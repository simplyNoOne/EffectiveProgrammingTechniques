#pragma once
#include <sstream>	//gotta get rid of this and create custom string, will be able to forward declare instead

const int iDefualtNumberLength = 10;
const int iDecimalSystemBase = 10;


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

	void operator=(const int iValue);
	CNumber operator+(int iValue);
	CNumber operator*(int iValue);
	CNumber operator-(int iValue);
	CNumber operator/(int iValue);

	std::string sToString();

private:
	int* piNumber;
	int iLength;

	//edit this so that my eyes don't bleed
	/** Subtracts corresponding digits from 2 arrays; Returns true when loop was executed, false otherwise*/
	bool  bExecuteSubtractionLoop(int iStart, int iStop, const int* piLhs, const int* piRhs, int* piLtR, int* piRtL, int* iCarryL, int* iCarryR);

	int getResultingDigit(int iIndex, const int* piLhs, const int* piRhs, int* iCarry);

};
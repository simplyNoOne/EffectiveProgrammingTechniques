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
};
#pragma once

#include<vector>
#include <string>
#include "ETokens.h"

class CNode;
class CTree {

public:
	inline CTree() { pcTreeRoot = nullptr; }
	inline ~CTree() { delete pcTreeRoot; }

	void vParseFormula(std::string sFormula, std::string& sErrorMessage, char cSeparator);
	std::string sReturnFormula();
	std::string sReturnVariables();
	void vClearTree();
	std::string sCompute( std::string& sUserResponse, char cSeparator);
	bool bExists() { return pcTreeRoot != nullptr; }

	void operator=(const CTree& cOther);
	CTree operator+(const CTree& cOther)const;

private:
	E_ERROR_TYPE eParseVariableValues(std::string sInput, char cSeparator);
	CNode* pcTreeRoot;
	std::vector<std::string> vsVariables;
	std::vector<int> viVariableOccurences;
	std::vector<double> vdVariableValues;
};

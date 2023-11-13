#pragma once

#include<vector>
#include <string>
#include "ETokens.h"
#include "CVariablesData.h"

class CNode;
class CError;
//class CVariablesData;

class CTree {

public:
	inline CTree() { pcTreeRoot = nullptr; }
	inline ~CTree() { delete pcTreeRoot; }

	void vParseFormula(std::string sFormula, CError& cError, char cSeparator);
	std::string sReturnFormula();
	std::string sReturnVariables();
	void vClearTree();
	std::string sCompute( std::string& sUserResponse, char cSeparator, CError& cError);
	bool bExists() { return pcTreeRoot != nullptr; }

	void operator=(const CTree& cOther);
	CTree operator+(const CTree& cOther)const;

	CTree cReplaceNodeWNode(CNode* pcToRep, CNode* pcNew);

private:
	E_ERROR_TYPE eParseVariableValues(std::string sInput, char cSeparator);
	CNode* pcTreeRoot;
	CVariablesData cVaraiblesData;
};

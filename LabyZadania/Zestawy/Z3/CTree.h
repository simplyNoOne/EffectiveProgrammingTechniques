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
	inline CTree() { pcTreeRoot = nullptr; pcVariablesData = nullptr; }
	inline ~CTree() { delete pcTreeRoot; delete pcVariablesData; }

	void operator=(const CTree& cOther);
	CTree operator+(const CTree& cOther)const;

	void vParseFormula(std::string sFormula, CError& cError, char cSeparator);
	void vClearTree();

	std::string sReturnFormula();
	std::string sReturnVariables();
	std::string sCompute( std::string& sUserResponse, char cSeparator, CError& cError);
	
	/**Replaces one node with another otherwise maintaining the inner structure of nodes and returns the resulting tree*/
	CTree cReplaceNodeWNode(CNode* pcToRep, CNode* pcNew);

	inline bool bExists() { return pcTreeRoot != nullptr; }

private:
	CNode* pcTreeRoot;
	CVariablesData* pcVariablesData;

private:
	/**Tries to match values from input with variable list and returns appropriate error. On success - EET_NONE */
	E_ERROR_TYPE eParseVariableValues(std::string sInput, char cSeparator);
	/**Iterates through the entire tree and creates matching variable list*/
	void vVariablesDataFromTree();
};

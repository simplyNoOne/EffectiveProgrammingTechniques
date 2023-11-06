#pragma once

#include <vector>
#include <string>
#include "ETokens.h"

class CNode {
public:
	inline CNode(CNode* pcParent) { this->pcParent = pcParent; }
	~CNode();

	bool bParseNode(std::string sFormula, int& iOffset, char iSeparator, std::string& sErrorMessage);

private:
	CNode* pcParent;
	std::vector<CNode*> vpcChildren;
	E_OPERATION_TYPE eOperationType;
	E_NODE_TYPE eNodeType;
	std::string sVariableName;
	double dConstantValue;

	void vMakeDefualt();

	//static
	static char cSeparator;
	static std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> vOperationDefs;
	static E_NODE_TYPE eDetermineNodeType(std::string input, int &iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue);
	static bool bIsNum(std::string& sToCheck);

};
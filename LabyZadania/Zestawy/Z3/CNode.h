#pragma once

#include <vector>
#include <string>
#include "ETokens.h"

class CNode {
public:
	inline CNode(CNode* pcParent) { this->pcParent = pcParent; }
	CNode(CNode* pcParent, CNode& cOriginal);
	~CNode();

	bool bParseNode(std::string sFormula, int& iOffset, char cSeparator, std::string& sErrorMessage, E_ERROR_TYPE& eError, std::vector<std::string> &vVars, std::vector<int>&vVarCount);
	std::string sNodeRepresentation();
	
	double dEvaluateNode(std::vector<std::string>& vsVariableNames, std::vector<double>& vdVariableValues);
	static std::string sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset);
	static bool bIsNum(std::string& sToCheck);

	inline E_NODE_TYPE eGetNodeType() { return eNodeType; }

	void vAttachNodeToLastElement(CNode* pcOtherRoot, std::vector<std::string>& vVars, std::vector<int>& vVarCount);
private:
	CNode* pcParent;
	std::vector<CNode*> vpcChildren;
	E_OPERATION_TYPE eOperationType;
	E_NODE_TYPE eNodeType;
	std::string sVariableName;
	double dConstantValue;

	void vMakeDefualt();

	//static
	static std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> vOperationDefs;
	static E_NODE_TYPE eDetermineNodeType(std::string input, int &iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue, E_ERROR_TYPE& eError);
	
	double dCalculateOperation(std::vector<std::string>& vsVariableNames, std::vector<double>& vdVariableValues);

};
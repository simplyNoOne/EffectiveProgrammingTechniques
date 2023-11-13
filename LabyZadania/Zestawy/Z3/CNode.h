#pragma once

#include <vector>
#include <string>
#include "ETokens.h"

class CError;
class CVariablesData;

class CNode {
public:
	inline CNode(CNode* pcParent) { this->pcParent = pcParent; }
	CNode(CNode* pcParent, CNode& cOriginal);
	~CNode();

	void bParseNode(std::string sFormula, int& iOffset, char cSeparator, CError& cError, CVariablesData* cVariables);
	std::string sNodeRepresentation();
	
	inline CNode* pcGetParent() { return pcParent; }
	inline void vSetParent(CNode* pcNewParent) { pcParent = pcNewParent; }
	inline std::vector<CNode*>* pvpcGetChildren() { return &vpcChildren; }

	double dEvaluateNode(CVariablesData* cVariables);
	static std::string sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset);
	static bool bIsNum(std::string& sToCheck);

	inline E_NODE_TYPE eGetNodeType() { return eNodeType; }

	void vAttachNodeToLastElement(CNode* pcOtherRoot, CVariablesData* cVariables);
	std::string sGetVarName() { return sVariableName; }
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
	
	double dCalculateOperation(CVariablesData* cVariables);

};
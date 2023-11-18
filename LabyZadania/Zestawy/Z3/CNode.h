#pragma once

#include <vector>
#include <string>
#include "ETokens.h"

class CError;
class CVariablesData;

class CNode {
public:
	CNode(CNode* pcParent);
	CNode(CNode* pcParent, CNode& cOriginal);
	~CNode();

	/** Builds node based on the inputed sFormula*/
	void bParseNode(std::string sFormula, int& iOffset, char cSeparator, CError& cError);
	/** Finds the last node in the hierarchy and replaces it*/
	void vAttachNodeToLastElement(CNode* pcOtherRoot);
	/** Adds all the variables from node and its children into the variable list*/
	void vAddVariablesFromNode(CVariablesData* cVariables);

	std::string sNodeRepresentation();
	
	double dEvaluateNode(CVariablesData* cVariables);

	inline std::string sGetVarName() { return sVariableName; }
	inline E_NODE_TYPE eGetNodeType() { return eNodeType; }
	inline CNode* pcGetParent() { return pcParent; }
	inline std::vector<CNode*>* pvpcGetChildren() { return &vpcChildren; }

	inline void vSetParent(CNode* pcNewParent) { pcParent = pcNewParent; }	
		
private:
	CNode* pcParent;
	std::vector<CNode*> vpcChildren;
	E_OPERATION_TYPE eOperationType;
	E_NODE_TYPE eNodeType;
	std::string sVariableName;
	double dConstantValue;

	//static
	static std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> vOperationDefs;

private:
	void vMakeDefualt();
	double dCalculateOperation(CVariablesData* cVariables);
	
	static E_NODE_TYPE eDetermineNodeType(std::string input, int &iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue, E_ERROR_TYPE& eError);
	
};

class CInputParsing {
public:
	static std::string sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset);
	static bool bIsNum(std::string sToCheck);
};
#include "CNode.h"

char CNode::cSeparator = ' ';

std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> CNode::vOperationDefs = {
	{EOT_ADDITION,{"+", 2}},
	{EOT_SUBTRACTION,{"-", 2}},
	{EOT_MULTIPLICATION,{"*", 2}},
	{EOT_DIVISION,{"/", 2}},
	{EOT_SIN,{"sin", 2}},
	{EOT_COS,{"cos", 2}},
	{EOT_TAN,{"tan", 2}},
	{EOT_LOG,{"log", 2}},
	{EOT_POWER,{"^", 2}}
};

CNode::~CNode(){
	for (int i = 0; i < vpcChildren.size(); i++) {
		delete vpcChildren.at(i);
	}
}

bool CNode::bParseNode(std::string sFormula, int& iOffset, char iSeparator, std::string& sErrorMessage){
	if (sFormula.substr(iOffset).length() == 0) {
		this->vMakeDefualt();
		sErrorMessage = "missing argument at" + iOffset;
		return false;
	}
	int iEnd = sFormula.find(cSeparator, iOffset);
	if (iEnd == std::string::npos) {
		iEnd = sFormula.length();
	}
	std::string part = sFormula.substr(iOffset, iEnd - iOffset);
	iOffset = iEnd + 1;
	int iNumArguments;
	eNodeType = eDetermineNodeType(part, iNumArguments, eOperationType, sVariableName, dConstantValue);
	if (eNodeType == ENT_OPERATION) {
		bool bRet = true;
		for (int i = 0; i < iNumArguments; i++) {
			vpcChildren.push_back(new CNode(this));
			if (!vpcChildren.at(i)->bParseNode(sFormula, iOffset, iSeparator, sErrorMessage)) {
				bRet = false;
			}
		}
		return bRet;
	}
	else {
		return true;
	}
}

void CNode::vMakeDefualt()
{
	eNodeType = ENT_CONSTANT;
	eOperationType = EDT_NONE;
	dConstantValue = 1.;
}

E_NODE_TYPE CNode::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue)
{
	int iOperationId = 0;
	while (iOperationId < vOperationDefs.size()) {
		if (vOperationDefs.at(iOperationId).second.first == input) {
			iNumArguments = vOperationDefs.at(iOperationId).second.second;
			eOpType = vOperationDefs.at(iOperationId).first;
			return ENT_OPERATION;
		}
		iOperationId++;
	}
	iNumArguments = 0;
	eOpType = EDT_NONE;
	if (bIsNum(input)) {
		dValue = strtod(input.c_str(), NULL);		
		return ENT_CONSTANT;
	}
	sVarName = input;
	return ENT_VARIABLE;
}

bool CNode::bIsNum(std::string& sToCheck){
	bool bDecimal = false;
	for (char c : sToCheck) {
		if (!isdigit(c) && c!='.') {
			return false;
		}
		if (c == '.') {
			if (bDecimal) {
				return false;
			}
			else {
				bDecimal = true;
			}
		}
	}
	return true;
}
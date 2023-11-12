#include "CNode.h"
#include <sstream>
#include <iomanip>

std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> CNode::vOperationDefs = {
	{EOT_ADDITION,{"+", 2}},
	{EOT_SUBTRACTION,{"-", 2}},
	{EOT_MULTIPLICATION,{"*", 2}},
	{EOT_DIVISION,{"/", 2}},
	{EOT_SIN,{"sin", 1}},
	{EOT_COS,{"cos", 1}},
	{EOT_TAN,{"tan", 1}},
	{EOT_LOG,{"log", 2}},
	{EOT_POWER,{"^", 2}}
};

CNode::CNode(CNode* pcParent, CNode& cOriginal)
{
	this->pcParent = pcParent;
	this->eOperationType = cOriginal.eOperationType;
	this->eNodeType = cOriginal.eNodeType;
	this->sVariableName = cOriginal.sVariableName;
	this->dConstantValue = cOriginal.dConstantValue;
	for (int i = 0; i < cOriginal.vpcChildren.size(); i++) {
		vpcChildren.push_back(new CNode(this, *cOriginal.vpcChildren.at(i)));
	}
}

CNode::~CNode(){
	for (int i = 0; i < vpcChildren.size(); i++) {
		delete vpcChildren.at(i);
	}
}

bool CNode::bParseNode(std::string sFormula, int& iOffset, char cSeparator, std::string& sErrorMessage, E_ERROR_TYPE& eError, std::vector<std::string>& vVars, std::vector<int>& vVarCount){
	
	if (sFormula.length() <= iOffset) {
		this->vMakeDefualt();
		eError = EET_MISSING_ARGUMENT;
		return false;
	}
	std::string part = sGetNextTokenFromInput(sFormula, cSeparator, iOffset);
	int iNumArguments;
	eNodeType = eDetermineNodeType(part, iNumArguments, eOperationType, sVariableName, dConstantValue, eError);
	if (eNodeType == ENT_OPERATION) {
		bool bRet = true;
		for (int i = 0; i < iNumArguments; i++) {
			vpcChildren.push_back(new CNode(this));
			if (!vpcChildren.at(i)->bParseNode(sFormula, iOffset, cSeparator, sErrorMessage, eError, vVars, vVarCount)) {
				sErrorMessage = "Missing arguments for operator. Default value inserted";
				bRet = false;
			}
		}
		return bRet;
	}
	else if (eNodeType == ENT_VARIABLE){
		int it = 0;
		while(it < vVars.size() && vVars.at(it) != sVariableName) {
			it++;
		}
		if (it == vVars.size()) {
			vVars.push_back(sVariableName);
			vVarCount.push_back(1);
		}
		else {
			vVarCount.at(it)++;
		}
		return true;
	}
	else {
		return true;
	}
}

std::string CNode::sNodeRepresentation()
{
	std::string sToReturn;
	if (eNodeType == ENT_CONSTANT) {
		std::ostringstream oss; 
		oss<<std::fixed<< std::setprecision(2)<<dConstantValue<<" ";
		sToReturn = oss.str();
	}
	else if (eNodeType == ENT_VARIABLE) {
		sToReturn = sVariableName + " ";
	}
	else {
		bool found = false;
		for (int i = 0; !found; i++) {
			if (vOperationDefs.at(i).first == eOperationType) {
				found = true;
				sToReturn = vOperationDefs.at(i).second.first+ " ";
				for (int j = 0; j < vOperationDefs.at(i).second.second; j++) {
					sToReturn += vpcChildren.at(j)->sNodeRepresentation();
				}
			}
		}
	}
	return sToReturn;
}

double CNode::dEvaluateNode(std::vector<std::string>& vsVariableNames, std::vector<double>& vdVariableValues)
{
	if (eNodeType == ENT_CONSTANT) {
		return dConstantValue;
	}
	else if (eNodeType == ENT_VARIABLE) {
		int it = 0;
		while (vsVariableNames.at(it) != sVariableName) {
			it++;
		}
		return vdVariableValues.at(it);
	}
	else {
		return dCalculateOperation(vsVariableNames, vdVariableValues);
	}
}



void CNode::vMakeDefualt()
{
	eNodeType = ENT_CONSTANT;
	eOperationType = EDT_NONE;
	dConstantValue = 1.;
}

E_NODE_TYPE CNode::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue, E_ERROR_TYPE& eError)
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
	for (int i = 0; i < input.length(); i++) {
		if (isdigit(input.at(i)) || isalpha(input.at(i))) {
			sVarName += input.at(i);
		}
		else {
			eError = EET_INVALID_ARGUMENT;
		}
	}
	return ENT_VARIABLE;
}

double CNode::dCalculateOperation(std::vector<std::string>& vsVariableNames, std::vector<double>& vdVariableValues)
{
	if (eOperationType == EOT_ADDITION) {
		return vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues) + vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues);
	}
	else if (eOperationType == EOT_SUBTRACTION) {
		return vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues) - vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues);
	}
	else if (eOperationType == EOT_MULTIPLICATION) {
		return vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues) * vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues);
	}
	else if (eOperationType == EOT_DIVISION) {
		return vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues) / vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues);
	}
	else if (eOperationType == EOT_POWER) {
		return std::pow(vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues), vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues));
	}
	else if (eOperationType == EOT_LOG) {
		return std::log2(vpcChildren.at(1)->dEvaluateNode(vsVariableNames, vdVariableValues)) / std::log2(vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues));
	}
	else if (eOperationType == EOT_SIN) {
		return std::sin(vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues));
	}
	else if (eOperationType == EOT_COS) {
		return std::cos(vpcChildren.at(0)->dEvaluateNode(vsVariableNames, vdVariableValues));
	}
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

void CNode::vAttachNodeToLastElement(CNode* pcOtherRoot, std::vector<std::string>& vVars, std::vector<int>& vVarCount)
{
	if (eNodeType == ENT_OPERATION) {
		CNode* pcLastChild = vpcChildren.at(vpcChildren.size() - 1);
		if (pcLastChild->eNodeType == ENT_OPERATION) {
			pcLastChild->vAttachNodeToLastElement(pcOtherRoot, vVars, vVarCount);
		}
		else {
			if (pcLastChild->eNodeType == ENT_VARIABLE) {
				int i = 0;
				while (i < vVars.size() && vVars.at(i) != pcLastChild->sVariableName) {
					i++;
				}
				if (--vVarCount.at(i) == 0) {
					std::vector<int>::iterator itToRemC = vVarCount.begin() + i;
					vVarCount.erase(itToRemC);
					std::vector<std::string>::iterator itToRemV = vVars.begin() + i; 
					vVars.erase(itToRemV);
				}
				
			}
			delete pcLastChild;
			vpcChildren.at(vpcChildren.size() - 1) = new CNode(this, *pcOtherRoot);
		}
	}
}


std::string CNode::sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset)
{
	while (sInput.at(iOffset) == cSeparator) {
		iOffset++;
	}
	int iEnd = sInput.find(cSeparator, iOffset);
	if (iEnd == std::string::npos) {
		iEnd = sInput.length();
	}
	std::string token = sInput.substr(iOffset, iEnd - iOffset);
	iOffset = iEnd + 1;
	return token;
}

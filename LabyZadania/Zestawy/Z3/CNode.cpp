#include "CNode.h"
#include "CError.h"
#include <sstream>
#include <iomanip>
#include "CVariablesData.h"

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

CNode::CNode(CNode* pcParent)
{
	this->pcParent = pcParent;
	eNodeType = ENT_NONE;
	eOperationType = EDT_NONE;
	dConstantValue = 0;
}

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

void CNode::bParseNode(std::string sFormula, int& iOffset, char cSeparator, CError& cError){

	if (sFormula.length() <= iOffset) {
		this->vMakeDefualt();
		cError.vSetType(EET_MISSING_ARGUMENT);
		return;
	}

	std::string part = CInputParsing::sGetNextTokenFromInput(sFormula, cSeparator, iOffset);
	if (part == "") {
		this->vMakeDefualt();
		cError.vSetType(EET_MISSING_ARGUMENT);
		return;
	}

	int iNumArguments;
	E_ERROR_TYPE eError = EET_NONE;
	eNodeType = eDetermineNodeType(part, iNumArguments, eOperationType, sVariableName, dConstantValue, eError);
	cError.vAppendMessage(false, eError, part);
	if (eNodeType == ENT_OPERATION) {
		bool bRet = true;
		for (int i = 0; i < iNumArguments; i++) {
			vpcChildren.push_back(new CNode(this));
			vpcChildren.at(i)->bParseNode(sFormula, iOffset, cSeparator, cError);
			cError.vAppendMessage(false, part);
		}
	}
	
}

void CNode::vAttachNodeToLastElement(CNode* pcOtherRoot)
{
	if (eNodeType == ENT_OPERATION) {
		CNode* pcLastChild = vpcChildren.at(vpcChildren.size() - 1);
		if (pcLastChild->eNodeType == ENT_OPERATION) {
			pcLastChild->vAttachNodeToLastElement(pcOtherRoot);
		}
		else {
			delete pcLastChild;
			vpcChildren.at(vpcChildren.size() - 1) = new CNode(this, *pcOtherRoot);
		}
	}
}

void CNode::vAddVariablesFromNode(CVariablesData* cVariables)
{
	if (eNodeType == ENT_VARIABLE){
		cVariables->vAddVariable(sVariableName);
	}
	else if (eNodeType == ENT_OPERATION) {
		for (int i = 0; i < vpcChildren.size(); i++) {
			vpcChildren.at(i)->vAddVariablesFromNode(cVariables);
		}
	}
}


std::string CNode::sNodeRepresentation()
{
	std::string sToReturn;
	if (eNodeType == ENT_CONSTANT) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << dConstantValue << " ";
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
				sToReturn = vOperationDefs.at(i).second.first + " ";
				for (int j = 0; j < vOperationDefs.at(i).second.second; j++) {
					sToReturn += vpcChildren.at(j)->sNodeRepresentation();
				}
			}
		}
	}
	return sToReturn;
}

double CNode::dEvaluateNode(CVariablesData* cVariables)
{
	if (eNodeType == ENT_CONSTANT) {
		return dConstantValue;
	}
	else if (eNodeType == ENT_VARIABLE) {
		return cVariables->dGetVariableData(sVariableName);
	}
	else {
		return dCalculateOperation(cVariables);
	}
}


//private section

void CNode::vMakeDefualt()
{
	eNodeType = ENT_CONSTANT;
	eOperationType = EDT_NONE;
	dConstantValue = 1.;
}

double CNode::dCalculateOperation(CVariablesData* cVariables)
{
	if (eOperationType == EOT_ADDITION) {
		return vpcChildren.at(0)->dEvaluateNode(cVariables) + vpcChildren.at(1)->dEvaluateNode(cVariables);
	}
	else if (eOperationType == EOT_SUBTRACTION) {
		return vpcChildren.at(0)->dEvaluateNode(cVariables) - vpcChildren.at(1)->dEvaluateNode(cVariables);
	}
	else if (eOperationType == EOT_MULTIPLICATION) {
		return vpcChildren.at(0)->dEvaluateNode(cVariables) * vpcChildren.at(1)->dEvaluateNode(cVariables);
	}
	else if (eOperationType == EOT_DIVISION) {
		double dDiv = vpcChildren.at(1)->dEvaluateNode(cVariables);
		if (dDiv == 0) {
			throw divByZero();
		}
		return vpcChildren.at(0)->dEvaluateNode(cVariables) / dDiv;
	}
	else if (eOperationType == EOT_POWER) {
		return std::pow(vpcChildren.at(0)->dEvaluateNode(cVariables), vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_LOG) {
		return std::log2(vpcChildren.at(1)->dEvaluateNode(cVariables)) / std::log2(vpcChildren.at(0)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_SIN) {
		return std::sin(vpcChildren.at(0)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_COS) {
		return std::cos(vpcChildren.at(0)->dEvaluateNode(cVariables));
	}
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
	if (CInputParsing::bIsNum(input)) {
		dValue = strtod(input.c_str(), NULL);
		return ENT_CONSTANT;
	}
	bool bHasLetter = false;
	for (int i = 0; i < input.length(); i++) {
		if (isdigit(input.at(i)) || isalpha(input.at(i))) {
			sVarName += input.at(i);
			if (isalpha(input.at(i))) {
				bHasLetter = true;
			}
		}
		else {
			eError = EET_INVALID_ARGUMENT;
		}
	}
	if (!bHasLetter) {
		eError = EET_INVALID_ARGUMENT;
		sVarName += 'x';
	}
	return ENT_VARIABLE;
}

//InputParsing


std::string CInputParsing::sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset)
{
	while (iOffset < sInput.length() && sInput.at(iOffset) == cSeparator) {
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


bool CInputParsing::bIsNum(std::string sToCheck) {
	bool bDecimal = false;
	if (sToCheck.at(0) == '-') {
		sToCheck = sToCheck.substr(1);
	}
	for (char c : sToCheck) {
		if (!isdigit(c) && c != '.') {
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
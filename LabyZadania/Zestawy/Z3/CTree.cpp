#include "CTree.h"

#include "CNode.h"
#include "CError.h"
#include <sstream>

void CTree::vParseFormula(std::string sFormula, CError& cError, char cSeparator)
{
	pcTreeRoot = new CNode(NULL);
	pcVariablesData = new CVariablesData();
	int iOffset = 0;
	E_ERROR_TYPE eError = EET_NONE;
	pcTreeRoot->bParseNode(sFormula, iOffset, cSeparator, cError);
	if (iOffset < sFormula.length()) {
		if (CInputParsing::sGetNextTokenFromInput(sFormula, cSeparator, iOffset) != "") {
			cError.vAppendMessage(false, EET_TOO_MANY_ARGUMENTS);
		}
	}
	
}

std::string CTree::sReturnFormula()
{
	if (pcTreeRoot == nullptr) {
		return "You haven't entered a formula yet.";
	}
	return pcTreeRoot->sNodeRepresentation();
}

std::string CTree::sReturnVariables()
{
	if (pcTreeRoot == nullptr) {
		return "In order to see variables, enter a formula first, idiot!";
	}

	if (!pcVariablesData->bGetDataValid()) {
		vVariablesDataFromTree();
	}

	if (pcVariablesData->iGetVarsNum()  == 0) {
		return "There are no variables in your formula.";
	}
	std::string sResult = "Variables in your formula: ";
	for (int i = 0; i < pcVariablesData->iGetVarsNum() - 1; i++) {
		sResult += pcVariablesData->sGetVarAtPos(i) + ", ";
	}
	sResult += pcVariablesData->sGetVarAtPos(pcVariablesData->iGetVarsNum() - 1);
	return sResult;
}

void CTree::vClearTree()
{
	if (pcTreeRoot != nullptr) {
		delete pcTreeRoot;
		pcTreeRoot = nullptr;
	}
	if (pcVariablesData != nullptr) {

		pcVariablesData->vClearVariables();
		pcVariablesData->vSetDataValid(false);
	}
}

std::string CTree::sCompute(std::string& sUserResponse, char cSeparator, CError& cError)
{
	if (!pcVariablesData->bGetDataValid()) {
		vVariablesDataFromTree();
	}
	E_ERROR_TYPE eError = eParseVariableValues(sUserResponse, cSeparator);
	if (eError != EET_NONE) {
		cError.vAppendMessage(true, eError);
		return cError.sGetErrorMessage();
	}
		
	std::ostringstream oss;
	oss << "Result for the formula: ";
	oss << sReturnFormula();
	if (pcVariablesData->iGetVarsNum() > 0) {
		oss << "\nWhere: ";
		for (int i = 0; i < pcVariablesData->iGetVarsNum(); i++) {
			oss << pcVariablesData->sGetVarAtPos(i) << " = ";
			oss << pcVariablesData->dGetValAtPos(i) << ", ";
		}
	}
	oss << "\n";
	oss << pcTreeRoot->dEvaluateNode(pcVariablesData);
	return oss.str();
	
}

void CTree::operator=(const CTree& cOther)
{
	pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	pcVariablesData = new CVariablesData(*cOther.pcVariablesData);
}

CTree CTree::operator+(const CTree& cOther)const
{
	CTree cResult = *this;
	cResult.pcVariablesData->vSetDataValid(false);
	if (cResult.pcTreeRoot->eGetNodeType() == ENT_OPERATION) {

		cResult.pcTreeRoot->vAttachNodeToLastElement(cOther.pcTreeRoot);
	}
	else {
		delete cResult.pcTreeRoot;
		cResult.pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	}
	return cResult;
}

CTree CTree::cReplaceNodeWNode(CNode* pcToRep, CNode* pcNew)
{
	CNode* pcParent = pcToRep->pcGetParent();
	std::vector<CNode*>* pvpcKids = pcParent->pvpcGetChildren();
	int i = 0;
	while (pvpcKids->at(i) != pcToRep) {
		i++;
	}
	pcVariablesData->vSetDataValid(false);
	delete pvpcKids->at(i);
	pvpcKids->at(i) = pcNew;
	pcNew->vSetParent(pcParent);

	return CTree();
}

E_ERROR_TYPE CTree::eParseVariableValues(std::string sInput, char cSeparator)
{
	pcVariablesData->vClearValues();
	int iOffset = 0;
	while (iOffset < sInput.length()) {
		std::string token = CInputParsing::sGetNextTokenFromInput(sInput, cSeparator, iOffset);
		if (token != "") {
			bool bIsNum = CInputParsing::bIsNum(token);
			if (bIsNum) {
				pcVariablesData->vInsertValue(strtod(token.c_str(), NULL));
			}
			else {
				return EET_INVALID_ARGUMENT;
			}
		}
	}
	if (pcVariablesData->iSizeComparison() == 1) {
		return EET_TOO_MANY_ARGUMENTS;
	}
	if (pcVariablesData->iSizeComparison() == -1) {
		return EET_MISSING_ARGUMENT;
	}
	return EET_NONE;
}

void CTree::vVariablesDataFromTree()
{
	pcVariablesData->vClearVariables();
	pcTreeRoot->vAddVariablesFromNode(pcVariablesData);
	pcVariablesData->vSetDataValid(true);
}

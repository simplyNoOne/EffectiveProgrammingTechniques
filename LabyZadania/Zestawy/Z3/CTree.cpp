#include "CTree.h"

#include "CNode.h"
#include "CError.h"
#include <sstream>

void CTree::vParseFormula(std::string sFormula, CError& cError, char cSeparator)
{
	/*int iSkip = 0;
	while (sFormula[iSkip] == cSeparator) {
		iSkip++;
	}
	sFormula = sFormula.substr(iSkip);*/
	pcTreeRoot = new CNode(NULL);
	int iOffset = 0;
	E_ERROR_TYPE eError = EET_NONE;
	pcTreeRoot->bParseNode(sFormula, iOffset, cSeparator, cError, &cVaraiblesData);
	if (iOffset < sFormula.length()) {
		cError.vAppendMessage(false, EET_TOO_MANY_ARGUMENTS);
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
	if (cVaraiblesData.iGetVarsNum()  == 0) {
		return "There are no variables in your formula.";
	}
	std::string sResult = "Variables in your formula: ";
	for (int i = 0; i < cVaraiblesData.iGetVarsNum() - 1; i++) {
		sResult += cVaraiblesData.sGetVarAtPos(i) + ", ";
	}
	sResult += cVaraiblesData.sGetVarAtPos(cVaraiblesData.iGetVarsNum() - 1);
	return sResult;
}

void CTree::vClearTree()
{
	if (pcTreeRoot != nullptr) {
		delete pcTreeRoot;
		pcTreeRoot = nullptr;
	}
	cVaraiblesData.vClearVariables();
}

std::string CTree::sCompute(std::string& sUserResponse, char cSeparator, CError& cError)
{
	E_ERROR_TYPE eError = eParseVariableValues(sUserResponse, cSeparator);
	if (eError != EET_NONE) {
		cError.vAppendMessage(true, eError);
		return cError.sGetErrorMessage();
	}
		
	std::ostringstream oss;
	oss << "Result for the formula: ";
	oss << sReturnFormula();
	if (cVaraiblesData.iGetVarsNum() > 0) {
		oss << "\nWhere: ";
		for (int i = 0; i < cVaraiblesData.iGetVarsNum(); i++) {
			oss << cVaraiblesData.sGetVarAtPos(i) << " = ";
			oss << cVaraiblesData.dGetValAtPos(i) << ", ";
		}
	}
	oss << "\n";
	oss << pcTreeRoot->dEvaluateNode(&cVaraiblesData);
	return oss.str();
	
}

void CTree::operator=(const CTree& cOther)
{
	pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	cVaraiblesData = cOther.cVaraiblesData;
}

CTree CTree::operator+(const CTree& cOther)const
{
	CTree cResult = *this;
	if (cResult.pcTreeRoot->eGetNodeType() == ENT_OPERATION) {

		cResult.pcTreeRoot->vAttachNodeToLastElement(cOther.pcTreeRoot, &cResult.cVaraiblesData);
	}
	else {
		cResult.cVaraiblesData.vClearVariables();
		delete cResult.pcTreeRoot;
		cResult.pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	}
	for (int i = 0; i < cOther.cVaraiblesData.iGetVarsNum() ; i++) {
		cResult.cVaraiblesData.vAddVariable(cOther.cVaraiblesData.sGetVarAtPos(i));
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
	cVaraiblesData.vRemoveVarsInNode(pcToRep);
	delete pvpcKids->at(i);
	pvpcKids->at(i) = pcNew;
	pcNew->vSetParent(pcParent);

	return CTree();
}

E_ERROR_TYPE CTree::eParseVariableValues(std::string sInput, char cSeparator)
{
	cVaraiblesData.vClearValues();
	int iOffset = 0;
	while (iOffset < sInput.length()) {
		std::string token = CNode::sGetNextTokenFromInput(sInput, cSeparator, iOffset);
		bool bIsNum = CNode::bIsNum(token);
		if (bIsNum) {
			cVaraiblesData.vInsertValue(strtod(token.c_str(), NULL));
		}
		else {
			return EET_INVALID_ARGUMENT;
		}
	}
	if (cVaraiblesData.iSizeComparison() == 1) {
		return EET_TOO_MANY_ARGUMENTS;
	}
	if (cVaraiblesData.iSizeComparison() == -1) {
		return EET_MISSING_ARGUMENT;
	}
	return EET_NONE;
}

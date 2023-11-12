#include "CTree.h"

#include "CNode.h"
#include <sstream>

void CTree::vParseFormula(std::string sFormula, std::string& sErrorMessage, char cSeparator)
{
	/*int iSkip = 0;
	while (sFormula[iSkip] == cSeparator) {
		iSkip++;
	}
	sFormula = sFormula.substr(iSkip);*/
	pcTreeRoot = new CNode(NULL);
	int iOffset = 0;
	E_ERROR_TYPE eError = EET_NONE;
	pcTreeRoot->bParseNode(sFormula, iOffset, cSeparator, sErrorMessage, eError,vsVariables, viVariableOccurences);
	if (iOffset < sFormula.length()) {
		eError = EET_TOO_MANY_ARGUMENTS;
		sErrorMessage = "To many arguments bro, R U blind?";
	}
	if (eError != EET_NONE) {
		sErrorMessage += "\nResulting formula:\n";
		sErrorMessage += sReturnFormula() + "\n";
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
	if (vsVariables.size() == 0) {
		return "There are no variables in your formula.";
	}
	std::string sResult = "Variables in your formula: ";
	for (int i = 0; i < vsVariables.size() - 1; i++) {
		sResult += vsVariables.at(i) + ", ";
	}
	sResult += vsVariables.at(vsVariables.size() - 1);
	return sResult;
}

void CTree::vClearTree()
{
	if (pcTreeRoot != nullptr) {
		delete pcTreeRoot;
		pcTreeRoot = nullptr;
	}
	vsVariables.clear();
	viVariableOccurences.clear();
}

std::string CTree::sCompute(std::string& sUserResponse, char cSeparator)
{
	E_ERROR_TYPE eError = eParseVariableValues(sUserResponse, cSeparator);
	if (eError == EET_INVALID_ARGUMENT) {
		return "Variables can only be numbers, didn't you know?";
	}
	else if (eError == EET_MISSING_ARGUMENT) {
		return "You didn't pass in enough values for variables, can't you count?";
	}
	else if(eError == EET_TOO_MANY_ARGUMENTS){
		return "You passed in too many values, learn to count for God's sake!";
	}
	std::ostringstream oss;
	oss << "Result for the formula: ";
	oss << sReturnFormula();
	if (vsVariables.size() > 0) {
		oss << "\nWhere: ";
		for (int i = 0; i < vsVariables.size(); i++) {
			oss << vsVariables.at(i) + " = ";
			oss << vdVariableValues.at(i);
		}
	}
	oss << "\n";
	oss << pcTreeRoot->dEvaluateNode(vsVariables, vdVariableValues);
	return oss.str();
	
}

void CTree::operator=(const CTree& cOther)
{
	pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	vsVariables = cOther.vsVariables;
	viVariableOccurences = cOther.viVariableOccurences;
}

CTree CTree::operator+(const CTree& cOther)const
{
	CTree cResult = *this;
	if (cResult.pcTreeRoot->eGetNodeType() == ENT_OPERATION) {

		cResult.pcTreeRoot->vAttachNodeToLastElement(cOther.pcTreeRoot, cResult.vsVariables, cResult.viVariableOccurences);
	}
	else {
		cResult.vsVariables.clear();
		cResult.viVariableOccurences.clear();
		delete cResult.pcTreeRoot;
		cResult.pcTreeRoot = new CNode(NULL, *cOther.pcTreeRoot);
	}
	for (int i = 0; i < cOther.vsVariables.size(); i++) {
		int it = 0;
		while (it < cResult.vsVariables.size() && cResult.vsVariables.at(it) != cOther.vsVariables.at(i)) {
			it++;
		}
		if (it == cResult.vsVariables.size()) {
			cResult.vsVariables.push_back(cOther.vsVariables.at(i));
			cResult.viVariableOccurences.push_back(1);
		}
		else {
			cResult.viVariableOccurences.at(it)++;
		}
	}
	return cResult;
}

E_ERROR_TYPE CTree::eParseVariableValues(std::string sInput, char cSeparator)
{
	vdVariableValues.clear();
	int iOffset = 0;
	while (iOffset < sInput.length()) {
		std::string token = CNode::sGetNextTokenFromInput(sInput, cSeparator, iOffset);
		bool bIsNum = CNode::bIsNum(token);
		if (bIsNum) {
			vdVariableValues.push_back(strtod(token.c_str(), NULL));
		}
		else {
			return EET_INVALID_ARGUMENT;
		}
	}
	if (vdVariableValues.size() > vsVariables.size()) {
		return EET_TOO_MANY_ARGUMENTS;
	}
	if (vdVariableValues.size() < vsVariables.size()) {
		return EET_MISSING_ARGUMENT;
	}
	return EET_NONE;
}

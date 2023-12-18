#pragma once

#include<vector>
#include <string>
#include "ETokens.h"
#include <utility>
#include <iostream>
//#include "CVariablesData.h"

#include "CNode.h"


#include "CError.h"
#include <sstream>
class CError;
//class CVariablesData;

template <typename T> class CTree {

public:
	inline CTree() { pcTreeRoot = nullptr; pcVariablesData = nullptr; }
	inline ~CTree() {
		if (pcTreeRoot != nullptr) {
			delete pcTreeRoot;
		}
		if (pcVariablesData != nullptr) {
			delete pcVariablesData;
		}
	}
	CTree(CTree&& cOther);
	CTree(const CTree& cOther);


	CTree<T> operator=(const CTree& cOther);
	CTree<T>& operator=(CTree&& cOther);
	CTree operator+(const CTree& cOther)const;

	void vParseFormula(std::string sFormula, CError& cError, char cSeparator);
	void vClearTree();

	std::string sReturnFormula();
	std::string sReturnVariables();
	std::string sCompute( std::string& sUserResponse, char cSeparator, CError& cError);
	
	/**Replaces one node with another otherwise maintaining the inner structure of nodes and returns the resulting tree*/
	CTree cReplaceNodeWNode(CNode<T>* pcToRep, CNode<T>* pcNew);

	inline bool bExists() { return pcTreeRoot != nullptr; }

private:
	CNode<T>* pcTreeRoot;
	CVariablesData<T>* pcVariablesData;

private:
	/**Tries to match values from input with variable list and returns appropriate error. On success - EET_NONE */
	E_ERROR_TYPE eParseVariableValues(std::string sInput, char cSeparator);
	/**Iterates through the entire tree and creates matching variable list*/
	void vVariablesDataFromTree();
};


template <typename T>
inline void CTree<T>::vParseFormula(std::string sFormula, CError& cError, char cSeparator)
{
	pcTreeRoot = new CNode<T>(NULL);
	pcVariablesData = new CVariablesData<T>();
	int iOffset = 0;
	E_ERROR_TYPE eError = EET_NONE;
	pcTreeRoot->bParseNode(sFormula, iOffset, cSeparator, cError);
	if (iOffset < sFormula.length()) {
		if (CInputParsing::sGetNextTokenFromInput(sFormula, cSeparator, iOffset) != "") {
			cError.vAppendMessage(false, EET_TOO_MANY_ARGUMENTS);
		}
	}

}

template <typename T>
inline std::string CTree<T>::sReturnFormula()
{
	if (pcTreeRoot == nullptr) {
		return "You haven't entered a formula yet.";
	}
	return std::move(pcTreeRoot->sNodeRepresentation());
}

template <typename T>
inline std::string CTree<T>::sReturnVariables()
{
	if (pcTreeRoot == nullptr) {
		return "In order to see variables, enter a formula first, idiot!";
	}

	if (!pcVariablesData->bGetDataValid()) {
		vVariablesDataFromTree();
	}

	if (pcVariablesData->iGetVarsNum() == 0) {
		return "There are no variables in your formula.";
	}
	std::string sResult = "Variables in your formula: ";
	for (int i = 0; i < pcVariablesData->iGetVarsNum() - 1; i++) {
		sResult += pcVariablesData->sGetVarAtPos(i) + ", ";
	}
	sResult += pcVariablesData->sGetVarAtPos(pcVariablesData->iGetVarsNum() - 1);
	return std::move(sResult);
}

template <typename T>
inline void CTree<T>::vClearTree()
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

template <typename T>
inline std::string CTree<T>::sCompute(std::string& sUserResponse, char cSeparator, CError& cError)
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

template<typename T>
inline CTree<T>::CTree(CTree&& cOther)
{
	pcTreeRoot = cOther.pcTreeRoot;
	pcVariablesData = cOther.pcVariablesData;
	cOther.pcTreeRoot = nullptr;
	cOther.pcVariablesData = nullptr;
	std::cout << "MOVE CTOR\n";
}

template<typename T>
inline CTree<T>::CTree(const CTree& cOther)
{
	pcTreeRoot = new CNode<T>(NULL, *cOther.pcTreeRoot);
	pcVariablesData = new CVariablesData<T>(*cOther.pcVariablesData);
}

template <typename T>
inline CTree<T> CTree<T>::operator=(const CTree<T>& cOther)
{
	if (pcTreeRoot != nullptr) {
		delete pcTreeRoot;
	}
	if (pcVariablesData != nullptr) {
		delete pcVariablesData;
	}
	pcTreeRoot = new CNode<T>(NULL, *cOther.pcTreeRoot);
	pcVariablesData = new CVariablesData<T>(*cOther.pcVariablesData);
	return *this;
}

template<typename T>
inline CTree<T>& CTree<T>::operator=(CTree&& cOther) 
{
	if (pcTreeRoot != nullptr) {
		delete pcTreeRoot;
	}
	if (pcVariablesData != nullptr) {
		delete pcVariablesData;
	}
	pcTreeRoot = cOther.pcTreeRoot;
	pcVariablesData = cOther.pcVariablesData;
	cOther.pcTreeRoot = nullptr;
	cOther.pcVariablesData = nullptr;
	std::cout << "MOVE OPR\n";
	return *this;
}


template <typename T>
inline CTree<T> CTree<T>::operator+(const CTree<T>& cOther)const
{
	CTree<T> cResult = *this;
	cResult.pcVariablesData->vSetDataValid(false);
	if (cResult.pcTreeRoot->eGetNodeType() == ENT_OPERATION) {

		cResult.pcTreeRoot->vAttachNodeToLastElement(cOther.pcTreeRoot);
	}
	else {
		delete cResult.pcTreeRoot;
		cResult.pcTreeRoot = new CNode<T>(NULL, *cOther.pcTreeRoot);
	}
	return (std::move(cResult));
}

template <typename T>
inline CTree<T> CTree<T>::cReplaceNodeWNode(CNode<T>* pcToRep, CNode<T>* pcNew)
{
	CNode<T>* pcParent = pcToRep->pcGetParent();
	std::vector<CNode<T>*>* pvpcKids = pcParent->pvpcGetChildren();
	int i = 0;
	while (pvpcKids->at(i) != pcToRep) {
		i++;
	}
	pcVariablesData->vSetDataValid(false);
	delete pvpcKids->at(i);
	pvpcKids->at(i) = pcNew;
	pcNew->vSetParent(pcParent);

	return this;
}

template <typename T>
inline E_ERROR_TYPE CTree<T>::eParseVariableValues(std::string sInput, char cSeparator)
{
	pcVariablesData->vClearValues();
	int iOffset = 0;
	while (iOffset < sInput.length()) {
		std::string token = CInputParsing::sGetNextTokenFromInput(sInput, cSeparator, iOffset);
		if (token != "") {
			bool bIsNum = false;
			double d;
			if (typeid(T) == typeid(double)) {
				bIsNum = CInputParsing::bIsNum(token, false);
			}
			else if (typeid(T) == typeid(bool)) {
				bIsNum = (token == "1" || token == "0");
			}
			else {
				bIsNum = CInputParsing::bIsNum(token);
			}
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

template <>
inline E_ERROR_TYPE CTree<std::string>::eParseVariableValues(std::string sInput, char cSeparator)
{
	pcVariablesData->vClearValues();
	int iOffset = 0;
	while (iOffset < sInput.length()) {
		std::string token = CInputParsing::sGetNextTokenFromInput(sInput, cSeparator, iOffset);
		if (token != "") {
			if (CInputParsing::bIsStringConst(token)) {
				pcVariablesData->vInsertValue(token.substr(1, token.length()-2));			}
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

template <typename T>
inline void CTree<T>::vVariablesDataFromTree()
{
	pcVariablesData->vClearVariables();
	pcTreeRoot->vAddVariablesFromNode(pcVariablesData);
	pcVariablesData->vSetDataValid(true);
}

#pragma once

#include <vector>
#include <string>
#include "ETokens.h"



#include "CError.h"
#include <sstream>
#include <iomanip>
#include "CVariablesData.h"


class CError;
template <typename T>
class CVariablesData;

std::string operator-(const std::string& sL, const std::string& sR);
std::string operator*(const std::string& sL, const std::string& sR);
std::string operator/(const std::string& sL, const std::string& sR);

template <typename T> class CNode {
public:

	
public:
	CNode(CNode<T>* pcParent);
	CNode(CNode<T>* pcParent, CNode<T>& cOriginal);
	~CNode();

	/** Builds node based on the inputed sFormula*/
	void bParseNode(std::string sFormula, int& iOffset, char cSeparator, CError& cError);
	/** Finds the last node in the hierarchy and replaces it*/
	void vAttachNodeToLastElement(CNode<T>* pcOtherRoot);
	/** Adds all the variables from node and its children into the variable list*/
	void vAddVariablesFromNode(CVariablesData<T>* cVariables);

	std::string sNodeRepresentation();
	
	T dEvaluateNode(CVariablesData<T>* cVariables);

	inline std::string sGetVarName() { return sVariableName; }
	inline E_NODE_TYPE eGetNodeType() { return eNodeType; }
	inline CNode<T>* pcGetParent() { return pcParent; }
	inline std::vector<CNode*>* pvpcGetChildren() { return &vpcChildren; }

	inline void vSetParent(CNode* pcNewParent) { pcParent = pcNewParent; }	
		
private:
	CNode* pcParent;
	std::vector<CNode*> vpcChildren;
	E_OPERATION_TYPE eOperationType;
	E_NODE_TYPE eNodeType;
	std::string sVariableName;
	T dConstantValue;

	//static
	static std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> vOperationDefs;

private:
	void vMakeDefualt();
	T dCalculateOperation(CVariablesData<T>* cVariables);
	
	static E_NODE_TYPE eDetermineNodeType(std::string input, int &iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, T& dValue, E_ERROR_TYPE& eError);
	
};

class CInputParsing {
public:
	static std::string sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset);
	static bool bIsNum(std::string sToCheck, bool bIsInt = true);
	static bool bIsStringConst(std::string sToCheck);
};

template <typename T>
std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> CNode<T>::vOperationDefs = {
	{EOT_ADDITION,{"+", 2}},
	{EOT_SUBTRACTION,{"-", 2}},
	{EOT_MULTIPLICATION,{"*", 2}},
	{EOT_DIVISION,{"/", 2}},
	{EOT_SIN,{"sin", 1}},
	{EOT_COS,{"cos", 1}},
	{EOT_LOG,{"log", 2}},
	{EOT_POWER,{"^", 2}},
	{EOT_SUPERSUM, {"supersum", 4}}
};

template <>
std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> CNode<std::string>::vOperationDefs = {
	{EOT_ADDITION,{"+", 2}},
	{EOT_SUBTRACTION,{"-", 2}},
	{EOT_MULTIPLICATION,{"*", 2}},
	{EOT_DIVISION,{"/", 2}},
	{EOT_SUPERSUM, {"supersum", 4}}
};


template <>
std::vector<std::pair<E_OPERATION_TYPE, std::pair<std::string, int>>> CNode<bool>::vOperationDefs = {
	{EOT_ADDITION,{"+", 2}},
	{EOT_SUBTRACTION,{"-", 2}},
	{EOT_MULTIPLICATION,{"*", 2}},
	{EOT_DIVISION,{"/", 2}},
};


template <typename T>
inline CNode<T>::CNode(CNode<T>* pcParent)
{
	this->pcParent = pcParent;
	eNodeType = ENT_NONE;
	eOperationType = EDT_NONE;
	dConstantValue = 0;
}

template <>
inline CNode<std::string>::CNode(CNode<std::string>* pcParent)
{
	this->pcParent = pcParent;
	eNodeType = ENT_NONE;
	eOperationType = EDT_NONE;
	dConstantValue = "";
}

template <typename T>
inline CNode<T>::CNode(CNode* pcParent, CNode<T>& cOriginal)
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

template <typename T>
inline CNode<T>::~CNode() {
	for (int i = 0; i < vpcChildren.size(); i++) {
		delete vpcChildren.at(i);
	}
}

template <typename T>
inline void CNode<T>::bParseNode(std::string sFormula, int& iOffset, char cSeparator, CError& cError) {

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

		for (int i = 0; i < iNumArguments; i++) {
			vpcChildren.push_back(new CNode<T>(this));
			vpcChildren.at(i)->bParseNode(sFormula, iOffset, cSeparator, cError);
			cError.vAppendMessage(false, part);
		}
	}

}

template <typename T>
inline void CNode<T>::vAttachNodeToLastElement(CNode<T>* pcOtherRoot)
{
	if (eNodeType == ENT_OPERATION) {
		CNode<T>* pcLastChild = vpcChildren.at(vpcChildren.size() - 1);
		if (pcLastChild->eNodeType == ENT_OPERATION) {
			pcLastChild->vAttachNodeToLastElement(pcOtherRoot);
		}
		else {
			delete pcLastChild;
			vpcChildren.at(vpcChildren.size() - 1) = new CNode<T>(this, *pcOtherRoot);
		}
	}
}

template <typename T>
inline void CNode<T>::vAddVariablesFromNode(CVariablesData<T>* cVariables)
{
	if (eNodeType == ENT_VARIABLE) {
		cVariables->vAddVariable(sVariableName);
	}
	else if (eNodeType == ENT_OPERATION) {
		for (int i = 0; i < vpcChildren.size(); i++) {
			vpcChildren.at(i)->vAddVariablesFromNode(cVariables);
		}
	}
}

template <>
inline std::string CNode<std::string>::sNodeRepresentation()
{
	std::string sToReturn;
	if (eNodeType == ENT_CONSTANT) {
		std::ostringstream oss;
		oss << "\"" << dConstantValue << "\" ";
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

template <typename T>
inline std::string CNode<T>::sNodeRepresentation()
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

template <typename T>
inline T CNode<T>::dEvaluateNode(CVariablesData<T>* cVariables)
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
template <typename T>
inline void CNode<T>::vMakeDefualt()
{
	eNodeType = ENT_CONSTANT;
	eOperationType = EDT_NONE;
	dConstantValue = 1;
}

template <>
inline void CNode<std::string>::vMakeDefualt()
{
	eNodeType = ENT_CONSTANT;
	eOperationType = EDT_NONE;
	dConstantValue = "";
}

template <>
inline double CNode<double>::dCalculateOperation(CVariablesData<double>* cVariables)
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
	else if (eOperationType == EOT_SUPERSUM) {
		double sSum = 0;
		for (int i = 0; i < 4; i++) {
			sSum += (vpcChildren.at(i)->dEvaluateNode(cVariables));
		}
		return sSum;
	}

}

//mnozymy razy 1000
template <>
inline int CNode<int>::dCalculateOperation(CVariablesData<int>* cVariables)
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
		int dDiv = vpcChildren.at(1)->dEvaluateNode(cVariables);
		if (dDiv == 0) {
			throw divByZero();
		}
		return int(vpcChildren.at(0)->dEvaluateNode(cVariables) / dDiv);
	}
	else if (eOperationType == EOT_POWER) {
		return int(std::pow(vpcChildren.at(0)->dEvaluateNode(cVariables), vpcChildren.at(1)->dEvaluateNode(cVariables)));
	}
	else if (eOperationType == EOT_LOG) {
		return int(1000 * (std::log2(vpcChildren.at(1)->dEvaluateNode(cVariables)) / std::log2(vpcChildren.at(0)->dEvaluateNode(cVariables))));
	}
	else if (eOperationType == EOT_SIN) {
		return int(1000 * std::sin(vpcChildren.at(0)->dEvaluateNode(cVariables)));
	}
	else if (eOperationType == EOT_COS) {
		return int(1000 * std::cos(vpcChildren.at(0)->dEvaluateNode(cVariables)));
	}
	else if (eOperationType == EOT_SUPERSUM) {
		int sSum = 0;
		for (int i = 0; i < 4; i++) {
			sSum += (vpcChildren.at(i)->dEvaluateNode(cVariables));
		}
		return sSum;
	}
}

template <>
inline std::string CNode<std::string>::dCalculateOperation(CVariablesData<std::string>* cVariables)
{
	if (eOperationType == EOT_ADDITION) {
		return std::string(vpcChildren.at(0)->dEvaluateNode(cVariables)) + std::string(vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_SUBTRACTION) {
		return std::string(vpcChildren.at(0)->dEvaluateNode(cVariables)) - std::string(vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_MULTIPLICATION) {
		return std::string(vpcChildren.at(0)->dEvaluateNode(cVariables)) * std::string(vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_DIVISION) {

		return std::string(vpcChildren.at(0)->dEvaluateNode(cVariables)) / std::string(vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	
	else if (eOperationType == EOT_SUPERSUM) {
		std::string sSum = "";
		for (int i = 0; i < 4; i++) {
			sSum += (vpcChildren.at(i)->dEvaluateNode(cVariables));
		}
		return sSum;
	}
}

template <>
inline bool CNode<bool>::dCalculateOperation(CVariablesData<bool>* cVariables)
{
	if (eOperationType == EOT_ADDITION) {
		return bool(vpcChildren.at(0)->dEvaluateNode(cVariables) + vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_SUBTRACTION) {
		return bool(vpcChildren.at(0)->dEvaluateNode(cVariables) - vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_MULTIPLICATION) {
		return bool(vpcChildren.at(0)->dEvaluateNode(cVariables) * vpcChildren.at(1)->dEvaluateNode(cVariables));
	}
	else if (eOperationType == EOT_DIVISION) {
		int dDiv = vpcChildren.at(1)->dEvaluateNode(cVariables);
		if (dDiv == 0) {
			throw divByZero();
		}
		return bool(vpcChildren.at(0)->dEvaluateNode(cVariables) / dDiv);
	}

}



template <typename T>
inline E_NODE_TYPE CNode<T>::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, T& dValue, E_ERROR_TYPE& eError)
{
	return ENT_NONE;
}


template <>
inline E_NODE_TYPE CNode<double>::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, double& dValue, E_ERROR_TYPE& eError)
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
	if (CInputParsing::bIsNum(input, false)) {
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


template <>
inline E_NODE_TYPE CNode<int>::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, int& dValue, E_ERROR_TYPE& eError)
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

template <>
inline E_NODE_TYPE CNode<bool>::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, bool& dValue, E_ERROR_TYPE& eError)
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
	if (input == "0" || input == "1") {
		dValue = bool(strtod(input.c_str(), NULL));
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


template <>
inline E_NODE_TYPE CNode<std::string>::eDetermineNodeType(std::string input, int& iNumArguments, E_OPERATION_TYPE& eOpType, std::string& sVarName, std::string& dValue, E_ERROR_TYPE& eError)
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
	if (CInputParsing::bIsStringConst(input)) {
		dValue = input.substr(1, input.length() - 2);
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


inline std::string CInputParsing::sGetNextTokenFromInput(std::string sInput, char cSeparator, int& iOffset)
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


inline bool CInputParsing::bIsNum(std::string sToCheck, bool bIsInt) {
	if (sToCheck.at(0) == '-') {
		sToCheck = sToCheck.substr(1);
	}
	if (bIsInt) {
		for (char c : sToCheck) {
			if (!isdigit(c)) {
				return false;
			}
		}
	}
	else {
		bool bDecimal = false;
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
	}
	return true;
}

inline bool CInputParsing::bIsStringConst(std::string sToCheck)
{
	return sToCheck.at(0) == '"' && sToCheck.at(sToCheck.length() - 1) == '"';
}


inline std::string operator-(const std::string& sL, const std::string& sR)
{
	int pos = sL.rfind(sR);

	if (pos != std::string::npos) {
		std::string sToRet = sL;
		return sToRet.erase(pos, sR.length());
	}
	return sL;
}

inline std::string operator*(const std::string& sL, const std::string& sR)
{
	if (sR == "") {
		return sL;
	}
	char cFirst = sR.at(0);
	std::string sRes = "";
	for (int i = 0; i < sL.length(); i++) {
		if (sL.at(i) == cFirst) {
			sRes += sR;
		}
		else {
			sRes += sL.at(i);
		}
	}
	return sRes;
}

inline std::string operator/(const std::string& sL, const std::string& sR)
{
	if (sR == "") {
		return sL;
	}
	bool bKeepLooking = true;
	int i = 0;
	std::string sRes;
	while (bKeepLooking && i < sL.length()) {
		int iFound = sL.find(sR, i);
		if (iFound == std::string::npos) {
			bKeepLooking = false;
		}
		else {
			sRes += sL.substr(i, iFound + 1 - i);
			i = iFound + sR.length();
		}
	}
	if (i < sL.length()) {

		sRes += sL.substr(i);
	}
	return sRes;
}

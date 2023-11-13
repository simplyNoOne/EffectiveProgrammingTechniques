#pragma once

#include <vector>
#include <string>

class CNode;

class CVariablesData {


	std::vector<std::string> vsVariables;
	std::vector<int> viVariableOccurences;
	std::vector<double> vdVariableValues;

public:
	int iGetVarsNum() const { return vsVariables.size(); }
	void vAddVariable(std::string sVar);
	double dGetVariableData(std::string sVar);
	void vClearVariables() {
		vsVariables.clear();
		viVariableOccurences.clear();
		vdVariableValues.clear();
	}
	void vClearValues() { vdVariableValues.clear(); }
	void vInsertValue(double dValue) { vdVariableValues.push_back(dValue); }
	std::string sGetVarAtPos(int iPos) const { return vsVariables.at(iPos); }
	double dGetValAtPos(int iPos)const { return vdVariableValues.at(iPos); }
	int iSizeComparison()const {
		if (vdVariableValues.size() > vsVariables.size()) {
			return 1;
		}
		if (vdVariableValues.size() < vsVariables.size()) {
			return -1;
		}
		return 0;
	}
	void vDecreaseVarUsage(std::string sVar);
	void vRemoveVarsInNode(CNode* pcNode);
};
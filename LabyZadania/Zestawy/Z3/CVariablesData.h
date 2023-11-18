#pragma once

#include <vector>
#include <string>

class CNode;

class CVariablesData {


public:
	CVariablesData() { bValid = false; }
	inline CVariablesData(const CVariablesData& cOther) {
		bValid = cOther.bValid;
	}

	void vAddVariable(std::string sVar);
	void vClearVariables();

	/*Returns 1 if there is more values than variables, -1 if less, and 0 if both vectors are equal*/
	int iSizeComparison()const;

	double dGetVariableData(std::string sVar);

	inline std::string sGetVarAtPos(int iPos) const { return vsVariables.at(iPos); }
	inline double dGetValAtPos(int iPos)const { return vdVariableValues.at(iPos); }
	inline int iGetVarsNum() const { return vsVariables.size(); }
	inline bool bGetDataValid() { return bValid; }

	inline void vSetDataValid(bool bValid) { this->bValid = bValid; }

	inline void vClearValues() { vdVariableValues.clear(); }
	inline void vInsertValue(double dValue) { vdVariableValues.push_back(dValue); }
	

private:
	bool bValid;
	std::vector<std::string> vsVariables;
	std::vector<int> viVariableOccurences;
	std::vector<double> vdVariableValues;
};
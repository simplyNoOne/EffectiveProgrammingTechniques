#pragma once

#include <vector>
#include <string>


template <typename T> class CVariablesData {


public:
	CVariablesData() { bValid = false; }
	inline CVariablesData(const CVariablesData& cOther) { bValid = cOther.bValid; }

	void vAddVariable(std::string sVar);
	void vClearVariables();

	/*Returns 1 if there is more values than variables, -1 if less, and 0 if both vectors are equal*/
	int iSizeComparison()const;

	T dGetVariableData(std::string sVar);
	T dGetValAtPos(int iPos)const;

	inline std::string sGetVarAtPos(int iPos) const { return vsVariables.at(iPos); }
	
	inline int iGetVarsNum() const { return vsVariables.size(); }
	inline bool bGetDataValid() { return bValid; }

	inline void vSetDataValid(bool bValid) { this->bValid = bValid; }

	inline void vClearValues() { vdVariableValues.clear(); }
	inline void vInsertValue(T dValue) { vdVariableValues.push_back(dValue); }
	

private:
	bool bValid;
	std::vector<std::string> vsVariables;
	std::vector<int> viVariableOccurences;
	std::vector<T> vdVariableValues;
};


template <typename T>
inline void CVariablesData<T>::vAddVariable(std::string sVar) {
	int it = 0;
	while (it < vsVariables.size() && vsVariables.at(it) != sVar) {
		it++;
	}
	if (it == vsVariables.size()) {
		vsVariables.push_back(sVar);
		viVariableOccurences.push_back(1);
	}
	else {
		viVariableOccurences.at(it)++;
	}
}

template <typename T>
inline T CVariablesData<T>::dGetVariableData(std::string sVar) {
	int it = 0;
	while (vsVariables.at(it) != sVar) {
		it++;
	}
	return vdVariableValues.at(it);
}
template<typename T>
inline T CVariablesData<T>::dGetValAtPos(int iPos) const
{
	return vdVariableValues.at(iPos);
}

template<>
inline std::string CVariablesData<std::string>::dGetValAtPos(int iPos) const
{
	return '"' + vdVariableValues.at(iPos) + '"';
}

template <>
inline std::string CVariablesData<std::string>::dGetVariableData(std::string sVar) {
	int it = 0;
	while (vsVariables.at(it) != sVar) {
		it++;
	}
	return vdVariableValues.at(it);
}

template <typename T>
inline void CVariablesData<T>::vClearVariables() {
	vsVariables.clear();
	viVariableOccurences.clear();
	vdVariableValues.clear();
}

template <typename T>
inline int CVariablesData<T>::iSizeComparison() const {
	if (vdVariableValues.size() > vsVariables.size()) {
		return 1;
	}
	if (vdVariableValues.size() < vsVariables.size()) {
		return -1;
	}
	return 0;
}

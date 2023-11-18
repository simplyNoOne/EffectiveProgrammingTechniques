#include "CVariablesData.h"

#include "CNode.h"


void CVariablesData::vAddVariable(std::string sVar)
{
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

double CVariablesData::dGetVariableData(std::string sVar)
{
	int it = 0;
	while (vsVariables.at(it) != sVar) {
		it++;
	}
	return vdVariableValues.at(it);
}

void CVariablesData::vClearVariables()
{
	vsVariables.clear();
	viVariableOccurences.clear();
	vdVariableValues.clear();
}
int CVariablesData::iSizeComparison() const
{
	if (vdVariableValues.size() > vsVariables.size()) {
		return 1;
	}
	if (vdVariableValues.size() < vsVariables.size()) {
		return -1;
	}
	return 0;
}

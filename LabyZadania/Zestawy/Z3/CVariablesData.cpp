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

void CVariablesData::vDecreaseVarUsage(std::string sVar)
{
	int i = 0;
	while (i < vsVariables.size() && vsVariables.at(i) != sVar) {
		i++;
	}
	if (--viVariableOccurences.at(i) == 0) {
		std::vector<int>::iterator itToRemC = viVariableOccurences.begin() + i;
		viVariableOccurences.erase(itToRemC);
		std::vector<std::string>::iterator itToRemV = vsVariables.begin() + i;
		vsVariables.erase(itToRemV);
	}
}

void CVariablesData::vRemoveVarsInNode(CNode* pcNode)
{
	if (pcNode->eGetNodeType() == ENT_VARIABLE) {
		vDecreaseVarUsage(pcNode->sGetVarName());
	}
	else if (pcNode->eGetNodeType() == ENT_OPERATION) {
		for (int i = 0; i < pcNode->pvpcGetChildren()->size(); i++) {
			vRemoveVarsInNode(pcNode->pvpcGetChildren()->at(i));
		}
	}
}

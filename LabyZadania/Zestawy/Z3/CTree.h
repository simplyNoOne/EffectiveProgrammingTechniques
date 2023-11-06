#pragma once

#include<vector>
#include <string>

class CNode;

class CTree {

public:
	inline ~CTree() { delete pcTreeRoot; }

	void vParseFormula(std::string sFormula, std::string& sErrorMessage, char cSeparator);

private:
	CNode* pcTreeRoot;
	std::vector<std::string> vsVariables;
};

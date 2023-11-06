#include "CTree.h"

#include "CNode.h"

void CTree::vParseFormula(std::string sFormula, std::string& sErrorMessage, char cSeparator)
{
	int iSkip = 0;
	while (sFormula[iSkip] == cSeparator) {
		iSkip++;
	}
	sFormula = sFormula.substr(iSkip);
	pcTreeRoot = new CNode(NULL);
	int iOffset = 0;
	pcTreeRoot->bParseNode(sFormula, iOffset, cSeparator, sErrorMessage);
	
}

#include "CInterface.h"

#include "CTree.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>

char CInterface::cSeparator = ' ';

void CInterface::vDisplayOperations()
{
	std::cout << "Available operations:\n"
		<< "\tenter <formula>\n"
		<< "\tvars - show variables\n"
		<< "\tprint - show tree\n"
		<< "\tcomp <var1> ... <varN> - calculate formula with provided variables\n"
		<< "\tjoin <formula> - try adding new formula tree to the current tree\n"
		<< "\tquit\n";
		
}

std::string CInterface::vGetUserInput()
{
	std::cout << "Enter operation: ";
	std::cin >> std::ws;
	std::string sResponse;
	std::getline(std::cin, sResponse);
	return sResponse;
}

E_USER_ACTION CInterface::eInterpretUserAction(std::string& sUserResponse)
{
	int iSepFound = sUserResponse.find(cSeparator);
	std::string keyword; 
	if (iSepFound != std::string::npos) {
		keyword = sUserResponse.substr(0, iSepFound);
		sUserResponse = sUserResponse.substr(iSepFound);
	}
	else {
		keyword = sUserResponse;
		sUserResponse = "";
	}
	for (int i = 0; i < keyword.size(); i++) {
		keyword[i] = std::tolower(keyword[i]);
	}

	if (keyword == "enter") {
		return EUA_ENTER;
	}else if (keyword == "print") {
		return EUA_PRINT;
	}else if (keyword == "comp") {
		return EUA_COMP;
	}else if (keyword == "join") {
		return EUA_JOIN;
	}else if (keyword == "vars") {
		return EUA_VARS;
	}else if (keyword == "quit") {
		return EUA_QUIT;
	}
	return EUA_NONE;
}

void CInterface::vRunInterface()
{
	std::cout << std::fixed << std::setprecision(3);
	pcTree = new CTree();
	E_USER_ACTION eAction = EUA_NONE;
	while (eAction != EUA_QUIT) {
		std::system("cls");
		vDisplayOperations();
		std::string sUserResponse = vGetUserInput();
		eAction = eInterpretUserAction(sUserResponse);
		if (eAction != EUA_QUIT && eAction != EUA_NONE) {
			if (eAction == EUA_ENTER) {
				std::string sMessage;
				pcTree->vClearTree();
				pcTree->vParseFormula(sUserResponse, sMessage, cSeparator);
				std::cout << sMessage<<std::endl;
			}
			else if (eAction == EUA_PRINT) {
				std::cout << "Your current formula in Prefix Notation:\n";
				std::cout << pcTree->sReturnFormula()<<std::endl;
			}
			else if (eAction == EUA_VARS) {
				std::cout<<pcTree->sReturnVariables()<<std::endl;
			}
			else if (eAction == EUA_COMP) {
				if (!pcTree->bExists()) {
					std::cout << "There is nothing to compute yet, for the love of G...\n";
				}
				else {
					std::cout << pcTree->sCompute(sUserResponse, cSeparator)<<std::endl;
				}
			}
			else {
				CTree* pcTempTree = new CTree();
				std::string sMessage;
				pcTempTree->vParseFormula(sUserResponse, sMessage, cSeparator);
				std::cout << sMessage << std::endl;

				std::cout << "\nParsing...\n";
				*pcTree = *pcTree + *pcTempTree;
				delete pcTempTree;
			}
			std::cout << "Press enter to proceed.";
			std::getline(std::cin, sUserResponse);
			
		}
	}
}


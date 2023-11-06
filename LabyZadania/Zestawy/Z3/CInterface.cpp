#include "CInterface.h"

#include "CTree.h"
#include <iostream>
#include <string>
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
				pcTree->vParseFormula(sUserResponse, sMessage, cSeparator);
			}

			std::cout << "Enter to proceed: ";
			std::getline(std::cin, sUserResponse);
			
		}
	}
}


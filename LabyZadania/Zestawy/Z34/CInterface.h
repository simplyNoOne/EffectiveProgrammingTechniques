#pragma once

#include "ETokens.h"
#include <string>

#include "CTree.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "CError.h"
//#include <iostream>

//#include <iomanip>
template <typename T> class CInterface {
public:
	inline CInterface() { pcTree = new CTree<T>(); }
	inline ~CInterface() { delete pcTree; }

	bool vRunInterface();

	//static
	static void vDisplayOperations();
	static std::string vGetUserInput();
	static E_USER_ACTION eInterpretUserAction(std::string& sUserResponse);
	
private:
	CTree<T>* pcTree;

	//static
	static char cSeparator;
};


template <typename T>
char CInterface<T>::cSeparator = ' ';

template <typename T>
inline void CInterface<T>::vDisplayOperations()
{
	std::cout << "Look at all the stuff you can do:\n"
		<< "\tenter <formula>\n"
		<< "\tvars - show variables\n"
		<< "\tprint - show tree\n"
		<< "\tcomp <var1> ... <varN> - calculate formula with provided variables\n"
		<< "\tjoin <formula> - try adding new formula tree to the current tree\n"
		<< "\tswitch - change the formula type\n"
		<< "\tquit\n";

}

template <typename T>
inline std::string CInterface<T>::vGetUserInput()
{
	std::cout << "So, what do you want?: ";
	std::cin >> std::ws;
	std::string sResponse;
	std::getline(std::cin, sResponse);
	return sResponse;
}

template <typename T>
inline E_USER_ACTION CInterface<T>::eInterpretUserAction(std::string& sUserResponse)
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
	}
	else if (keyword == "print") {
		return EUA_PRINT;
	}
	else if (keyword == "comp") {
		return EUA_COMP;
	}
	else if (keyword == "join") {
		return EUA_JOIN;
	}
	else if (keyword == "vars") {
		return EUA_VARS;
	}
	else if (keyword == "switch") {
		return EUA_SWITCH;
	}
	else if (keyword == "quit") {
		return EUA_QUIT;
	}
	return EUA_NONE;
}

template <typename T>
inline bool CInterface<T>::vRunInterface()
{
	pcTree->vClearTree();
	E_USER_ACTION eAction = EUA_NONE;
	while (eAction != EUA_QUIT && eAction != EUA_SWITCH) {
		std::system("cls");
		vDisplayOperations();
		std::string sUserResponse = vGetUserInput();
		eAction = eInterpretUserAction(sUserResponse);
		if (eAction != EUA_QUIT && eAction != EUA_SWITCH && eAction != EUA_NONE) {
			if (eAction == EUA_ENTER) {
				CError cError;
				pcTree->vClearTree();
				pcTree->vParseFormula(sUserResponse, cError, cSeparator);
				if (cError.bErrorFound()) {
					std::cout << cError.sGetErrorMessage() << std::endl;
					std::cout << "\nResulting formula:\n" << pcTree->sReturnFormula();
				}
			}
			else if (eAction == EUA_PRINT) {
				std::cout << "Your current formula in Prefix Notation:\n";
				std::cout << pcTree->sReturnFormula() << std::endl;
			}
			else if (eAction == EUA_VARS) {
				std::cout << pcTree->sReturnVariables() << std::endl;
			}
			else if (eAction == EUA_COMP) {
				if (!pcTree->bExists()) {
					std::cout << "There is nothing to compute yet, for the love of G...\n";
				}
				else {
					try {
						CError cError;
						std::string sResult = pcTree->sCompute(sUserResponse, cSeparator, cError);
						std::cout << sResult << std::endl;
					}
					catch (divByZero& err) {
						std::cout << err.what() << "\nat formula\n" << pcTree->sReturnFormula() << std::endl;
					}
				}
			}
			else {
				if (!pcTree->bExists()) {
					std::cout << "What do you want to join your formula to? There is nothing there yet, you imbecile\n";
				}
				else {
					CTree<T>* pcTempTree = new CTree<T>();
					CError cError;
					pcTempTree->vParseFormula(sUserResponse, cError, cSeparator);
					if (cError.bErrorFound()) {
						std::cout << cError.sGetErrorMessage() << std::endl;
						std::cout << "\nFormula to be appended:\n" << pcTempTree->sReturnFormula();
					}

					std::cout << "\nParsing...\n";
					*pcTree = *pcTree + *pcTempTree;
					delete pcTempTree;
				}

			}
			std::cout << "\nPress enter to proceed.";
			std::getline(std::cin, sUserResponse);

		}
	}
	return eAction == EUA_SWITCH;
}



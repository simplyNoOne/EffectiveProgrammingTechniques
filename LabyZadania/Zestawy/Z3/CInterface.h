#pragma once

#include "ETokens.h"
#include <string>

class CTree;

class CInterface {
public:
	inline CInterface() { pcTree = nullptr; }
	inline ~CInterface() { delete pcTree; }

	void vRunInterface();

	//static
	static void vDisplayOperations();
	static std::string vGetUserInput();
	static E_USER_ACTION eInterpretUserAction(std::string& sUserResponse);
	
private:
	CTree* pcTree;

	//static
	static char cSeparator;
};
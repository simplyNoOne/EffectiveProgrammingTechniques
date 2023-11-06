#pragma once

#include "ETokens.h"
#include <string>

class CTree;

class CInterface {
public:
	inline CInterface() { pcTree = nullptr; }
	inline ~CInterface() { delete pcTree; }
	static void vDisplayOperations();
	static std::string vGetUserInput();
	static E_USER_ACTION eInterpretUserAction(std::string& sUserResponse);
	void vRunInterface();

private:
	CTree* pcTree;
	static char cSeparator;
};
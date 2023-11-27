#pragma once
#include "ETokens.h"
#include <string>
#include "CInterface.h"


class CBossInterface {
public:
	CBossInterface();
	~CBossInterface();
	void vRun();

private:
	E_USER_TYPE eGetType();
	bool bRunSubInterface();

	E_USER_TYPE eChosenType = EUT_NONE;
	CInterface<int>* iInterface;
	CInterface<double>* dInterface;
	CInterface<std::string>* sInterface;
};

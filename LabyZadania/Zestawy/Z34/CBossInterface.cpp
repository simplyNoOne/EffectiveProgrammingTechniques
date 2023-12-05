#include "CBossInterface.h"
//#include "CInterface.h"

#include <iostream>

CBossInterface::CBossInterface() {
	iInterface = new CInterface<int>();
	dInterface = new CInterface<double>();
	sInterface = new CInterface<std::string>();
	bInterface = new CInterface<bool>();
}
CBossInterface::~CBossInterface() {
	delete iInterface;
	delete dInterface;
	delete sInterface;
	delete bInterface;
}

void CBossInterface::vRun() {

	std::cout << std::fixed << std::setprecision(3);
	bool bRun = true;
	while (bRun) {
		eChosenType = eGetType();
		bRun = bRunSubInterface();
	}

}

E_USER_TYPE CBossInterface::eGetType()
{
	std::string sChoice = "";
	while (sChoice == "") {
		std::cout << "Choose type: (int/string/double/bool): ";
		std::cin >> sChoice;
		if (sChoice == "int") {
			return EUT_INT;
		}
		if (sChoice == "double") {
			return EUT_DOUBLE;
		}
		if (sChoice == "string") {
			return EUT_STRING;
		}
		if (sChoice == "bool") {
			return EUT_BOOL;
		}
		sChoice = "";
		std::cout << "\nIllegal type, choose again!\n";
	}
	return EUT_NONE;
}

bool CBossInterface::bRunSubInterface()
{
	if (eChosenType == EUT_INT) {
		return iInterface->vRunInterface();
	}
	else if (eChosenType == EUT_DOUBLE) {
		return dInterface->vRunInterface();
	}
	else if (eChosenType == EUT_STRING) {
		return sInterface->vRunInterface();
	}
	else if (eChosenType == EUT_BOOL) {
		return bInterface->vRunInterface();
	}
	return false;
}

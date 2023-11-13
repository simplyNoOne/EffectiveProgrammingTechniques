#pragma once

#include "ETokens.h"
#include <string>

class CError {
	E_ERROR_TYPE eType;
	std::string sErrorMessage;
	bool bIsError;
public:
	CError() { bIsError = false; eType = EET_NONE; }
	void vSetType(E_ERROR_TYPE eErrorType) { eType = eErrorType; }
	E_ERROR_TYPE eGetType() { return eType; }
	void vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType, std::string sOperation);
	void vAppendMessage(bool bComputing, std::string sOperation);
	void vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType);
	bool bErrorFound() { return bIsError; }
	inline std::string sGetErrorMessage() { return sErrorMessage; }
	inline void vClearMessage() {
		sErrorMessage.clear();
		bIsError = false;
	}
	std::string sMatchMessage(bool bComputing, E_ERROR_TYPE eType);
};
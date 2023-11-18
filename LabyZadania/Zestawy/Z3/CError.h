#pragma once

#include "ETokens.h"
#include <string>
#include <stdexcept>

class divByZero : public std::exception {
public:
	inline virtual const char* what() const throw() {
		return "Division by zero is forbidden, are you retarded?";
	}
};

class CError {

public:
	inline CError() { bIsError = false; eType = EET_NONE; }

	void vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType, std::string sOperation);
	void vAppendMessage(bool bComputing, std::string sOperation);
	void vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType);

	inline void vClearMessage() { sErrorMessage.clear(); bIsError = false; }
	
	inline bool bErrorFound() { return bIsError; }
	inline E_ERROR_TYPE eGetType() { return eType; }
	inline std::string sGetErrorMessage() { return sErrorMessage; }
	
	inline void vSetType(E_ERROR_TYPE eErrorType) { eType = eErrorType; bIsError = true; }


private:
	E_ERROR_TYPE eType;
	std::string sErrorMessage;
	bool bIsError;

private:
	std::string sMatchMessage(bool bComputing, E_ERROR_TYPE eType);
};
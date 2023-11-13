#include "CError.h"

void CError::vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType, std::string sOperation)
{
	if (eErrorType == EET_NONE) {
		return;
	}
	sErrorMessage += "\n\t";
	sErrorMessage += sMatchMessage(bComputing, eErrorType);
	sErrorMessage += " ( Operation " + sOperation;
	sErrorMessage += ")";
	bIsError = true;
}

void CError::vAppendMessage(bool bComputing, std::string sOperation)
{
	if (eType == EET_NONE) {
		return;
	}
	sErrorMessage += "\n\t";
	sErrorMessage += sMatchMessage(bComputing, eType);
	sErrorMessage += " ( Operation " + sOperation;
	sErrorMessage += ")";
	bIsError = true;
}

void CError::vAppendMessage(bool bComputing, E_ERROR_TYPE eErrorType)
{
	if (eErrorType == EET_NONE) {
		return;
	}
	sErrorMessage += "\n\t";
	sErrorMessage += sMatchMessage(bComputing, eErrorType);
	bIsError = true;
}

std::string CError::sMatchMessage(bool bComputing, E_ERROR_TYPE eType){
	
	if (bComputing) {
		if (eType == EET_INVALID_ARGUMENT) {
			return "Variables can only be numbers, didn't you know?";
		}
		if (eType == EET_MISSING_ARGUMENT) {
			return "You didn't pass in enough values for variables, can't you count?";
		}
		if (eType == EET_TOO_MANY_ARGUMENTS) {
			return "You passed in too many values, learn to count for God's sake!";
		}
	}
	else {
		if (eType == EET_TOO_MANY_ARGUMENTS) {
			return "To many arguments bro, R U blind?";
		}
		if (eType == EET_INVALID_ARGUMENT) {
			return "Jokes on you, variable names cannot contain special symbols";
		}
		if (eType == EET_MISSING_ARGUMENT) {
			return "Bruh, you didn't inclue the whole formula";
		}
	}
	return "";
}

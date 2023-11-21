#include "z34.h"
#include "CInterface.h"

#include "CTree.h"
#include "CError.h"



void vZ34start()
{
	CInterface < std::string > * cUserInterface = new CInterface<std::string>();

	cUserInterface->vRunInterface();

	delete cUserInterface;


}

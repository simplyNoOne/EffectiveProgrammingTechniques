#include "z3.h"
#include "CInterface.h"

#include "CTree.h"
#include "CError.h"


template class CInterface<int>; 

void vZ3start()
{
	CInterface < std::string > * cUserInterface = new CInterface<std::string>();

	cUserInterface->vRunInterface();

	delete cUserInterface;


}

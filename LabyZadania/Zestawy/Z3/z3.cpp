#include "z3.h"
#include "CInterface.h"

void vZ3start()
{
	CInterface* cUserInterface = new CInterface();

	cUserInterface->vRunInterface();

	delete cUserInterface;

}

#include "z34.h"
#include "CBossInterface.h"

void vZ34start()
{
	//CNode<int>* dsdf = new CNode<int>(nullptr);
	CBossInterface* cUserInterface = new CBossInterface();

	cUserInterface->vRun();

	delete cUserInterface;


}

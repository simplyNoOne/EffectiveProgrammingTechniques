#include "z3.h"
#include "CInterface.h"

#include "CTree.h"
#include "CError.h"


void vZ3start()
{
	//CInterface* cUserInterface = new CInterface();

	//cUserInterface->vRunInterface();

	//delete cUserInterface;

	CTree t1;
	CError cErr;
	t1.vParseFormula("+ 7 - y - 3 * u + P 2", cErr , ' ');

	CTree t2;
	t2.vParseFormula("+ * 3 y + 2 - + e 6 * u - P 2", cErr, ' ');

}

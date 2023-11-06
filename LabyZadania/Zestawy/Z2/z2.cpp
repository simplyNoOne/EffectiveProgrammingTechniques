#include "z2.h"

#include <iostream>
#include "CNumber.h"
#include "tests.h"

using namespace Tests;

void vZ2start()
{
	
	
    vRunTests();
  
    
    CNumber::vSetSystemBase(7);
    CNumber test("1000000000000000000000000000000000000000000000");
    std::cout << (test / 100) << std::endl;
    std::cout << (test / CNumber("202"))<<std::endl;
    std::cout << (test / 49) << std::endl;
    std::cout << (test / CNumber("100")) << std::endl;
    std::cout << (test % 7) << std::endl;

    CNumber::vSetSystemBase(13);
    CNumber n1("12 12 11 10 8 9 3 0 1 12 2 1 2 3 4 5 6 7 8 9 0 0 0 0 0 10 12 11 10 9 10 2 1 3 7 6 5 0");
    CNumber n2("-10 0 0 0 0 10 12 11 10 9 10 2 1 3 7 6 5 0 9 8 12 6 7");
    std::cout << (n1 + n2) << std::endl;
    std::cout << (n1 - n2) << std::endl;
    std::cout << (n1 * n2) << std::endl;
    std::cout << (n1 / n2) << std::endl;
    std::cout << (n2 % 13) << std::endl;
    
    

    CNumber::vSetSystemBase(10);
    CNumber n3("-7326835628476235864523875632876239857329876395878753290523895283753029253728657869835909023507823059832032859");
    CNumber n4("-1837298789375298532737");
    std::cout << (n3 + n4) << std::endl;
    std::cout << (n3 - n4) << std::endl;
    std::cout << (n3 * n4) << std::endl;
    std::cout << (n3 / n4) << std::endl;
    std::cout << (n3 % 10) << std::endl;


    CNumber dzielna = 45;
    CNumber rest;
    std::cout << dzielna.cDivRest(8,&rest) << std::endl;
    std::cout << rest << std::endl;

    CNumber dzielna2 = -45;
    CNumber rest2;
    std::cout << dzielna2.cDivRest(-3,&rest2) << std::endl;
    std::cout << rest2 << std::endl;
    
}

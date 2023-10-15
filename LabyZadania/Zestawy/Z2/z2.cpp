#include "z2.h"

#include <iostream>
#include "CNumber.h"

void vZ2start()
{
	CNumber c123;
	c123 = 123;

	CNumber c99;
	c99 = 99;

	CNumber res;
	res = c99 - c123;
	std::cout<<res.sToString()<<std::endl;

}

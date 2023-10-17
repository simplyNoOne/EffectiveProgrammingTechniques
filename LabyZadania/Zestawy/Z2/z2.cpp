#include "z2.h"

#include <iostream>
#include "CNumber.h"

void vZ2start()
{
	CNumber c123;
	c123 = 123;

	CNumber c99;
	c99 = 99;

	CNumber c1024(1024);

	CNumber res;
	res = c99 - c123;
	std::cout<<res.sToString()<<std::endl;

	res = c1024 - c99;
	std::cout << res.sToString() << std::endl;

	res = c99 - c1024;
	std::cout << res.sToString() << std::endl;

	res = c123 - 666;
	std::cout << res.sToString() << std::endl;

	
}

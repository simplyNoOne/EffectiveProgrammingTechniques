#include "z2.h"

#include <iostream>
#include "CNumber.h"

void vZ2start()
{
	//CNumber::vSetSystemBase(12);

	CNumber test(187379090);
	//test / 12;
	std::cout << test << std::endl;

	
	CNumber c123;
	c123 = 123;

	CNumber c99;
	c99 = 99;

	CNumber c1024(1024);

	CNumber res;
	res = c99 * c123;		//12177
	std::cout<<res.sToString()<<std::endl;

	res = c1024 - c99;
	res  += 1;
	std::cout << res.sToString() << std::endl;

	res = c99 - c1024;
	std::cout << res.sToString() << std::endl;

	res = c123 - 666;
	std::cout << res.sToString() << std::endl;

	CNumber c12 = 12;
	res = c12 * c12;
	std::cout << res.sToString() << std::endl;

	c12 = -c12;

	c12 = "001";
	std::cout << c12 << std::endl;

	res = res / c12;
	std::cout << res.sToString() << std::endl;

	/*CNumber cBigDividend(195950421);
	//CNumber cBigDividend (19921);
	std::cout << (cBigDividend / 1).sToString() << std::endl;
	std::cout << (cBigDividend / 8975).sToString() << std::endl;
	std::cout << (cBigDividend / 643).sToString() << std::endl;
	std::cout << (cBigDividend / 21).sToString() << std::endl;
	std::cout << (cBigDividend / 12034).sToString() << std::endl;
	std::cout << (cBigDividend / 991).sToString() << std::endl;
	std::cout << (cBigDividend / 3592).sToString() << std::endl;
	std::cout << (cBigDividend / -5253).sToString() << std::endl;
	std::cout << (cBigDividend / -7819).sToString() << std::endl;
	std::cout << (cBigDividend / -38) << std::endl;
	std::cout << (cBigDividend / -9) << std::endl;

	*/
	//BUT HOW ABOUT NUMBERS GREATER THAN INT?
	//HOW TO GET 972361962_41564218_897421???
	// Answer: EASY:
	//take 41564218, multiply by 10^6
	//add 897421
	//multiply 10^8 by 10^6 to get 10^14
	//take 972361962, multiply by 10^14
	//add the previous result

	CNumber biggie(((CNumber(41564218) * std::pow(10, 6)) + CNumber(897421)) + (CNumber(972361962) * CNumber(std::pow(10, 8)) * std::pow(10, 6)));
	//see, it works
	std::cout << biggie<< std::endl;

	std::cout << 7 + biggie << std::endl;

	//res = c1024 / 1;
	//std::cout << res.sToString() << std::endl;

	
	/*
	CNumber c8 = 22;
	CNumber nc3 = -10;
	CNumber res;

	res = 0;

	//return;
	res = c8 + nc3;
	std::cout << res.sToString() << std::endl;

	res = c8 - nc3;
	std::cout << res.sToString() << std::endl;

	res = nc3 - c8;
	std::cout << res.sToString() << std::endl;

	res = c8 * nc3;
	std::cout << res.sToString() << std::endl;

	CNumber c5 = -5;

	res = c5/ 2;
	std::cout << res.sToString() << std::endl;

	res = c8 / nc3;
	std::cout << res.sToString() << std::endl;

	*/

}

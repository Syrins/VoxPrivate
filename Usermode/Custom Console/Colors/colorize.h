#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include "colorout.h"

namespace colorize {

	void PrintColorful(std::string item, int color)
	{
		switch (color)
		{
			case 0: // red
				std::cout << clr::red << item << clr::reset << std::endl;
				break;
			case 1: // cyan
				std::cout << clr::cyan << item << clr::reset << std::endl;
				break;
			case 2: // yellow
				std::cout << clr::yellow << item << clr::reset << std::endl;
				break;
			case 3: // green
				std::cout << clr::green << item << clr::reset << std::endl;
				break;
			case 4: // blue 
				std::cout << clr::blue << item << clr::reset << std::endl;
				break;
			case 5: // purple
				std::cout << clr::magenta << item << clr::reset << std::endl;
				break;
			case 6:
				std::cout << clr::grey << item << clr::reset << std::endl;
				break;


		}

	}



	void coolPrint(std::string item, int type)
	{
		switch (type)
		{
			case 0:
				std::cout << clr::reset << " \n [ " << clr::green << "Logged In" << clr::reset << " ] "  << item;
				break;
			case 1:
				std::cout << clr::reset << "\n  [ " << clr::magenta << "<-->" << clr::reset << " ] " << item;
				break;
			case 2:
				std::cout << clr::reset << "\n  [ " << clr::blue << "->" << clr::reset << " ] " << item;
				break;
			case 3:
				std::cout << clr::reset <<  "\n  [ " << clr::red << "fail" << clr::reset << " ] " << item;
				break;
		}
	}
}
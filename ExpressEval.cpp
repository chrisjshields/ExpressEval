// ExpressEval.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Expression.h"

int main()
{
	const std::string str = "7*28-(4+(2*(33)/3))";
	float result;
	const bool success = Expression::Evaluate(str, result);
	if (success)
		std::cout << "= " << result; 
	else
		std::cout << std::endl << "Error in input." << std::endl;
}

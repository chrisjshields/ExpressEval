// ExpressEval.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Expression.h"

void test(const std::string expression)
{
	float result;
	const bool success = Expression::Evaluate(expression, result);
	
	if (success)
		std::cout << expression << " = " << result << std::endl;
	else
		std::cout << expression << ": " << "Error in input." << std::endl;
}

int main()
{
	test("1 + 3");
	test("(1 + 3) * 2");
	test("(4 / 2) + 6");
	test("4 + (12 / (1 * 2))");
	test("(1 + (12 * 2)");
}
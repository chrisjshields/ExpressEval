#pragma once

class Expression
{
public:
	static bool Evaluate(std::string expression, float &result);
private:
	static bool Calc(char op, float operand, float &total);
};

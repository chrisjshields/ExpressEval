#pragma once

class Expression
{
public:
	static bool Evaluate(std::string expression, float & result);
private:
	static bool Calc(char op, const float operand, float & total);
};

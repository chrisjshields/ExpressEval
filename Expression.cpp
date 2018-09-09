#include "pch.h"
#include "Expression.h"

bool Expression::Evaluate(std::string expression, float & result)
{
	float operand = 0;
	bool operand_set = false;
	int parentheses_start_offset = -1;
	int parentheses_open = 0;
	char op = '+'; // Perform addition first to load initial value
	result = 0;

#if _DEBUG
	std::cout << expression;
#endif

	for (unsigned int i = 0; i < expression.length(); i++)
	{
		const char c = expression.at(i);

		if (c == ' ') // Ignore whitespace
			continue;

		// Store starting character offset of opening parentheses  and incremement open count
		if (c == '(')
		{
			if (parentheses_open == 0)
				parentheses_start_offset = i + 1;

			parentheses_open++;
			continue;
		}

		// Decrement open parentheses count
		if (c == ')')
			parentheses_open--;

		// Do not allow closed parentheses without corresponding open
		if (parentheses_open < 0)
			return false;

		// If all parentheses have been closed, calculate nested expression  
		if (parentheses_start_offset != -1 && parentheses_open == 0)
		{
			const int parenthesis_length = i - parentheses_start_offset;

			if (parenthesis_length > 0)
			{
				float sub_result;
#if _DEBUG
				std::cout << std::endl << op << "[";
#endif
				if (!Evaluate(expression.substr(parentheses_start_offset, parenthesis_length), sub_result))
					return false;

#if _DEBUG
				std::cout << "]=" << sub_result;
#endif
				operand = sub_result;
				operand_set = true;
				parentheses_start_offset = -1;
				continue;
			}
		}

		// Skip over remaining characters if parenthesis are open (before evaluating them recursively)
		if (parentheses_open)
			continue;

		if (std::isdigit(c))
		{
			// Convert character to integer
			const int digit = c - '0';

			// Mulitply any previous digit by 10 to shift it left in base 10
			operand = (operand * 10) + digit;
			operand_set = true;
		}
		else if (operand_set)
		{
			if (!Calc(op, operand, result))
				return false; // Invalid operator

			operand = 0;
			operand_set = false;
			op = c;
		}
		else
		{
			return false; // No operand specified - invalid input
		}
	}

	if (parentheses_open)
		return false; // Open parentheses at end of input

	// Perform final calculation
	return Calc(op, operand, result);
}

/*
 * Perform simple calculation on total, given an operator and operand
 */
bool Expression::Calc(const char op, const float operand, float & total)
{
	switch (op)
	{
		case '+': total += operand; break;
		case '-': total -= operand; break;
		case '*': total *= operand; break;
		case '/': total /= operand; break;
		default: return false;
	}

	return true;
}

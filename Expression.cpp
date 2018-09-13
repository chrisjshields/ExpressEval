#include "pch.h"
#include "Expression.h"

/**
 * Evaluate mathematical expression within a string.
 * 
 * expression: Expression to evaluate (using std::string for substr function - otherwise we would need to malloc and memcopy
 * result: Pointer to location in which to store the result
 * 
 * Returns true on success
 */
bool Expression::Evaluate(const std::string expression, float &result)
{
	float operand = 0;
	bool operand_set = false;
	int nested_expression_start_offset = -1;
	int parentheses_open = 0;
	char op = '+'; // Perform addition first to load initial value
	result = 0;

	for (unsigned int i = 0; i < expression.length(); i++)
	{
		const char c = expression.at(i);

		switch (c)
		{
		case ' ': break; // Ignore whitespace
		case '(':
			// Store starting character offset of opening parentheses and incremement open count
			if (!parentheses_open)
				nested_expression_start_offset = i + 1;

			parentheses_open++;
			break;
		case ')':
			// Decrement open parentheses count
			parentheses_open--;

			if (parentheses_open < 0)
				return false; // Closed parentheses without corresponding open

			if (!parentheses_open) // If all parentheses have been closed, calculate nested expression 
			{
				const int nested_expression_length = i - nested_expression_start_offset;

				if (nested_expression_length == 0)
					return false; // Empty nested expression

				float sub_result;

				// Recursively call self
				if (!Evaluate(expression.substr(nested_expression_start_offset, nested_expression_length), sub_result))
					return false;

				// Load operand with result of nested expression
				operand = sub_result;
				operand_set = true;
				nested_expression_start_offset = -1;
			}
			break;
		default:
			if (parentheses_open) // Skip calculation if parentheses open
				break;

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
					return false; // Invalid operator or divide by 0

				operand = 0;
				operand_set = false;
				op = c;
			}
			else
			{
				return false; // Not a digit and no operand specified - invalid input
			}
		}
	}

	if (parentheses_open)
		return false; // Fail if there are open parentheses at end of expression

	// Perform final calculation
	return Calc(op, operand, result);
}

/*
 * Perform simple calculation on total, given an operator and operand
 * 
 * op: Operation to perform
 * operand: Operand for operation
 * total: Running total
 * 
 * Returns true on success
 */
bool Expression::Calc(const char op, const float operand, float &total)
{
	// Check for divide by zero, as opposed to allowing an exception
	if (op == '/' && operand == 0)
		return false;

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

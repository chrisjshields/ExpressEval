#include "pch.h"
#include "Expression.h"

/**
 * Evaluates a mathematical expression within a string
 * 
 * expression: Expression to evaluate (using std::string for substr function - otherwise we would need to malloc and memcopy)
 * result: Pointer to location in which to store the result
 * 
 * Returns: true on success
 */
bool Expression::Evaluate(const std::string expression, float &result)
{
	char op = '+'; // Perform addition first to load initial value
	float operand = 0;
	bool operand_set = false;
	int parentheses_open = 0;
	int nested_expression_start_offset = 0;
	result = 0;

	for (unsigned int i = 0; i < expression.length(); i++)
	{
		const char c = expression.at(i);

		switch (c)
		{
			case ' ': break; // Ignore whitespace
			case '(':
			{
				// Store starting character offset of opening parentheses and incremement open count
				if (!parentheses_open++)
					nested_expression_start_offset = i + 1;
				break;
			}
			case ')':
			{
				if (--parentheses_open < 0)
					return false; // Closed parentheses without corresponding open

				if (parentheses_open)
					break; // If original parentheses are still open, continue to next character

				const int nested_expression_length = i - nested_expression_start_offset;
				if (nested_expression_length < 1)
					return false; // Empty nested expression

				float sub_result;
				if (!Evaluate(expression.substr(nested_expression_start_offset, nested_expression_length), sub_result))
					return false; // Syntax error in nested expression

				// Load operand with result of nested expression
				operand = sub_result;
				operand_set = true;
				break;
			}
			case '+':
			case '-':
			case '*':
			case '/':
			{
				if (parentheses_open)
					break; // Continue to next character if parentheses open

				if (!operand_set)
					return false; // Operation without an operand

				// Perform previous operation (operand input is complete)
				if (!Calc(op, operand, result))
					return false; // Divide by zero

				// Reset operand and store new operator for next calculation
				operand = 0;
				operand_set = false;
				op = c;
				break;
			}
			default:
			{
				if (parentheses_open)
					break;  // Continue to next character if parentheses open

				if (!std::isdigit(c))
					return false; // Not a recognised character

				// Convert character to integer
				const int digit = c - '0';

				// Mulitply any previous digit by 10 to shift it left in base 10
				operand = (operand * 10) + digit;
				operand_set = true;
			}
		}
	}

	if (parentheses_open)
		return false; // Open parentheses at end of expression

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
 * Returns: true on success
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

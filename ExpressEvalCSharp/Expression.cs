using System;

namespace ExpressEvalCSharp
{
    internal class Expression
    {
        /// <summary>
        /// Evaluates a mathematical expression within a string
        /// </summary>
        /// <param name="expression">Expression to evaluate </param>
        /// <param name="result">Location to store result</param>
        /// <returns>True on success</returns>
        internal static bool Evaluate(string expression, out float result)
        {
            char op = '+'; // Perform addition first to load initial value
            float operand = 0;
            bool operandSet = false;
            int parenthesesOpen = 0;
            int nestedExpressionStartOffset = 0;
            result = 0;

            for (int i = 0; i < expression.Length; i++)
            {
                char c = expression[i];

                switch (c)
                {
                    case ' ': break; // Ignore whitespace
                    case '(':
                    {
                        // Store starting character offset of opening parentheses and incremement open count
                        if (parenthesesOpen++ == 0)
                            nestedExpressionStartOffset = i + 1;
                        break;
                    }
                    case ')':
                    {
                        if (--parenthesesOpen < 0)
                            return false; // Closed parentheses without corresponding open

                        if (parenthesesOpen > 0)
                            break; // If original parentheses are still open, continue to next character

                        int nestedExpressionLength = i - nestedExpressionStartOffset;
                        if (nestedExpressionLength < 1)
                            return false; // Empty nested expression

                        // Evaluate nested expression
                        if (!Evaluate(expression.Substring(nestedExpressionStartOffset, nestedExpressionLength), out float subResult))
                            return false; // Syntax error in nested expression

                        // Load operand with result of nested expression
                        operand = subResult;
                        operandSet = true;
                        break;
                    }
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    {
                        if (parenthesesOpen > 0)
                            break; // Continue to next character if parentheses open

                        if (!operandSet)
                            return false; // Operation without an operand

                        // Perform previous operation (operand input is complete)
                        if (!Calc(op, operand, ref result))
                            return false; // Divide by zero

                        // Reset operand and store new operator for next calculation
                        operand = 0;
                        operandSet = false;
                        op = c;
                        break;
                    }
                    default:
                    {
                        if (parenthesesOpen > 0)
                            break; // Continue to next character if parentheses open

                        if (!char.IsDigit(c))
                            return false; // Not a recognised character

                        // Convert character to integer
                        int digit = c - '0';

                        // Mulitply any previous digit by 10 to shift it left in base 10
                        operand = (operand * 10) + digit;
                        operandSet = true;
                        break;
                    }
                }
            }

            if (parenthesesOpen > 0)
                return false; // Open parentheses at end of expression

            // Perform final calculation
            return Calc(op, operand, ref result);
        }

        /// <summary>
        /// Perform simple calculation on total, given an operator and operand
        /// </summary>
        /// <param name="op">Operation to perform</param>
        /// <param name="operand">Operand for operation</param>
        /// <param name="total">Running total</param>
        /// <returns>True on success</returns>
        internal static bool Calc(char op, float operand, ref float total)
        {
            // Check for divide by zero, as opposed to allowing an exception
            if (op == '/' && operand == 0)
                return false;

            switch (op)
            {
                case '+':
                    total += operand;
                    return true;
                case '-':
                    total -= operand;
                    return true;
                case '*':
                    total *= operand;
                    return true;
                case '/':
                    total /= operand;
                    return true;
                default:
                    return false;
            }
        }
    }
}

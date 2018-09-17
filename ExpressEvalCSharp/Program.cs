using System;

namespace ExpressEvalCSharp
{
    internal class Program
    {
        private static void Test(string expression)
        {
            bool success = Expression.Evaluate(expression, out float result);
            Console.WriteLine(success ? $"{expression} = {result}" : $"{expression}: Error in input.");
        }

        private static void Main(string[] args)
        {
            Test("1 + 3");
            Test("(1 + 3) * 2");
            Test("(4 / 2) + 6");
            Test("4 + (12 / (1 * 2))");
            Test("(1 + (12 * 2)");
            Console.ReadLine();
        }
    }
}

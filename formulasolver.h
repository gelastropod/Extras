#include <stack>
#include <map>
#include <functional>
#include <string>

int precedence(char oper)
{
	switch (oper)
	{
	case '+':
	case '-':
		return 0;
	case '*':
	case '/':
		return 1;
	case '^':
		return 2;
	default:
		return -1;
	}
}

std::string pop_until(std::function<bool(char)> until, std::stack<char> &pop)
{
	std::string out = "";
	if (pop.empty()) return out;
	while (until(pop.top()))
	{
		out += pop.top();
		pop.pop();
	}
	return out;
}

std::string infix_to_postfix(std::string infix)
{
	std::string postfix = "";
	std::stack<char> stac;
	for (char i : infix)
	{
		if (isspace(i)) continue;
		if (isdigit(i))
		{
			postfix += i;
			continue;
		}
		if (stac.empty() || i == '(')
		{
			postfix += ' ';
			stac.push(i);
			continue;
		}
		if (i == ')')
		{
			postfix += pop_until([](char i) -> bool { return i != '('; }, stac);
			stac.pop();
			continue;
		}
		if (stac.top() == '(')
		{
			postfix += ' ';
			stac.push(i);
			continue;
		}
		if (precedence(i) < precedence(stac.top()))
		{
			postfix += pop_until([=](char in) -> bool { return precedence(i) < precedence(in); }, stac);
		}
		else if (precedence(i) > precedence(stac.top()))
		{
			postfix += ' ';
			stac.push(i);
			continue;
		}
		if (precedence(i) != precedence(stac.top())) continue;
		if (i == '^')
		{
			postfix += ' ';
			stac.push(i);
		}
		else postfix += i;
	}
	while (!stac.empty())
	{
		postfix += stac.top();
		stac.pop();
	}
	return postfix;
}

long double solveoper(char oper, long double operand1, long double operand2)
{
	switch (oper)
	{
	case '+':
		return operand1 + operand2;
	case '-':
		return operand1 - operand2;
	case '*':
		return operand1 * operand2;
	case '/':
		return operand1 / operand2;
	case '^':
		return pow(operand1, operand2);
	}
}

long double postfixsolver(std::string postfix)
{
	std::stack<long double> stac;
	std::string num = "";
	long double a, b;
	for (char i : postfix)
	{
		if (isdigit(i))
		{
			num += i;
			continue;
		}
		if (num != "")
		{
			stac.push(atoi(num.c_str()));
			num = "";
		}
		if (isspace(i)) continue;
		b = stac.top(); stac.pop();
		a = stac.top(); stac.pop();
		stac.push(solveoper(i, a, b));
	}
	return stac.top();
}

std::string formulasolver(std::string formula)
{
	return std::to_string(postfixsolver(infix_to_postfix(formula)));
}
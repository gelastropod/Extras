#include <string>
#include <functional>
#include <vector>
#include <set>
#include <cstdio>
#include <fstream>

const double euler = std::exp(1.0);

long double decimalcomp(long double num)
{
	return num - floorl(num);
}

long double exponent(long double base, long double power)
{
	return exp2l(power * log2l(base));
}

long double logarithm(long double base, long double oper)
{
	return exp2l(oper) / exp2l(base);
}

long double intexp(long double base, int power)
{
	int ans = 1;
	for (int i = 0; i < power; i++)
	{
		ans *= base;
	}
	return ans;
}

std::string roundtodp(std::string inp, int dp)
{
	std::string out = "";
	int count = 0;
	bool decimalpt = false;
	for (char i : inp)
	{
		if (decimalpt)
			count++;
		if (i == '.')
			decimalpt = true;
		if (count > dp)
			return out;
		out += i;
	}
	if (!decimalpt)
		out += '.';
	for (; count < dp; count++)
		out += ' ';
	return out;
}

struct bigNumber
{
	std::string data;
	long double first, second;
	int state;

	void converttostring()
	{
		if (state == 1)
			data = roundtodp(std::to_string(first), 3);
		else if (state == 2)
			data = roundtodp(std::to_string(first), 3) + "e" + roundtodp(std::to_string(second), 3);
		else if (state == 3)
			data = "ee" + roundtodp(std::to_string(first), 3);
	}

	void check()
	{
		if (state == 1 && first > 1000000000)
		{
			state = 2;
			while (first > 10)
			{
				first /= 10;
				second++;
			}
		}
		else if (state == 2 && second > 1000000)
		{
			state = 3;
			first = log10l(log10l(first) + second);
			second = 0;
		}
		if (state == 3 && first <= 6)
		{
			state = 2;
			second = floorl(exponent(10, first));
			first = exponent(10, decimalcomp(exponent(10, first)));
		}
		if (state == 2)
		{
			while (first < 1 && second > 0)
			{
				second--;
				first *= 10;
			}
			while (first > 10)
			{
				second++;
				first /= 10;
			}
		}
		if (state == 2 && second <= 9)
		{
			state = 1;
			first = first * exponent(10, second);
		}
		converttostring();
	}
	
	bigNumber(long double first_, long double second_, int state_)
	{
		first = first_;
		second = second_;
		state = state_;
		check();
	}

	bigNumber operator+(bigNumber other)
	{
		bigNumber out(0, 0, 1);
		if (state != other.state)
		{
			std::cerr << "Tried to add two bigNumbers with different \"state\".";
			abort();
		}
		if (state == 1)
			out.first = first + other.first;
		else if (state == 2)
		{
			out.state = 2;
			out.second = std::max(second, other.second);
			if (second > other.second)
				out.first = first + other.first / exponent(10, second - other.second);
			else
				out.first = other.first + first / exponent(10, other.second - second);
		}
		else if (state == 3)
		{
			out.state = 3;
			out.first = std::max(first, other.first);
		}
		out.check();
		return out;
	}
	
	bigNumber operator-(bigNumber other)
	{
		bigNumber out(0, 0, 1);
		if (state != other.state)
		{
			std::cerr << "Tried to subtract two bigNumbers with different \"state\".";
			abort();
		}
		if (state == 1)
			out.first = first - other.first;
		else if (state == 2)
		{
			out.state = 2;
			out.second = std::max(second, other.second);
			if (second > other.second)
				out.first = first - other.first / exponent(10, second - other.second);
			else
				out.first = other.first - first / exponent(10, other.second - second);
		}
		else if (state == 3)
		{
			if (first == other.first)
			{
				out.state = 1;
				out.check();
				return out;
			}
			out.state = 3;
			out.first = std::max(first, other.first);
		}
		out.check();
		return out;
	}

	bigNumber operator*(bigNumber other)
	{
		bigNumber out(0, 0, 1);
		if (state != other.state)
		{
			std::cerr << "Tried to multiply two bigNumbers with different \"state\".";
			abort();
		}
		if (state == 1)
			out.first = first * other.first;
		else if (state == 2)
		{
			out.state = 2;
			out.first = first * other.first;
			out.second = second + other.second;
		}
		else if (state == 3)
		{
			out.state = 3;
			if (fabsl(first - other.first) > 6)
			{
				out.first = std::max(first, other.first);
				out.check();
				return out;
			}
			if (first > other.first)
				out.first = other.first + log10l(1 + exponent(10, first - other.first));
			else
				out.first = first + log10l(1 + exponent(10, other.first - first));
		}
		out.check();
		return out;
	}

	bigNumber operator/(bigNumber other)
	{
		bigNumber out(0, 0, 1);
		if (state != other.state)
		{
			std::cerr << "Tried to divide two bigNumbers with different \"state\".";
			abort();
		}
		if (state == 1)
			out.first = first / other.first;
		else if (state == 2)
		{
			out.state = 2;
			out.first = first / other.first;
			out.second = second - other.second;
		}
		else if (state == 3)
		{
			out.state = 3;
			if (fabsl(first - other.first) > 6)
			{
				out.first = std::max(first, other.first);
				out.check();
				return out;
			}
			if (first > other.first)
				out.first = other.first + log10l(exponent(10, first - other.first) - 1);
			else
				out.first = first + log10l(exponent(10, other.first - first) - 1);
		}
		out.check();
		return out;
	}
};

bigNumber exponent(long double base, bigNumber power)
{
	bigNumber out(0, 0, power.state);
	if (power.state == 1)
	{
		out.state = 2;
		out.second = floorl(power.first * log10l(base));
		out.first = exponent(10, decimalcomp(power.first * log10l(base)));
	}
	else if (power.state == 2)
	{
		out.state = 3;
		out.first = (power.second + log10l(power.first)) * log10l(base);
	}
	else if (power.state == 3)
	{
		std::cerr << "Cannot raise long double to the power of a bigNumber of \"state\" 3";
		abort();
	}
	out.check();
	return out;
}

void func(bigNumber &inp, bigNumber x, bigNumber dt, bigNumber phi, bigNumber b)
{
	inp = inp * exponent(euler, x * dt * phi * b);
}

long long createData(int &numSaves, std::string name)
{
	std::ofstream outfile("Data\\Saves.txt");

}

void loadData(std::vector<bigNumber> &vals, long long id)
{
	
}

std::string run(long double first, long double second, int state)
{
	bigNumber big(first, second, state);
	return exponent(2, big).data;
}
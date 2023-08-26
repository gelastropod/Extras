#include <random>
#include <vector>
#include "bigNumbers.h"

long double price = -1;
long double delta = 5;
long double restingValue = -1;
int state = 0;
int duration = 0;

long double randrange(long double low, long double high)
{
	return (std::rand() / 32768.0L * (high - low)) + low;
}

bool chance(long double pct)
{
	return randrange(0, 100) <= pct;
}

template <typename T>
T weightedChance(std::vector<T> choices, std::vector<long double> chances)
{
	int pos = 0;
	long double pct = randrange(0, 100);
	for (long double i : chances)
	{
		if (pct < i)
			break;
		pct -= i;
		pos++;
	}
	return choices[pos];
}

int generateNewMode()
{
	duration = randrange(5, 50);
	return weightedChance<int>({ 0, 1, 2, 3, 4, 5 }, { 12.5, 25.0, 25.0, 12.5, 12.5, 12.5 });
}

void handleMode()
{
	switch (state)
	{
	case 0:
	case 1:
	case 2:
	case 5:
	{
		if (duration < 0)
			state = generateNewMode();
		duration--;
		break;
	}
	case 3:
	{
		if (duration < 0)
			state = generateNewMode();
		if (chance(3))
		{
			state = 4;
			duration = randrange(5, 50);
			if (chance(70))
			{
				state = 5;
				duration = randrange(5, 50);
			}
		}
		duration--;
		break;
	}
	case 4:
	{
		if (duration < 0)
			state = generateNewMode();
		if (chance(70))
		{
			state = 5;
			duration = randrange(5, 50);
		}
		duration--;
		break;
	}
	}
}

void mode0()
{
	delta *= 0.95;
	delta += randrange(-0.025, 0.025);
}

void mode1()
{
	delta *= 0.99;
	delta += randrange(-0.005, 0.045);
}

void mode2()
{
	delta *= 0.99;
	delta += randrange(-0.045, 0.005);
}

void mode3()
{
	price += randrange(0, 5);
	delta += randrange(-0.015, 0.135);
	if (chance(30))
	{
		price += randrange(-7, 3);
		delta += randrange(-0.05, 0.05);
	}
}

void mode4()
{
	price += randrange(-5, 0);
	delta += randrange(-0.135, 0.015);
	if (chance(30))
	{
		price += randrange(-3, 7);
		delta += randrange(-0.05, 0.05);
	}
}

void mode5()
{
	delta += randrange(-0.15, 0.15);
	if (chance(50))
		delta += randrange(-5, 5);
	if (chance(20))
		delta += randrange(-1, 1);
}

void instantModeChange()
{
	if (chance(25))
		state = generateNewMode();
	int globD = randrange(-1, 1);
	price -= globD * (2 + 7 * intexp(randrange(0, 1), 3) + 7 * delta * intexp(randrange(0, 1), 3));
	delta += globD * randrange(1, 5);
}

void handleModeFunc()
{
	handleMode();
	switch (state)
	{
	case 0:
		mode0();
		break;
	case 1:
		mode1();
		break;
	case 2:
		mode2();
		break;
	case 3:
		mode3();
		break;
	case 4:
		mode4();
		break;
	case 5:
		mode5();
		break;
	}
}

long double run()
{
	if (price == -1)
		price = randrange(0, 100);
	if (restingValue == -1)
		restingValue = randrange(0, 100);
	handleModeFunc();
	delta *= 0.97;
	price += delta;
	price += (restingValue - price) * 0.01;
	price += 3 * intexp(randrange(-1, 1), 11);
	delta += randrange(-0.05, 0.05);
	if (chance(15))
		delta += randrange(-1.5, 1.5);
	if (chance(3))
		delta += randrange(-5, 5);
	if (chance(10))
		delta += randrange(-0.15, 0.15);
	if (price < 5)
	{
		price += (5 - price) / 2;
		if (delta < 0)
			delta *= 0.95;
	}
	if (price > 200 && delta < 0)
		delta *= 0.9;
	return price;
}
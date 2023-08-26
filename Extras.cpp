#include <iostream>
#include <string>
#include <ctime>
#include "StockMarket.h"
#include "formulasolver.h"

int evaluate(int program)
{
    switch (program)
    {
    case 1:
    {
        std::cout << "Program chosen: formulasolver\n"
                     "Please input the formula to solve below:" << std::endl;
        std::string formula = "";
        std::cin.ignore();
        char c = ' ';
        while (c != '\n')
        {
            std::cin.get(c);
            formula += c;
        }
        std::cout << "Evaluated formula:" << std::endl << formulasolver(formula) << std::endl;
        return 0;
    }
    case 2:
    {
        std::cout << "Program chosen: bigNumbers\n"
                     "Please input numbers: ";
        long double a, b;
        int c;
        std::cin >> a >> b >> c;
        std::cout << "Number: " << run(a, b, c) << std::endl;
        return 0;
    }
    case 3:
    {
        int startTime = std::time(NULL);
        std::cout << "Program chosen: StockMarket\n"
                     "Please enter the seed: ";
        int seed;
        std::cin >> seed;
        srand(seed);
        std::cout << "Please enter the number of iterations to run: ";
        int endTime, iter, deltaTime, prevDeltaTime;
        std::cin >> iter;
        prevDeltaTime = std::time(NULL) - startTime;
        int i = 1;
        run();
        do
        {
            endTime = std::time(NULL);
            deltaTime = endTime - startTime;
            if (prevDeltaTime != deltaTime)
            {
                std::cout << "Stock market price at " << i << "th iteration: " << run() << std::endl;
                i++;
            }
            prevDeltaTime = deltaTime;
        } while (i <= iter);
        return 0;
    }
    case 4:
    {
        std::cout << "Are you sure you want to exit the program? Y/N" << std::endl;
        char exit;
        std::cin >> exit;
        if (exit == 'Y')
            return -1;
        return 0;
    }
    default:
        std::cout << "Invalid program chosen." << std::endl;
        return 0;
    }
}

int main()
{
    int program;
    do
    {
        std::cout << "What program would you like to run?\n"
                     "Options:\n"
                     "1. formulasolver\n"
                     "2. bigNumbers\n"
                     "3. StockMarket\n"
                     "4. Exit program" << std::endl;
        std::cin >> program;
    } while (evaluate(program) == 0);
    return 0;
}
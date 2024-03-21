#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

// Função para contar números primos até n
int prime_number(int n)
{
    int total = 0;
    for (int i = 2; i <= n; i++)
    {
        int prime = 1;
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
        }
        total += prime;
    }
    return total;
}

int main(int argc, char *argv[])
{
    int entry_number = atoi(argv[1]);

	auto t_start = std::chrono::high_resolution_clock::now();

    int total_periodo = prime_number(entry_number);

	auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "O número total de primos até " << entry_number << " é: " << total_periodo << std::endl;
    std::cout << "Execution time(s): " << std::chrono::duration<double>(t_end-t_start).count() << std::endl;

    return 0;
}
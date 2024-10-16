#include <iostream>
#include <vector>
#include <cmath>
#include <gmp.h>
#include <chrono>
#include <iomanip>

// Function to check if a number is a power of 2
bool is_power_of_two(unsigned long long num) {
    return num > 0 && (num & (num - 1)) == 0;
}

// Function to check if a number is a Mersenne number
bool is_mersenne_number(unsigned long long num) {
    return is_power_of_two(num + 1);
}

// Function to check if a number is a Mersenne prime
bool is_mersenne_prime(int n, const std::vector<unsigned long long>& known_primes) {
    // Calculate 2^n - 1
    mpz_t mersenne_number;
    mpz_init(mersenne_number);
    mpz_ui_pow_ui(mersenne_number, 2, n);
    mpz_sub_ui(mersenne_number, mersenne_number, 1);

    // Check against known primes
    for (const auto& prime : known_primes) {
        if (mpz_fdiv_ui(mersenne_number, prime) == 0) {
            mpz_clear(mersenne_number);
            return false;
        }
    }

    // Check if the number is prime
    bool is_prime = mpz_probab_prime_p(mersenne_number, 25) > 0;
    mpz_clear(mersenne_number);
    return is_prime;
}

int main() {
    std::vector<unsigned long long> known_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}; // Initial known primes
    int n =  69369389; // Start checking from 2

    while (1) { // Limit the range for demonstration purposes
        if (is_mersenne_prime(n, known_primes)) {
            // Get current time
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);

            // Print Mersenne prime with timestamp
            std::cout << "Mersenne prime found with exponent: " << n
                      << " at " << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << std::endl;
        }

        // Update known primes list
        mpz_t mersenne_number;
        mpz_init(mersenne_number);
        mpz_ui_pow_ui(mersenne_number, 2, n);
        mpz_sub_ui(mersenne_number, mersenne_number, 1);

        if (mpz_probab_prime_p(mersenne_number, 25) > 0) {
            known_primes.push_back(n);
        }

        mpz_clear(mersenne_number);
        n++;
    }

    return 0;
}
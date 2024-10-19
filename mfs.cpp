#include <iostream>
#include <vector>
#include <cmath>
#include <gmp.h>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <atomic>

// Function to check if a number is a Mersenne prime
bool is_mersenne_prime(int n) {
    mpz_t mersenne_number;
    mpz_init(mersenne_number);
    mpz_ui_pow_ui(mersenne_number, 2, n);
    mpz_sub_ui(mersenne_number, mersenne_number, 1);

    bool is_prime = mpz_probab_prime_p(mersenne_number, 25) > 0;
    mpz_clear(mersenne_number);
    return is_prime;
}

// Function to listen for keyboard input
void listenForInput(std::atomic<bool>& showValue, std::atomic<bool>& running) {
    char input;
    while (running) {
        std::cin >> input;
        if (input == 'v' || input == 'V') {
            showValue = true;
        }
    }
}

int main() {
    std::vector<unsigned long long> known_primes = {};
    unsigned long long int n = 2;
    std::atomic<bool> showValue(false);
    std::atomic<bool> running(true);

    std::cout << "Press 'V' to view the current value of n." << std::endl;

    // Start a separate thread to listen for input
    std::thread inputThread(listenForInput, std::ref(showValue), std::ref(running));

    while (true) {
        if (showValue) {
            std::cout << "Current value of n: " << n << std::endl;
            showValue = false;
        }

        if (sqrt(n) != floor(sqrt(n) + 0.49) && is_mersenne_prime(n)) {
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Mersenne prime found with exponent: " << n
                      << " at " << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << std::endl;
        }

        n++;
    }

    running = false;
    inputThread.join();

    return 0;
}

#include <iostream>
using namespace std;

const int MOD = 1000003;

// Helper function to multiply two 2x2 matrices modulo MOD
void multiply(long long F[2][2], long long M[2][2]) {
    long long x = (F[0][0] * M[0][0] + F[0][1] * M[1][0]) % MOD;
    long long y = (F[0][0] * M[0][1] + F[0][1] * M[1][1]) % MOD;
    long long z = (F[1][0] * M[0][0] + F[1][1] * M[1][0]) % MOD;
    long long w = (F[1][0] * M[0][1] + F[1][1] * M[1][1]) % MOD;

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

// Helper function to compute F^n using binary exponentiation
void power(long long F[2][2], long long n) {
    if (n == 0 || n == 1)
        return;

    long long M[2][2] = {{1, 1}, {1, 0}};

    // Recursively square the matrix
    power(F, n / 2);
    multiply(F, F);

    // If n is odd, multiply once more with the base matrix
    if (n % 2 != 0)
        multiply(F, M);
}

// Function to compute the nth Fibonacci number modulo MOD
int fibonacci(long long n) {
    if (n == 1) return 0; // F(1) = 0
    if (n == 2) return 1; // F(2) = 1

    long long F[2][2] = {{1, 1}, {1, 0}};
    power(F, n - 2); // Compute F^(n-2)

    return F[0][0]; // The result is F(n)
}

int main() {
    long long n;
    cin >> n;
    cout << fibonacci(n) << "\n";
    return 0;
}

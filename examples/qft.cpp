// Quantum Fourier transform
// Source: ./examples/qft.cpp
#include <iostream>
#include <cmath>

#include "qpp.h"

int main() {
    using namespace qpp;
    std::vector<idx> qubits{1, 0, 1, 1, 0}; // initial state
    ket psi = mket(qubits);
    ket result = psi;
    idx N = qubits.size();  // number of qubits
    idx D = std::pow(2, N); // dimension
    std::cout << ">> The succession of applied gates are:\n";
    for (idx i = 0; i < N; ++i) {
        std::cout << "H" << i << " ";
        result = apply(result, gt.H, {i}); // apply Hadamard on qubit i
        // apply controlled rotations
        for (idx j = 2; j <= N - i; ++j) {
            cmat Rj(2, 2);
            Rj << 1, 0, 0, omega(std::pow(2, j));
            result = applyCTRL(result, Rj, {i + j - 1}, {i});
            std::cout << "R" << j << "(" << i + j - 1 << ", " << i << ") ";
        }
        std::cout << std::endl;
    }
    // check that indeed we got the Fourier transform
    // we have the qubits in reversed order, we must reverse them
    std::vector<idx> reversed(N);
    std::iota(std::begin(reversed), std::end(reversed), 0);
    std::reverse(std::begin(reversed), std::end(reversed));
    result = syspermute(result, reversed);
    // compute the norm diference
    std::cout << ">> Norm difference: " << norm(result - gt.Fd(D) * psi)
              << '\n';
}

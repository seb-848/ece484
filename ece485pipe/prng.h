//
// Created by sebas on 1/29/2025.
//

#ifndef ECE485PIPE_PRNG_H
#define ECE485PIPE_PRNG_H

#include <string>
#include <bits/stdc++.h>

using namespace std;

//int SEED = 0b101011;//43
//int const INFO = 0b00001000;
//int PARITY = 0b11110101;
int const BITS = 8;
string const GIVEN_FILE = "helloworldfile.txt";

//char strToChar(const char* str) {
//    char parsed = 0;
//    for (int i = 0; i < 8; i++) {
//        if (str[i] == '1') {
//            parsed |= 1 << (7 - i);
//        }
//    }
//    return parsed;
//}

/**
 * takes the B and P to and then xor together for B n-1
 * @param b[] - array of seed
 * @param p[] - array of parity
 * @return result xor value
 */
int parityXOR(int (&b)[BITS], int (&p)[BITS]) {
    int a[BITS];

    for(int i = 0; i < BITS; i++) {
        a[i] = b[i] & p[i];
    }
    int result = a[0];
    for (int i = 1; i < BITS; i++) {
        result ^= a[i];
    }
    return result;
}

/**
 * shifts the array to the right and adds new value from xor
 * use after encrypting bit
 * @param b array of B bits
 * @param newValue xor value
 * @return new array
 */
void shiftRegister(int (&b)[BITS], int (&p)[BITS]) {
    int xorValue = parityXOR(b, p);
    int n = sizeof(b)/sizeof(b[0]);
    int d = BITS - 1;

    rotate(b, b + n - d, b + n);
    b[0] = xorValue;
}

/**
 * take the last bit in b and xor with the info to be encrypted
 * use before shifting
 * @param b array of B
 * @param infoToEncrypt info to be encrypted
 * @return encryptedValue
 */
int encryption(int (&b)[BITS], int infoToEncrypt) {
    int encryptionValue = b[BITS - 1];
    int encryptedValue = infoToEncrypt ^ encryptionValue;
    return encryptedValue;
}



#endif //ECE485PIPE_PRNG_H

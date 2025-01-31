/**
 *
 */

#include <iostream>
#include <cstdlib>
//#include <string>
//#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include "prng.h"

using namespace std;

// one index at a time
int main() {
    srand((unsigned) time(nullptr));
    int rand_bits[BITS];
    int parity_bits[BITS];

    // populate
    for (int i = 0; i < BITS; i++) {
        rand_bits[i] = (rand() % 2);
        parity_bits[i] = (rand() % 2);
    }

    vector<int> sequence;
    vector<int> encrypted;

    // open the given file
    fstream readfile(GIVEN_FILE, ios:: binary | ios:: in);

    // char variable to take in the given file
    char c;

    // read file character by character
    while (readfile.get(c)) {

        // take in a byte at a time
        // or for (int i = 0; i < 8; i++)  if you want reverse bit order in bytes
        for (int i = 7; i >= 0; i--)
        {
            // encrypt and put into encrypted bits vector for later decryption
            encrypted.push_back(encryption(rand_bits,((c >> i) & 1)));

            // push the random sequence of bits for later encryption
            sequence.push_back(rand_bits[BITS - 1]);

            // shift the bits right
            shiftRegister(rand_bits, parity_bits);
        }
    }

    // close given file
    readfile.close();

    // open file to encrypt
    fstream newreadfile(GIVEN_FILE, ios::trunc);

    // write encrypted vector to file
    for (int i : encrypted) {
        newreadfile << i;
    }

    // close encrypted file
    newreadfile.close();

    // user input to leave file encrypted or decrypt
    int answer = 0;
    cout << "Enter 1 to decrypt\nEnter 2 to encrypt\nAnswer: " << endl;
    cin >> answer;

    // decrypt option
    if (answer == 1) {
        // open encrypted file and truncate the leftover characters after encryption
        ofstream writefile(GIVEN_FILE, ios::out | ios::trunc);

        // delcare/intialize count and string variables
        int count = 0;
        string binaryNum;
        string fileString;

        // TODO: try bitset or uint_

        // decrypt byte by byte
        for (int i = 0; i < encrypted.size(); i++) {
            // decryption
            binaryNum += to_string(encrypted.at(i) ^ sequence.at(i));
            count++;

            // once byte is obtained
            if (count == 8) {
                // reset count for next byte
                count = 0;

                // write back to file in readable text
                writefile << (char) stoi(binaryNum, nullptr,2);

                // reset binaryNum string for next character
                binaryNum = "";
            }
        }
        // close file written to
        writefile.close();
    }

        // reset file
//        newreadfile.clear();
//        newreadfile.seekg(0);
    return 0;
}

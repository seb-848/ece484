#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cstdlib>
#include <string>
#include <bits/stdc++.h>
int const BITS = 8;
string const GIVEN_FILE = "test_bits.txt";

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
std::vector<bool> generateRandomBits(size_t numBits) {
    std::vector<bool> bits(numBits);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (size_t i = 0; i < numBits; ++i) {
        bits[i] = dis(gen);
    }
    return bits;
}

void saveBitsToFile(const std::vector<bool>& bits, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    for (bool bit : bits) {
        outFile << (bit ? '1' : '0');
    }

    outFile.close();
    std::cout << "Bits successfully written to " << filename << std::endl;
}

int main() {
    size_t numBits = 8388608;  // 1 MB test
    auto bits = generateRandomBits(numBits);

    saveBitsToFile(bits, "test_bits.txt");

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
    fstream readfile("test_bits.txt", ios:: binary | ios:: in);

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



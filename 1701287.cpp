#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
using namespace std;

typedef unsigned long long u64;

// helper functions
u64 strHex_to_u64(char* data);


string hex_to_bin(string hex);
string bin_to_hex(string bin);
int bin_to_decimal(string s);
string decimal_to_bin(int x);
string xor_bin(string x, string y);

// round blocks
string initial_permutation(string data);
string inverse_initial_permutation(string data);
string permutated_choice_1(string key);
string permutated_choice_2(string shifted_key);
string s1(string s);
string s2(string s);
string s3(string s);
string s4(string s);
string s5(string s);
string s6(string s);
string s7(string s);
string s8(string s);
string permutatation(string s_out);
string left_circular_shift(int round, string key);
string extend_32bits_to_48bits(string r);
string f_function(string r, string k);

// encrypt and decrypt
string encrypt(string text, string key);
string decrypt(string cipher, string key);

int main(int argc, char* argv[]){
    char* op = argv[1];
    char* data = argv[2];
    char* key = argv[3];

    printf("Data: %016llX %lld\n", strHex_to_u64(data), strHex_to_u64(data));
    printf("Key: %016llX %lld\n", strHex_to_u64(key), strHex_to_u64(key));
    
    // if (op == "encrypt"){
    //     string cipher = encrypt(data, key);
    //     cout << "cipher: " << cipher << '\n';
    // } else {
    //     string text = decrypt(data, key);
    //     cout << "plain: " << text << "\n";
    // }
    return 0;
}

u64 strHex_to_u64(char* data){
    assert(strlen(data) == 16);

    u64 result = 0;
    for(int i = 0; i < 16; i++){
        if (data[i] >= '0' && data[i] <= '9'){
            result |= (u64)(data[i] - '0')<<((15 - i) << 2);
        } else if (data[i] >= 'A' && data[i] <= 'F'){
            result |= (u64)(data[i] - 'A' + 10)<<((15 - i) << 2);
        } else if (data[i] >= 'a' && data[i] <= 'f'){
            result |= (u64)(data[i] - 'a' + 10)<<((15 - i) << 2);
        }
    }
    return result;
}


string encrypt(string text, string key){
    // convert text, and key to binary
    string bin_text = hex_to_bin(text);
    string bin_key = hex_to_bin(key);

    // get the initial permutation text
    string initial_permutation_text = initial_permutation(bin_text);
    string l = initial_permutation_text.substr(0, 32);
    string r = initial_permutation_text.substr(32, 64);

    // get the 56 bits key
    string key_56 = permutated_choice_1(bin_key);
    string c = key_56.substr(0, 28);
    string d = key_56.substr(28, 56);
    string k = "";

    // generate the 16 keys
    string keys[16];
    for (int i = 0; i < 16; i++){
        // generate the key
        key_56 = left_circular_shift(i + 1, key_56);
        k = permutated_choice_2(key_56);
        keys[i] = k;
    }

    // 16 rounds
    for (int i = 0; i < 16; i++){
        // update l, and r
        string old_l = l, old_r = r;
        l = old_r;
        r = xor_bin(old_l, f_function(old_r, keys[i]));
    }
    // swap
    string text_swap = r + l;
    // inverse permutation
    string inverse_permutation_text = inverse_initial_permutation(text_swap);
    string cipher = bin_to_hex(inverse_permutation_text);
    return cipher;
}

string decrypt(string cipher, string key){
    // convert cipher, and key to binary
    string bin_cipher = hex_to_bin(cipher);
    string bin_key = hex_to_bin(key);

    // get the initial permutation cipher
    string initial_permutation_cipher = initial_permutation(bin_cipher);
    string l = initial_permutation_cipher.substr(0, 32);
    string r = initial_permutation_cipher.substr(32, 64);

    // get the 56 bits key
    string key_56 = permutated_choice_1(bin_key);
    string c = key_56.substr(0, 28);
    string d = key_56.substr(28, 56);
    string k = "";

    // generate the 16 keys
    string keys[16];
    for (int i = 0; i < 16; i++){
        // generate the key
        key_56 = left_circular_shift(i + 1, key_56);
        k = permutated_choice_2(key_56);
        keys[i] = k;
    }

    // 16 rounds
    for (int i = 0; i < 16; i++){
        // update l, and r
        string old_l = l, old_r = r;
        l = old_r;
        r = xor_bin(old_l, f_function(old_r, keys[15 - i]));
    }
    // swap
    string cipher_swap = r + l;
    // inverse permutation
    string inverse_permutation_cipher = inverse_initial_permutation(cipher_swap);
    string text = bin_to_hex(inverse_permutation_cipher);
    return text;
}

string hex_to_bin(string hex){
    string bin = "";
    for (int i = 0; i < hex.length(); i++){
        switch (hex[i])
        {
        case '0':
            bin += "0000";
            break;
        case '1':
            bin += "0001";
            break;
        case '2':
            bin += "0010";
            break;
        case '3':
            bin += "0011";
            break;
        case '4':
            bin += "0100";
            break;
        case '5':
            bin += "0101";
            break;
        case '6':
            bin += "0110";
            break;
        case '7':
            bin += "0111";
            break;
        case '8':
            bin += "1000";
            break;
        case '9':
            bin += "1001";
            break;
        case 'A':
            bin += "1010";
            break;
        case 'B':
            bin += "1011";
            break;
        case 'C':
            bin += "1100";
            break;
        case 'D':
            bin += "1101";
            break;
        case 'E':
            bin += "1110";
            break;
        case 'F':
            bin += "1111";
            break;
        case 'a':
            bin += "1010";
            break;
        case 'b':
            bin += "1011";
            break;
        case 'c':
            bin += "1100";
            break;
        case 'd':
            bin += "1101";
            break;
        case 'e':
            bin += "1110";
            break;
        case 'f':
            bin += "1111";
            break;
        }
    }
    return bin;
}

string bin_to_hex(string bin){
    string hex = "";
    for (int i = 0; i < bin.length(); i++){
        string _bin = "";
        for (int j = 0; j < 4; j++)
            _bin += bin[i + j];
        i += 3;

        if (_bin == "0000")
            hex += "0";
        else if (_bin == "0001")
            hex += "1";
        else if (_bin == "0010")
            hex += "2";
        else if (_bin == "0011")
            hex += "3";
        else if (_bin == "0100")
            hex += "4";
        else if (_bin == "0101")
            hex += "5";
        else if (_bin == "0110")
            hex += "6";
        else if (_bin == "0111")
            hex += "7";
        else if (_bin == "1000")
            hex += "8";
        else if (_bin == "1001")
            hex += "9";
        else if (_bin == "1010")
            hex += "A";
        else if (_bin == "1011")
            hex += "B";
        else if (_bin == "1100")
            hex += "C";
        else if (_bin == "1101")
            hex += "D";
        else if (_bin == "1110")
            hex += "E";
        else if (_bin == "1111")
            hex += "F";
    }
    return hex;
}

int bin_to_decimal(string s){
    int decimal = 0;
    int n = s.length();
 
    for (int i = 0; i < n; i++){
        if (s[n-i - 1] == '1')
            decimal += pow(2, i);
    }
 
    return decimal;
}

string decimal_to_bin(int x){
    string binary = bitset<4>(x).to_string();
    return binary;
}

string xor_bin(string x, string y){
    string z = "";
    for (int i = 0; i < x.length(); i++)
        (x[i] == y[i]) ? z += "0" : z += "1";
    return z;
}

string initial_permutation(string data){
    string permutation = "";
    permutation += data[57];
    permutation += data[49];
    permutation += data[41];
    permutation += data[33];
    permutation += data[25];
    permutation += data[17];
    permutation += data[9];
    permutation += data[1];
    permutation += data[59];
    permutation += data[51];
    permutation += data[43];
    permutation += data[35];
    permutation += data[27];
    permutation += data[19];
    permutation += data[11];
    permutation += data[3];
    permutation += data[61];
    permutation += data[53];
    permutation += data[45];
    permutation += data[37];
    permutation += data[29];
    permutation += data[21];
    permutation += data[13];
    permutation += data[5];
    permutation += data[63];
    permutation += data[55];
    permutation += data[47];
    permutation += data[39];
    permutation += data[31];
    permutation += data[23];
    permutation += data[15];
    permutation += data[7];
    permutation += data[56];
    permutation += data[48];
    permutation += data[40];
    permutation += data[32];
    permutation += data[24];
    permutation += data[16];
    permutation += data[8];
    permutation += data[0];
    permutation += data[58];
    permutation += data[50];
    permutation += data[42];
    permutation += data[34];
    permutation += data[26];
    permutation += data[18];
    permutation += data[10];
    permutation += data[2];
    permutation += data[60];
    permutation += data[52];
    permutation += data[44];
    permutation += data[36];
    permutation += data[28];
    permutation += data[20];
    permutation += data[12];
    permutation += data[4];
    permutation += data[62];
    permutation += data[54];
    permutation += data[46];
    permutation += data[38];
    permutation += data[30];
    permutation += data[22];
    permutation += data[14];
    permutation += data[6];
    return permutation;
}

string inverse_initial_permutation(string data){
    string inverse_permutation = "";
    inverse_permutation += data[39];
    inverse_permutation += data[7];
    inverse_permutation += data[47];
    inverse_permutation += data[15];
    inverse_permutation += data[55];
    inverse_permutation += data[23];
    inverse_permutation += data[63];
    inverse_permutation += data[31];
    inverse_permutation += data[38];
    inverse_permutation += data[6];
    inverse_permutation += data[46];
    inverse_permutation += data[14];
    inverse_permutation += data[54];
    inverse_permutation += data[22];
    inverse_permutation += data[62];
    inverse_permutation += data[30];
    inverse_permutation += data[37];
    inverse_permutation += data[5];
    inverse_permutation += data[45];
    inverse_permutation += data[13];
    inverse_permutation += data[53];
    inverse_permutation += data[21];
    inverse_permutation += data[61];
    inverse_permutation += data[29];
    inverse_permutation += data[36];
    inverse_permutation += data[4];
    inverse_permutation += data[44];
    inverse_permutation += data[12];
    inverse_permutation += data[52];
    inverse_permutation += data[20];
    inverse_permutation += data[60];
    inverse_permutation += data[28];
    inverse_permutation += data[35];
    inverse_permutation += data[3];
    inverse_permutation += data[43];
    inverse_permutation += data[11];
    inverse_permutation += data[51];
    inverse_permutation += data[19];
    inverse_permutation += data[59];
    inverse_permutation += data[27];
    inverse_permutation += data[34];
    inverse_permutation += data[2];
    inverse_permutation += data[42];
    inverse_permutation += data[10];
    inverse_permutation += data[50];
    inverse_permutation += data[18];
    inverse_permutation += data[58];
    inverse_permutation += data[26];
    inverse_permutation += data[33];
    inverse_permutation += data[1];
    inverse_permutation += data[41];
    inverse_permutation += data[9];
    inverse_permutation += data[49];
    inverse_permutation += data[17];
    inverse_permutation += data[57];
    inverse_permutation += data[25];
    inverse_permutation += data[32];
    inverse_permutation += data[0];
    inverse_permutation += data[40];
    inverse_permutation += data[8];
    inverse_permutation += data[48];
    inverse_permutation += data[16];
    inverse_permutation += data[56];
    inverse_permutation += data[24];
    return inverse_permutation;
}

string permutated_choice_1(string key){
    string permutated_choice_1 = "";
    permutated_choice_1 += key[56];
    permutated_choice_1 += key[48];
    permutated_choice_1 += key[40];
    permutated_choice_1 += key[32];
    permutated_choice_1 += key[24];
    permutated_choice_1 += key[16];
    permutated_choice_1 += key[8];
    permutated_choice_1 += key[0];
    permutated_choice_1 += key[57];
    permutated_choice_1 += key[49];
    permutated_choice_1 += key[41];
    permutated_choice_1 += key[33];
    permutated_choice_1 += key[25];
    permutated_choice_1 += key[17];
    permutated_choice_1 += key[9];
    permutated_choice_1 += key[1];
    permutated_choice_1 += key[58];
    permutated_choice_1 += key[50];
    permutated_choice_1 += key[42];
    permutated_choice_1 += key[34];
    permutated_choice_1 += key[26];
    permutated_choice_1 += key[18];
    permutated_choice_1 += key[10];
    permutated_choice_1 += key[2];
    permutated_choice_1 += key[59];
    permutated_choice_1 += key[51];
    permutated_choice_1 += key[43];
    permutated_choice_1 += key[35];
    permutated_choice_1 += key[62];
    permutated_choice_1 += key[54];
    permutated_choice_1 += key[46];
    permutated_choice_1 += key[38];
    permutated_choice_1 += key[30];
    permutated_choice_1 += key[22];
    permutated_choice_1 += key[14];
    permutated_choice_1 += key[6];
    permutated_choice_1 += key[61];
    permutated_choice_1 += key[53];
    permutated_choice_1 += key[45];
    permutated_choice_1 += key[37];
    permutated_choice_1 += key[29];
    permutated_choice_1 += key[21];
    permutated_choice_1 += key[13];
    permutated_choice_1 += key[5];
    permutated_choice_1 += key[60];
    permutated_choice_1 += key[52];
    permutated_choice_1 += key[44];
    permutated_choice_1 += key[36];
    permutated_choice_1 += key[28];
    permutated_choice_1 += key[20];
    permutated_choice_1 += key[12];
    permutated_choice_1 += key[4];
    permutated_choice_1 += key[27];
    permutated_choice_1 += key[19];
    permutated_choice_1 += key[11];
    permutated_choice_1 += key[3];
    return permutated_choice_1;
}

string permutated_choice_2(string shifted_key){
    string permutated_choice_2 = "";
    permutated_choice_2 += shifted_key[13];
    permutated_choice_2 += shifted_key[16];
    permutated_choice_2 += shifted_key[10];
    permutated_choice_2 += shifted_key[23];
    permutated_choice_2 += shifted_key[0];
    permutated_choice_2 += shifted_key[4];
    permutated_choice_2 += shifted_key[2];
    permutated_choice_2 += shifted_key[27];
    permutated_choice_2 += shifted_key[14];
    permutated_choice_2 += shifted_key[5];
    permutated_choice_2 += shifted_key[20];
    permutated_choice_2 += shifted_key[9];
    permutated_choice_2 += shifted_key[22];
    permutated_choice_2 += shifted_key[18];
    permutated_choice_2 += shifted_key[11];
    permutated_choice_2 += shifted_key[3];
    permutated_choice_2 += shifted_key[25];
    permutated_choice_2 += shifted_key[7];
    permutated_choice_2 += shifted_key[15];
    permutated_choice_2 += shifted_key[6];
    permutated_choice_2 += shifted_key[26];
    permutated_choice_2 += shifted_key[19];
    permutated_choice_2 += shifted_key[12];
    permutated_choice_2 += shifted_key[1];
    permutated_choice_2 += shifted_key[40];
    permutated_choice_2 += shifted_key[51];
    permutated_choice_2 += shifted_key[30];
    permutated_choice_2 += shifted_key[36];
    permutated_choice_2 += shifted_key[46];
    permutated_choice_2 += shifted_key[54];
    permutated_choice_2 += shifted_key[29];
    permutated_choice_2 += shifted_key[39];
    permutated_choice_2 += shifted_key[50];
    permutated_choice_2 += shifted_key[44];
    permutated_choice_2 += shifted_key[32];
    permutated_choice_2 += shifted_key[47];
    permutated_choice_2 += shifted_key[43];
    permutated_choice_2 += shifted_key[48];
    permutated_choice_2 += shifted_key[38];
    permutated_choice_2 += shifted_key[55];
    permutated_choice_2 += shifted_key[33];
    permutated_choice_2 += shifted_key[52];
    permutated_choice_2 += shifted_key[45];
    permutated_choice_2 += shifted_key[41];
    permutated_choice_2 += shifted_key[49];
    permutated_choice_2 += shifted_key[35];
    permutated_choice_2 += shifted_key[28];
    permutated_choice_2 += shifted_key[31];
    return permutated_choice_2;
}

string s1(string s){
    int arr[4][16] = {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s2(string s){
    int arr[4][16] = {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10 ,1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s3(string s){
    int arr[4][16] = {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10 ,14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s4(string s){
    int arr[4][16] = {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s5(string s){
    int arr[4][16] = {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3} 
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s6(string s){
    int arr[4][16] = {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13} 
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s7(string s){
    int arr[4][16] = {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12} 
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string s8(string s){
    int arr[4][16] = {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    };

    string row = "", column = "";
    row += s[0];
    row += s[5];
    column = s.substr(1, 4);
    int r = bin_to_decimal(row);
    int c = bin_to_decimal(column);
    int value = arr[r][c];
    return decimal_to_bin(value);
}

string permutatation(string s_out){
    string permutatation = "";
    permutatation += s_out[15];
    permutatation += s_out[6];
    permutatation += s_out[19];
    permutatation += s_out[20];
    permutatation += s_out[28];
    permutatation += s_out[11];
    permutatation += s_out[27];
    permutatation += s_out[16];
    permutatation += s_out[0];
    permutatation += s_out[14];
    permutatation += s_out[22];
    permutatation += s_out[25];
    permutatation += s_out[4];
    permutatation += s_out[17];
    permutatation += s_out[30];
    permutatation += s_out[9];
    permutatation += s_out[1];
    permutatation += s_out[7];
    permutatation += s_out[23];
    permutatation += s_out[13];
    permutatation += s_out[31];
    permutatation += s_out[26];
    permutatation += s_out[2];
    permutatation += s_out[8];
    permutatation += s_out[18];
    permutatation += s_out[12];
    permutatation += s_out[29];
    permutatation += s_out[5];
    permutatation += s_out[21];
    permutatation += s_out[10];
    permutatation += s_out[3];
    permutatation += s_out[24];
    return permutatation;
}
string left_circular_shift(int round, string key){
    int shift = 0;
    if (round == 1 || round == 2 || round == 9 || round == 16)
        shift = 1;
    else
        shift = 2;
    string c = key.substr(0, 28);
    string d = key.substr(28, 56);
    string c_shifted = "", d_shifted = "";
    for (int i = shift; i < 28; i++){
        c_shifted += c[i];
        d_shifted += d[i]; 
    }
    for(int i = 0; i < shift; i++){
        c_shifted += c[i];
        d_shifted += d[i];
    }
    return (c_shifted + d_shifted);
}
string extend_32bits_to_48bits(string r){
    string r_48bits = "";
    r_48bits += r[31];
    r_48bits += r[0];
    r_48bits += r[1];
    r_48bits += r[2];
    r_48bits += r[3];
    r_48bits += r[4];
    r_48bits += r[3];
    r_48bits += r[4];
    r_48bits += r[5];
    r_48bits += r[6];
    r_48bits += r[7];
    r_48bits += r[8];
    r_48bits += r[7];
    r_48bits += r[8];
    r_48bits += r[9];
    r_48bits += r[10];
    r_48bits += r[11];
    r_48bits += r[12];
    r_48bits += r[11];
    r_48bits += r[12];
    r_48bits += r[13];
    r_48bits += r[14];
    r_48bits += r[15];
    r_48bits += r[16];
    r_48bits += r[15];
    r_48bits += r[16];
    r_48bits += r[17];
    r_48bits += r[18];
    r_48bits += r[19];
    r_48bits += r[20];
    r_48bits += r[19];
    r_48bits += r[20];
    r_48bits += r[21];
    r_48bits += r[22];
    r_48bits += r[23];
    r_48bits += r[24];
    r_48bits += r[23];
    r_48bits += r[24];
    r_48bits += r[25];
    r_48bits += r[26];
    r_48bits += r[27];
    r_48bits += r[28];
    r_48bits += r[27];
    r_48bits += r[28];
    r_48bits += r[29];
    r_48bits += r[30];
    r_48bits += r[31];
    r_48bits += r[0];
    return r_48bits;
}

string f_function(string r, string k){
    string r_extend = extend_32bits_to_48bits(r);
    string r_xor_k = xor_bin(r_extend, k);
    string s1_ = s1(r_xor_k.substr(0, 6));
    string s2_ = s2(r_xor_k.substr(6, 12));
    string s3_ = s3(r_xor_k.substr(12, 18));
    string s4_ = s4(r_xor_k.substr(18, 24));
    string s5_ = s5(r_xor_k.substr(24, 30));
    string s6_ = s6(r_xor_k.substr(30, 36));
    string s7_ = s7(r_xor_k.substr(36, 42));
    string s8_ = s8(r_xor_k.substr(42, 48));

    string s_32bits = s1_ + s2_ + s3_ + s4_ + s5_ + s6_ + s7_ + s8_;
    return permutatation(s_32bits);
}
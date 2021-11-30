#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef unsigned long long u64;

#ifdef __GNUC__
#define __rdtsc __builtin_ia32_rdtsc
#else
#include <intrin.h>
#endif
// --------------------------------------------------------------------------------------------
// DES Permutation Tables
// Initial Permutation Table
const int initial_perm[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                            60, 52, 44, 36, 28, 20, 12, 4,
                            62, 54, 46, 38, 30, 22, 14, 6,
                            64, 56, 48, 40, 32, 24, 16, 8,
                            57, 49, 41, 33, 25, 17, 9, 1,
                            59, 51, 43, 35, 27, 19, 11, 3,
                            61, 53, 45, 37, 29, 21, 13, 5,
                            63, 55, 47, 39, 31, 23, 15, 7 };

// Inverse Initial Permutation Table
const int inverse_initial_perm[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                                    39, 7, 47, 15, 55, 23, 63, 31,
                                    38, 6, 46, 14, 54, 22, 62, 30,
                                    37, 5, 45, 13, 53, 21, 61, 29,
                                    36, 4, 44, 12, 52, 20, 60, 28,
                                    35, 3, 43, 11, 51, 19, 59, 27,
                                    34, 2, 42, 10, 50, 18, 58, 26,
                                    33, 1, 41, 9, 49, 17, 57, 25 };

// Extend 32 bits to 48 bits table
const int extend[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                        6, 7, 8, 9, 8, 9, 10, 11,
                        12, 13, 12, 13, 14, 15, 16, 17,
                        16, 17, 18, 19, 20, 21, 20, 21,
                        22, 23, 24, 25, 24, 25, 26, 27,
                        28, 29, 28, 29, 30, 31, 32, 1 };

// Permutation Choice 1 Table
const int pc1[56] = { 57, 49, 41, 33, 25, 17, 9,
                    1, 58, 50, 42, 34, 26, 18,
                    10, 2, 59, 51, 43, 35, 27,
                    19, 11, 3, 60, 52, 44, 36,
                    63, 55, 47, 39, 31, 23, 15,
                    7, 62, 54, 46, 38, 30, 22,
                    14, 6, 61, 53, 45, 37, 29,
                    21, 13, 5, 28, 20, 12, 4 };

// Permutation Choice 2 Table
const int pc2[48] = {14, 17, 11, 24, 1, 5,
                    3, 28, 15, 6, 21, 10,
                    23, 19, 12, 4, 26, 8,
                    16, 7, 27, 20, 13, 2,
                    41, 52, 31, 37, 47, 55,
                    30, 40, 51, 45, 33, 48,
                    44, 49, 39, 56, 34, 53,
                    46, 42, 50, 36, 29, 32 };

// Permutation Table
const int per[32] = {16, 7, 20, 21,
                    29, 12, 28, 17,
                    1, 15, 23, 26,
                    5, 18, 31, 10,
                    2, 8, 24, 14,
                    32, 27, 3, 9,
                    19, 13, 30, 6,
                    22, 11, 4, 25 };
                
// S-box Table
const int s[8][4][16] = {   { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                            0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                            4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                            15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },

                            { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                            3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                            0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                            13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                            { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                            13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                            13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                            1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },

                            { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                            13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                            10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                            3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },

                            { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                            14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                            4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                            11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },

                            { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                            10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                            9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                            4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },

                            { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                            13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                            1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                            6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                            
                            { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                            1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                            7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                            2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

// --------------------------------------------------------------------------------------------

// Keys for 16-round DES
u64 keys[16] = {0};

// helper functions
void key_gen(u64 key);
u64 strHex_to_u64(char* data);

// round blocks
u64 permutation(u64 data, const int table[], int in, int out);
u64 s_box(u64 data);
u64 f_function(u64 r, u64 k);

// encrypt and decrypt
u64 encrypt(u64 text, u64 key);
u64 decrypt(u64 cipher, u64 key);

int main(int argc, char* argv[]){
    // Get inputs
    char* op = argv[1];
    char* str_data = argv[2];
    char* str_key = argv[3];

    // convert strings to u64
    u64 data = strHex_to_u64(str_data);
    u64 key = strHex_to_u64(str_key);
    
    // Generate the keys.
    key_gen(key);

    if (strcmp(op, "encrypt") == 0){
        long long t1 = __rdtsc();
        u64 cipher = encrypt(data, key);
        long long t2 = __rdtsc();
        printf("Cipher: %016llX\n", cipher);
        printf("Cycles: %lld\n", t2 - t1);
    } else if (strcmp(op, "decrypt") == 0){
        long long t1 = __rdtsc();
        u64 plain = decrypt(data, key);
        long long t2 = __rdtsc();
        printf("Plain: %016llX\n", plain);
        printf("Cycles: %lld\n", t2 - t1);
    }

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


u64 encrypt(u64 text, u64 key){
    // get the initial permutation text
    u64 initial_permutation_text = permutation(text, initial_perm, 64, 64);
    
    u64 l = (initial_permutation_text >> 32) & 0xFFFFFFFF;
    u64 r = initial_permutation_text & 0xFFFFFFFF;

    // 16 rounds
    for (int i = 0; i < 16; i++){
        // update l, and r
        u64 old_l = l, old_r = r;
        l = old_r;
        u64 f = f_function(old_r, keys[i]);
        r = old_l ^ f;
    }
    // swap
    u64 text_swap = (r << 32) | l;
    // inverse permutation
    u64 inverse_permutation_text = permutation(text_swap, inverse_initial_perm, 64, 64);
    return inverse_permutation_text;
}

u64 decrypt(u64 cipher, u64 key){
    // get the initial permutation text
    u64 initial_permutation_cipher = permutation(cipher, initial_perm, 64, 64);
    
    u64 l = (initial_permutation_cipher >> 32) & 0xFFFFFFFF;
    u64 r = initial_permutation_cipher & 0xFFFFFFFF;

    // 16 rounds
    for (int i = 0; i < 16; i++){
        // update l, and r
        u64 old_l = l, old_r = r;
        l = old_r;
        u64 f = f_function(old_r, keys[15 - i]);
        r = old_l ^ f;
    }
    // swap
    u64 cipher_swap = (r << 32) | l;
    // inverse permutation
    u64 inverse_permutation_cipher = permutation(cipher_swap, inverse_initial_perm, 64, 64);
    return inverse_permutation_cipher;
}

u64 permutation(u64 data, const int table[], int in, int out){
    u64 per = 0;
    for (int i = 0; i < out; i++)
        per |= (data >> (in - table[out - 1 - i]) & 1) << i;
    return per;
}

void key_gen(u64 key){
    // Permutation Choice 1.
    u64 key_56 = permutation(key, pc1, 64, 56);
    u64 c = (key_56 >> 28) & 0xFFFFFFF;
    u64 d = key_56 & 0xFFFFFFF;
    for (int i = 1; i <= 16; i++){ 
        // Left shift
        if (i == 1 || i == 2 || i == 9 || i == 16){
            u64 c_1bit = (c >> 27) & 0x1;
            u64 d_1bit = (d >> 27) & 0x1;
            c = ((c << 1) & 0x000000000FFFFFFF) | c_1bit;
            d = ((d << 1) & 0x000000000FFFFFFF) | d_1bit;
        } else {
            u64 c_2bits = (c >> 26) & 0x3;
            u64 d_2bits = (d >> 26) & 0x3;
            c = ((c << 2) & 0x000000000FFFFFFF) | c_2bits;
            d = ((d << 2) & 0x000000000FFFFFFF) | d_2bits;
        }
        // Permutation Choice 2.
        u64 k = (c << 28) | d;
        keys[i - 1] = permutation(k, pc2, 56, 48);
    }
}

u64 s_box(u64 data){
    u64 s_32bits = 0;
    for (int i = 1; i <= 8; i++){
        int shift = 48 - (6 * i);
        int _6_bits = (data >> shift) & 0x3F;
        int row = (((_6_bits >> 5) & 0x1) << 1) | (_6_bits & 0x1);
        int column = (_6_bits >> 1) & 0xF;
        int ans = s[i - 1][row][column] & 0xF;
        s_32bits = (s_32bits << 4) | ans;
    }
    return s_32bits;
}

u64 f_function(u64 r, u64 k){
    u64 r_extend = permutation(r, extend, 32, 48);
    u64 r_xor_k = r_extend ^ k;
    u64 s_32bits = s_box(r_xor_k);
    return permutation(s_32bits, per, 32, 32);
}
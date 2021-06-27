#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<inttypes.h>
#include<string.h>
#include<math.h>
#define MAX_LIMIT 1024
uint32_t f(uint32_t X,uint32_t Y,uint32_t Z){
    return ( X & Y) | ((~X) & Z);
}
uint32_t g(uint32_t X,uint32_t Y,uint32_t Z){
    return ( X & Z) | ((~Z) & Y);
}
uint32_t h(uint32_t X,uint32_t Y,uint32_t Z){
    return X ^ Y ^ Z;
}
uint32_t ie(uint32_t X,uint32_t Y,uint32_t Z){
    return Y ^ (X | (~Z));
}

uint32_t rotateleft( uint32_t r, int N ){
    return ( (r<<N) | (r>>(32-N)) );
}
uint8_t swap64(uint64_t k, int i )
{
    return (k >> (i*8)) & 0xff;
}

uint32_t getword32(uint8_t x[], int start_address) {
    uint64_t p = 0;
    int i;
    for(i=0;i<4;i++) p = p | (x[start_address+i] << (i*8));
    return p;
}

uint32_t little_endian(uint32_t x) {
    uint32_t temp = 0;
    int i =0;
    for(i=0;i<4;i++) {
        temp = (temp << 8) | (x & 0xFF);
        x = x >> 8;
    }
    return temp;
}

int main() {
    uint32_t A,B,C,D,AA,BB,CC,DD;
    A = 0x67452301;
    B = 0xefcdab89;
    C = 0x98badcfe;
    D = 0x10325476;

    uint32_t T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    int s[64] = {
     7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
     5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
     4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
     6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };


    uint8_t string[MAX_LIMIT];
    printf("Enter NAME to be hashed: ");

    fgets(string,MAX_LIMIT,stdin);

    char *original_string = string;

    int length,i,j,k,block;
    uint64_t initlength;

    length = strlen(string)-1;
    initlength = length*8;

    unsigned char append = 0x80;
    while((length * 8) % 512 != 448){
            string[length] = append;
            append = 0X00;
            length+=1;
    }
    printf("After padding\n");
    for(i=0;i<length;i++) printf("%02x ",string[i]);
    

    for(i=0;i<8;i++){
        string[length] = swap64(initlength,i);
        length+=1;
    }
    
    

    uint64_t X[16];
    uint32_t F;

    int num_blocks = length / 64;

    for(block=0;block<num_blocks;block++){
        for(j=0;j<16;j++) X[j] = getword32(string,block*64+j*4);
        AA = A;
        BB = B;
        CC = C;
        DD = D;

        for(i=0;i<64;i++){
            if(i<16){
                F = f(BB,CC,DD);
                k = i;
            }else if(i<32){
                F = g(BB,CC,DD);
                k = (5*i+1) %16;
            }else if(i<48){
                F = h(BB,CC,DD);
                k = (3*i + 5) % 16;
            }else {
                F = ie(BB,CC,DD);
                k = (7*i) % 16;
            }
            F = AA + F + X[k] + T[i];
            uint32_t temp = DD;
            DD = CC;
            CC = BB;
            BB = BB + rotateleft(F,s[i]);
            AA = temp;
        }

        A = little_endian(A + AA);
        B = little_endian(B + BB);
        C = little_endian(C + CC);
        D = little_endian(D + DD);
    }
    printf("\nHash(MD5) of %s\b is: ",original_string);
    printf("%08X%08X%08X%08X\n",A,B,C,D);
    return 0;
}

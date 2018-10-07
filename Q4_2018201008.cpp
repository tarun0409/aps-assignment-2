#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;
class Map
{
    public:
    unsigned char * T;
    unsigned char * shuffle_array(unsigned char * arr, int n)
    {
        srand(time(NULL));
        for(int i=(n-1); i>0; i--)
        {
            int j = rand()%(i+1);
            unsigned char c = arr[i];
            arr[i] = arr[j];
            arr[j] = c;
        }
        return arr;
    }
    Map()
    {
        T = (unsigned char *)calloc(256,sizeof(unsigned char));
        for(int i=0; i<256; i++)
        {
            T[i] = i;
        }
        T = shuffle_array(T,256);
    }

    unsigned int get_murmur_hash(unsigned char* key, size_t len, unsigned int seed) 
    {
        unsigned int h = seed;
        if (len > 3) 
        {
            unsigned int * key_x4 = (unsigned int *)malloc(len*sizeof(unsigned int));
            for(int j=0; j<len; j++)
            {
                key_x4[j] = (unsigned int)key[j];
            }
            size_t i = len >> 2;
            do 
            {
                unsigned int k = *key_x4++;
                k *= 0xcc9e2d51;
                k = (k << 15) | (k >> 17);
                k *= 0x1b873593;
                h ^= k;
                h = (h << 13) | (h >> 19);
                h = (h * 5) + 0xe6546b64;
            } while (--i);
            // /key = (const uint8_t*) key_x4;
            for(int j=0; j<len; j++)
            {
                key[j] = (unsigned char)key[j];
            }
        }
        if (len & 3) 
        {
            size_t i = len & 3;
            unsigned int k = 0;
            key = &key[i - 1];
            do 
            {
                k <<= 8;
                k |= *key--;
            } while (--i);
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
        }
        h ^= len;
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h; 
    }

    unsigned int get_murmur_hash(int x)
    {
        vector<int> digits;
        while(x>0)
        {
            int d = x%10;
            digits.push_back(d);
            x = x/10;
        }
        reverse(digits.begin(),digits.end());
        int n = digits.size();
        unsigned char * digit_array = (unsigned char *)malloc(n*sizeof(unsigned char));
        return get_murmur_hash(digit_array,n,9999);
    }

    unsigned int get_pearson_hash(vector<int> x)
    {
        int len = x.size();
        size_t i,j;
        char * hex = (char *)malloc(17*sizeof(char));
        unsigned char hh[8];
        unsigned char h;
        for(int j=0; j<8; j++)
        {
            h = T[(x[0]+j)%256];
            for(int i=1; i<len; i++)
            {
                h = T[h ^ x[i]];
            }
            hh[j] = h;
        }
        snprintf(hex,17,"%02x%02x%02x%02x%02x%02x%02x%02x",hh[0],hh[1],hh[2],hh[3],hh[4],hh[5],hh[6],hh[7]);
        unsigned int hash_value;
        sscanf(hex,"%x",&hash_value);
        return hash_value;
    }
    unsigned int get_pearson_hash(int x)
    {
        vector<int> digits;
        while(x>0)
        {
            int d = x%10;
            digits.push_back(d);
            x = x/10;
        }
        reverse(digits.begin(),digits.end());
        return get_pearson_hash(digits);
    }
};

int main()
{
    
}
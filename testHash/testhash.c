/*
 * Author: Jianmeng Huang 
 * <jianmenghuang<AT>gmail.com>
 *
 * File: testhash.c
 * Create Date: 2015-10-11 20:52:01
 *
 */

#include<stdio.h>

unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}


int main(int argc, char * agrv[])
{
    char str[] = "Ljava/security/MessageDigest;digest,([B)[B";
    printf("%u\n", BKDRHash(str));
}




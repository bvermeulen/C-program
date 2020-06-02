/* playing with memory addresses, integers, and bytes

*/
#include <stdio.h>
#include <stdlib.h>

#define MAXNUM 3

typedef unsigned char BYTE;

void convertToBytes(int value, BYTE *bytes)
{
    /*
        one byte = 1111 1111 (bit) = FF (hex) = 255 (dec)

        Store in little Endian format - least significant byte in the
        lowest memory location

        shift 24, 16, 8 and zero bits to the right and mask to one byte
    */
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8)  & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
}

int convertToValue(BYTE *bytes)
{
    /*
        shift most significant byte 24 bits to the left and add value to
        the second most signficant byte 18 bits to the left, etc.
    */
    return ((bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + bytes[0]);
}

int main()
{
    BYTE bytes[4];
    unsigned int value, *intPtr;
    unsigned char *myint, *address;

    printf("Convert value to bytes .... ");
    printf("size of integer %d\n", sizeof(value));
    while (1)
    {
        printf("Give a value: ");
        scanf("%d", &value);

        convertToBytes(value, bytes);
        printf("%02x - %02x - %02x - %02x\n", bytes[0], bytes[1], bytes[2], bytes[3]);

        value = convertToValue(bytes);
        printf("is %d\n", value);

        // write an integer to an address - little endian [least significant byte at lowest address]
        myint = malloc(4*sizeof(BYTE));
        address = myint;
        for (int i = 0; i < 4; i++)
        {
            *address = bytes[i];
            printf("[%08x: %02x]/", address, bytes[i]);
            ++address;
        }
        printf("\n");

        // reset the address and read the 4 bytes integer
        address = myint;
        for (int i = 0; i < 4; i++)
        {
            bytes[i] = *address;
            ++address;
        }
        // convert bytes to an integer and print
        value = convertToValue(bytes);
        printf("is %d\n", value);

        intPtr = (unsigned int*) myint;
        value = *intPtr;
        printf("is %d\n", value);
    }
}
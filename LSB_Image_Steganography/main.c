#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

union bit_set {
    struct {
        unsigned char bit0: 1;
        unsigned char bit1: 1;
        unsigned char bit2: 1;
        unsigned char bit3: 1;
        unsigned char bit4: 1;
        unsigned char bit5: 1;
        unsigned char bit6: 1;
        unsigned char bit7: 1;
    } bits;
    uint8_t byte;
};

int encode(const char *input, char *txt, const char *output)
{
    if(input == NULL || txt == NULL || output == NULL)
        return 1;

    FILE *fin = fopen(input, "r");
    if(fin == NULL)
        return 2;

    FILE *fout = fopen(output, "w");
    if(fout == NULL)
        return 4;


    return 0;
}

int decode(const char * filename, char *txt, int size)
{
    if(filename == NULL || txt == NULL || size <= 0)
        return 1;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return 2;

    char binary_message[100000];
    int pixel_value;
    int byte_count = 0;

    while(fscanf(fp, "%d", &pixel_value) == 1){
        if(pixel_value < 0 || pixel_value > 255) {
            fclose(fp);
            return 3;
        }
        union bit_set bs;
        bs.byte = pixel_value;
        *(binary_message + byte_count) = bs.bits.bit0 + '0';
        byte_count++;
    }


    *(binary_message + size - 1) = '\0';

    int len = 0;
    union bit_set bs;
    *txt = '\0';

    for(int i=0; i<size; i+=8){
        bs.bits.bit7 = *(binary_message + i);
        bs.bits.bit6 = *(binary_message + i + 1);
        bs.bits.bit5 = *(binary_message + i + 2);
        bs.bits.bit4 = *(binary_message + i + 3);
        bs.bits.bit3 = *(binary_message + i + 4);
        bs.bits.bit2 = *(binary_message + i + 5);
        bs.bits.bit1 = *(binary_message + i + 6);
        bs.bits.bit0 = *(binary_message + i + 7);

        char ch = (char)bs.byte;
        *(txt + len) = ch;
        len++;
    }

    return 0;
}


int main() {
    char mode;
    char message[1001];
    char filename_input[30], filename_output[30];
    printf("Do you want to encode (E/e) or decode (D/d) a message?");
    if(scanf("%c", &mode) != 1){
        printf("Incorrect input");
        return 1;
    }

    mode = (char)toupper(mode);
    if(mode != 'E' && mode != 'D'){
        printf("Incorrect input data");
        return 2;
    }

    switch(mode)
    {
        case 'D':

            printf("Enter input file name:");
            if(scanf("%29s", filename_input) != 1){
                printf("Incorrect input");
            }
            FILE *fin = fopen(filename_input, "r");
            if(fin == NULL){
                printf("Couldn't open file");
                return 4;
            }

            fclose(fin);
            char txt[100000];
            int res = decode(filename_input, txt, sizeof(txt));
            if(res != 0){
                printf("File corrupted");
                return 6;
            }
            printf("%s", txt);
            break;

        case 'E':
            printf("Enter a message to be encoded:");
            if(scanf("1000%[^\n]", message) != 1){
                printf("Incorrect input");
                return 1;
            }

            printf("Enter input file name:");
            if(scanf("%39s", filename_input) != 1){
                printf("Incorrect input");
            }
            while(getchar() != '\n'){}
            fin = fopen(filename_input, "r");
            if(fin == NULL){
                printf("Couldn't open file");
                return 4;
            }

            printf("Enter output file name:");
            if(scanf("%39s", filename_output) != 1){
                printf("Incorrect input");
            }
            FILE *fout = fopen(filename_output, "w");
            if(fout == NULL){
                printf("Couldn't create file");
                return 5;
            }


            break;

        default:
            break; // condition is already checked
    }



    return 0;
}

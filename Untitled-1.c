#include <stdio.h>
#include <stdlib.h>
int main(void){
    unsigned char header[0x36];
    unsigned char **data;
    unsigned char *temp_data;
    char in_f_name[20] = "./LENNA.bmp", out_f_name[20];
    FILE *in_f, *out_f;
    long start_addr, end_addr;
    int width, height, color_bit;
    if ((in_f=fopen(in_f_name, "rb"))==NULL){
        printf("Error on open file.\n");
    }

    /* header */
    for (int i=0; i<0x36; i++){
        header[i] = fgetc(in_f);
        // printf("%x: %x\n", i, header[i]);
    }
    start_addr = header[0x0A] + header[0x0B] * 0x100;
    end_addr = header[0x02] + header[0x03]*0x100 + header[0x04]*0x10000 + header[0x05]*0x1000000;
    width = header[0x12] + header[0x13] * 0x100;
    height = header[0x16] + header[0x17] * 0x100;
    color_bit = header[0x1C];
    printf("data_addr: 0x%x to 0x%x\n", start_addr, end_addr);
    printf("width: %d\n", width);
    printf("height: %d\n", height);
    printf("color: %xbit\n", color_bit);

    /* color parett*/
    for (int i=0x36; i<start_addr; i++){
        fgetc(in_f);
    }

    /* data */
    data = (char **)malloc(sizeof(char *) * height);
    for(int i=0; i<height; i++){
        data[i] = (char *) malloc(sizeof(char)*width);
    }
    temp_data = (char *)malloc(sizeof(char) * (end_addr - start_addr));
    for(int i=start_addr; i<end_addr; i++){
        temp_data[i-start_addr] = fgetc(in_f);
    }

    free(temp_data);
    free(data);

}
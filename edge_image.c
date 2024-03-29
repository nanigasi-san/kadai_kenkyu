#include <stdio.h>
#include <stdlib.h>

int is_frame(int index, int width, int height);
int check_cross(int index, int width, int height, unsigned char *temp_data);

int main(void){
    unsigned char header[0x36];
    unsigned char color_parett[0x435-0x36];
    unsigned char **data;
    unsigned char *temp_data, *conv_data;
    char in_f_name[20] = "./LENNA512.bmp", out_f_name[20] = "./output_edge.bmp";
    FILE *in_f, *out_f;
    long start_addr, end_addr;
    int width, height, color_bit;
    int i, j, k;
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
        color_parett[i-0x36] = fgetc(in_f);
    }

    /* data */
    data = (char **)malloc(sizeof(char *) * height);
    for(i=0; i<height; i++){
        data[i] = (char *) malloc(sizeof(char)*width);
    }

    temp_data = (char *) malloc(sizeof(char)*height*width);

    // 1D data for 2D data
    temp_data = (char *)malloc(sizeof(char) * (end_addr - start_addr));
    for(i=start_addr; i<end_addr; i++){
        temp_data[i-start_addr] = fgetc(in_f);
    }
    /* convert to 2D data*/

    /*get edge*/
    conv_data = (char *)malloc(sizeof(char) * (end_addr - start_addr));

    for(i=start_addr; i<end_addr; i++){
        if (is_frame(i-start_addr, width, height)) {
            // printf("index: %x is a frame of image.\n", i);
        }
        else {
            if (check_cross(i-start_addr, width, height, temp_data)) {
                conv_data[i-start_addr] = 0xFF;
            }
            else {
                conv_data[i-start_addr] = temp_data[i-start_addr];
            }
        }
    }


    /*data output*/
    for (int i = 0; i < 0x36; i++){
        // printf("%x, ", header[i]);
    }
    for (int i = 0x36; i < start_addr; i++){
        // printf("%x", color_parett[i]);
    }
    for (i = 0; i < end_addr - start_addr; i++){
        // printf("%x", temp_data[i]);
    }

    /* writing */
    if ((out_f = fopen(out_f_name, "wb")) == NULL){
        printf("Error on open file.\n");
    }
    fprintf(out_f, "%c", 0x42);
    for (i = 1; i < 0x36; i++){
        fprintf(out_f, "%c", header[i]);
    }
    for (i = 0; i < 0x435-0x36; i++){
        fprintf(out_f, "%c", color_parett[i]);
    }
    for (i = 0; i < end_addr - start_addr; i++){
        fprintf(out_f, "%c", conv_data[i]);
    }
    fclose(out_f);

    free(temp_data);
    free(data);

}

int is_frame(int index, int width, int height) {
    if ((index / width) == 0 || (index / width) == height-1){
        return 1;
    }
    else if ((index%width)==0 || (index%width)==width-1){
        return 1;
    }
    else {
        return 0;
    }
}

int check_cross(int index, int width, int height, unsigned char *temp_data) {
    int flag=0, range=10;
    int a, b;
    // 上下
    a = index - width;
    b = index + width;
    if (abs(temp_data[a] - temp_data[b]) < range) {
        flag += 1;
    }
    // 左右
    a = index - 1;
    b = index + 1;
    if (abs(temp_data[a] - temp_data[b]) < range) {
        flag += 1;
    }
    // 斜め1
    a = index - width - 1;
    b = index + width + 1;
    if (abs(temp_data[a] - temp_data[b]) < range) {
        flag += 1;
    }
    // 斜め2
    a = index - width + 1;
    b = index + width - 1;
    if (abs(temp_data[a] - temp_data[b]) < range) {
        flag += 1;
    }
    return flag;
}
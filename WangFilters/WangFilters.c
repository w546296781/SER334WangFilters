/**
* A simple program that use multiple threads to process a bmp
*
* Completion time: 12h
*
* @author Xinkai Wang
* @version 2020/3/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

#define THREAD_COUNT 4

int imageWidth;
int imageHeight;
// P1,P2,P3
// P4,P0,P5
// P6,P7,P8
// 0-head, 1-tail, 2-other
void boxblur_process(void* arg) {
    struct Pixel** pArr = arg;
    
    int startCol = 0;
    int endCol = imageWidth;
    
    for (int i = 0; i < imageHeight; i++) {
        for (int j = startCol; j < endCol; j++) {

            int p1_r = 0, p1_g = 0, p1_b = 0;
            int p2_r = 0, p2_g = 0, p2_b = 0;
            int p3_r = 0, p3_g = 0, p3_b = 0;
            int p4_r = 0, p4_g = 0, p4_b = 0;
            int p5_r = 0, p5_g = 0, p5_b = 0;
            int p6_r = 0, p6_g = 0, p6_b = 0;
            int p7_r = 0, p7_g = 0, p7_b = 0;
            int p8_r = 0, p8_g = 0, p8_b = 0;
            int count = 1;

            if (i - 1 >= 0 && j - 1 >= 0) {
                p1_r = pArr[i - 1][j - 1].rgbred;
                p1_g = pArr[i - 1][j - 1].rgbgreen;
                p1_b = pArr[i - 1][j - 1].rgbblue;
                count++;
            }
            if (i - 1 >= 0 && j >= 0) {
                p2_r = pArr[i - 1][j].rgbred;
                p2_g = pArr[i - 1][j].rgbgreen;
                p2_b = pArr[i - 1][j].rgbblue;
                count++;
            }
            if (i - 1 >= 0 && j + 1 < imageWidth) {
                p3_r = pArr[i - 1][j + 1].rgbred;
                p3_g = pArr[i - 1][j + 1].rgbgreen;
                p3_b = pArr[i - 1][j + 1].rgbblue;
                count++;
            }
            if (i >= 0 && j - 1 >= 0) {
                p4_r = pArr[i][j - 1].rgbred;
                p4_g = pArr[i][j - 1].rgbgreen;
                p4_b = pArr[i][j - 1].rgbblue;
                count++;
            }
            if (i >= 0 && j + 1 < imageWidth) {
                p5_r = pArr[i][j + 1].rgbred;
                p5_g = pArr[i][j + 1].rgbgreen;
                p5_b = pArr[i][j + 1].rgbblue;
                count++;
            }
            if (i +1 <imageHeight && j - 1 >= 0) {
                p6_r = pArr[i + 1][j - 1].rgbred;
                p6_g = pArr[i + 1][j - 1].rgbgreen;
                p6_b = pArr[i + 1][j - 1].rgbblue;
                count++;
            }
            if (i + 1 <imageHeight && j >= 0) {
                p7_r = pArr[i + 1][j].rgbred;
                p7_g = pArr[i + 1][j].rgbgreen;
                p7_b = pArr[i + 1][j].rgbblue;
                count++;
            }
            if (i + 1 < imageHeight && j + 1 < imageWidth) {
                p8_r = pArr[i + 1][j + 1].rgbred;
                p8_g = pArr[i + 1][j + 1].rgbgreen;
                p8_b = pArr[i + 1][j + 1].rgbblue;
                count++;
            }

            pArr[i][j].rgbred = (p1_r + p2_r + p3_r + p4_r + p5_r + p6_r + p7_r + p8_r + pArr[i][j].rgbred ) / count;
            pArr[i][j].rgbgreen = (p1_g + p2_g + p3_g + p4_g + p5_g + p6_g + p7_g + p8_g + pArr[i][j].rgbgreen) / count;
            pArr[i][j].rgbblue = (p1_b + p2_b + p3_b + p4_b + p5_b + p6_b + p7_b + p8_b + pArr[i][j].rgbblue) / count;

        }
    }
    printf("End");
}

void cheese_process(struct Pixel** pArr) {
    for (int i = 0; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth; j++) {
            if (pArr[i][j].token == 1) {
                pArr[i][j].rgbblue = 0;
                pArr[i][j].rgbgreen = 0;
                pArr[i][j].rgbred = 0;
            }
        }
    }

    colorShiftPixels(pArr, imageWidth, imageHeight, 0, 20, 20);
}

void cheese_prepare(struct Pixel** pArr, int width, int height) {
    int radius = 0;
    if (width < height) {
        radius = width / 10;
    }
    else {
        radius = height / 10;
    }

    int nholeNum = radius / 2;
    int nholeRadius = radius;
    int sholeNum = (radius - nholeNum) / 2;
    int sholeRadius = radius / 3 * 2;
    int lholeNum = radius - nholeNum - sholeNum;
    int lholeRadius = radius / 2 * 3;

    int** center = (int**)malloc(sizeof(int*) * radius);
    for (int i = 0; i < radius; i++) {
        center[i] = (int*)malloc(sizeof(int) * 2);
    }

    for (int i = 0; i < radius; i++) {
        center[i][0] = rand() % width;
        center[i][1] = rand() % height;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < sholeNum; k++) {
                if (sqrt((i - center[k][1]) * (i - center[k][1]) + (j - center[k][0]) * (j - center[k][0])) < sholeRadius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }

            for (int k = sholeNum; k < sholeNum + nholeNum; k++) {
                if (sqrt((i - center[k][1]) * (i - center[k][1]) + (j - center[k][0]) * (j - center[k][0])) < nholeRadius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }

            for (int k = sholeNum + nholeNum; k < radius; k++) {
                if (sqrt((i - center[k][1]) * (i - center[k][1]) + (j - center[k][0]) * (j - center[k][0])) < lholeRadius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < radius; i++) {
        free(center[i]);
    }
    free(center);
}

int main(int argc, char** argv)
{
    int type = 0;
    char* inputName = "";
    char* outputName = "";
    
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-i") == 0)
            inputName = argv[i+1];
        else if(strcmp(argv[i], "-o") == 0)
            outputName = argv[i+1];
        else if(strcmp(argv[i], "b") == 0)
            type = 1;
    }

    //read file
    FILE* readfile = fopen(inputName, "rb");

    struct BMP_Header* bmp_header;
    bmp_header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* dib_header;
    dib_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));

    readBMPHeader(readfile, bmp_header);
    readDIBHeader(readfile, dib_header);

    struct Pixel** pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->ImageHight);
    for (int i = 0; i < dib_header->ImageHight; i++) {
        pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * dib_header->ImageWidth);
    }

    readPixelsBMP(readfile, pArr, dib_header->ImageWidth, dib_header->ImageHight);

    pthread_t threads[THREAD_COUNT];
    int threads_result[THREAD_COUNT];

    //cut pArr
    int cutSize = dib_header->ImageWidth / THREAD_COUNT;
    int cutRemainder = dib_header->ImageWidth % THREAD_COUNT;
    
    imageHeight = dib_header->ImageHight;
    imageWidth = cutSize + cutRemainder;

    if(type == 1){
        struct Pixel*** cutpArr = (struct Pixel***)malloc(sizeof(struct Pixel**) * THREAD_COUNT);
        for (int i = 0; i < THREAD_COUNT; i++) {
            cutpArr[i] = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->ImageHight);
            for (int j = 0; j < dib_header->ImageHight; j++) {
                cutpArr[i][j] = (struct Pixel*)malloc(sizeof(struct Pixel) * (cutSize + cutRemainder + 1));
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == 0) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + 1; k++) {
                        cutpArr[i][j][k] = pArr[j][k];
                    }
                }
            }
            else if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + 1 + cutRemainder; k++) {
                        cutpArr[i][j][k] = pArr[j][k + i * cutSize - 1];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + 2; k++) {
                        cutpArr[i][j][k] = pArr[j][k + i * cutSize - 1];
                    }
                }
            }
        }

        //create threads
        for (int i = 0; i < THREAD_COUNT; i++) {

            threads_result[i] = pthread_create(&threads[i], NULL, boxblur_process, cutpArr[i]);

            if (threads_result[i] != 0) {
                printf("Failed to create thread %d", i);
            }
            else {
                printf("Success create thread %d", i);
            }
        }

        //waiting threads
        void* retval;
        for (int i = 0; i < THREAD_COUNT; i++) {
            pthread_join(threads[i], &retval);
        }
    
        //reassemble
    
        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == 0) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + 1; k++) {
                        pArr[j][k] = cutpArr[i][j][k];
                    }
                }
            }
            else if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + cutRemainder; k++) {
                        pArr[j][k + i * cutSize] = cutpArr[i][j][k+1];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize; k++) {
                        pArr[j][k + i * cutSize] = cutpArr[i][j][k+1];
                    }
                }
            }
        }    

    
/*
        for (int i = 0; i < THREAD_COUNT; i++) {
            for (int j = 0; j < dib_header->ImageHight; j++) {
                free(cutpArr[i][j]);
            }
            free(cutpArr[i]);
        }
        free(cutpArr);
*/
    }
    else {
        cheese_prepare(pArr, dib_header->ImageWidth, dib_header->ImageHight);

        struct Pixel*** cutpArr = (struct Pixel***)malloc(sizeof(struct Pixel**) * THREAD_COUNT);
        for (int i = 0; i < THREAD_COUNT; i++) {
            cutpArr[i] = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->ImageHight);
            for (int j = 0; j < dib_header->ImageHight; j++) {
                cutpArr[i][j] = (struct Pixel*)malloc(sizeof(struct Pixel) * (cutSize + cutRemainder));
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + cutRemainder; k++) {
                        cutpArr[i][j][k] = pArr[j][k + i * cutSize];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize; k++) {
                        cutpArr[i][j][k] = pArr[j][k + i * cutSize];
                    }
                }
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {

            threads_result[i] = pthread_create(&threads[i], NULL, cheese_process, cutpArr[i]);

            if (threads_result[i] != 0) {
                printf("Failed to create thread %d", i);
            }
            else {
                printf("Success create thread %d", i);
            }
        }

        //waiting threads
        void* retval;
        for (int i = 0; i < THREAD_COUNT; i++) {
            pthread_join(threads[i], &retval);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize + cutRemainder; k++) {
                        pArr[j][k + i * cutSize] = cutpArr[i][j][k];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->ImageHight; j++) {
                    for (int k = 0; k < cutSize; k++) {
                        pArr[j][k + i * cutSize] = cutpArr[i][j][k];
                    }
                }
            }
        }

    }

    //write file
    FILE* writefile = fopen(outputName, "wb");
    writeBMPHeader(writefile, bmp_header);
    writeDIBHeader(writefile, dib_header);
    writePixelsBMP(writefile, pArr, dib_header->ImageWidth, dib_header->ImageHight);


    fclose(writefile);
    for (int i = 0; i < dib_header->ImageHight; i++) {
        free(pArr[i]);
    }
    free(pArr);
    free(dib_header);
    free(bmp_header);
    printf("Complete!\n");
}
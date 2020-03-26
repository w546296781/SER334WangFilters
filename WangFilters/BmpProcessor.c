/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "PixelProcessor.h"
#include "BmpProcessor.h"

void readBMPHeader(FILE* file, struct BMP_Header* header){

    
    fread(&header->Signatue, sizeof(U8)*2, 1, file);
    fread(&header->FileSize, sizeof(U32), 1, file);
    fread(&header->Reserv1, sizeof(U16), 1, file);
    fread(&header->Reserv2, sizeof(U16), 1, file);
    fread(&header->FileOffset, sizeof(U32), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header){
    fwrite(&header->Signatue, sizeof(U8)*2, 1, file);
    fwrite(&header->FileSize, sizeof(U32), 1, file);
    fwrite(&header->Reserv1, sizeof(U16), 1, file);
    fwrite(&header->Reserv2, sizeof(U16), 1, file);
    fwrite(&header->FileOffset, sizeof(U32), 1, file);
}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header){
    fread(&header->DIBHeaderSize, sizeof(U32), 1, file);
    fread(&header->ImageWidth, sizeof(U32), 1, file);
    fread(&header->ImageHight, sizeof(U32), 1, file);
    fread(&header->Planes, sizeof(U16), 1, file);
    fread(&header->BPP, sizeof(U16), 1, file);
    fread(&header->Compression, sizeof(U32), 1, file);
    fread(&header->ImageSize, sizeof(U32), 1, file);
    fread(&header->XPPM, sizeof(U32), 1, file);
    fread(&header->YPPM, sizeof(U32), 1, file);
    fread(&header->CCT, sizeof(U32), 1, file);
    fread(&header->ICC, sizeof(U32), 1, file);
}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header){
    fwrite(&header->DIBHeaderSize, sizeof(U32), 1, file);
    fwrite(&header->ImageWidth, sizeof(U32), 1, file);
    fwrite(&header->ImageHight, sizeof(U32), 1, file);
    fwrite(&header->Planes, sizeof(U16), 1, file);
    fwrite(&header->BPP, sizeof(U16), 1, file);
    fwrite(&header->Compression, sizeof(U32), 1, file);
    fwrite(&header->ImageSize, sizeof(U32), 1, file);
    fwrite(&header->XPPM, sizeof(U32), 1, file);
    fwrite(&header->YPPM, sizeof(U32), 1, file);
    fwrite(&header->CCT, sizeof(U32), 1, file);
    fwrite(&header->ICC, sizeof(U32), 1, file);
}

/**
 * make BMP header based on width and height. 
 * The purpose of this is to create a new BMPHeader struct using the information 
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height){
    header->Signatue[0] = 'B';
    header->Signatue[1] = 'M';
    header->Reserv1 = 0;
    header->Reserv2 = 0;
    header->FileOffset = sizeof(struct BMP_Header) + sizeof(struct DIB_Header) - 2;
    header->FileSize = header->FileOffset + width * height * 24 / 8;
}


 /**
 * Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeDIBHeader(struct DIB_Header* header, int width, int height){
    header->DIBHeaderSize = sizeof(struct DIB_Header);
    header->ImageHight = height;
    header->ImageWidth = width;
    header->ImageSize = width * height * 24 / 8;
    header->Planes = 1;
    header->BPP = 24;
    header->CCT = 0;
    header->Compression = 0;
    header->ICC = 0;
    header->XPPM = 3780;
    header->YPPM = 3780;
}


/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fread(&pArr[i][j].rgbred, sizeof(U8), 1, file);
            fread(&pArr[i][j].rgbblue, sizeof(U8), 1, file);
            fread(&pArr[i][j].rgbgreen, sizeof(U8), 1, file);
        }
    }
}


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fwrite(&pArr[i][j].rgbred, sizeof(U8), 1, file);
            fwrite(&pArr[i][j].rgbblue, sizeof(U8), 1, file);
            fwrite(&pArr[i][j].rgbgreen, sizeof(U8), 1, file);
        }
    }
}


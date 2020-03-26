/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "PixelProcessor.h"

void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){

            if (pArr[i][j].rgbred + rShift > 255)
                pArr[i][j].rgbred = 255;
            else
                pArr[i][j].rgbred += rShift;

            if (pArr[i][j].rgbgreen + gShift > 255)
                pArr[i][j].rgbgreen = 255;
            else
                pArr[i][j].rgbgreen += gShift;

            if (pArr[i][j].rgbblue + bShift > 255)
                pArr[i][j].rgbblue = 255;
            else
                pArr[i][j].rgbblue += bShift;

            /*
            pArr[i][j].rgbblue += gShift;
            pArr[i][j].rgbgreen += rShift;
            pArr[i][j].rgbred += bShift;
            
            if(pArr[i][j].rgbred > 255)
                pArr[i][j].rgbred = 255;
            else if(pArr[i][j].rgbred < 0)
                pArr[i][j].rgbred = 0;
            
            if(pArr[i][j].rgbgreen > 255)
                pArr[i][j].rgbgreen = 255;
            else if(pArr[i][j].rgbgreen < 0)
                pArr[i][j].rgbgreen = 0;
            
            if(pArr[i][j].rgbblue > 255)
                pArr[i][j].rgbblue = 255;
            else if(pArr[i][j].rgbblue < 0)
                pArr[i][j].rgbblue = 0;
                */
        }
    }
}
#include <Arduino.h>


#define IMG_WIDTH  320
#define IMG_HEIGHT 240

#define BITS_PER_PIXEL    (16)
#define BI_BITFIELD       (3)
#define IMG_SIZE (IMG_WIDTH*IMG_HEIGHT*(BITS_PER_PIXEL/8))
#define HEADER_SIZE       (54)
#define INFO_HEADER_SIZE  (40)
#define MASK_SIZE         (12)

/* Bitmap Header parameters */
uint16_t bfType = 0x4D42; /* "BM" */
uint32_t bfSize = IMG_SIZE + HEADER_SIZE + MASK_SIZE; /* image size + 54 + 12*/
uint16_t bfReserved1 = 0;
uint16_t bfReserved2 = 0;
uint32_t bfOffBits = HEADER_SIZE + MASK_SIZE;
uint32_t biSize = INFO_HEADER_SIZE;
uint32_t biWidth = IMG_WIDTH;
uint32_t biHeight = IMG_HEIGHT;
uint16_t biPlanes = 1;
uint16_t biBitCount = BITS_PER_PIXEL;
uint32_t biCompression = BI_BITFIELD;
uint32_t biSizeImage = IMG_SIZE;
uint32_t biXPelsPerMeter = 4724; /* dummy */
uint32_t biYPelsPerMeter = 4724; /* dummy */
uint32_t biClrUsed = 0;
uint32_t biClrImportant = 0;
uint32_t biRmask = 0x0000f800; /* 16bit mask */
uint32_t biGmask = 0x000007e0; /* 16bit mask */
uint32_t biBmask = 0x0000001f; /* 16bit mask */

uint8_t WORD[2];
uint8_t* swap16(uint16_t word16) {
  WORD[0] = (uint8_t)(word16 & 0x00ff);
  WORD[1] = (uint8_t)((word16 & 0xff00) >> 8);
  return WORD;
}

uint8_t DWORD[4];
uint8_t* swap32(uint32_t dword32) {
  DWORD[0] = (uint8_t)(dword32 & 0x000000ff);
  DWORD[1] = (uint8_t)((dword32 & 0x0000ff00) >> 8);
  DWORD[2] = (uint8_t)((dword32 & 0x00ff0000) >> 16);
  DWORD[3] = (uint8_t)((dword32 & 0xff000000) >> 24);
  return DWORD;
}

uint8_t BMP_HEADER[HEADER_SIZE+MASK_SIZE];

uint8_t* make_bmp_header(int* bmp_header_size) {
  uint8_t* word16;
  uint8_t* word32;
  int n = 0;

  *bmp_header_size = HEADER_SIZE+MASK_SIZE;
  
  word16 = swap16(bfType); /* "BM" */
  for (int i = 0; i < 2; ++i) {
    BMP_HEADER[n++] = word16[i];  
  }
  word32 = swap32(bfSize); /* File Size */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word16 = swap16(bfReserved1); /* Reserved */
  for (int i = 0; i < 2; ++i) {
    BMP_HEADER[n++] = word16[i];  
  }
  word16 = swap16(bfReserved2); /* Reserved */
  for (int i = 0; i < 2; ++i) {
    BMP_HEADER[n++] = word16[i];  
  }
  word32 = swap32(bfOffBits); /* Offset to image data */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biSize); /* Bitmap info structure size (40) */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }  
  word32 = swap32(biWidth); /* Image width */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }  
  word32 = swap32(biHeight); /* Image height */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }    
  word16 = swap16(biPlanes); /* Image plane (almost 1) */
  for (int i = 0; i < 2; ++i) {
    BMP_HEADER[n++] = word16[i];  
  }   
  word16 = swap16(biBitCount); /* Pixel per bits */
  for (int i = 0; i < 2; ++i) {
    BMP_HEADER[n++] = word16[i];  
  }   
  word32 = swap32(biCompression); /* Complession type */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biSizeImage); /* Image size */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biXPelsPerMeter); /* Resolution (dummy) */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biYPelsPerMeter); /* Resolution (dummy) */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biClrUsed); /* Color used */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biClrImportant); /* Important Color */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biRmask); /* Bitmask for red in case of 16bits color */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biGmask); /* Bitmask for green in case of 16bits color */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }
  word32 = swap32(biBmask); /* Bitmask for blue in case of 16bits color */
  for (int i = 0; i < 4; ++i) {
    BMP_HEADER[n++] = word32[i];  
  }

  if (n != HEADER_SIZE + MASK_SIZE)  {
    Serial.println("HEADER_SIZE ERROR");
    exit(1);
  }   

  return BMP_HEADER;
}

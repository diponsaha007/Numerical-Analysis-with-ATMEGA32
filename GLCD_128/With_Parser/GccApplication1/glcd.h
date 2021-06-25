#include <avr/io.h>
#include <string.h>
//#include <avr/pgmspace.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#define RS    PC0  
#define E     PC4  
#define CS1    PC3  
#define CS2    PC2
#define RST    PC1

#define DATA   PORTD
//PA1 PC0
//PA3 PC4

void Delay1();
void InitLCD();
void LCDout(char a);
void Epulse();
void PutPixel(unsigned char x,unsigned char y,char color);
void LCD_Clear();
void line(char x0, char y0, char x1, char y1,char color);
void rect(char x0, char y0, char x1, char y1,char color);
//void circle(int xCenter, int yCenter, int radius, char c);
void DisplayChar(unsigned char x,unsigned char y, char d);
void DisplayText(unsigned char x, unsigned char y, char d[21]);
void DisplayCharSmall(unsigned char x,unsigned char y, char d);
char Pix[128][8];
//char mystr[16], Height[4], Height1[4],bits;
//uint32_t WordOfData; 
//long Cunt;
//int Height;
/*--------------------------------------------------------------------------------------------------
                                     Character generator

             This table defines the standard ASCII characters in a 5x7 dot format.
--------------------------------------------------------------------------------------------------*/
PROGMEM static const char FontLookup [][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00},// (space) 
 {0x00, 0x00, 0x5F, 0x00, 0x00},// ! 
 {0x00, 0x07, 0x00, 0x07, 0x00},// " 
 {0x14, 0x7F, 0x14, 0x7F, 0x14},// # 
 {0x24, 0x2A, 0x7F, 0x2A, 0x12},// $ 
 {0x23, 0x13, 0x08, 0x64, 0x62},// % 
 {0x36, 0x49, 0x55, 0x22, 0x50},// & 
 {0x00, 0x05, 0x03, 0x00, 0x00},// ' 
 {0x00, 0x1C, 0x22, 0x41, 0x00},// ( 
 {0x00, 0x41, 0x22, 0x1C, 0x00},// ) 
 {0x08, 0x2A, 0x1C, 0x2A, 0x08},// * 
 {0x08, 0x08, 0x3E, 0x08, 0x08},// + 
 {0x00, 0x50, 0x30, 0x00, 0x00},// , 
 {0x08, 0x08, 0x08, 0x08, 0x08},// - 
 {0x00, 0x30, 0x30, 0x00, 0x00},// . 
 {0x20, 0x10, 0x08, 0x04, 0x02},// / 
 {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0 
 {0x00, 0x42, 0x7F, 0x40, 0x00},// 1 
 {0x42, 0x61, 0x51, 0x49, 0x46},// 2 
 {0x21, 0x41, 0x45, 0x4B, 0x31},// 3 
 {0x18, 0x14, 0x12, 0x7F, 0x10},// 4 
 {0x27, 0x45, 0x45, 0x45, 0x39},// 5 
 {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6 
 {0x01, 0x71, 0x09, 0x05, 0x03},// 7 
 {0x36, 0x49, 0x49, 0x49, 0x36},// 8 
 {0x06, 0x49, 0x49, 0x29, 0x1E},// 9 
 {0x00, 0x36, 0x36, 0x00, 0x00},// : 
 {0x00, 0x56, 0x36, 0x00, 0x00},// ; 
 {0x00, 0x08, 0x14, 0x22, 0x41},// < 
 {0x14, 0x14, 0x14, 0x14, 0x14},// = 
 {0x41, 0x22, 0x14, 0x08, 0x00},// > 
 {0x02, 0x01, 0x51, 0x09, 0x06},// ? 
 {0x32, 0x49, 0x79, 0x41, 0x3E},// @ 
 {0x7E, 0x11, 0x11, 0x11, 0x7E},// A 
 {0x7F, 0x49, 0x49, 0x49, 0x36},// B 
 {0x3E, 0x41, 0x41, 0x41, 0x22},// C 
 {0x7F, 0x41, 0x41, 0x22, 0x1C},// D 
 {0x7F, 0x49, 0x49, 0x49, 0x41},// E 
 {0x7F, 0x09, 0x09, 0x01, 0x01},// F 
 {0x3E, 0x41, 0x41, 0x51, 0x32},// G 
 {0x7F, 0x08, 0x08, 0x08, 0x7F},// H 
 {0x00, 0x41, 0x7F, 0x41, 0x00},// I 
 {0x20, 0x40, 0x41, 0x3F, 0x01},// J 
 {0x7F, 0x08, 0x14, 0x22, 0x41},// K 
 {0x7F, 0x40, 0x40, 0x40, 0x40},// L 
 {0x7F, 0x02, 0x04, 0x02, 0x7F},// M 
 {0x7F, 0x04, 0x08, 0x10, 0x7F},// N 
 {0x3E, 0x41, 0x41, 0x41, 0x3E},// O 
 {0x7F, 0x09, 0x09, 0x09, 0x06},// P 
 {0x3E, 0x41, 0x51, 0x21, 0x5E},// Q 
 {0x7F, 0x09, 0x19, 0x29, 0x46},// R 
 {0x46, 0x49, 0x49, 0x49, 0x31},// S 
 {0x01, 0x01, 0x7F, 0x01, 0x01},// T 
 {0x3F, 0x40, 0x40, 0x40, 0x3F},// U 
 {0x1F, 0x20, 0x40, 0x20, 0x1F},// V 
 {0x7F, 0x20, 0x18, 0x20, 0x7F},// W 
 {0x63, 0x14, 0x08, 0x14, 0x63},// X 
 {0x03, 0x04, 0x78, 0x04, 0x03},// Y 
 {0x61, 0x51, 0x49, 0x45, 0x43},// Z 
 {0x00, 0x00, 0x7F, 0x41, 0x41},// [ 
 {0x02, 0x04, 0x08, 0x10, 0x20},// "\" 
 {0x41, 0x41, 0x7F, 0x00, 0x00},// ] 
 {0x04, 0x02, 0x01, 0x02, 0x04},// ^ 
 {0x40, 0x40, 0x40, 0x40, 0x40},// _ 
 {0x00, 0x01, 0x02, 0x04, 0x00},// ` 
 {0x20, 0x54, 0x54, 0x54, 0x78},// a 
 {0x7F, 0x48, 0x44, 0x44, 0x38},// b 
 {0x38, 0x44, 0x44, 0x44, 0x20},// c 
 {0x38, 0x44, 0x44, 0x48, 0x7F},// d 
 {0x38, 0x54, 0x54, 0x54, 0x18},// e 
 {0x08, 0x7E, 0x09, 0x01, 0x02},// f 
 {0x08, 0x14, 0x54, 0x54, 0x3C},// g 
 {0x7F, 0x08, 0x04, 0x04, 0x78},// h 
 {0x00, 0x44, 0x7D, 0x40, 0x00},// i 
 {0x20, 0x40, 0x44, 0x3D, 0x00},// j 
 {0x00, 0x7F, 0x10, 0x28, 0x44},// k 
 {0x00, 0x41, 0x7F, 0x40, 0x00},// l 
 {0x7C, 0x04, 0x18, 0x04, 0x78},// m 
 {0x7C, 0x08, 0x04, 0x04, 0x78},// n 
 {0x38, 0x44, 0x44, 0x44, 0x38},// o 
 {0x7C, 0x14, 0x14, 0x14, 0x08},// p 
 {0x08, 0x14, 0x14, 0x18, 0x7C},// q 
 {0x7C, 0x08, 0x04, 0x04, 0x08},// r 
 {0x48, 0x54, 0x54, 0x54, 0x20},// s 
 {0x04, 0x3F, 0x44, 0x40, 0x20},// t 
 {0x3C, 0x40, 0x40, 0x20, 0x7C},// u 
 {0x1C, 0x20, 0x40, 0x20, 0x1C},// v 
 {0x3C, 0x40, 0x30, 0x40, 0x3C},// w 
 {0x44, 0x28, 0x10, 0x28, 0x44},// x 
 {0x0C, 0x50, 0x50, 0x50, 0x3C},// y 
 {0x44, 0x64, 0x54, 0x4C, 0x44},// z 
 {0x00, 0x08, 0x36, 0x41, 0x00},// { 
 {0x00, 0x00, 0x7F, 0x00, 0x00},// | 
 {0x00, 0x41, 0x36, 0x08, 0x00},// } 
 {0x08, 0x08, 0x2A, 0x1C, 0x08},// -> 
 {0x08, 0x1C, 0x2A, 0x08, 0x08} // <- 
};

//Hex And Decimal Numbers in 3 X 5 Format 
PROGMEM static const char FontLookup1 [][3] =
{
    { 0xF8, 0x88, 0xF8 },   // 0
    { 0x48, 0xF8, 0x08 },   // 1
    { 0xB8, 0xA8, 0xE8 },   // 2
    { 0xA8, 0xA8, 0xF8 },   // 3
    { 0xE0, 0x20, 0xF8 },   // 4
    { 0xE8, 0xA8, 0xB8 },   // 5
    { 0xF8, 0xA8, 0xB8 },   // 6
    { 0x80, 0x80, 0xF8 },   // 7
    { 0xF8, 0xA8, 0xF8 },   // 8
    { 0xE8, 0xA8, 0xF8 },   // 9
 { 0xF8, 0xA0, 0xF8 },   // A
    { 0xF8, 0xA8, 0x50 },   // B
    { 0xF8, 0x88, 0x88 },   // C
    { 0x88, 0xF8, 0xF8 },   // D
    { 0xF8, 0xA8, 0xA8 },   // E
    { 0xF8, 0xA0, 0xA0 },   // F
 { 0x00, 0x00, 0x00 },   // Space
};

/*********************************************************************************/
/*         MAIN PROGRAM                                 */
/*********************************************************************************/
/*********************************************************************************/
/*         LCD out                                   */
/*********************************************************************************/
void LCDout(char a)
{
 PORTD = a;
 Epulse();
}

/*********************************************************************************/
/*         DELAY 100mSec                                */
/*********************************************************************************/
void Delay1()
{
	_delay_ms(50);
}

/*********************************************************************************/
/*         Init LCD                                     */
/*********************************************************************************/

void InitLCD()
{
    DDRD = 0xFF;
 DDRC |=(1<<RS);
 DDRC |=(1<<E);
 DDRC |=(1<<CS1);
 DDRC |=(1<<CS2);
 DDRC |=(1<<RST);

 PORTC &= ~(1<<RST);   //Make sure rst state
 Delay1();
 PORTC |= (1<<RST);   //Make sure rst state
 Delay1();

 PORTC &=~(1<<RS);  //Command mode
    PORTC |= (1<<CS1);  //CS1 CS2 selected
    PORTC |= (1<<CS2);  //CS1 CS2 selected
 LCDout(0x3F);   //LCD on
 LCDout(0xC0);   //z-axis to zero 

 Delay1();
 Delay1();
 PORTC &=~ (1<<CS1);  
    PORTC &=~ (1<<CS2);  
}

/*********************************************************************************/
/*         Enable Pulse                                 */
/*********************************************************************************/
void Epulse()
{
 //E=PA3
 PORTC = PORTC | 0x10;
 PORTC = PORTC & 0xEF;
}

/*********************************************************************************/
/*         PUT PIXEL(X,Y,color)                         */
/*********************************************************************************/
void PutPixel(unsigned char x,unsigned char y, char color)
{

//Inverse Display
//x = 127-x;
//y = 63-y;

unsigned char c,y1;
c=0;
y1=y; //+3
   
   while(y1 >= 8)
   {
     y1=y1-8;
  c++;
   }

if (x < 64)
{
 PORTC &=~(1<<RS);  //Command mode
    PORTC |= (1<<CS1);  //CS1 selected
 PORTC &=~(1<<CS2);
 c = c & 0x07;
 LCDout(0xB8 | c);  //C (y-axis) is having 3-bit address
 x = x & 0x3F;
 LCDout(0x40 | x);  //C (x-axis) is having 3-bit address
}
else
{
 x = x - 64;
 PORTC &=~(1<<RS);  //Command mode
 PORTC &=~(1<<CS1);  
    PORTC |= (1<<CS2);  //CS2 selected
 c = c & 0x07;
 LCDout(0xB8 | c);  //C (y-axis) is having 3-bit address
 x = x & 0x3F;
 LCDout(0x40 | x);  //C (x-axis) is having 3-bit address
 x = x+64;
}
 
 PORTC |= (1<<RS); 
  if(color==1)
  {
  Pix [x][c]=(Pix [x][c]) | (1<<y1) ;  //y1 is having bit position to set
  LCDout(Pix [x][c]);
 }
 else
 {
  Pix [x][c] &=~ (1<<y1);  //y1 is having bit position to reset
  LCDout(Pix [x][c]);
 }
}

/*********************************************************************************/
/*         LCD CLEAR                                    */
/*********************************************************************************/

void LCD_Clear()
{
unsigned char x,y;
 
unsigned char c;
c=0;
   for(y=0;y<8;y++)
    {
 c=y;
   for(x=0;x<128;x++)
    {
     Pix[x][y]=0x00;
  //=======================
   if ( x < 64)
   {
    PORTC &=~(1<<RS);  //Command mode
       PORTC |= (1<<CS1);  //CS1 selected
    PORTC &=~(1<<CS2);
    c = c & 0x07;
    LCDout(0xB8 | c);  //C (y-axis) is having 3-bit address
    x = x & 0x3F;
    LCDout(0x40 | x);  //C (x-axis) is having 3-bit address
   }
   else
   {
    x = x - 64;
    PORTC &=~(1<<RS);  //Command mode
    PORTC &=~(1<<CS1);  
       PORTC |= (1<<CS2);  //CS2 selected
    c = c & 0x07;
    LCDout(0xB8 | c);  //C (y-axis) is having 3-bit address
    x = x & 0x3F;
    LCDout(0x40 | x);  //C (x-axis) is having 3-bit address
    x = x+64;
   }
   PORTC |=(1<<RS);
   LCDout(0x00);
  //=======================
   
    }
    }
}
/*********************************************************************************/
/*         LINE(X0,Y0,X1,Y1,color)                      */
/*********************************************************************************/

void line(char x0, char y0, char x1, char y1,char color) 
{
 int dy = y1 - y0;
 int dx = x1 - x0;
 int stepx, stepy;
 if (dy < 0) { dy = -dy; stepy = -1; 
 } else { stepy = 1; 
 }
  if (dx < 0) { dx = -dx; stepx = -1; 
 } else { stepx = 1; 
 }
 dy <<= 1;        // dy is now 2*dy
 dx <<= 1;        // dx is now 2*dx
 
 PutPixel(x0, y0,color);

 if (dx > dy) {
  int fraction = dy - (dx >> 1); // same as 2*dy - dx
  while (x0 != x1) {
   if (fraction >= 0) {
    y0 += stepy;
    fraction -= dx;   // same as fraction -= 2*dx
   }
     x0 += stepx;
     fraction += dy;     // same as fraction -= 2*dy
     PutPixel(x0, y0,color);
  }
 } else {
  int fraction = dx - (dy >> 1);
  while (y0 != y1) {
   if (fraction >= 0) {
    x0 += stepx;
    fraction -= dy;
   }
  y0 += stepy;
  fraction += dx;
  PutPixel(x0, y0,color);
  }
 }
} 
/*********************************************************************************/
/*         RECT(X0,Y0,X1,Y1,color)                      */
/*********************************************************************************/
void rect(char x0, char y0, char x1, char y1,char color)
{
 line(x0,y0,x1,y0,color);
 line(x0,y0,x0,y1,color);
 line(x0,y1,x1,y1,color);
 line(x1,y0,x1,y1,color); 
}

/*********************************************************************************/
/*         Circle(X0,Y0,radius,color)                   */
/*********************************************************************************/
void circle(int xCenter, int yCenter, int radius, char c)
    {
        int x, y, r2;

        r2 = radius * radius;
        PutPixel(xCenter, yCenter + radius,c);
        PutPixel(xCenter, yCenter - radius,c);
        PutPixel(xCenter + radius, yCenter,c);
        PutPixel(xCenter - radius, yCenter,c);
        x = 1;
        y = (int) (sqrt(r2 - 1) + 0.5);
        while (x < y) {
            PutPixel(xCenter + x, yCenter + y,c);
            PutPixel(xCenter + x, yCenter - y,c);
            PutPixel(xCenter - x, yCenter + y,c);
            PutPixel(xCenter - x, yCenter - y,c);
            PutPixel(xCenter + y, yCenter + x,c);
            PutPixel(xCenter + y, yCenter - x,c);
            PutPixel(xCenter - y, yCenter + x,c);
            PutPixel(xCenter - y, yCenter - x,c);
            x += 1;
            y = (int) (sqrt(r2 - x*x) + 0.5);
        }
        if (x == y) {
            PutPixel(xCenter + x, yCenter + y,c);
            PutPixel(xCenter + x, yCenter - y,c);
            PutPixel(xCenter - x, yCenter + y,c);
            PutPixel(xCenter - x, yCenter - y,c);
        }
    }
/*********************************************************************************/
/*         Display Text (5x7)                       */
/*********************************************************************************/
void DisplayChar(unsigned char x,unsigned char y, char d)
{
unsigned char r,c,c1;
d=d-32;
 for(r=0;r<5;r++)
 {
  c1=0;   //4
  for(c=0;c<8;c++)
  {
   PutPixel(x+r,y+c1,((0x01) & (pgm_read_byte(&FontLookup[d][r]) >> c )));//>>(c+3)
   c1=c1+1;
  }
 }
}
/*********************************************************************************/
/*         Display Text (3x5)                       */
/*********************************************************************************/
void DisplayCharSmall(unsigned char x,unsigned char y, char d)
{
unsigned char r,c,c1;
 for(r=0;r<3;r++)
 {
  c1=4;  
  for(c=0;c<5;c++)
  {
   PutPixel(x+r,y+c1,((0x01) & (pgm_read_byte(&FontLookup1[d][r]) >> (c+3))));
   c1=c1-1;
  }
 }
}

/*********************************************************************************/
/*         Display Text (5x7)                       */
/*********************************************************************************/
void DisplayText(unsigned char x, unsigned char y, char d[21])
{
int i;
int j;
j=strlen(d);
 for (i=0;i<j;i++)
 {
  DisplayChar((i*6)+x,y,d[i]);
 } 
}
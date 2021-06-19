/*********************************************************************************/
/*   blog.circuits4you.com */
/*  128x64 Graphic LCD Demo Code */
/*********************************************************************************/
#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"


/*********************************************************************************/
/*         MAIN PROGRAM                                 */
/*********************************************************************************/
int main(void)
{
	//_delay_ms(1000);
	InitLCD();
	LCD_Clear();
	
	DDRB=0xFF;
	
	char s[14];
	int len=0;
	
	char mat[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
	
	int where=1;
	
	while (1)
	{
		for(int c=4;c<8;c++)
		{
			PORTB=0;
			PORTB=1<<c;

			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					_delay_ms(200);
					
					s[len]=mat[r][c-4];
					len+=1;
					s[len]=0;
					DisplayText(1,where,s);
					
					if(len==10)
					{
						where+=10;
						len=0;
						s[0]=0;
					}
					
				}
				else PORTA=0;
			}
			
			_delay_ms(10);
		}
	}
	
	
	
	
}
//1705045	4.5	ADC5	Right	32
#define F_CPU 1000000
#include <avr/io.h>
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include "lcd.h"


int main(void)
{
	DDRB=0xFF;
	
	DDRA=0xFF;
	
	PORTB=0;
	PINB=0;
	
	char s[14];
	int len=0;
	
	char mat[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
	
	
	DDRC=0xFF;
	DDRD=0xFF;
	Lcd4_Init();
	
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
					PORTA=(1<<(r));
					_delay_ms(1000);
					s[len]=mat[r][c-4];
					len+=1;
					s[len]=0;
				}
				else PORTA=0;
			}
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String(s);
			_delay_ms(10);
		}
    }
}


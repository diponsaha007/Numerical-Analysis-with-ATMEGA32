#define  F_CPU 8000000
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"
#include "parse.h"
#include "extras.h"
#include "Gauss.h"
#include "Simpson.h"
#include "graph_plot.h"
#include "Newton.h"
#include "calculator.h"

void initial_page()
{
	char s[] = "1.Calculator";
	DisplayText(1,1,s);
	char s2[] = "2.Gauss Elimination";
	DisplayText(1,12,s2);
	char s3[] = "3.Newton Raphson";
	DisplayText(1,24,s3);
	char s4[] = "4.Integration";
	DisplayText(1,36,s4);
	char s5[]="5.Graph Plot";
	DisplayText(1,48,s5);
}



char mode_selector()
{
	while (1)
	{
		for(int c=4;c<8;c++)
		{
			PORTB=1<<c;
			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					if(mat[r][c-4]>='1' && mat[r][c-4]<='5')
					{
						_delay_ms(500);
						return mat[r][c-4];
					}
					_delay_ms(500);
				}
			}
			_delay_ms(10);
		}
	}
}




int main(void)
{
	InitLCD();
	LCD_Clear();
	initial_page();
	DDRB=0xFF;
	DDRA=0xFF;
	
	//take input
	char input = mode_selector();
	if(input=='1')
	{
		calculator();	
	}
	else if(input=='2')
	{
		gauss();
	}
	else if(input=='3')
	{
		newton_raphson();
	}
	else if(input=='4')
	{
		simpson();
	}
	else if(input=='5')
	{
		graph_plot();
	}
	
}
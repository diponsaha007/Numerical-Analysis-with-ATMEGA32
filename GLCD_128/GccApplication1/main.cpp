#define  F_CPU 8000000
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"
#include "parse.h"
#include "extras.h"
#include "graph_plot.h"
#include "Newton.h"
#include "calculator.h"
#include "Simpson.h"
#include "Gauss.h"
void initial_page()
{
	DisplayText(1,1,"1.Calculator");
	DisplayText(1,12,"2.Gauss Elimination");
	DisplayText(1,24,"3.Newton Raphson");
	DisplayText(1,36,"4.Integration");
	DisplayText(1,48,"5.Graph Plot");
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
	
	DDRB=0xFF;
	DDRA=0xFF;
	
	while(true){
		LCD_Clear();
		initial_page();
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
		
		bool f=1;
		while (f)
		{
			for(int c=4;c<8&&f;c++)
			{
				PORTB=1<<c;
				for(int r=0;r<4&&f;r++)
				{
					if(PINB&(1<<r))
					{
						if(mat[r][c-4]=='C')
						{
							f=0;
						}
						_delay_ms(500);
					}
				}
				_delay_ms(10);
			}
		}
		
	}
	
}
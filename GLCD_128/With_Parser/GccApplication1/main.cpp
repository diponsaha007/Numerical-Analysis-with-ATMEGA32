#define  F_CPU 8000000
#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"
#include <math.h>
#include "parse.h"
#include <assert.h>



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
}

char mat[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
char mat2[4][4][6]={{"sin(","cos(","tan(","sqrt("},{"^","X","Y","<-"},{",",".","(",")"},{"C","C","C","C"}};

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
					if(mat[r][c-4]>='1' && mat[r][c-4]<='4')
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

void append(char s[] , int len , char s2[])
{
	for(int i=0;s2[i]!=0;i++)
	{
		s[len] = s2[i];
		len++;
	}
	s[len] = 0;
	
}
char s_50[50]="50";
char s_100[50]="100";


void calculator()
{
	LCD_Clear();
	char s[50];
	int len=0;
	int where=1;
	while (1)
	{
		int done=0;
		for(int c=4;c<8;c++)
		{
			PORTB=1<<c;
			PORTA=1<<c;
			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					if(mat[r][c-4]=='C')
					{
						len=0;
						s[0]=0;
						where=1;
						LCD_Clear();
					}
					else
					{	
						if(mat[r][c-4]=='=')
						{
							parser ob;
							double ans=ob.eval_exp(s);
							_delay_ms(100);
							dtostrf(ans,1,2,s);
							_delay_ms(100);
							LCD_Clear();
							_delay_ms(100);
							DisplayText(1,1,s);
							break;
						}
						s[len]=mat[r][c-4];
						len+=1;
						s[len]=0;
					}
					DisplayText(1,where,s);
					if(len==20)
					{
						where+=12;
						len=0;
						s[0]=0;
					}
					_delay_ms(500);
				}
				else if(PINA&(1<<r))
				{
					if(mat2[r][c-4][0]=='<')
					{
						if(len==0){
							_delay_ms(500);
							continue;
						}
						s[len-1]=' ';
						DisplayText(1,where,s);
						len--;
						s[len]=0;
					}
					else
					{
						append(s , len , mat2[r][c-4]);
						len = strlen(s);
					}
					DisplayText(1,where,s);
					if(len==20)
					{
						where+=12;
						len=0;
						s[0]=0;
					}
					_delay_ms(500);
				}
			}
			
			_delay_ms(10);
		}
	}
}

void gauss()
{
	//todo
}

void newton_raphson()
{
	//todo
}

void simpson()
{
	//todo
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
	
}
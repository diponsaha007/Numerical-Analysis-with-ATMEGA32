#define  F_CPU 8000000
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"
#include "parse.h"
#include "Gauss.h"
#include "Simpson.h"


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
char mat2[4][4][6]={{"sin(","cos(","tan(","sqrt("},{"^","x","y","<-"},{",",".","(",")"},{"C","C","C","C"}};

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


void calculator()
{
	LCD_Clear();
	char s[50];
	int len=0;
	int where=1;
	while (1)
	{
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


//helper functions and gauss
int int_number_input(int pos)
{
	//pos is position where u want to show the text
	while (1)
	{
		for(int c=4;c<8;c++)
		{
			PORTB=1<<c;
			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					if(mat[r][c-4]>='1' && mat[r][c-4]<='7')
					{
						char tmp[2];
						tmp[0] = mat[r][c-4];
						tmp[1] = 0;
						DisplayText(1,pos,tmp);
						_delay_ms(500);
						return mat[r][c-4]-'0';
					}
					_delay_ms(500);
				}
			}
			_delay_ms(10);
		}
	}
}


double string_to_double(char s[], int id)
{
	double d = 0;
	bool flag = 0;
	int koto = 1;
	bool minus = 0;
	for(int i=0;i<id;i++)
	{
		if(s[i]=='-')
		{
			minus = 1;
			continue;
		}
		if(s[i]=='.')
		{
			flag = 1;
			continue;
		}
		int now = (int)(s[i]-'0');
		d = d*10.0 + (double)now;
		if(flag)
		{
			koto*=10;
		}
	}
	d/= (double)koto;
	if(minus)
	d= -d;
	return d;
}

double double_num_input(int pos)
{
	//pos is position where u want to show the text
	char s[10];
	int id = 0;
	bool dot = 0;
	bool done = 0;
	while (1)
	{
		for(int c=4;c<8;c++)
		{
			PORTB=1<<c;
			PORTA=1<<c;
			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					if(mat[r][c-4]>='0' && mat[r][c-4]<='9')
					{
						s[id] = mat[r][c-4];
						id++;
						s[id] = 0;
						DisplayText(1,pos , s);
					}
					else if(mat[r][c-4]=='=')
					{
						done = 1;
						break;
					}
					else if(id==0 && mat[r][c-4]=='-')
					{
						s[id] = mat[r][c-4];
						id++;
						s[id] = 0;
						DisplayText(1,pos , s);
					}
					_delay_ms(500);
				}
				else if(PINA&(1<<r))
				{
					if(mat2[r][c-4][0]=='<')
					{
						if(id==0){
							_delay_ms(500);
							continue;
						}
						s[id-1]=' ';
						DisplayText(1,pos,s);
						id--;
						s[id]=0;
						
					}
					else if(mat2[r][c-4][0]=='.' && dot==0)
					{
						dot = 1;
						s[id] = mat2[r][c-4][0];
						id++;
						s[id] = 0;
						DisplayText(1,pos , s);
					}
					_delay_ms(500);
				}
				if (id==9)
				{
					done = 1;
					break;
				}
			}
			_delay_ms(10);
			if(done == 1)
			{
				break;
			}
		}
		if(done == 1)
		{
			break;
		}
	}
	
	return string_to_double(s,id);
}
void show_text(char txt[], int pos1)
{
	if(pos1==1){
		LCD_Clear();
	}
	int len = strlen(txt);
	char s[21];
	int cnt = pos1;
	int id  = 0;
	for(int i=0;i<len;i++)
	{
		s[id] = txt[i];
		id++;
		if(id==20)
		{
			s[id] = 0;
			DisplayText(1,cnt,s);
			id = 0;
			cnt += 12;
		}
		
	}
	if(id)
	{
		s[id] = 0;
		DisplayText(1,cnt,s);
	}

}


void gauss()
{
	char text1[] = "Number of Variables (1-7)?";
	char text2[] = "Number of equations (1-7)?";
	char text3[] = "No Solution!";
	char text4[] = "Infinite Solution!";
	show_text(text1,1);
	int m = int_number_input(24);
	show_text(text2,1);
	int n = int_number_input(24);
	
	double *ans = (double*)malloc(m * sizeof(double));
	double ** a = (double**)malloc(n*sizeof(double *));
	for(int i=0; i< n; i++) 
		a[i] = (double*) malloc((m+1)*sizeof(double));

	
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			char text[14] = "Input a[";
			char ek[2];
			ek[0] = (char)(i+'0');
			ek[1] = 0;
			strcat(text , ek);
			ek[0] = ']';
			strcat(text , ek);
			ek[0] = '[';
			strcat(text , ek);
			ek[0] = (char)(j+'0');
			strcat(text , ek);
			ek[0] = ']';
			strcat(text , ek);
			show_text(text,1);
			a[i][j] = double_num_input(12);
		}
		{
			char text[18] = "Input column d[";
			char ek[2];
			ek[0] = (char)(i+'0');
			ek[1] = 0;
			strcat(text , ek);
			ek[0] = ']';
			strcat(text , ek);
			show_text(text,1);
			a[i][m] = double_num_input(12);
		}
	}
	int koto = gauss_algo(n,m,ans,a);
	if(koto==0)
	{
		show_text(text3,1);
		return;
	}
	else if(koto==1)
	{
		_delay_ms(1500);
	}
	else if(koto==2)
	{
		show_text(text4,1);
		_delay_ms(1500);
	}
	
	for(int i=0;i<m;i++)
	{
		char text[14] = "Ans x[";
		char ek[2];
		ek[0] = (char)(i+'0');
		ek[1] = 0;
		strcat(text , ek);
		ek[0] = ']';
		strcat(text , ek);
		show_text(text,1);
		char s[50];
		dtostrf(ans[i],1,2,s);
		show_text(s,12);
		//wait for equal
		bool done = 0;
		while (1)
		{
			for(int c=4;c<8;c++)
			{
				PORTB=1<<c;
				for(int r=0;r<4;r++)
				{
					if(PINB&(1<<r))
					{
						if(mat[r][c-4]=='=')
						{
							done = 1;
							break;
						}
					}
				}
				if(done)
				{
					break;
				}
			}
			if(done)
			{
				break;
			}
		}
		_delay_ms(500);
	}
	for(int i = 0; i < n; i++)
		free(a[i]);
	free(a);

	free(ans);
}

//done with gauss
void newton_raphson()
{
	//todo
}

void simpson()
{
	LCD_Clear();
	char text1[] = "Input Equation of x:";
	char text2[] = "Input lower limit:";
	char text3[] = "Input upper limit:";
	DisplayText(1,1,text1);
	//input function
	char s[50];
	int len=0;
	int where=12;
	while (1)
	{
		bool done = 0;
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
							done = 1;
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
			if(done)
				break;
		}
		if(done)
			break;
	}
	LCD_Clear();
	double l , r;
	DisplayText(1,1,text2);
	l = double_num_input(12);
	LCD_Clear();
	DisplayText(1,1,text3);
	r = double_num_input(12);
	double ans = simpson_integration(s,l,r);
	
	//printing ans
	char text[] = "Ans =";
	show_text(text,1);
	dtostrf(ans,1,2,s);
	show_text(s,12);
	//wait for equal
	bool done = 0;
	while (1)
	{
		for(int c=4;c<8;c++)
		{
			PORTB=1<<c;
			for(int r=0;r<4;r++)
			{
				if(PINB&(1<<r))
				{
					if(mat[r][c-4]=='=')
					{
						done = 1;
						break;
					}
				}
			}
			if(done)
			{
				break;
			}
		}
		if(done)
		{
			break;
		}
	}
	_delay_ms(500);
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
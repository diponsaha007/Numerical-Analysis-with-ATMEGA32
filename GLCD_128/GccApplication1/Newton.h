double diffentiation(char str[],double x)
{
	double h = 1e-6;
	return (evaluate_function(str , x+h)-evaluate_function(str, x-h))/(2.00*h);
}

double newton(char str[],double guess)
{
	double x = guess;
	for (int rep = 0; rep<=80; rep++)
	{
		double nx = x - (evaluate_function(str, x)/diffentiation(str , x));
		if(!isfinite(nx))
		{
			break;
		}
		x = nx;
	}
	return x;
}
void newton_raphson()
{
	LCD_Clear();
	char text1[] = "Input Equation of x:";
	char text2[] = "Input initial approximation:";
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
	double l;
	show_text(text2,1);
	l = double_num_input(24);
	LCD_Clear();
	double ans = newton(s,l);
	
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
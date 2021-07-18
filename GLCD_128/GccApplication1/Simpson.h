


double simpson_integration(char str[] , double a, double b){
	const int N = 50;
	double h = (b - a) / N;
	double s = evaluate_function(str, a) + evaluate_function(str , b);
	for (int i = 1; i <= N - 1; ++i) {
		double x = a + h * i;
		s += evaluate_function(str , x) * ((i & 1) ? 4 : 2);
	}
	s *= h / 3;
	return s;
}
void simpson()
{
	LCD_Clear();
	char text1[] = "Input Function of x:";
	char text2[] = "Input lower limit:";
	char text3[] = "Input upper limit:";
	DisplayText(1,1,text1);
	//input function
	char s[21];
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
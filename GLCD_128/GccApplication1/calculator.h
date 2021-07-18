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
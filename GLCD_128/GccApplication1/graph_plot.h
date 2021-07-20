void show_and_get(char *F, char * s)
{
	LCD_Clear();
	_delay_ms(200);
	DisplayText(1,1,F);
	_delay_ms(200);
	
	int len=0;
	int where=12;
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
							return;
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


void graph_plot()
{
	char F1[]="F(x) = ";
	char s1[20];
	show_and_get(F1,s1);
	
	strcpy(F1,"minX = ");
	char s2[20];
	show_and_get(F1,s2);
	double l=ob.eval_exp(s2);
	
	strcpy(F1,"maxX = ");
	show_and_get(F1,s2);
	double r=ob.eval_exp(s2);
	
	const int T=16;

	if(l>r)
	{
		double t=r;
		r=l;
		l=t;
	}
	
	double save_y[T];
	
	int w=0;
	for(double x=l;w<T;w++)
	{
		save_y[w]=evaluate_function(s1,x);
		
		
		/*dtostrf(save_x[w],1,2,t2);
		show_and_get(t2,t2);
		_delay_ms(500);
		dtostrf(save_y[w],1,2,t2);
		show_and_get(t2,t2);
		*/
		//_delay_ms(1000);
		x+=(r-l)/(T-1);
	}
	//l+i*(r-l)/(T-1)

	
	double mxX=0,mxY=0,mnX=0,mnY=0;
	for(int i=0;i<T;i++)
	{
		if(l+i*(r-l)/(T-1)>mxX) mxX=l+i*(r-l)/(T-1);
		if(save_y[i]>mxY) mxY=save_y[i];
		
		if(l+i*(r-l)/(T-1)<mnX) mnX=l+i*(r-l)/(T-1);
		if(save_y[i]<mnY) mnY=save_y[i];	
	}
	for(int i=0;i<T;i++)
	{
		save_y[i]-=mnY;
	}
	//((l+i*(r-l)/(T-1))-mnX)
	double scaleX=120/(mxX-mnX);
	double scaleY=60/(mxY-mnY);

	LCD_Clear();
	_delay_ms(500);
	
	
	for(int i=0;i<T;i++)
	{
		//i_x[i]=round(save_x[i]*scaleX);
		//i_y[i]=63-round(save_y[i]*scaleY);
	}
	for(int y=0;y<64;y++)
	{
		int x=round((-mnX)*scaleX);
		PutPixel(x,y,1);
	}
	for(int x=0;x<128;x++)
	{
		int y=63-round((-mnY)*scaleY);
		PutPixel(x,y,1);
	}
	
	for(int i=1;i<T;i++)
	{
		//PutPixel(i_x[i],i_y[i],1);
		line(round(((l+(i-1)*(r-l)/(T-1))-mnX)*scaleX),63-round(save_y[i-1]*scaleY),round(((l+i*(r-l)/(T-1))-mnX)*scaleX),63-round(save_y[i]*scaleY),1);
		_delay_ms(50);
	}

	
}
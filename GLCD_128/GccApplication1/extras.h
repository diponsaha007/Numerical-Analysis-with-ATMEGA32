char mat[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
char mat2[4][4][6]={{"sin(","cos(","tan(","sqrt("},{"^","x","3.14","<-"},{"exp(",".","(",")"},{"log(","ln(","asin(","acos("}};
	
void append(char s[] , int len , char s2[])
{
	for(int i=0;s2[i]!=0;i++)
	{
		s[len] = s2[i];
		len++;
	}
	s[len] = 0;
	
}

char s[40];
char tmp[15];
parser ob;
double evaluate_function(char str[], double a)
{
	
	int idx = 0;
	for(int i=0;str[i]!=0;i++)
	{
		if(str[i]=='x' && (i-1<0 || s[i-1]!='e' ))
		{
			dtostrf(a,1,8,tmp);
			for(int j=0;tmp[j]!=0;j++)
			{
				s[idx] = tmp[j];
				idx++;
			}
		}
		else{
			s[idx] = str[i];
			idx++;
		}
	}
	s[idx] = 0;
	
	double ans=ob.eval_exp(s);
	return ans;
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
					if(mat[r][c-4]>='1' && mat[r][c-4]<='3')
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
parser ob;
double evaluate_function(char str[], double a)
{
	char s[40];
	int idx = 0;
	for(int i=0;str[i]!=0;i++)
	{
		if(str[i]=='x')
		{
			char tmp[15];
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

double simpson_integration(char str[] , double a, double b){
	const int N = 500;
	double h = (b - a) / N;
	double s = evaluate_function(str, a) + evaluate_function(str , b);
	for (int i = 1; i <= N - 1; ++i) {
		double x = a + h * i;
		s += evaluate_function(str , x) * ((i & 1) ? 4 : 2);
	}
	s *= h / 3;
	return s;
}

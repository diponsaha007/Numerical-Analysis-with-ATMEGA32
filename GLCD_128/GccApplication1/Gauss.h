int gauss_algo (int n, int m, double *ans , double **a)
{
	//returns 1 if a single solution
	//returns 2 if infinite solution
	//returns 0 if no solution
	//returns a solution if there is at least one solution
	const double EPS = 1e-7;
	int *where = (int*)malloc(m * sizeof(int));
	for(int i=0; i<m; i++)
		where[i] = -1;
	for (int col=0, row=0; col<m && row<n; ++col)
	{
		int sel = row;
		for (int i=row; i<n; ++i)
		if (abs (a[i][col]) > abs (a[sel][col]))
		sel = i;
		if (abs (a[sel][col]) < EPS)
		continue;
		for (int i=col; i<=m; ++i)
		{
			double tmp = a[sel][i];
			a[sel][i] = a[row][i];
			a[row][i] = tmp;
		}
		where[col] = row;
		for (int i=0; i<n; ++i)
		if (i != row)
		{
			double c = a[i][col] / a[row][col];
			for (int j=col; j<=m; ++j)
			a[i][j] -= a[row][j] * c;
		}
		++row;
	}

	for(int i=0; i<m; i++)
	ans[i] =0;
	for (int i=0; i<m; ++i)
	if (where[i] != -1)
	ans[i] = a[where[i]][m] / a[where[i]][i];
	for (int i=0; i<n; ++i)
	{
		double sum = 0;
		for (int j=0; j<m; ++j)
		sum += ans[j] * a[i][j];
		if (abs (sum - a[i][m]) > EPS)
		return 0;
	}

	for (int i=0; i<m; ++i){
		if (where[i] == -1){
			free(where);
			return 2;
		}
	}
	free(where);
	return 1;
}
void gauss()
{
	char text1[] = "Number of Variables (1-7)?";
	show_text(text1,1);
	int m = int_number_input(24);
	strcpy(text1,"Number of equations (1-7)?");
	show_text(text1,1);
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
		strcpy(text1,"No Solution!");
		show_text(text1,1);
		return;
	}
	else if(koto==1)
	{
		_delay_ms(1500);
	}
	else if(koto==2)
	{
		strcpy(text1,"Infinite Solution!");
		show_text(text1,1);
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
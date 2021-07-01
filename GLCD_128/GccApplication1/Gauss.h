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

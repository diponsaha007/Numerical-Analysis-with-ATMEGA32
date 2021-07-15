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

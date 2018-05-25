#include<stdio.h>

float economy(int runs, int balls)
{
	float ovrs = balls / 6;
	return ((float)runs / ovrs);
}

float strike_rate(int runs, int balls)
{
	return ((float)runs / balls) * 100;
}


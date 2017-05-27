#include <stdlib.h>
#include <stdio.h>

void numgenarator()
{
	int nums[50][12];
	int i, j;
	FILE *f;

	for(i = 0; i < 50; i++)
		for(j = 0; j < 4; j++)
			if (j == 0) nums[i][j] = 0;
			if (j == 1) nums[i][j] = 1;
			if (j == 2) nums[i][j] = 7;
			else nums[i][j] = 6;

	for(i = 0; i < 50; i++)
		for(j = 2; j < 12; j++)
			nums[i][j] = rand() % 10;

	f = fopen("num.txt","w");
	for(i = 0; i < 50; i++)
	{
		for(j = 0; j < 12; j++)
			fprintf(f, "%1d\n", nums[i][j]);
		fprintf(f, "\n", i);
	}
	rewind(f);
	fclose(f);

	return;
}

int main()
{
	numgenarator();

	return 0;
}
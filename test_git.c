#include <stdio.h>

int main(int argc, const char *argv[])
{
	printf("HelloWorld!\n");
	int sum = 0;
	for (int i = 0; i < 100; i++)
		sum += i;
	printf("sum == %d\n", sum);
	return 0;
}

#include <stdio.h>

/* Fibonacci.c
 * List the first few numbers..
 *
 *
 * 0, 1, 1, 2, 3, 5, 8
 *
 * Start with 0,
 * then 1,
 * each following number is the sum of the two previous..
 *
 */

int main(void)
{
long long int first = 0;
long long int second = 1;
int third = first + second;
int n = 0;
int k = 0;

printf("Please enter n: ");
scanf("%d", &n);

printf("%20lld\n", first);
printf("%20lld\n", second);

for (int k; k < 30; k++){
printf("%20lld\n",third);
first = second;
second = third;
}

return 0;
}

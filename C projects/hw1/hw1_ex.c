/* 
 * Robert Au
 * ECS36a A04
 * Bishop
 * 
 * 
 * program to count using a for loop, a while loop, and a do...while loop
 * with an increment
 * this is the framework; the counting is done in functions to be supplied
 * in another file
 * 
 * this is for homework 1, extra credit question 1, for ECS 36A Fall 2019
 *
 * Matt Bishop, ECS 36A
 * version 1	September 20, 2019
 *	original version
 * version 2	September 28, 2019
 *  modified so students only need to add the loop code
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/*
 * the "for" loop routine
 * m is the starting number
 * n is the ending number
 * d is the increment
 */
void do_for(int m, int n, int d)
{
	/*
	 * if/else statement puts 
	 * the integers in the correlating
	 * decreasing/increasing sequence
	 */
	if (m < n){				
		for (; m < n; m += d){		/* increasing sequence */
			printf("%d", m);		/* print the number */
			if (m + d <= n){		/* check if next number does not go over n */
				printf(", ");		/* then continue the list of numbers */

			}else if (m + d > n){	/* otherwise if next number goes over n */
				printf("\n");		/* then end the list here */
				return;
			}	
		}
	}
	else if (m > n){
		for (; m > n; m -= d){		/* decreasing sequence */
			printf("%d", m);		/* print the number */
			if (m - d >= n){		/* check if next number does not go past n*/
				printf(", ");		/* then continue the list of numbers */

			}else if (m - d < n){	/* otherwise if next number goes past n */
				printf("\n");		/* then end the list here */
				return;
			}
		}
	}

	/* the loops exit when m == n */
	/* prints last number and makes a new line */
	if (m == n)
		printf("%d\n", m);
}

/*
 * the "while" loop routine
 * m is the starting number
 * n is the ending number
 */
void do_while(int m, int n, int d)
{
	while (m != n){

		if (m > n){					/* decreasing sequence */
			printf("%d", m);		
			if (m - d >= n){			
				printf(", ");		

			}else if (m - d < n){	
				printf("\n");		
				return;
			}
			m -= d;					
		}
		else if (m < n){			/* increasing sequence */
			
			printf("%d", m);		
			if (m + d <= n){		
				printf(", ");		

			}else if (m + d > n){	
				printf("\n");		
				return;
			}
			m += d;					
		}
	}

	/* when sequence ends or if no increasing/decreasing was done */
	if (m == n)
		printf("%d\n", m);	
}

/*
 * the "do . . . while" loop routine
 * m is the starting number
 * n is the ending number
 */
void do_do(int m, int n, int d)
{	
	if (m > n)
	{
		do		/* decreasing sequence */
		{
			printf("%d", m);		
			if (m - d >= n){			
				printf(", ");		

			}else if (m - d < n){	
				printf("\n");		
				return;
			}
			m -= d;	
		}while (m > n);
		
	}else if (m < n)
	{
		do		/* increasing sequence */
		{
			printf("%d", m);		
			if (m + d <= n){		
				printf(", ");		

			}else if (m + d > n){	
				printf("\n");		
				return;
			}
			m += d;
		}while (m < n);

	}

	/* the loop above ends or is not executed, print for m == n */
	if (m == n)
		printf("%d\n", m);	
}

/*
 * getint -- get an integer
 *
 * in parameters:
 * char *str	the string to be converted to an integer
 * int *error	location for integer error code
 *
 * out parameters:
 * int *error	1 if there is an error, 0 otherwise
 *
 * return value:
 * on success, integer resulting from converting str
 * on failure, -1; *error is set to 1 and error message is printed
 */
int getint(char *str, int *error)
{
	long m;			/* holds value of number in str */
	char *mend = str;	/* points to char beyond end of number */

	/*
 	 * initialize error codes
	 */
	*error = 0;
	errno = 0;

	/*
	 * convert the string to a number
	 */
	m = strtol(str, &mend, 10);
	/* possible error 1: non-0integer char in string */
	if (*mend != '\0' || str == mend){
		fprintf(stderr, "%s is not an integer\n", str);
		*error = 1;
		return(-1);
	}
	/* it's a number but is too big or small to be an int */
	if (errno == ERANGE &&
		((m > 0 && m > INT_MAX) ||
			(m < 0 && m < INT_MIN))){
		fprintf(stderr, "%s is too big or small\n", str);
		*error = 1;
		return(-1);
	}

	/* okay, it's a valid integer; convert it and return */
	return((int) m);
}

/*
 * the main return
 */
int main(int argc, char *argv[])
{
	int m;		/* starting point */
	int n;		/* ending point */
	int d;		/* increment */
	int err;	/* place for error code */

	/*
	 * check that you got 3 arguments,
	 * the starting point, the ending point,
	 * and the increment
	 */
	if (argc != 4){
		fprintf(stderr, "Usage: %s m n\n", argv[0]);
		return(1);
	}

	/* get the starting point */
	if ((m = getint(argv[1], &err)) == -1 && err)
		return(1);

	/* get the ending point */
	if ((n = getint(argv[2], &err)) == -1 && err)
		return(1);

	/* get the increment */
	if ((d = getint(argv[3], &err)) == -1 && err)
		return(1);

	/* outputs error statement when invalid increment is used */
	if(d <= 0){
		printf("Error: invalid increment %d.\n", d);
		return(0);
	}
	

	/*
	 * do the loops
	 */
	/* announce the for loop and do it */
	printf("for loop:\n");
	do_for(m, n, d);

	/* announce the while loop and do it */
	printf("while loop:\n");
	do_while(m, n, d);

	/* announce the do ... while loop and do it */
	printf("do ... while loop:\n");
	do_do(m, n, d);


	/*
	 * all done -- give good exit code
	 */
	return(0);
}

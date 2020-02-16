/*
 * Robert Au
 * ECS36a A04
 * Bishop
 * 
 * 
 * program to count using a for loop, a while loop, and a do...while loop
 * this is the framework; the counting is done in functions to be supplied
 * in another file
 * 
 * this is for homework 1, question 3, for ECS 36A Fall 2019
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
 */
void do_for(int m, int n)
{
	/*
	 * if/else statement separates
	 * decreasing/increasing sequences
	 */

	if (m < n){					
		for (; m < n; m++){ 	/* the increasing sequence */
			printf("%d\n", m);
		}	
	}
	else if (m > n){
		for (; m > n; m--){		/* the decreasing sequence*/
			printf("%d\n", m);
		}
	}

	/*
	 * the loops dont print 'm' when m equals n
	 * this also accounts for when m == n
	 */
	if (m == n){
		printf("%d\n", m);
	}
}

/*
 * the "while" loop routine
 * m is the starting number
 * n is the ending number
 */
void do_while(int m, int n)
{
	
	while (m != n){
		if (m > n){				/* decreasing sequence */
			printf("%d\n", m); 
			m--;
		}
		else if (m < n){		/* increasing sequence */
			printf("%d\n", m);
			m++;
		}
	}
	/* when the loop ends or if no increasing/decreasing was done */
	printf("%d\n", m);			
}

/*
 * the "do . . . while" loop routine
 * m is the starting number
 * n is the ending number
 */
void do_do(int m, int n)
{

	if (m > n)
	{
		do		/* decreasing sequence */
		{
			printf("%d\n", m); 
			m--;
		}while (m > n);
		
	}else if (m < n)
	{
		do		/* increasing sequence */
		{
			printf("%d\n", m); 
			m++;
		}while (m < n);

	}
	/* when sequence ends or if no increasing/decreasing was done */
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
	error = 0;
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
	int err;	/* place for error code */


	/*
	 * check that you got 2 arguments,
	 * the starting point and the ending point
	 */
	if (argc != 3){
		fprintf(stderr, "Usage: %s m n\n", argv[0]);
		return(1);
	}

	/* get the starting point */
	if ((m = getint(argv[1], &err)) == -1 && err)
		return(1);

	/* get the ending point */
	if ((n = getint(argv[2], &err)) == -1 && err)
		return(1);
	
	/*
	 * do the loops
	 */
	/* announce the for loop and do it */
	printf("for loop:\n");
	do_for(m, n);

	/* announce the while loop and do it */
	printf("while loop:\n");
	do_while(m, n);

	/* announce the do ... while loop and do it */
	printf("do ... while loop:\n");
	do_do(m, n);

	/*
	 * all done -- give good exit code
	 */
	return(0);
}

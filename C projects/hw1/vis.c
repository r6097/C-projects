/*
 * Robert Au
 * ECS36a A04
 * Bishop
 * 
 * this is for homework 1, question 4, for ECS 36A Fall 2019
 * 
 * VIS -- program that takes in user input and prints it 
 * in the output, turning any non-printing characters 
 * into their C character escape sequence 
 * 
 * Usage: vis
 * 
 * Inputs: any letters and numbers, and most commands
 * 
 * Outputs: same letters and numbers, along with the commands
 * converted to C character escape sequence
 * 
 * Exit code: 0, and there is no failure exit code
 */
#include <stdio.h>
#include <ctype.h>

int main(void)
{
	int ch;

	while((ch = getchar()) != EOF){
		if (isprint(ch)){
			/* 
			 * '\\' is read as a printable character 
			 * so it would never reach the switch case 
			 */
			if (ch == '\\'){  
				printf("\\"); 
			}
			putchar(ch);
		}
		else switch (ch){
		case '\t':	printf("\\t");  break;
		case '\b':	printf("\\b");  break;
		case '\f':	printf("\\f");  break;
		case '\0':	printf("\\0");  break;
		/*removed case '\\' because it is a printable character */								
		case '\v':	printf("\\v");  break;
		case '\r':	printf("\\r");  break;
		case '\a':	printf("\\a");  break;	/* added colon after case '\a' */
		case '\n':	printf("\\n\n"); break;
		default:	printf("\\%03o", ch); break;
		}
	}

	return(0);
}

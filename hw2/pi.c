/*
 * Robert Au
 * ECS36A A04
 * Bishop
 * 
 * This is for HW2 Extra Credit
 */ 
#include <stdio.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795028841971693993751058209
int main(void)
{
    /* initialize variables*/
    float pi2 = 0;
    int i;

    printf("%.11f pi\n", PI); 
    /* 
     * output is 3.14159265359 
     * the variable float has limited memory allocated for stuff
     * so it holds 7 significant digits, and the rest are off
     * 
     * This is because computers have limited memory, so computers
     * represent floats with slight error from rounding
     */

    for(i = 0; i < 10; i++){
        pi2 += PI;
    }
    printf("%.11f sum of 10 pi\n", pi2);
    /*
     * output is 31.41592407227
     * like above, the first 7 digits
     * are the same as from PI
     * 
     * the numbers after are off as well,
     * but are different from the first pi printed
     * 
     * This is because we have added two floats that were
     * slightly off from round off error, which amplifies
     * the result when we do it 10 times
     * 
     */
    return 0;
}
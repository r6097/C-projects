/*
 * Robert Au 
 * ECS36A
 * Bishop
 *
 * This is for HW 2 problem 3 for ECS 36A Fall 2019
 * 
 * PERMUTE - takes in input as a string, then creates all possible
 * combinations of the string by swapping characters recursively
 * 
 * usage: permute
 * 
 * Inputs: Any string less than 100 characters that is not 
 * empty nor only blank spaces
 * Outputs: All combinations of the string. 
 * (How many combinations? Equal to the factorial of string length)
 * 
 * Exit Code: 0 Success !
 *            1 Error !
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * swaps characters x and y in a string
 */
void swap(char *x, char *y)
{
    char tempchar;
    tempchar = *x;
    *x = *y;
    *y = tempchar;
}

/*
 * To permute the string recursively and print all
 * permutations
 * 
 * Swaps first character with next character and calls
 * itself again, shrinking the string by 1 each time it passes
 * 
 * After it reaches the base case, it prints the permuted string,
 * and backtracks
 * 
 * 
 * char *str is pointer to string to be permuted
 * int st_num is starting index number of string
 * int ed_num is the ending index number of string
 */
void permute(char *str, int st_num, int ed_num)
{
    /* increment */
    int rec_level;
    
    /* base case: if string is 0 or 1 character long, just return it */
    if(st_num == ed_num){
        printf("%s\n", str);
    }
    /*
     * for loop keeps track of letter of recursion
     * rec_level is recursion level:
     *      the character 
     * 
     */
    else{
        for((rec_level = st_num); (rec_level <= ed_num); rec_level++){
            swap((str + st_num), (str + rec_level));
            permute(str, st_num + 1, strlen(str));
            swap((str + st_num), (str + rec_level));
        }
    }
}

int main(void)
{
    char str[100];

    while(1){
        printf("> ");
        /* reads input and deletes trailing newline */
        if (fgets(str, 100, stdin) == NULL)
            return 1; 
        if (str[strlen(str)-1] == '\n')
            str[strlen(str)-1] = '\0';

        /* check if string is empty */
        if (*str == '\0'){
            return 1;       /*failure: no string inputted */
        }else{
            /*
             * compare amount of characters to amount of spaces
             * if all characters
             */
            int i;
            int wspace = 0;
            for (i = 0; i < strlen(str); i++){
                if (isspace(str[i])){
                    wspace++;
                } 
            }
            if (wspace == i){
                return 1;       /*failure: string is spaces */
            }  
        }      
         /* permute the string */
        permute(str, 0, (strlen(str) - 1));
         /* it is strlen(str) - 1 because there is a null byte where \n used to be */
    }
    
    /* success exit code*/
    return 0;
}
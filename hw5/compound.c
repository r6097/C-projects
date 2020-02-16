/*
 * Robert Au
 * ECS 36a A04
 * Bishop
 * 
 * HW 5 problem 3
 * 
 * COMPOUND - Reads in a file containing atomic weights, names, and symbols. Then prompts user to 
 * input a molecule, and calculate the total atomic weight of the molecule. 
 * 
 * Usage: ./comound.c weightsfile
 * Inputs: File containing atomic weights, names, and symbols
 * Outputs: The total weight of inputted molecules, after checking that the molecule is proper.
 * Exit Code:0  Success
 *           1  Error: bad input file; something went wrong when reading the file
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* macros */
#define MAXLINES 118    /* number of rows in atomic weights file */
#define MAXLENGTH 13    /* max length of element name */
/* structures */
typedef struct element{
    char sym[3];        /* symbol for element */
    char name[MAXLENGTH];      /* name of element */
    float atomW;       /* atomic weight of element*/
}EL;

/* global variables */
int i, j, k;           /* counters used in a for loop*/
char elm[3] = "00"; /*store an element from user inputted molecule*/
char dig[4] = "000"; /*store subscript for element from user inputted molecule*/

/* prototypes */
float getWeight(char* inpStr, char* element, EL arr[], int MAXROWS);

/*
 * MAIN FUNCTION
 */
int main(int argc, char *argv[])
{
    char inpStr[10];   /* stores inputted compound as string */
    float totW;    /* stores total weight of inputted compound */
    int c;          /* to get input by character */
    int scanCt;     /* count of scanned fields by fscanf*/
    float buffW = 0;   /* temporary storage of weight of an element */
    float buffSub = 1; /* temporary storage of subscript number*/
    int isValid = 1;    /* determines what to print at end (error message types and normal output) */
    EL arr[MAXLINES];    /* array of structures */
    FILE *fp;   /*pointer to file*/
    /* check format of command line */
    if (argc != 2){
        fprintf(stderr, "Usage: %s weightsfile\n", argv[0]);
        exit(1);
    }
    /*
     * storing data from file
     */
    /* open the atomic weights file */ 
    fp = fopen(argv[1], "r");
    if (fp == NULL){ /* complain if file isnt there */
        perror(argv[1]);
        exit (1);
    }
    /* get data one line at a time */
    i = 0;
    while (!feof(fp) && i < MAXLINES){
        /* store data inside structures */
        scanCt = fscanf(fp, "%f %s %s", &arr[i].atomW, arr[i].sym, arr[i].name);
        /* check for errors in file format */
        if (scanCt == 0){
            fprintf(stderr, "%s: no atomic weights there!\n", argv[1]);
            exit(1);
        }else if(scanCt != 3){
            fprintf(stderr, "%s: malformed line %d\n", argv[1], i);
            exit(1);
        }
        i++;
    }
    /* we are done copying data into arr*/
    fclose(fp);

    /* keep reprompting user*/
    while(1){
    /*
     * get user input
     */    
        printf("Chemical composition? ");
        /* read in input one char at time*/
        i = 0;
        while( (c = getchar()) != '\n'){
            /* if user presses ^D program will exit*/
            if (c == EOF){
                printf("\n");
                return(0);
            }
            /*store in string by char*/
            inpStr[i] = c;
            i++;
        }
        /* add terminiating null byte */
        inpStr[i] = '\0';
        /* reprompt when user put in nothing */
        if (inpStr[0] =='\0'){
            continue;
        }
    /*
     * interpreting user input and storing element weights
     */
        totW = 0.00;
        for (i = 0; i < strlen(inpStr); i++){
            /*first character in a compound cannot be a number*/
            if (i == 0){
                if (isdigit(inpStr[i]) || islower(inpStr[i])){
                    isValid = 0;
                    break;
                }
            }
            if (isupper(inpStr[i])){
                elm[0] = inpStr[i];
                    /* check if element is 2 chars long*/
                if (islower(inpStr[i + 1])){
                    elm[1]= inpStr[i + 1];
                }else{
                    elm[1]= '\0';
                }
                elm[2] = '\0';
                /* search for weight of current element*/
                buffW = getWeight(inpStr, elm, arr, MAXLINES);
                if (buffW == 0){
                    isValid = 0;
                    break;
                }else{
                    isValid = 1;
                }
                /* store it in total weight*/
                totW += buffW;
            }else if (islower(inpStr[i])){
                /* check if previous letter is uppercase */
                if (i != 0 && isupper(inpStr[i - 1]) == 0 ){
                    /* we're not looking at a valid element */
                    isValid = 0;
                    break;
                }
            }else if (isdigit(inpStr[i])){
                /* check that theres a letter before it*/
                if (isalpha(inpStr[i - 1])){
                    /* capture entire number as a string*/
                    j = i;
                    k = 0;
                    while (isdigit(inpStr[j])){
                        dig[k] = inpStr[j++];
                        k++;
                    }
                    dig[k] = '\0';
                    /* turn character string of character numbers into an integer */
                    buffSub = atoi(dig); 
                    /* check if negative*/
                    if (buffSub < 0){
                        isValid = 0;
                        break;
                    }
                    /*
                     * The digit is a subscript for the previous element, 
                     * so we need to subtract the previous number
                     * and add the proper amount
                     */
                    /* subtract the amount we added initially for a single element */
                    totW -= buffW;
                    /* multiply the subscript with element's weight and add it to total */
                    buffW = (float)buffW * (float)buffSub;
                    totW += buffW;
                    /* reset buffsub multiplier*/
                    buffSub = 1;

                /* if theres a number before it, we've already counted this number in previous iteration*/
                }else if (isdigit(inpStr[i - 1])){
                    continue;
                /* some unknown character that isnt a letter or number */
                }else{
                    isValid = 0;
                    break;
                }
            }else{
                isValid = 0;
            }
        }
    /*
     * printing output
     */
    /*
     * what to print is determined by value of isValid:
     * 0: error in input -- print error message
     * 1: input is correct -- print out the weight!
     */
        switch (isValid)
        {
        case 0:
            fprintf(stderr, "%s: not a valid compound\n", inpStr);
            break;
        case 1:
            printf("The atomic weight of %s is %.2f\n", inpStr, totW);
            break;
        default:
            break;
        }
    }
    /* done and dusted */
   return (0);
}
/* function to get weight of an element */
float getWeight(char* inpStr, char* element, EL arr[], int MAXROWS){
    /* check if element exists in elements file*/
    for (j = 0; j < MAXROWS; j++){
        if (strcmp(element, arr[j].sym) == 0){
            /* return the matched element's weight */
            return arr[j].atomW;
        }
    }
    /* no match was found */
    fprintf(stderr, "%s: no such element\n", element);
    return 0;
}

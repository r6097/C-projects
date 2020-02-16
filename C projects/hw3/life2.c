/*
 * Robert Au
 * ECS 36A A04
 * Bishop
 * 
 * This is for HW3 Problem 4
 */
#include <stdio.h>
#include <string.h>
#define BUFFERSIZE 100
/* Builder functions */
/* 
 * prints initial 'corner' piece, 
 * dashes equal to the length of the inside box, 
 * and the ending 'corner' piece
 */
void row_builder(int rect_wid)
{
    int i;
    printf("-");    /* initial cornerpiece */

    for (i = 0; i < rect_wid; i++){
        printf("-");
    }
    printf("-");

    printf("\n");   /* ending cornerpiece */
}
/* prototype */
void row_builder(int a);

/*
 * main function
 */ 
int main(int argc, char *argv[])
{
    long file_size;     /* to check if input file is empty */
    char ch_XO;         /* used to check if character is an X or O */
    char ch_l1;         /* what character of the first line we're on */
    int rows = 0;       /* count how many rows there are */
    int row_len;        /* counts length of current row */
    int l1_count = 0;   /* how long first line is */
    char ch_l;          /* the character the current line is on */
    int l_count = 0;    /* length of the line we are on */
    int iloop;          /* counter for a loop */
    char ch_f;          /* used for printing characters in output */

    FILE* fp; /* file pointer to input file */

    /* first, check for file input */
    if (argc < 2){
        fprintf(stderr,"Usage: %s board_pattern\n", argv[0]);
        return (1);
    }
    /* open the file */
    fp = fopen(argv[1],"r"); /* fp is now pointing to the file inputted */
    /* check if file is real */
    if(fp == NULL){
        fprintf(stderr, "%s: No such file or directory\n",argv[1]);
        return (1);
    }
    
    /*
     * 1. Check if file's not empty
     */ 
    fseek(fp, 0, SEEK_END); /* move pointer to end of file */
    /* 
     * get size of text before file pointer
     * if anything's inside it shouldn't be 0
     */
    file_size = ftell(fp);
    if (file_size == 0){
        printf("Error: file appears to be empty\n");
        return(1);
    }
    fseek(fp, 0, SEEK_SET); /* move pointer back to start to check for correct input */
    
    /*
     * 2. Check if file contains only X and Os
     */ 
    iloop = 0; /* use to count rows */
    while (1){
        ch_XO = fgetc(fp);
        if(ch_XO == '\n')
            iloop++;
        if (feof(fp)){
            break;
        }
        if (ch_XO != 'X' && ch_XO != 'O' && ch_XO != '\n'){
            fprintf(stderr, "%s: bad character ’%c’ in line% d\n", argv[1], ch_XO, iloop + 1);
            return (1);
        }
    }
    /* 
     * reset file pointer to beginning since
     * we will be comparing lengths of rows next
     */
    fseek(fp, 0, SEEK_SET);
     

    /* 
    * 3. Check that all rows are same length
    */

    /* count rows in text file*/
    while(ch_l != EOF){
        ch_l =fgetc(fp);
        if(ch_l == '\n'){
            rows++;
        }
    }
    fseek(fp, 0, SEEK_SET); /* reset pointer to count length of 1 line */

    /* let's get length of first line, to compare with the rest */
    do 
    {
        ch_l1 = fgetc(fp);
        if(ch_l1 == '\n' || ch_l1 == EOF){
            break;
        }
        ++l1_count;
    } while (1);
    fseek(fp, 0, SEEK_SET); /* reset for comparing strings */
    
    iloop = 1;
    l_count = 0;
    /* loop for each row*/
    for (; iloop <= rows; ++iloop)
    {
        /* loop for letters in a row */
        do 
        {
            ch_l = fgetc(fp);
            if(ch_l == '\n' || ch_l == EOF){
                break;
            }
            ++l_count;
        } while (1);
        /* let's compare the lines now */
        if (l_count == l1_count){
            l_count = 0;/* reset counter for next line */
            continue; 
        }
        else{   /* input is not a rectangle */
            fprintf(stderr, "%s: line %d is wrong length\n", argv[1], iloop);
        return (1);
        }
        
        
    }
    fseek(fp, 0, SEEK_SET); /* reset before printing */

    /*
     * Board is valid; build it
     */
    row_len = l1_count;
    row_builder(row_len);    /*print top row*/
    printf("|");    /* workaround--since while loop starts with the text file */
    
    iloop = 0;          /* use iloop to check what row we are on */
    while(ch_f != EOF){
        ch_f = fgetc(fp);
        if (ch_f == '\n'){      /* for middle rows */
            iloop++;
            if (iloop >= rows){
                /* DONT PRINT! we are on the last line (the blank one) */
            }
            else{
                printf("|\n|");
            }
        }
        else if (ch_f == EOF)   /* for second to last row */
        {
            printf("|\n");
        }
        else{
            printf("%c", ch_f);
        }
    }
    /* print enclosing bottom row */  
    row_builder(row_len);

    /* Close file and we're done */
    fclose(fp);
    return (0);
}
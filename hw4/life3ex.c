#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int genmax = 50,         /* number of generations to run for */
    geni = 5;           /* print the board at intervals of this many */
int totCh;          /* total characters overall (not including newline)*/
int i, j;              /* counters for a loop */
int gen;            /* keep track of generations */
int rowi;       /* length of first row */
int whereisInp; /* show which argv has input file*/


typedef struct lifecell{
    char c;     /* the character of the cell */
    int row; /* what row the array character is in */
    int popCt; /* counter for how many populated cells it adjoins */
    int status; /* 1 is populated; 0 is unpopulated */
    int isLeft, /*
                 * if char is on bounding edge from left (1), right (0),
                 * both (2), or neither (-1)
                 */
        isTop; /*  
                * if on bounding edge from top (1), bottom (0), 
                * both (2), or neither (-1) 
                */

} lstruct;

/*prototypes*/
int isPop(int status_a, int status_b);
void checkC(lstruct *arr, int i);
void rowBuilder(int rect_wid);
void printBox(lstruct *arr, int row_len, int row_wid);
void changeC(lstruct *arr);

/*
 *  main function
 */
int main(int argc, char *argv[])
{
    long filesize;
    int ch = 0;
    int chct = 0;       /* character counter */
    int curRow = 1,     /* start with 1 for row 1 (simplicity's sake) */
        totRows = 0;    /* is one less because the last row is blank spaces */    

    FILE* fp; /* file pointer to input file */

    /* variables for checking our options */
    char* argn; /* will store value of option -n */
    char* argp; /* will store value of option -p */
    int option; /* keep track of what argument we're checking */
    int nflag = 0;  /* flags to check that options exist */
    int pflag = 0;
    i = 0;
    /* check for options -n and -p and store them*/
    while((option = getopt(argc, argv, "n:p:" )) != -1)
    {
        i++;
        switch (option)
        {
        case 'n':
            argn = optarg;
            /* check that argument is a positive integer */
            if (atoi(argn) <= 0){
                fprintf(stderr, "%s: %s must be followed by a positive number\n", argv[0], argv[i]);
                exit(1);
            }
            nflag = 1;
            break;
        case 'p':
            argp = optarg;
            /* check that argument is a positive integer */
            if (atoi(argp) <= 0){
                fprintf(stderr, "%s: %s must be followed by a positive number\n", argv[0], argv[i]);
                exit(1);
            }
            pflag = 1;
            break;
        case '?':
            fprintf(stderr,"Usage: %s [ -n num ] [ -p num ] board_pattern\n", argv[0]);
            exit (1);
        default:
            fprintf(stderr, "%s: %s: invalid option\n", argv[0], argv[i]);
            exit(1);
        }
    }
    /* open the file */
    fp = fopen(argv[argc - 1], "r");
    /* check if file is real */
    if(fp == NULL){
        fprintf(stderr, "%s: No such file or directory\n", argv[0]);
        return (1);
    }

    /*
     * reassign generation intervals and
     * generation max only if there was 
     * an option for them
     */
    if (nflag > 0){
        genmax = atoi(argn);
    }
    if (pflag > 0){
        geni = atoi(argp);
    }
    /*
     * Check if file's not empty
     */ 
    fseek(fp, 0, SEEK_END); /* move pointer to end of file */
    /* 
     * get size of text before file pointer
     * if anything's inside it shouldn't be 0
     */
    filesize = ftell(fp);  
    if (filesize == 0){
        printf("Error: file appears to be empty\n");
        return(1);
    }
    fseek(fp, 0, SEEK_SET); /* move pointer back to start to check for correct input */

    while ( (ch = fgetc(fp)) != EOF) /* while we are pointing to a char in the file */
    {
        /*check that the character is valid: X, O, or \n */
        if (ch != 'X' && ch != 'O' && ch != '\n'){
            fprintf(stderr, "%s: bad character ’%c’ in line% d\n", argv[1], ch, curRow);
            return (1);
        }
        if (ch == '\n'){        
            if (curRow == 1)
                rowi = chct;
            curRow++;
            totRows++;
            chct = 0;  
        }
        ++chct;
    }
    /* reset pointer to start to compare lengths of rows*/
    fseek(fp, 0, SEEK_SET); 
    ch = 0;
    ch = fgetc(fp);
    /*
     * compare length of rows
     */    
    totCh = rowi* totRows;
    for (curRow = 1; curRow <= totRows; curRow++){
        do 
        {
            ch = fgetc(fp);
            if(ch == '\n' || ch == EOF){
                break;
            }
            ++chct;
        } while (1);
        /* let's compare the lines now */
        if (chct == rowi){
            chct = 0;/* reset counter for next line */
            continue; 
        }
        else{   /* input is not a rectangle */
            fprintf(stderr, "%s: line %d is wrong length\n", argv[1], curRow);
            return (1);
        }
    }

    /* allocate space equal to total characters and complain if needed */
    lstruct lifebox[totCh];       
    lstruct *lifeptr = lifebox;    
    lifeptr = malloc(totCh * sizeof(lstruct));
    if (lifeptr == NULL) 
        perror("error");
    
    /* clear data inside structure components */
    for ( i = 0; i < totCh; i++){
        /*
         *  overwrite whatever data that may be already inside when we allocated it
         *  ensures that no space contains 1 or 0 that may mess with comparisons
         */
        lifebox[i].isLeft = 69;
        lifebox[i].isTop = 69;
        lifebox[i].popCt = 0; /* in case any cells contain a number already */
    }
    /* reset pointer to store characters into array */
    fseek(fp, 0, SEEK_SET); 
    ch = 0;
    curRow = 1;

    /* 
     * Assignment of component data in structure for each cell
     * - character, status, isLeft, isTop
     */
    /* NOTE: assignment of isLeft is spread out */
    i = 0;
    while ( (ch = fgetc(fp)) != EOF){
        /* make sure to not go over total number of characters*/
        if (i >= totCh)
            break;
        
        if (ch == '\n'){
            /* 
             * update the next character with isLeft
             * - as long as we aren't dealing with
             *   a 1 wide pattern
             */
            if (lifebox[i -1].isLeft != 2){
                lifebox[i].isLeft = 1;
                lifebox[i - 1].isLeft = 0;
            }
            /* ignore everything else and move to next row */   
            curRow++;
            continue;
        }else
            lifebox[i].row = curRow;

        /* assignment of character */
        lifebox[i].c = ch;

        /* check if cell is populated*/
        if (ch == 'X'){
            lifebox[i].status = 0;
        }else if (ch == 'O'){
            lifebox[i].status = 1;
        }

        /* check if cell isLeft */
        /* isLeft special cases */
        if (rowi == 1){ /* row is one character long ; both left and right bounded */
            lifebox[i].isLeft = 2;
        }else if (i == 0){
            lifebox[i].isLeft = 1;  /* first cell in array is always left */
        }else if ( (i + 1) == totCh){     /* last cell in array is always right */
            lifebox[i].isLeft = 0;
        /* check if isLeft wasn't determined earlier */
        }else if ( lifebox[i].isLeft == 69){                  
            lifebox[i].isLeft = -1;
        }

        /* check if cell is on top bounding edge */
        if (totRows == 1)
            lifebox[i].isTop = 2;
        else if (curRow == 1)
            lifebox[i].isTop = 1;
        else if (curRow == totRows)
            lifebox[i].isTop = 0;
        else
            lifebox[i].isTop = -1;
        
        /* move to next character */
        i++;
    }

    fseek(fp, 0, SEEK_SET); 
    ch = 0;
    fclose(fp);
    /*
     * Board is valid; start the game
     */
    for ( gen = 0; gen <= genmax; gen++)
    {
        if ( (gen % geni) == 0){
            printf("Generation %d:\n", gen);
            printBox(lifebox, rowi, totRows);
        /* if the final configuration doesnt fit in the interval */
        /* we still need to print the last one */
        }else if( ((gen + 1) > genmax) && ((gen + geni) >= genmax) ){
            printf("Generation %d:\n", gen);
            printBox(lifebox, rowi, totRows);
        }
        /* update values for popCt*/
        for (i = 0; i < totCh; i++){
            checkC(lifebox, i);
        }
        /* change the pattern for next gen */
        changeC(lifebox);
        /* reset values for popCt in each cell to 0*/
        for (i = 0; i < totCh; i++){
            lifebox[i].popCt = 0;
        }
    }
    
    /*
     * deallocte space
     */
    (void) free(lifeptr);

    return (0);
}

/*
 * FOR UPDATING CELL GENERATIONS
 */
/*
 * checks if the second argument is populated,
 * and if so return 1; otherwise return 0
 * 
 * status_a is the cell that will have popct be increased/or not
 * status_b is the value we are checking for population
 */
/* or char a, char b*/
/* or lstruct a.c, lstruct b.c */
int isPop(int status_a, int status_b)
{
    if ( status_b == 1 ){
        return (1);
    }else{
        return (0);
    }
}
/*
 * updates future status of a cell 
 * based on rules from game of life
 */
void checkC(lstruct *arr, int i)
{
    /*
     * first we sort by where the cell is positioned
     * - a corner cell
     * - a border cell
     * - neither-- cell is not touching borders
     */
    /* if pattern is a single cell box */
    if ( (arr[i].isLeft == 2) && (arr[i].isTop == 2) ){
        /*
         * adjoins nothing so counter remains at 0
         */
        arr[i].popCt = 0;
    
    /* a cell in a WIDE, but 1 cell TALL box */
    }else if ( (arr[i].isLeft == -1) && (arr[i].isTop == 2) ){
        /*
         * check cells to the left and right of the cell
         */
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
    /* a cell in a TALL, but 1 cell WIDE box */
    }else if ( (arr[i].isLeft == 2) && (arr[i].isTop == -1) ){
        /*
         * check cells above and below the cell
         */
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
    
    /* is a LEFT corner cell for a WIDE, but 1 cell TALL box*/
    }else if ( (arr[i].isLeft == 1) && (arr[i].isTop == 2) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
    /* is a RIGHT corner cell for a WIDE, but 1 cell TALL box*/
    }else if ( (arr[i].isLeft == 0) && (arr[i].isTop == 2) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);

    /* is a TOP corner cell for a TALL, but 1 cell WIDE box */
    }else if ( (arr[i].isLeft == 2) && (arr[i].isTop == 1) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
    /* is a BOTTOM corner cell for a TALL, but 1 cell WIDE box */
    }else if ( (arr[i].isLeft == 2) && (arr[i].isTop == 0) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
    /*
     * now for more 'tradtional' pattern cells
     */
    /* is a TOP LEFT corner cell */
    }else if ( (arr[i].isLeft == 1) && (arr[i].isTop == 1) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) + 1].status);
    /* is a TOP RIGHT corner cell */
    }else if ( (arr[i].isLeft == 0) && (arr[i].isTop == 1) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
    /* is a BOTTOM LEFT corner cell */
    }else if ( (arr[i].isLeft == 1) && (arr[i].isTop == 0) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
    /* is a BOTTOM RIGHT corner cell */
    }else if ( (arr[i].isLeft == 0) && (arr[i].isTop == 0) ){ 
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) - 1].status);
    /* is a left border cell */
    }else if ( (arr[i].isLeft == 1) && (arr[i].isTop == -1) ){
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) + 1].status);
    /* is a right border cell */
    }else if ( (arr[i].isLeft == 0) && (arr[i].isTop == -1) ){
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) - 1].status);
    /* is a top border cell */
    }else if ( (arr[i].isLeft == -1) && (arr[i].isTop == 1) ){
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) - 1].status);
    /* is a bottom border cell */
    }else if ( (arr[i].isLeft == -1) && (arr[i].isTop == 0 ) ){
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) - 1].status);
    }else if ( (arr[i].isLeft == -1) && (arr[i].isTop == -1) ){
    /* is a normal, non-bordering cell */
        arr[i].popCt += isPop(arr[i].status, arr[i + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i + (rowi) - 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi)].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) + 1].status);
        arr[i].popCt += isPop(arr[i].status, arr[i - (rowi) - 1].status);
    }
}
/*
 * changes characters of all cells 
 * based on their values of popCt
 */
void changeC(lstruct *arr)
{
    for (j = 0; j <= totCh; j++){
        /*
         * If a populated cell adjoins less than 2 
         * other populated cells, or more than 3 
         * populated cells, it becomes unpopulated.
         */ 
        if ( arr[j].status == 1 && (arr[j].popCt < 2 || arr[j].popCt > 3) ){
            arr[j].status = 0;
            arr[j].c = 'X';
        /*
         * If a populated cell adjoins 2 or 3 
         * populated cells,it remains populated.
         */
        }else if ( arr[j].status == 1 && (arr[j].popCt == 2 || arr[j].popCt == 3) ){
            /* do nothing */
        /*
         * If an unpopulated cell adjoins exactly 
         * 3 populated cells, it becomes populated.
         */
        }else if ( arr[j].status == 0 && (arr[j].popCt == 3) ){
            arr[j].status = 1;
            arr[j].c = 'O';
        }
    }
}

/*
 * FOR PRINTING THE BOX
 */
/*
 * prints row of dashes for top and bottom row
 */
void rowBuilder(int rect_wid)
{
    int i;
    printf("-");    /* initial cornerpiece */

    for (i = 0; i < rect_wid; i++){
        printf("-");
    }
    printf("-");

    printf("\n");   /* ending cornerpiece */
}
/*
 * prints out box using life pattern and the bounding box
 */
void printBox(lstruct *arr, int row_len, int row_wid)
{
    rowBuilder(row_len);    /*print top row*/
     
    for (i = 0; i < totCh; i++){
        if (arr[i].isLeft == 1){
            printf("|");
            printf("%c", arr[i].c);
        }else if (arr[i].isLeft == 0){
            printf("%c", arr[i].c);
            printf("|\n");
        }else{
            printf("%c", arr[i].c);
        }
    }
    rowBuilder(row_len);    /*print bottom row*/
}
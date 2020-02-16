#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXWORDS 1000 /* max words that will be in the dictionary */
#define MAXLENGTH 100 /* max length of word that will be processed */

/* global variable */
char *dictionary[MAXWORDS];     /* array of pointers to dictionary words */
int totalct = 0;   /* total number of words seached until match is found */

/* prototypes */

/*
 * main function
 */ 
int main(int argc, char *argv[])
{
    long file_size; /* to check if input files are empty*/
    int word = 0;	/* current word of index */
    int searchct;   /* counter for how long until word is found */
    char *p;		/* used to copy input line to allocated storage */
    int i;		    /* counter in a for loop */
    int wordct = 0; /* how many words inside dictionary */
    int wordct2 = 0;/* word count for file 2*/
    int word_cmp;   /* stores value of the comparison of 2 words */

    char inp2_word[MAXLENGTH];  /* used to keep track of words in second file */
    char buf[MAXLENGTH];	    /* buffer for input line */

    /*
     * Get input files and open them
     */ 
    FILE *inp;
    FILE *inp2;
    /* first, check for file input */
    if (argc < 3){
        fprintf(stderr,"Usage: lookup [wordlist] [words] \n");
        return (1);
    }
    /* open inp file */
    inp = fopen(argv[1],"r"); 
    /* check if files are real */
    if(inp == NULL){
        fprintf(stderr, "%s: No such file or directory\n",argv[1]);
        return (1);
    }
    inp2 = fopen(argv[2],"r");
    if(inp2 == NULL){
        fprintf(stderr, "%s: No such file or directory\n",argv[2]);
        return (1);
    }

    /* check if files are empty */
    fseek(inp, 0, SEEK_END); /* move pointer to end of file */
    /* 
     * get size of text before file pointer
     * if anything's inside it shouldn't be 0
     */
    file_size = ftell(inp);
    if (file_size == 0){
        printf("No words to search!\n");
        return(1);
    }
    fseek(inp, 0, SEEK_SET); /* move pointer back to beginning to copy words */

    /* repeated for inp2 */
    fseek(inp2, 0, SEEK_END); 
    file_size = ftell(inp2);
    if (file_size == 0){
        printf("No words to find!\n");
        return(1);
    }
    fseek(inp2, 0, SEEK_SET); /* move pointer back to beginning to copy words */


    /*
     * Copy dictionary file into an array of pointers to each word
     */
    
    /*
     * loop through input file one line at a time,
     * allocate space for the line
     * then copy the line into the space
     */
    while (fgets(buf, MAXLENGTH, inp) != NULL){
        /* check if dictionary is no more than 1000 words */
        if (word > MAXWORDS){
            fprintf(stderr, "Over maximum words limit\n");
            return (1);
        }
        
        /* allocate space for line and complain if needed */
        p = malloc((strlen(buf) + 1) * sizeof(char));
        if (p == NULL){
            fprintf(stderr, "Line %d: no memory!\n", word + 1);
            return(1);
        }
        /* then save pointer p's location in the dictionary */
        dictionary[word] = p;
        word += 1; /* dictionary moves to the next word */
        wordct +=1; /* keep track of size of dictionary */

        /* now copy from inputted file to the allocated space */
        (void) strcpy(p, buf);
    }


    /*
     * search inside dictionary words from the second file
     */
    i = 0; /* counter for loop */
    
    /* iterate through each word in second file */
    while (fgets(inp2_word, MAXLENGTH, inp2) != NULL){
        /* reset counters for new word search */
        wordct2+=1;
        i = 0;
        searchct = 0;
        
        /* iterate through words in dictionary for matches */
        for (; &p[i] < &p[wordct]; i++){    

            p = dictionary[i]; /* point to element i of dictionary */
            word_cmp = strcmp(p, inp2_word); /* check if words match */
            if (word_cmp > 0 || word_cmp < 0){
                /* do nothing -- word is not matching*/
            }
            else if (word_cmp == 0){    /* word is matching */
                searchct = i + 1;       /* + 1 because i starts at 0 for word 1 */
                if (!feof(inp2)){        /* get rid trailing newline */
                    inp2_word[strlen(inp2_word)-1] = '\0';
                }
                printf("'%s' is word %d in the list\n", inp2_word, (searchct));
                totalct += searchct; /* add to total searches count*/
            }
        }
        if (searchct == 0){             /* word isn't in dictionary */
            if (!feof(inp2)){           /* get rid trailing newline */
                inp2_word[strlen(inp2_word)-1] = '\0';
            }
            printf("'%s' is not in the word list\n", inp2_word);
            totalct += i;/* since not found, just add max length to it*/
        }   
    }
    /* free the memory */
    i = 0;
    for (; i <= wordct; i++){
        (void) free(dictionary[i]);
    }
    /*
     * print the results
     */
    printf("Checked %d words while looking for %d words; average is %.2f\n",totalct , wordct2, (((double)totalct )/ ((double)wordct2)));
    
    /* the end */
    fclose(inp);
    fclose(inp2);
    return(0);
}
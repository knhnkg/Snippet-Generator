/* Program to generate term-biased snippets for paragraphs of text.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2022, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

/*Global variable definition*************************************/

    /* maximum number of characters per word */
#define MAX_WORD_LEN 23
    /* maximum number of words per paragraph */
#define MAX_PARA_LEN 10000

    /* return code from get_word if end of paragraph found */
#define PARA_END 1
    /* return code from get_word if a word successfully found */
#define WORD_FND 2

    /* terminating punctuation that may follow a word */
#define TERM_PUNCT ".,;:!?"
    /* terminating punctuation that needs dots added */
#define NEEDS_DOTS ",;:"
    /* the string the separates paragraphs */
#define PARA_SEPARATOR "\n\n"
    /* insertion characters to indicate "bold" */
#define BBOLD "**"
    /* length of those characters when wrapped around a word */
#define BBOLD_LEN (2*strlen(BBOLD))
    /* insertion characters to indicate omitted text */
#define DDOTS "..."

    /* maximum words in any output snippet */
#define MAX_SNIPPET_LEN 30
    /* minimum words in any output snippet */
#define MIN_SNIPPET_LEN 20
    /* maximum length of output lines */
#define MAX_OUTPUT_LINE 72

    /* maximum terms allowed on command line */
#define MAX_TERMS 50
   
/*Function declaration********************************************/

typedef char word_t[MAX_WORD_LEN+1];

int get_word(word_t w, int limit);
int get_paragraph(word_t p[], int limit, int argc, char *argv[]);
int get_match(word_t p, int argc, char *argv[]);

/****************************************************************/

/* main program controls all the action*/
int
main(int argc, char *argv[]) {
    word_t p[MAX_PARA_LEN];
    
    while (get_paragraph(p, MAX_PARA_LEN, argc, argv) != EOF){
    }
    
    return 0;
}

/* 
    This function counts the number of paragraphs within the 
    standard input and the amount of words in each paragraph 
    formed from taking a single word each at a time with 
    function 'get_word'.
    While doing so, also detect and bolden the query words with
    function get_match. 
    Print out the standard input with each line not exceeding
    the maximum length of output line. 
*/
int get_paragraph(word_t P[], int limit, int argc, char *argv[]) {
    word_t w;
    word_t p[MAX_PARA_LEN];
    int i = 0;
    int word_counter = 0;
    int para_counter = 0;
    int match = 0;
    int k = 0;
    int finder = 0; 
     
    while (limit >= 0) {
        /* take one word at each time */
        int word = get_word(w, MAX_WORD_LEN);
        
       /* word has been found */
        if (word == WORD_FND) {
            /* add the word to the current paragraph */
            strcpy(p[i], w);
            
            /* pass the word to another function to compare
               with the query words */
            char *check = p[i];
            if (get_match(check, argc, argv) == 1) {
                /* count the number of matches */
                match += 1;
            }
            i += 1;        
            word_counter += 1;
            limit -= 1;
        }
        
        /* end of a paragraph has been reached */
        else if (word == PARA_END) {
            para_counter += 1;
            
            /* print stage 1 output: para #, word amount */
            printf("\n======= Stage 1 [para %d; %d words]\n\n", 
                    para_counter, word_counter);
            /* print stage 2 output: para #, word amount, match amount */
            printf("======= Stage 2 [para %d; %d words; %d matches]\n", 
                    para_counter, word_counter, match);
            
            int line_len = MAX_OUTPUT_LINE;
            int used_len = 0;
            
            int word_len = strlen(p[k]);
            finder = k + word_counter;
            printf("%s", p[k]);
            k += 1;
            used_len = used_len + word_len; 
            
            /* print current paragraph, with each output line
               not exceeding the maximum output line length */
            while (k < finder) {
                int word_len = strlen(p[k]);
                if ((used_len + word_len + 1) > line_len) {
                    /* when output line reaches the max length
                       move to the next line */
                    printf("\n");
                    printf("%s", p[k]);
                    k += 1;
                    used_len = 0 + word_len; 
                }

                else {
                    /* print words on the same line, while current
                       output line length is smaller than limit */
                    printf(" ");
                    printf("%s", p[k]);
                    used_len = used_len + word_len + 1;
                    k += 1;
                }
            } 
            printf("\n");
            
            /* reset the word counter and limit to 0 and 
               count for next new paragraph */
            word_counter = 0;
            limit = MAX_PARA_LEN;
            match = 0;
        }
        
        else if (word == EOF) {
            para_counter += 1;
            
            /* print stage 1 output: para #, word amount */
            printf("\n======= Stage 1 [para %d; %d words]\n\n", 
                    para_counter, word_counter);
            /* print stage 2 output: para #, word amount, match amount */
            printf("======= Stage 2 [para %d; %d words; %d matches]\n", 
                    para_counter, word_counter, match);
                        
            int line_len = MAX_OUTPUT_LINE;
            int used_len = 0;
            
            int word_len = strlen(p[k]);
            finder = k + word_counter;
            printf("%s", p[k]);
            k += 1;
            used_len = used_len + word_len;
            
            /* print current paragraph, with each output line
               not exceeding the maximum output line length */
            while (k < finder) {
                int word_len = strlen(p[k]);  
                if ((used_len + word_len + 1) > line_len) {
                    /* when output line reaches the max length
                       move to the next line */
                    printf("\n");
                    printf("%s", p[k]);
                    k += 1;
                    used_len = 0 + word_len; 
                }

                else {
                    /* print words on the same line, while current
                       output line length is smaller than limit */
                    printf(" ");
                    printf("%s", p[k]);
                    used_len = used_len + word_len + 1;
                    k += 1;
                }
            }          
            printf("\n\n");
            return EOF; 
        }
    }
    return 0;
}

/* 
    This function detects and boldens the words from the 
    standard input are exacts matches to the the query words, 
    insensitive of case and trailing punctuations. 
*/
int get_match(word_t p, int argc, char *argv[]) {
    char *punc;
    char copy[strlen(p)];
    char result[strlen(p) + BBOLD_LEN + BBOLD_LEN + 1];
    
    /* if input word contains trailing punctuation, keep it to be 
       added back to the word after the comparison for match */
    if (ispunct(p[strlen(p)-1]) != 0) {
        punc = &p[strlen(p)-1];
    }
    
    /* iterate over the loop to compare all query words */ 
    for(int j = 1; j < argc; j++) {
        /* make a copy of the input word */
        strcpy(copy, p);
        
        /* temporarily remove any trailing punctuation from
           the word for compasion insensitive of punctuation */
        if (ispunct(copy[strlen(copy)-1]) != 0) {
            copy[strlen(copy)-1] = '\0';
        }
        
        /* check if input word is a exact math of query word
           insensitive of case */
        if (strcasecmp(copy, argv[j]) == 0) {
            /* yes it is */
            /* for case where input word contains punctuation,
                bolden the word and put back the punctuation */
            if (ispunct(p[strlen(p)-1]) != 0) {
                result[0] = '\0';
                strcat(result, BBOLD);
                strcat(result, copy);
                strcat(result, BBOLD);
                strcat(result, punc);
                strcpy(p, result);
                return 1;
            }
            
            /* yes it is */
            /* for case where input word does not contain any
               punctuation, just bolden the word */
            else if (ispunct(p[strlen(p)-1]) == 0) { 
                result[0] = '\0';
                strcat(result, BBOLD);
                strcat(result, copy);
                strcat(result, BBOLD);
                strcpy(p, result);
                return 1;
            }
        }
    }
    
    /* return false if there is not match */
    return 0;
}

/* 
    extract a single word out of the standard input, but not
    more than "limit" characters in total. One character of
    sensible trailing punctuation is retained.  
    argument array W must be limit+1 characters or bigger
*/
int get_word(word_t W, int limit) {
    int c;
    
    /* first, skip over any non alphanumerics */
    /* also check if the end of paragraph has been reached */
    while ((c=getchar())!=EOF && !isalnum(c)) {
        if (c == '\n') {
            W += 1;
            if ((c=getchar()) == '\n') {
                return PARA_END;
            }
            else {
                W -= 1;
                break;
            }
        }
    }
    
    /* check if the end of file has been reached */
    if (c==EOF) {
        return EOF;
    }
    
    /* ok, first character of next word has been found */
    *W = c;
    W += 1;
    limit -= 1;
    while (limit>0 && (c=getchar())!=EOF && isalnum(c)) {
        /* another character to be stored */
        *W = c;
        W += 1;
        limit -= 1;
    }
    
    /* take a look at that next character, is it a sensible trailing
       punctuation? */
    if (strchr(TERM_PUNCT, c) && (limit>0)) {
        /* yes, it is */
        /* in this case, keep the trailing punctuation*/
        *W = c;
        W += 1;
        limit -= 1;
    }

    /* now close off the string */
    /* the end of word has been reached */
    *W = '\0';
    return WORD_FND;
}

/* algorithms are fun */
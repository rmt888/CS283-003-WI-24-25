#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len) {
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str;   
    char *dest = buff;      
    int str_len = 0;        
    int space_flag = 1;     

    //process each character from user_str
    while (*src != '\0') {
        if (str_len >= len) {
            return -1;
        }
        //handles any whitespace
        if (*src == ' ' || *src == '\t') {
            if (!space_flag) {
                //adds a single space if the last character was not a space
                *dest++ = ' ';
                str_len++;
                space_flag = 1;
            }
        } else {
            //copies the non white space character and resets the space flag
            *dest++ = *src;
            str_len++;
            space_flag = 0;
        }
        src++;
    }
    //removes trailing space
    if (str_len > 0 && *(dest - 1) == ' ') {
        dest--;
        str_len--;
    }
    //fill remaining buffer with dots
    while (str_len < len) {
        *dest++ = '.';
        str_len++;
    }
    return str_len;
}

void print_buff(char *buff, int len){
    //changed the format to match test cases
    printf("Buffer:  [");
    for (int i = 0; i < len; i++) {
        putchar(*(buff + i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len) {
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        //check if it's not a word
        if (*(buff + i) == ' ' || *(buff + i) == '\t') {
            in_word = 0;
        } else {
            //if it is a word
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }
    return word_count;
}

void reverse_string(char *buff, int str_len) {
    char *start = buff;
    char *end = buff + str_len - 1;

    //skips all the dots to go to the last character
    while (end >= buff && *end == '.') {
        end--;
    }
    //swaps the characters from the start and end then moves towards the center
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void word_print(char *buff, int str_len) {
    int word_count = 0;
    int word_length = 0;

    printf("Word Print\n----------\n");
    //goes through the buffer to process each character
    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ' || buff[i] == '.') {
            if (word_length > 0) {
                //%.*s` ensures only word_length characters are printed from the word's starting position
                printf("%d. %.*s(%d)\n", ++word_count, word_length, buff + i - word_length, word_length);
                word_length = 0;
            }
        } else {
            word_length++;
        }
    }
    printf("\nNumber of words returned: %d\n", word_count);
}

//Extra Credit
void string_replace(char *buff, const char *old_word, const char *new_word, int buffer_len) {
    char temp_buff[buffer_len];
    char *src = buff;
    char *dest = temp_buff;
    char *old_word_start;
    int old_word_len = 0;
    int new_word_len = 0;
    int found = 0;

    //the length of the old_word
    while (*(old_word + old_word_len) != '\0') {
        old_word_len++;
    }
    //the length of the new_word
    while (*(new_word + new_word_len) != '\0') {
        new_word_len++;
    }
    //use temp_buff with empty characters
    memset(temp_buff, '.', buffer_len);
    while (*src != '\0' && dest - temp_buff < buffer_len) {
        //check if we have found the old_word
        old_word_start = src;
        for (int i = 0; i < old_word_len; i++) {
            if (*(src + i) != *(old_word + i)) {
                break;
            }
            if (i == old_word_len - 1) {
                //replaces old_word with new_word
                found = 1;
                for (int j = 0; j < new_word_len && dest - temp_buff < buffer_len; j++) {
                    *dest++ = *(new_word + j);
                }
                src += old_word_len;
            }
        }
        if (src == old_word_start) {
            //no match
            *dest++ = *src++;
        }
    }
    while (*src != '\0' && dest - temp_buff < buffer_len) {
        *dest++ = *src++;
    }
    //no old_word
    if (!found) {
        printf("Not Implemented!\n");
        exit(1);
    }
    //copy temp_buff back to buff
    memcpy(buff, temp_buff, buffer_len);
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string
    char *old_word = NULL;  //the old word
    char *new_word = NULL;  //the new word

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    This is safe becauce it handles cases where the user does not provide a enough arguments. 
    "argc < 2" verifies that the user entered at least one argument besides the program name. 
    The "*argv[1] != '-'" ensures the first character of the provided argument is a valid 
    option flag, by using this "'-'". These safety checks ensures we are getting the correct 
    input from the user. If the input is invalid, the program prints usage instructions and 
    exits with an error, preventing crashes or unexpected behavior.
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    The "if (argc < 3)"" statement ensures that the user has entered both an option flag 
    and a string to operate on, similar to #1. It checks whether there are at least three arguments, 
    where the first is the program name, the second is the option flag like (-h, -c), 
    and the third is the string input for processing. If there are fewer than three arguments, 
    it means the user didn't provide the required string. If the input is invalid it will follow the
    same with #1. The program prints usage instructions and exits with an error, preventing crashes or unexpected behavior.
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error: Memory allocation failed!\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            word_print(buff, user_str_len);
            break;

        //Extra Credit
        case 'x': 
            //this ensures the user entered all the arguments
            if (argc < 5) { 
                usage(argv[0]); 
                exit(1); 
            } 
            old_word = argv[3]; 
            new_word = argv[4]; 
            string_replace(buff, old_word, new_word, BUFFER_SZ); 
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
/*
Providing both the pointer and the length to the helper functions is a 
good practice because it helps the code be more readable, 
ensures the function is readable to anyone else, can be used multiple times (reusability), 
be maintable, and have more error handling. Even though buff always has 50 bytes 
allocated here, a separate length parameter makes the functions more reusable 
and flexible for varying buffer sizes. It also prevents potential issues 
if buffer size changes in the future.
*/
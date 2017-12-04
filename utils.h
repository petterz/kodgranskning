#ifndef __UTILS_H__
#define __UTILS_H__

extern char *strdup(const char *);
/// makes it possible for a function to return either an int or a string
typedef union {
  int   i;
  float f;
  char *s;
} answer_t;

/// This allows custom check functions to be passed as an argument to functions
typedef bool(*check_func)(char *);
/// Makes answer_t synonymous with a pointer to a function. Said function returns a string
typedef answer_t(*convert_func)(char *);


/// prints question prompts input from user which is then returned
///
/// \param question prints the question
/// \param check function used to check if input is valid
/// \param convert func is used to convert input to desired type
/// \returns: an answer_t that passes check, and is modified by convert_func
answer_t ask_question(char *question, check_func check, convert_func);

/// prints question and returns user given input
///
/// \param question is printed
/// \returns pointer to user input string
char *ask_question_string(char *question);

/// prints question and returns a user given int
///
/// \param question is printed
/// \returns: a user given int
int ask_question_int(char *question);


/// Clears the input buffer
int clean_input_buffer(void);


/// returns a bool based on input (true for yes, false for no)
bool yes_or_no();

/// prints the given string within a border
void print_message(char *message);

/// returns a char from user input in upper format
char get_a_char();

/// checks if input is equal given char c
bool check_character(char choice, char c);

#endif

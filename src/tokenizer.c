#include "tokenizer.h"


//theToken is the token to be populated
void read_token (token *theToken, char* str) 
{
  if (str[0] == ';') {
    strcpy(theToken->str, "EMPTY");
    return;
  } else if (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]))){
    theToken->type = LITERAL;
    char *ptr;
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
      theToken->literal_value = strtol(str, &ptr, 0);
    } else {
      theToken->literal_value = strtol(str, &ptr, 10);
    }
  } else if (str[0] == '+') {
    theToken->type = PLUS;
  } else if (str[0] == '-') {
    theToken->type = MINUS;
  } else if (str[0] == '*') {
    theToken->type = MUL;
  } else if (str[0] == '/') {
    theToken->type = DIV;
  } else if (str[0] == '%') {
    theToken->type = MOD;
  } else if (str[0] == '^') {
    theToken->type = POW;
  } else if (strcmp(str, "defun") == 0) {
    theToken->type = DEFUN;
  } else if (strcmp(str, "return") == 0) {
    theToken->type = RETURN;
  } else if (strcmp(str, "and") == 0) {
    theToken->type = AND;
  } else if (strcmp(str, "or") == 0) {
    theToken->type = OR;
  } else if (strcmp(str, "not") == 0) {
    theToken->type = NOT;
  } else if (strcmp(str, "and") == 0) {
    theToken->type = XOR;
  } else if (strcmp(str, "lt") == 0) {
    theToken->type = LT;
  } else if (strcmp(str, "le") == 0) {
    theToken->type = LE;
  } else if (strcmp(str, "eq") == 0) {
    theToken->type = EQ;
  } else if (strcmp(str, "ge") == 0) {
    theToken->type = GE;
  } else if (strcmp(str, "gt") == 0) {
    theToken->type = GT;
  } else if (strcmp(str, "drop") == 0) {
    theToken->type = DROP;
  } else if (strcmp(str, "dup") == 0) {
    theToken->type = DUP;
  } else if (strcmp(str, "swap") == 0) {
    theToken->type = SWAP;
  } else if (strcmp(str, "min") == 0) {
    theToken->type = MIN;
  } else if (strcmp(str, "abs") == 0) {
    theToken->type = ABS;
  } else if (strcmp(str, "avg") == 0) {
    theToken->type = AVG;
  } else if (strcmp(str, "if") == 0) {
    theToken->type = IF;
  } else if (strcmp(str, "else") == 0) {
    theToken->type = ELSE;
  } else if (strcmp(str, "endif") == 0) {
    theToken->type = ENDIF;
  } else if (str[0] == 'a' && str[1] == 'r' && str[2] == 'g') {
    theToken->type = ARG;
    char *ptr;
    theToken->arg_no = strtol(str + 3, &ptr, 10);
  } else {
    theToken->type = IDENT;
  }
  strcpy(theToken->str, str);
}

//used for debugging
void print_token (token *theToken) 
{
  printf("Value: %d, arg_no: %d, str: %s\n", 
         theToken->literal_value, theToken->arg_no, theToken->str);
}


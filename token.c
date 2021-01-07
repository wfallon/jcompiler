#include "token.h"

token* init_token() 
{
  //make sure you initialize char* in token. The length of str will be MAX_TOKEN_LENGTH
  token* val = malloc(sizeof (token));
  val->type = EMPTY;
  val->literal_value = 0;
  val->arg_no = 0;
  val->str = malloc(sizeof(char) * MAX_TOKEN_LENGTH);
  return val;
}

void delete_token(token* theToken)
{
  //make sure you delete char* in token
  free(theToken->str);
  free(theToken);
}

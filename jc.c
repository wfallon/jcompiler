#include "hash.h"
#include "tokenizer.h"
#include <libgen.h>

int loop_count = 0;
int branch_count = 0;

bool inIfStatement = false;

hash_struct* LOOPS;
int if_max = -1;
int final_if = -1;

char* filename_no_extension;

void writeLiteral(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "ADD R6, R6, #%d\n", -1);
  fprintf(dest_file, "CONST R1, #%d\n", (theToken->literal_value & 0xFF));
  if ((theToken->literal_value & 0xFF00) != 0) {
    fprintf(dest_file, "HICONST R1, #%d\n", (theToken->literal_value & 0xFF00)>>8);
  }
  fprintf(dest_file, "STR R1, R6, #0\n");
}

void writePower(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "CONST R3, #1\n");
  fprintf(dest_file, "LOOP_%d\n", loop_count);
  fprintf(dest_file, "MUL R3, R3, R1\n");
  fprintf(dest_file, "ADD R2, R2, #-1\n");
  fprintf(dest_file, "CMPI R2, #0\n");
  fprintf(dest_file, "BRp LOOP_%d\n", loop_count);
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R3, R6, #0\n");
  loop_count++;
}

void writeFunctionName(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "%s\n", theToken->str);
  fprintf(dest_file, "STR R7, R6, #-2\n");
  fprintf(dest_file, "STR R5, R6, #-3\n");
  fprintf(dest_file, "ADD R6, R6, #-3\n");
  fprintf(dest_file, "ADD R5, R6, #0\n");
}

void writeIdent(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "JSR %s\n", theToken->str);
}

void writeReturn(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "ADD R6, R5, #0\n");
  fprintf(dest_file, "STR R1, R6, #2\n");
  fprintf(dest_file, "ADD R6, R6, #3\n");
  fprintf(dest_file, "LDR R5, R6, #-3\n");
  fprintf(dest_file, "LDR R7, R6, #-2\n");
  fprintf(dest_file, "ADD R6, R6, #-1\n");
  fprintf(dest_file, "RET\n");
}

void writePlus(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R2, R2, R1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeMinus(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "SUB R2, R1, R2\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeMul(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "MUL R2, R2, R1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeDiv(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "DIV R2, R1, R2\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeMod(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "MOD R2, R1, R2\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeAnd(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "AND R2, R2, R1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeOr(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "OR R2, R2, R1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeXor(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "XOR R2, R2, R1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeNot(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "NOT R1, R1\n");
  fprintf(dest_file, "STR R1, R6, #0\n");
}

void writeLt(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRn BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #0\n");
  fprintf(dest_file, "BRnzp END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #1\n");
  fprintf(dest_file, "END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeLe(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRnz BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #0\n");
  fprintf(dest_file, "BRnzp END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #1\n");
  fprintf(dest_file, "END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeEq(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRz BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #0\n");
  fprintf(dest_file, "BRnzp END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #1\n");
  fprintf(dest_file, "END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeGe(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRzp BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #0\n");
  fprintf(dest_file, "BRnzp END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #1\n");
  fprintf(dest_file, "END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeGt(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRp BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #0\n");
  fprintf(dest_file, "BRnzp END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "CONST R1, #1\n");
  fprintf(dest_file, "END_BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeDrop(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "ADD R6, R6, #1\n");
}

void writeDup(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "ADD R6, R6, #-1\n");
  fprintf(dest_file, "STR R1, R6, #0\n");
}

void writeSwap(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "STR R1, R6, #1\n");
  fprintf(dest_file, "STR R2, R6, #0\n");
}

void writeMin(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMP R1, R2\n");
  fprintf(dest_file, "BRp BRANCH_%d\n", branch_count);
  fprintf(dest_file, "ADD R1, R2, #0\n");
  fprintf(dest_file, "BRANCH_%d\n", branch_count);
  fprintf(dest_file, "STR R1, R6, #0\n");
  branch_count++;
}

void writeAbs(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "SRA R2, R1, #15\n");
  fprintf(dest_file, "ADD R3, R2, R1\n");
  fprintf(dest_file, "XOR R1, R3, R2\n");
  fprintf(dest_file, "STR R1, R6, #0\n");
}

void writeAvg(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "LDR R2, R6, #1\n");
  fprintf(dest_file, "LDR R3, R6, #2\n");
  fprintf(dest_file, "ADD R3, R3, R2\n");
  fprintf(dest_file, "ADD R3, R3, R1\n");
  fprintf(dest_file, "CONST R2, #3\n");
  fprintf(dest_file, "DIV R3, R3, R2\n");
  fprintf(dest_file, "ADD R6, R6, #2\n");
  fprintf(dest_file, "STR R3, R6, #0\n");
}

void writeArg(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "ADD R6, R6, #-1\n");
  fprintf(dest_file, "LDR R1, R5, #%d\n", theToken->arg_no + 2);
  fprintf(dest_file, "STR R1, R6, #0\n");
}

void writeIf(token* theToken, FILE* dest_file) {
  put(LOOPS, if_max + 1, final_if);
  if_max++;
  final_if = if_max;
  fprintf(dest_file, "LDR R1, R6, #0\n");
  fprintf(dest_file, "ADD R6, R6, #1\n");
  fprintf(dest_file, "CMPI R1, #0\n");
  fprintf(dest_file, "BRz %s_ELSE_%d\n", filename_no_extension, final_if);
  fprintf(dest_file, "%s_IF_%d\n", filename_no_extension, final_if);
  inIfStatement = true;
}

void writeElse(token* theToken, FILE* dest_file) {
  fprintf(dest_file, "BRnzp %s_ENDIF_%d\n", filename_no_extension, final_if);
  fprintf(dest_file, "%s_ELSE_%d\n", filename_no_extension, final_if);
  inIfStatement = false;
}

void writeEndif(token* theToken, FILE* dest_file) {
  if (inIfStatement) {
    fprintf(dest_file, "%s_ELSE_%d\n", filename_no_extension, final_if);
  }
  fprintf(dest_file, "%s_ENDIF_%d\n", filename_no_extension, final_if);
  final_if = get(LOOPS, final_if)->value;
  inIfStatement = false;
}

void writeOutTokens (token* tokens, FILE* dest_file) {
  //for(int i = 0; i < token_count; i++) {
    //print_token(tokens[i]);
    if(tokens->type == LITERAL) {
      writeLiteral(tokens, dest_file); 
    } else if (tokens->type == POW) {
      writePower(tokens, dest_file);
    } else if (tokens->type == DEFUN) {
      //i++;
      writeFunctionName(tokens, dest_file);
    } else if (tokens->type == IDENT) {
      writeIdent(tokens, dest_file);
    } else if (tokens->type == RETURN) {
      writeReturn(tokens, dest_file);
    } else if (tokens->type == PLUS) {
      writePlus(tokens, dest_file);
    } else if (tokens->type == MINUS) {
      writeMinus(tokens, dest_file);
    } else if (tokens->type == MUL) {
      writeMul(tokens, dest_file);
    } else if (tokens->type == DIV) {
      writeDiv(tokens, dest_file);
    } else if (tokens->type == AND) {
      writeAnd(tokens, dest_file);
    } else if (tokens->type == OR) {
      writeOr(tokens, dest_file);
    } else if (tokens->type == XOR) {
      writeXor(tokens, dest_file);
    } else if (tokens->type == NOT) {
      writeNot(tokens, dest_file);
    } else if (tokens->type == LT) {
      writeLt(tokens, dest_file);
    } else if (tokens->type == LE) {
      writeLe(tokens, dest_file);
    } else if (tokens->type == EQ) {
      writeEq(tokens, dest_file);
    } else if (tokens->type == GE) {
      writeGe(tokens, dest_file);
    } else if (tokens->type == GT) {
      writeGt(tokens, dest_file);
    } else if (tokens->type == DROP) {
      writeDrop(tokens, dest_file);
    } else if (tokens->type == DUP) {
      writeDup(tokens, dest_file);
    } else if (tokens->type == SWAP) {
      writeSwap(tokens, dest_file);
    } else if (tokens->type == MIN) {
      writeMin(tokens, dest_file);
    } else if (tokens->type == ABS) {
      writeAbs(tokens, dest_file);
    } else if (tokens->type == AVG) {
      writeAvg(tokens, dest_file);
    } else if (tokens->type == ARG) {
      writeArg(tokens, dest_file);
    } else if (tokens->type == IF) {
      writeIf(tokens, dest_file);
    } else if (tokens->type == ELSE) {
      writeElse(tokens, dest_file);
    } else if (tokens->type == ENDIF) {
      writeEndif(tokens, dest_file);
    } else if (tokens->type == MOD) {
      writeMod(tokens, dest_file);
    }
  
}

int main(int argc, char *argv[]) 
{
  if (argc != 2) {
    printf("The correct usage is ./jc my_program.j");
    return 1;
  }
  FILE* file = fopen (argv[1], "r");
  
  if (file == NULL) {
    printf("The file does not exist");
    return 1;
  }
  
  char* filename_no_path = basename(argv[1]);
  
  filename_no_extension = malloc (strlen(filename_no_path)+1);
  strcpy(filename_no_extension, filename_no_path);
  char* lastdot;
  lastdot = strrchr (filename_no_extension, '.');
  if (lastdot != NULL){
    *lastdot = '\0';
  }
  //printf("%s\n", filename_no_extension);
  
  char filename_extension[strlen(filename_no_extension) + 5];
  strcpy(filename_extension, filename_no_extension);
  filename_extension[strlen(filename_no_extension)] = '.';
  filename_extension[strlen(filename_no_extension)+1] = 'a';
  filename_extension[strlen(filename_no_extension)+2] = 's';
  filename_extension[strlen(filename_no_extension)+3] = 'm';
  filename_extension[strlen(filename_no_extension)+4] = '\0';
  
  FILE* dest_file = fopen(filename_extension, "w");
 
  LOOPS = initTable(100);
  token* tokens[MAX_TOKEN_LENGTH];
  char line[MAX_TOKEN_LENGTH];
  int token_count = 0;
  
  fprintf(dest_file, ".CODE\n");
  fprintf(dest_file, ".FALIGN\n");
  int counter = 0;
  token* theToken;
  
  while (1) {
    fgets(line, MAX_TOKEN_LENGTH, file);
    char* curr = strtok(line, " \t \n");
    if (counter == 1 && curr!= NULL && strcmp(curr, "return") == 0) {
      break;
    } 
    counter = 0;
    while(curr) {
      theToken = init_token();
      read_token (theToken, curr);
      if (theToken->type == EMPTY) {
        delete_token(theToken);
        theToken = NULL;
        break;
      } else if(theToken->type == DEFUN) {
        delete_token(theToken);
        theToken = NULL;
        theToken = init_token();
        curr = strtok(NULL, " \t \n");
        read_token(theToken, curr);
        theToken->type = DEFUN;
      } 
      if (feof(file) || strcmp(theToken->str, "return") == 0) {
        counter = 1;
        writeOutTokens(theToken, dest_file);
        delete_token(theToken);
        theToken = NULL;
        token_count++;
        break;
      }
      token_count++;
      writeOutTokens(theToken, dest_file);
      delete_token(theToken);
      theToken = NULL;
      curr = strtok(NULL, " \t \n");
    }
    /*
    if (token_count != 0) {
      writeOutTokens(tokens, token_count, dest_file);
      for(int i = 0; i < token_count; i++) {
        //delete_token(tokens[i]);
        free(tokens[i]->str);
      }
      token_count = 0;
    }
    */
    if (feof(file)) {
      break;
    }
    
  }
  fclose(file);
  fclose(dest_file);
  done(LOOPS);
  free(filename_no_extension);
  return 0;
}

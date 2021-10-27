#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
int executive(char **tokens);
int change(char **tokens);
int is_pipe(char *line);
char **helper_tokens(char *line);
int pipping(char *line);

/* Splits the string by space and returns the array of tokens
*
*/

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i;


	while(1) {
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

	//	printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

     //just to compare the two letters
     if(strncmp(tokens[0],"cd",2) == 0){
      change(tokens);
    }else if(is_pipe(line)== 1){
      int pid = fork();
      if(pid==0){
        pipping(line);
      }else{
      wait(0);
    }
    }else{
      executive(tokens);
     }


		// Freeing the allocated memory
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
// it will fork and compare if it is a child, if so it will execute the command you are looking for
int executive(char **tokens) {
  int pid = fork();
  if(pid == 0){
    execvp(tokens[0],tokens);
    printf("This cmd couldn't execute\n");
    exit(0);
  }else{
    wait(0);
  }
  return 0;
}
// checks if the path is given, if so it will change the directory.
int change(char **tokens){
  if(tokens[1] != NULL){
    chdir(tokens[1]);
  }
}
//modify the tokenize to tokenize with pipe (|)
char **helper_tokens(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == '|' || readChar == '\n'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	       tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	       strcpy(tokens[tokenNo++], token);
         //add new line to end of the token
         tokenNo--;
         int length = strlen(tokens[tokenNo]);
         tokens[tokenNo][length] = '\n';
         tokens[tokenNo][length + 1] = '\0';
         tokenNo++;
	       tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

int pipping(char *line){
  int arr[2];
  char **pipe_tokens = helper_tokens(line);
  pipe(arr);
  int pid = fork();
  //printf("print\n");
  if(pid==0){
    //printf("l %s \n",pipe_tokens[0]);
    char ** left_side = tokenize(pipe_tokens[0]);
    //printf("left %s\n", left_side[0]);
//closing 1 help us to close the input of the terminal output
    close(1);
    //copy pipe into output
    dup2(arr[1],1);
    close(arr[0]);
    close(arr[1]);

    execvp(left_side[0],left_side);
  }else{
    //printf("r %s \n",pipe_tokens[1]);
    char ** right_side = tokenize(pipe_tokens[1]);
    //printf("right %s\n", right_side[0]);
    //closing 0  help us to close the output of the terminal output
    close(0);
    //copy pipe into input
    dup2(arr[0],0);
    close(arr[0]);
    close(arr[1]);

    execvp(right_side[0],right_side);
  }
  return 0;
}

//for figure out if command has pipe
int is_pipe(char *line){
  int ask_pipe = 0;
  for(int i=0;line[i] != 0; i++){
    if(line[i]=='|'){
      ask_pipe = 1;
    }
  }
  return ask_pipe;
}

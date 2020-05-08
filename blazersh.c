/*Blazersh!!
Made by: Kevin Mooney
to compile: gcc -lreadline -o blazersh blazersh

*/


#include<string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h> 

#define MAXCOM 100 
#define MAXLIST 100 


void intro() 
{ 
    char* uname = getenv("USER"); 
    printf("\n\n\nWelcome toBlazerSH!! %s", uname, "\n"); 

    sleep(3); 
} 

int inputData(char* str) 
{ 
    char* buffer; 
  
    buffer = readline("\nblazersh> "); 
    if (strlen(buffer) != 0) { 
        add_history(buffer); 
        strcpy(str, buffer); 
        return 0; 
    } else { 
        return 1; 
    } 
} 




void forkExecvp(char** parsed, char** parsedpipe) 
{ 
    // 0 is read end, 1 is write end 
    int fdin, fdout;
    int status;
    pid_t pid; 
  
   /* if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } */
    
    /* open file to read standard input stream,
       make sure the file stdin.txt exists, even if it is empty */
     if ((fdin = open("stdin.txt", O_RDONLY)) == -1) {
       printf("Error opening file stdin.txt for input\n");
       exit(-1);
    }

    /* open file to write standard output stream in append mode.
       create a new file if the file does not exist. */
    if ((fdout = open("stdout.txt", O_CREAT | O_APPEND | O_WRONLY)) == -1) {
       printf("Error opening file stdout.txt for output\n");
       exit(-1);
    }
    
    pid = fork(); 
    if (pid == 0) { 
        //file reader functions
        
        dup2(fdin, 0); 
        dup2(fdout, 1);
         
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else if (pid > 0){ 
        // Parent executing 
        
        printf("Child process active: please wait\n");
        wait(&status); /* wait for the child process to terminate */
        
        if (WIFEXITED(status)) {
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
            write(fdout, "Hey! This is the parent process\n", 32);
            close(fdout);
        } else { 
            // waiting for child process
            wait(NULL); 
            wait(NULL); 
        }
        }else { /*error */
        perror("fork"); 
        exit(EXIT_FAILURE);
    }
}

int pipeP(char* str, char** strP) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strP[i] = strsep(&str, "|"); 
        if (strP[i] == NULL) 
            break; 
    } 
  
    if (strP[1] == NULL) 
        return 0; 
    else { 
        return 1; 
    } 
} 

void space(char* str, char** parse) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parse[i] = strsep(&str, " "); 
  
        if (parse[i] == NULL) 
            break; 
        if (strlen(parse[i]) == 0) 
            i--; 
    } 
}
  
int parseStr(char* str, char** parse, char** pp) 
{ 
  
    char* strP[2]; 
    int p = 0; 
  
    p = pipeP(str, strP); 
  
    if (p) { 
        space(strP[0], parse); 
        space(strP[1], pp); 
  
    } else { 
  
        space(str, parse); 
    } 
  
    if (CommandHandler(parse)) 
        return 0; 
    else
        return 1 + p; 
} 

/* next few functions: set of commmands
*/

void environ() {
    return;
}

void set() {
    return;
}

void lsD() {
	struct dirent *d;
	DIR *od = opendir(".");
	if(od != NULL) {
		while((d = readdir(od)) != NULL)
			printf("%s\n", d->d_name);
		closedir(od);
	}
    return;
}

void cd() {
    chdir("cd ..");
    return;
}

void help()
{
	printf("\n+++Blazersh Help+++\n+++++++++++++++++++++++++"
		"\nCommand Operations:"
		"\n>environ - see name and values"
		"\n>set - set name and values"
		"\n>list - list the file and folder in current directory"
		"\n>cd - change directory"
		"\n>help - give commmands for shell"
		"\n>quit - exit shell");

	return;
}

int CommandHandler(char** read)
{
	int Commands = 6;
	int changeSetting = 0;
	int i = 0;
	char* commandsList[Commands];

	commandsList[0] = "environ";
	commandsList[1] = "set";
	commandsList[2] = "list";
	commandsList[3] = "cd";
	commandsList[4] = "help";
	commandsList[5] = "quit";

	for (i = 0; i < Commands; i++) {
		if (strcmp(read[0], commandsList[i]) == 0) {
			changeSetting = i + 1;
			break;
		}
	}

	switch (changeSetting) {
	case 1:
	    return 1;
		
	case 2:
	    return 1;
	    
	case 3:
	    lsD();
    	return 1;
		
	case 4:
		cd();
		return 1;
		
	case 5:
	    help();
		return 1;
	
	case 6:
	printf("--------------------bye---------------------\n");
        exit(0);
        
	default:
		break;
	}

	return 0;
}


int main() 
{ 
    char inputStr[MAXCOM], *parseA[MAXLIST]; 
    char* parseB[MAXLIST]; 
    int eFlag = 0;
    intro(); 
  
    while (1) { 
        // input data
        if (inputData(inputStr)) 
            continue; 
        // accepting command 
        eFlag = parseStr(inputStr, 
        parseA, parseB); 
        
  
        // execute 
            forkExecvp(parseA, parseB); 
    } 
    return 0; 
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

void help()
{
    printf("            This is implementation of microshell by Szymon Pawlak           \n\n");
    printf("My implementation commends list:\n");
    printf("help - print information about microshell and list of commend\n");
    printf("cd - changing directory\n");
    printf("exit - end program\n");
}
void cd(char **polecenie, char*path)
{
    if(polecenie[1] == NULL)
    	chdir("/");
    else if(strcmp(polecenie[1],"-") == 0)
        chdir(path);
    else
    {
        chdir(polecenie[1]);
        if(errno != 0)
        	fprintf(stderr, "No such file or directory\n");
        
    }    
    return;
}

void podzial(char *input, char **polecenie)
{
    int i=0;
    char *word = strtok(input," ");
    while(word != NULL)
    {
        polecenie[i] = word;
        i++;
        word = strtok(NULL," ");
    }
    polecenie[i] = NULL;
    return;
}

    int getlogin_r(char *buf, size_t bufsize);
    int main(int argc, char *argv[])
    {
        char *polecenie[100];
        char cwd[100];
        char *input;
        char napis[100];
        char login[100];
        int i=0;
        char PD[2][100];
        getcwd(cwd, sizeof(cwd));
        strcpy(PD[1],cwd);
        while(1)
        {
            getcwd(cwd,sizeof(cwd));
            getlogin_r(login,sizeof(login));
            printf("[{%s}%s]$ ", cwd, login);
            input = fgets(napis,sizeof(napis),stdin);
            i = strlen(napis);
            napis[i-1] ='\0';
            podzial(input,polecenie);
            if(strlen(napis) == 0)
            {
           	printf("Nie wpisano zadnego polecenia\n");
            	continue;
            }
            if(strcmp(polecenie[0],"help") == 0)
            {
                help();
            }
            else if(strcmp(polecenie[0],"exit") == 0)
                exit(0);
            else if(strcmp(polecenie[0], "cd") == 0)
            {
               
                	strcpy(PD[0],cwd);
                	cd(polecenie,PD[1]);
                	strcpy(PD[1],PD[0]);
            }
            else{
                int pid = fork();
                if(pid == -1)
                {
                	perror("fork");
                	exit(EXIT_FAILURE);
                } else if (pid ==0) 
                {
                	execvp(polecenie[0],polecenie);
                	perror("Niepoprawne polecenie");
                	exit(EXIT_FAILURE);
                } else
                	waitpid(pid, NULL, 0);
            }
        }
        exit(EXIT_SUCCESS);
        return 0;
    }

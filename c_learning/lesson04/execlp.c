#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc, char* argv[])
{
    if(fork() == 0)
        if(execlp("ps", "ps", "-u", NULL) < 0)
            perror("execlp");
    // execl("/bin/ps","ps","-u",NULL)

    // char *envp[]={"PATH=/tmp","USER=david", NULL};
    // execle("/usr/bin/env", "env", NULL, envp)

    // char *arg[] = {"env", NULL};
    // char *envp[] = {"PATH=/tmp", "USER=david", NULL};
    // execve("/usr/bin/env", arg, envp)

    // char *arg[] = {"ls","-l","/", NULL};
    // execv("/bin/ls", arg)

    // execl("/bin/ps", "ps", "-u", NULL)
    return 0;
}
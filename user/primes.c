#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDDER_FILENO 2

#define READEND 0
#define WRITEEND 1

typedef int pid_t;

int main(void)
{
    int numbers[36], fd[2];
    int prime = 2;
    pid_t pid;

    
    while (prime < 36)
    {
        pipe(fd); // 为什么每次都要创建pipe?
        if ((pid = fork()) < 0) 
        {
            fprintf(STDDER_FILENO, "fork error\n");
            exit(0);
        }
        else if (pid > 0)
        {   
            // 找到第一个素数
            while(prime < 36 && numbers[prime] == 1){
                prime++;
            }
            // 埃氏筛
            for(int temp = prime*prime; temp < 36; temp+=prime){
                numbers[temp] = 1;
            }

            if(prime > 35){
                exit(0);
            }

            close(fd[READEND]);
            write(fd[WRITEEND], &prime, sizeof(prime));
            close(fd[WRITEEND]);

            // 置位
            numbers[prime] = 1;
            prime++;

            wait((int *)0);
        
        }
        else 
        {
            if(prime > 35){
                exit(0);
            }

            int temp = 0;
            close(fd[WRITEEND]);
            read(fd[READEND], &temp, sizeof(temp) != 0);
            close(fd[READEND]);

            if(temp == 0){
                close(fd[READEND]);
                exit(0);
            }

            printf("prime %d\n", temp);
            // fork again until no prime
            exit(0);
        }
    }
    exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
int a[10000], n = 0, x = 0;

int search(int i) // it will used to search x in array
{
    int v = (10000 / n);
    if ((i + 1) == n)
    {
        for (int j = v * (i); j < 10000; j++)
        {
            if (a[j] == x)
            {
                printf("\n\tThe child process %d with PID %d got success in search\n", i + 1, getpid());
                return 1;
            }
        }
        return 0;
    }
    for (int j = v * (i); j < v * (i + 1); j++)
    {
        if (a[j] == x)
        {
            printf("\n\tThe child process %d with PID %d got success in search\n", i + 1, getpid());
            return 1;
        }
    }
    return 0;
}

int main()
{
    int status = 0;
    srand(time(0));
    for (int i = 0; i < 10000; i++)
    {
        a[i] = rand() % 1000 + 1;
        // a[i] = i + 1;
    }

    pid_t p, wpid;
    printf("Enter the integer x in the range [1-10000] to be searched:\n");
    scanf("%d", &x);

    printf("Enter the number of childs (n) :\n");
    scanf("%d", &n);
    int c[n], count = 0;

    int i;
    for (i = 0; i < n; i++)
    {
        c[i] = p = fork(); // using fork we create childs
        if (p == 0)
            break;
    }
    if (p > 0)
    {
        while ((wpid = wait(&status)) >= 0) // It means Parent process will wait all child exits
        {
            // status is greater than zero means search was successful so we can stop the searching
            if (status > 0)
            {
                printf("\n\tParent synchornzied with child %d death (ret Child PId %d ret Status %d)\n", status / 256, wpid, status);
                for (int i = 0; i < n; i++)
                {
                    if (i != ((status / 256) - 1))
                    {
                        kill(c[i], SIGINT);
                        printf("\n\tParent killed the Child %d PID %d", i + 1, c[i]);
                    }
                }
                break;
            }
            else
            {
                count++;
                // when all child process exit code with zero means the element is not present in array
                if (count == n)
                {
                    printf("\n\tThe element was not found in the array\n");
                }
            }
        }
    }
    if (p == 0)
    {
        sleep(1);
        if (search(i))
        {
            exit(i + 1); // when searching was success it will exits code with value > 0
        }
        else
        {
            exit(0); // when searching fails it will exit code with 0
        }
    }
    printf("\n");
    printf("\n\tNow parent dying...as it's task was completed\n");
}

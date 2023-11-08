#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
int a[10000], c[100] = {0}, count = 0, n = 0;
clock_t start_t, end_t;
double total_t;

void k(int x) // when any one child success in search
{
    for (int i = 0; i < n; i++)
    {
        kill(c[i], SIGINT);
        printf("\n\tParent killed the child processs with PID %d by sending signal SIGINT\n", c[i]);
    }

    printf("\n\tNow the Parent Process died as it's task was completed\n");
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("\n\tThe total time taken by the process is:\t%lf\n", total_t);
    kill(getpid(), SIGINT);
}

void f(int x) // when search process is compelete but it wont found the x
{
    count++;
    if (count == n)
    {
        printf("\n\tThe value x was not found and both the child process sended SIGUSR2 siginal\n");
        for (int i = 0; i < n; i++)
        {
            kill(c[i], SIGINT);
            printf("\n\tParent killed the child processs with PID %d by sending signal SIGINT\n", c[i]);
        }

        printf("\n\tParent process dying\n");
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("\n\t%lf\n", total_t);
        kill(getpid(), SIGINT);
    }
}

void search(int x, int i)
{
    int v = (10000 / n);
    if ((i + 1) == n)
    {
        for (int j = v * (i); j < 10000; j++)
        {
            if (a[j] == x)
            {
                printf("\n\tThe child process %d got success in search\n", i + 1);
                kill(getppid(), SIGUSR1);
                return;
            }
        }
    }
    else
    {
        for (int j = v * (i); j < v * (i + 1); j++)
        {
            if (a[j] == x)
            {
                printf("\n\tThe child process %d got success in search\n", i + 1);
                // sleep(1);
                kill(getppid(), SIGUSR1);
                return;
            }
        }
    }
    kill(getppid(), SIGUSR2);
}

int main()
{

    start_t = clock();

    srand(time(0));
    for (int i = 0; i < 10000; i++)
    {
        a[i] = rand() % 1000 + 1;
        // a[i]=i+1;
    }

    signal(SIGUSR1, k);
    signal(SIGUSR2, f);

    int x;
    printf("Enter the integer x in the range [1-1000] to be searched:\n");
    scanf("%d", &x);

    printf("Enter the number of childs (n) in the range [1-100]:\n");
    scanf("%d", &n);

    pid_t p;
    int i;
    for (i = 0; i < n; i++)
    {
        p = fork();
        c[i] = p;
        if (p == 0)
            break;
    }
    sleep(1);
    if (p == 0)
    {
        search(x, i);
    }
    for (;;);
}

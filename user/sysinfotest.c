#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void sinfo(struct sysinfo *info)
{
    if (sysinfo(info) < 0)
    {
        printf("FAIL: sysinfo failed");
        exit(1);
    }
}

// Use sbrk() to count how many free physical memory pages there are.
int countfree()
{
    uint64 sz0 = (uint64)sbrk(0);
    struct sysinfo info;
    int n = 0;

    while (1)
    {
        if ((uint64)sbrk(PGSIZE) == (uint64)-1)
        { // Use (uint64)-1 for failure case
            break;
        }
        n += PGSIZE;
    }

    sinfo(&info);

    // Use %ld to print uint64 values
    if (info.freemem != 0)
    {
        printf("FAIL: there is no free mem, but sysinfo.freemem=%ld\n", info.freemem);
        exit(1);
    }

    sbrk(-((uint64)sbrk(0) - sz0)); // Restore the memory to the initial state
    return n;
}

void testmem()
{
    struct sysinfo info;
    uint64 n = countfree();

    sinfo(&info);

    // Use %ld to print uint64 values
    if (info.freemem != n)
    {
        printf("FAIL: free mem %ld (bytes) instead of %ld\n", info.freemem, n);
        exit(1);
    }

    if ((uint64)sbrk(PGSIZE) == (uint64)-1)
    {
        printf("sbrk failed");
        exit(1);
    }

    sinfo(&info);

    if (info.freemem != n - PGSIZE)
    {
        printf("FAIL: free mem %ld (bytes) instead of %ld\n", info.freemem, n - PGSIZE);
        exit(1);
    }

    if ((uint64)sbrk(-PGSIZE) == (uint64)-1)
    {
        printf("sbrk failed");
        exit(1);
    }

    sinfo(&info);

    if (info.freemem != n)
    {
        printf("FAIL: free mem %ld (bytes) instead of %ld\n", info.freemem, n);
        exit(1);
    }
}

void testcall()
{
    struct sysinfo info;

    if (sysinfo(&info) < 0)
    {
        printf("FAIL: sysinfo failed\n");
        exit(1);
    }

    // Check for invalid argument for sysinfo
    if (sysinfo((struct sysinfo *)0xeaeb0b5b00002f5e) != (uint64)-1)
    {
        printf("FAIL: sysinfo succeeded with bad argument\n");
        exit(1);
    }
}

void testproc()
{
    struct sysinfo info;
    uint64 nproc;
    int status;
    int pid;

    sinfo(&info);
    nproc = info.nproc;

    pid = fork();
    if (pid < 0)
    {
        printf("sysinfotest: fork failed\n");
        exit(1);
    }
    if (pid == 0)
    {
        sinfo(&info);
        if (info.nproc != nproc + 1)
        {
            printf("sysinfotest: FAIL nproc is %ld instead of %ld\n", info.nproc, nproc + 1);
            exit(1);
        }
        exit(0);
    }
    wait(&status);
    sinfo(&info);
    if (info.nproc != nproc)
    {
        printf("sysinfotest: FAIL nproc is %ld instead of %ld\n", info.nproc, nproc);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    printf("sysinfotest: start\n");
    testcall();
    testmem();
    testproc();
    printf("sysinfotest: OK\n");
    exit(0);
}

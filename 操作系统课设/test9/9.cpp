// #include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>

// using namespace std;

#define NUM_PHILOSOPHERS 6
#define NUM_FORKS 6

// 自定义信号量联合体（变量之间共用内存空间，互斥）
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

// P 操作
void P(int semid, int sem_num)
{
    //信号量索引，操作值，操作标志（阻塞）
    struct sembuf op = {sem_num, -1, 0};
    // 信号量集标识符，指向操作数组的指针、操作数组的操作数量
    semop(semid, &op, 1);
}

// V 操作
void V(int semid, int sem_num)
{
    struct sembuf op = {sem_num, 1, 0};
    semop(semid, &op, 1);
}

// 哲学家进程
void philosopher(int id, int semid)
{
    srand(time(NULL) ^ (getpid() << 16)); // 随机数种子

    int left_fork = id;
    int right_fork = (id + 1) % NUM_FORKS;

    for (int i = 0; i < 3; i++)
    // while(1)
    {
        printf("哲学家 %d 正在思考...\n", id);
        sleep(rand() % 3 + 1); // 随机思考时间 (1-3 秒)

        // 获取叉子 (避免死锁策略)
        // 若为偶数哲学家，则先拿左边后拿右边
        if (id % 2 == 0)
        {
            P(semid, left_fork);
            printf("哲学家 %d 拿起左叉子 %d\n", id, left_fork);
            P(semid, right_fork);
            printf("哲学家 %d 拿起右叉子 %d\n", id, right_fork);
        }
        //若为奇数哲学家，则先拿右边，再拿左边
        else
        {
            P(semid, right_fork);
            printf("哲学家 %d 拿起右叉子 %d\n", id, right_fork);
            P(semid, left_fork);
            printf("哲学家 %d 拿起左叉子 %d\n", id, left_fork);
        }

        // 进餐
        printf("===== 哲学家 %d 开始进餐 =====\n", id);
        sleep(rand() % 2 + 1); // 随机进餐时间 (1-2 秒)
        printf("===== 哲学家 %d 结束进餐 =====\n", id);

        // 释放叉子 (固定顺序)
        V(semid, left_fork);
        printf("哲学家 %d 放下左叉子 %d\n", id, left_fork);
        V(semid, right_fork);
        printf("哲学家 %d 放下右叉子 %d\n", id, right_fork);
    }

    exit(0);
}

int main()
{
    int semid;
    union semun arg;

    // 创建信号量集合 (5 个叉子)
    if ((semid = semget(IPC_PRIVATE, NUM_FORKS, IPC_CREAT | 0666)) == -1)
    {
        perror("semget 失败");
        exit(1);
    }

    // 初始化信号量的值 (所有叉子初始为 1)
    unsigned short init_values[NUM_FORKS] = {1, 1, 1, 1, 1, 1};
    arg.array = init_values;
    // 信号量集标识符、信号量索引、控制命令、其他参数
    if (semctl(semid, 0, SETALL, arg) == -1)
    {
        perror("semctl SETALL 失败");
        semctl(semid, 0, IPC_RMID); // 清理
        exit(1);
    }

    // 创建 5 个哲学家进程
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            philosopher(i, semid); // 子进程执行哲学家行为
        }
        else if (pid < 0)
        {
            perror("fork 失败");
            exit(1);
        }
    }

    // 等待所有子进程结束
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        wait(NULL);
    }

    // 清理信号量集
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID 失败");
        exit(1);
    }

    printf("所有哲学家完成进餐。\n");
    return 0;
}

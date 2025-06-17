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

// �Զ����ź��������壨����֮�乲���ڴ�ռ䣬���⣩
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

// P ����
void P(int semid, int sem_num)
{
    //�ź�������������ֵ��������־��������
    struct sembuf op = {sem_num, -1, 0};
    // �ź�������ʶ����ָ����������ָ�롢��������Ĳ�������
    semop(semid, &op, 1);
}

// V ����
void V(int semid, int sem_num)
{
    struct sembuf op = {sem_num, 1, 0};
    semop(semid, &op, 1);
}

// ��ѧ�ҽ���
void philosopher(int id, int semid)
{
    srand(time(NULL) ^ (getpid() << 16)); // ���������

    int left_fork = id;
    int right_fork = (id + 1) % NUM_FORKS;

    for (int i = 0; i < 3; i++)
    // while(1)
    {
        printf("��ѧ�� %d ����˼��...\n", id);
        sleep(rand() % 3 + 1); // ���˼��ʱ�� (1-3 ��)

        // ��ȡ���� (������������)
        // ��Ϊż����ѧ�ң���������ߺ����ұ�
        if (id % 2 == 0)
        {
            P(semid, left_fork);
            printf("��ѧ�� %d ��������� %d\n", id, left_fork);
            P(semid, right_fork);
            printf("��ѧ�� %d �����Ҳ��� %d\n", id, right_fork);
        }
        //��Ϊ������ѧ�ң��������ұߣ��������
        else
        {
            P(semid, right_fork);
            printf("��ѧ�� %d �����Ҳ��� %d\n", id, right_fork);
            P(semid, left_fork);
            printf("��ѧ�� %d ��������� %d\n", id, left_fork);
        }

        // ����
        printf("===== ��ѧ�� %d ��ʼ���� =====\n", id);
        sleep(rand() % 2 + 1); // �������ʱ�� (1-2 ��)
        printf("===== ��ѧ�� %d �������� =====\n", id);

        // �ͷŲ��� (�̶�˳��)
        V(semid, left_fork);
        printf("��ѧ�� %d ��������� %d\n", id, left_fork);
        V(semid, right_fork);
        printf("��ѧ�� %d �����Ҳ��� %d\n", id, right_fork);
    }

    exit(0);
}

int main()
{
    int semid;
    union semun arg;

    // �����ź������� (5 ������)
    if ((semid = semget(IPC_PRIVATE, NUM_FORKS, IPC_CREAT | 0666)) == -1)
    {
        perror("semget ʧ��");
        exit(1);
    }

    // ��ʼ���ź�����ֵ (���в��ӳ�ʼΪ 1)
    unsigned short init_values[NUM_FORKS] = {1, 1, 1, 1, 1, 1};
    arg.array = init_values;
    // �ź�������ʶ�����ź������������������������
    if (semctl(semid, 0, SETALL, arg) == -1)
    {
        perror("semctl SETALL ʧ��");
        semctl(semid, 0, IPC_RMID); // ����
        exit(1);
    }

    // ���� 5 ����ѧ�ҽ���
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            philosopher(i, semid); // �ӽ���ִ����ѧ����Ϊ
        }
        else if (pid < 0)
        {
            perror("fork ʧ��");
            exit(1);
        }
    }

    // �ȴ������ӽ��̽���
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        wait(NULL);
    }

    // �����ź�����
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID ʧ��");
        exit(1);
    }

    printf("������ѧ����ɽ��͡�\n");
    return 0;
}

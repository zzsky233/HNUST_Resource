#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>

// ��ʾ������Ϣ
void show_help() {
    printf("\n/**********************************/\n");
    printf("echo <����>\t��ʾ���ݲ�����\n");
    printf("quit/exit/bye\t�˳�shell\n");
    printf("cd [Ŀ¼]\t���Ĺ���Ŀ¼(Ĭ����ʾ��ǰĿ¼)\n");
    printf("jobs\t\t��ʾ��ǰshell���ӽ�����Ϣ\n");
    printf("environ\t\t��ʾ���л�������\n");
    printf("help\t\t��ʾ�˰�����Ϣ\n");
    printf("/**********************************/\n");
}

// ��ʾ�˵�
void show_menu() {
    printf("\n --------------------------------------------------\n");
    printf("                 shell �����н�����");
    printf("\n --------------------------------------------------\n");
    printf("\n -----------------------------------\n");
    printf("              ����ѡ��                \n");
    printf(" -----------------------------------\n");
    printf("|1��echo            		     |\n");
    printf("|2��quit exit bye    		     |\n");
    printf("|3��cd      			     |\n");
    printf("|4��jobs			     |\n");
    printf("|5. environ                          |\n");
    printf("|6. help                             |\n");
    printf(" -----------------------------------\n");
    printf("\n");
}

int main() {
    char input[256];  // �洢�û�����
    char *args[10];   // �洢�ָ��Ĳ���
    int arg_count;    // ��������
    
    while (1) {
        show_menu();
        printf("��������Ĳ���: ");
        
        // ��ȡ��������
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // ����Ctrl+D
        }
        
        // �Ƴ����з�
        input[strcspn(input, "\n")] = '\0';
        
        // ����������
        if (strlen(input) == 0) continue;
        
        // �ָ�����Ϊ����
        arg_count = 0;
        char *token = strtok(input, " ");
        while (token != NULL && arg_count < 9) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // execvp��ҪNULL��β
        
        // �����˳�����
        if (strcmp(args[0], "quit") == 0 || 
            strcmp(args[0], "exit") == 0 || 
            strcmp(args[0], "bye") == 0) {
            printf("�ټ�!\n");
            break;
        }
        // ����help����
        else if (strcmp(args[0], "help") == 0) {
            show_help();
        }
        // ����cd����
        else if (strcmp(args[0], "cd") == 0) {
            if (arg_count == 1) {
                // ��ʾ��ǰĿ¼
                char cwd[256];
                if (getcwd(cwd, sizeof(cwd))) {
                    printf("��ǰĿ¼: %s\n", cwd);
                } else {
                    perror("getcwd");
                }
            } else {
                // ����Ŀ¼
                if (chdir(args[1])) {
                    perror("cdʧ��");
                }
            }
        }
        // ����echo����
        else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < arg_count; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        }
        // ����environ����
        else if (strcmp(args[0], "environ") == 0) {
            // ʹ��ϵͳenv������ʾ��������
            pid_t pid = fork();
            if (pid == 0) {
                execlp("env", "env", NULL);
                perror("execlp");
                exit(1);
            } else if (pid > 0) {
                wait(NULL);
            }
        }
        // ����jobs����
        else if (strcmp(args[0], "jobs") == 0) {
            // �򻯰�jobs - ��ʾ��ǰshell�Ľ�����Ϣ
            printf("��ǰshell����ID: %d\n", getpid());
            printf("������ID: %d\n", getppid());
        }
        // ������������
        else {
            pid_t pid = fork();
            
            if (pid < 0) {
                perror("forkʧ��");
            } else if (pid == 0) { // �ӽ���
                // ִ������
                execvp(args[0], args);
                
                // ���execvp���أ�˵������ִ��ʧ��
                perror("����ִ��ʧ��");
                exit(1);
            } else { // ������
                // �ȴ��ӽ��̽���
                wait(NULL);
            }
        }
    }
    
    return 0;
}

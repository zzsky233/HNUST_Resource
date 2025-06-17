#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>

// 显示帮助信息
void show_help() {
    printf("\n/**********************************/\n");
    printf("echo <内容>\t显示内容并换行\n");
    printf("quit/exit/bye\t退出shell\n");
    printf("cd [目录]\t更改工作目录(默认显示当前目录)\n");
    printf("jobs\t\t显示当前shell的子进程信息\n");
    printf("environ\t\t显示所有环境变量\n");
    printf("help\t\t显示此帮助信息\n");
    printf("/**********************************/\n");
}

// 显示菜单
void show_menu() {
    printf("\n --------------------------------------------------\n");
    printf("                 shell 命令行解释器");
    printf("\n --------------------------------------------------\n");
    printf("\n -----------------------------------\n");
    printf("              操作选择                \n");
    printf(" -----------------------------------\n");
    printf("|1、echo            		     |\n");
    printf("|2、quit exit bye    		     |\n");
    printf("|3、cd      			     |\n");
    printf("|4、jobs			     |\n");
    printf("|5. environ                          |\n");
    printf("|6. help                             |\n");
    printf(" -----------------------------------\n");
    printf("\n");
}

int main() {
    char input[256];  // 存储用户输入
    char *args[10];   // 存储分割后的参数
    int arg_count;    // 参数计数
    
    while (1) {
        show_menu();
        printf("请输入你的操作: ");
        
        // 读取整行输入
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // 处理Ctrl+D
        }
        
        // 移除换行符
        input[strcspn(input, "\n")] = '\0';
        
        // 跳过空命令
        if (strlen(input) == 0) continue;
        
        // 分割命令为参数
        arg_count = 0;
        char *token = strtok(input, " ");
        while (token != NULL && arg_count < 9) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // execvp需要NULL结尾
        
        // 处理退出命令
        if (strcmp(args[0], "quit") == 0 || 
            strcmp(args[0], "exit") == 0 || 
            strcmp(args[0], "bye") == 0) {
            printf("再见!\n");
            break;
        }
        // 处理help命令
        else if (strcmp(args[0], "help") == 0) {
            show_help();
        }
        // 处理cd命令
        else if (strcmp(args[0], "cd") == 0) {
            if (arg_count == 1) {
                // 显示当前目录
                char cwd[256];
                if (getcwd(cwd, sizeof(cwd))) {
                    printf("当前目录: %s\n", cwd);
                } else {
                    perror("getcwd");
                }
            } else {
                // 更改目录
                if (chdir(args[1])) {
                    perror("cd失败");
                }
            }
        }
        // 处理echo命令
        else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < arg_count; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        }
        // 处理environ命令
        else if (strcmp(args[0], "environ") == 0) {
            // 使用系统env命令显示环境变量
            pid_t pid = fork();
            if (pid == 0) {
                execlp("env", "env", NULL);
                perror("execlp");
                exit(1);
            } else if (pid > 0) {
                wait(NULL);
            }
        }
        // 处理jobs命令
        else if (strcmp(args[0], "jobs") == 0) {
            // 简化版jobs - 显示当前shell的进程信息
            printf("当前shell进程ID: %d\n", getpid());
            printf("父进程ID: %d\n", getppid());
        }
        // 处理其他命令
        else {
            pid_t pid = fork();
            
            if (pid < 0) {
                perror("fork失败");
            } else if (pid == 0) { // 子进程
                // 执行命令
                execvp(args[0], args);
                
                // 如果execvp返回，说明命令执行失败
                perror("命令执行失败");
                exit(1);
            } else { // 父进程
                // 等待子进程结束
                wait(NULL);
            }
        }
    }
    
    return 0;
}

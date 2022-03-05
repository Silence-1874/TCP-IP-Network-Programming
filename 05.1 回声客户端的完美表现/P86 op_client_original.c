#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char op_cnt[BUF_SIZE], operand[BUF_SIZE], operator[BUF_SIZE], result[BUF_SIZE];
    int result_len;
    struct sockaddr_in serv_adr;

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else 
        puts("Connected............");

    fputs("Operand count: ", stdout);
    fgets(op_cnt, BUF_SIZE, stdin);
    // 向服务器端发送操作数数目
    if (write(sock, op_cnt, sizeof(op_cnt)) == -1) 
        error_handling("send Operand cound error!");

    // 向服务器端分别发送操作数
    for (int i = 0; i < atoi(op_cnt); i++) {
        printf("Operand %d: ", i + 1);
        fgets(operand, BUF_SIZE, stdin);
        if (write(sock, operand, strlen(operand)) == -1)
            error_handling("send Operand error!");
    }

    // 向服务器端发送操作符
    fputs("Operator: ", stdout);
    fgets(operator, BUF_SIZE, stdin);
    if (write(sock, operator, strlen(operator)) == -1)
        error_handling("send Operator error!");

    // 读取服务器端发送回的计算结果
    if (read(sock, result, BUF_SIZE - 1) == -1)
        error_handling("read Opration result error!");
    printf("Operation result: %d\n", atoi(result));

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

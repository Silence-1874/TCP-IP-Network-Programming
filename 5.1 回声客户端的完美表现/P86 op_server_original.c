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
    int serv_sock, clnt_sock;
    char op_cnt[BUF_SIZE], operand[BUF_SIZE], operator[BUF_SIZE];

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1)
        error_handling("accept() error");

    // 读取客户端发来的操作数数目
    if (read(clnt_sock, op_cnt, BUF_SIZE) == -1)
        error_handling("read Operand counter error!");

    // 创建存储操作数的数组
    int len = atoi(op_cnt);
    int* operands = (int*)malloc(sizeof(int*) * len);    

    // 读取客户端发来的操作数，并存入数组钟
    for (int i = 0; i < len; i++)
    {
        if (read(clnt_sock, operand, BUF_SIZE) == -1)
            error_handling("read Operand error");
        operands[i] = atoi(operand);
    }

    // 读取客户端发来的操作符
    if (read(clnt_sock, operator, BUF_SIZE) == -1)
        error_handling("read Operator error");

    // 根据操作符，对数组中的操作数进行运算，将计算结果存储在result中
    int result;
    switch(operator[0]) {
        case '+':
            result = 0;
            for (int i = 0; i < len; i++) result += operands[i];
            break;
        case '-':
            result = operands[0];
            for (int i = 1; i < len; i++) result -= operands[i];
            break;
        case '*':
            result = 1;
            for (int i = 0; i < len; i++) result *= operands[i];
            break;
        default:
            error_handling("the operantor doesn't support");
    }

    // 将result转化为字符型，存储在ret中，并发送回客户端
    char *ret;
    sprintf(ret, "%d", result);
    write(clnt_sock, ret, sizeof(ret));

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

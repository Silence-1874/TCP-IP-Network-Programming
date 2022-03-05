#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
    // 创建一个文件和两个套接字
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM, 0);

    // 输出创建的文件描述符的整数值
    printf("file descriptor 1: %d\n", fd1); // fd1 = 3
    printf("file descriptor 2: %d\n", fd2); // fd2 = 4
    printf("file descriptor 3: %d\n", fd3); // fd3 = 5

    close(fd1);close(fd2);close(fd3);
    return 0;
}
// 标准输入（Standard Input）  fd = 0
// 标准输出（Standard Output） fd = 1
// 标准错误（Standard Error）  fd = 2

#pragma warning(disable:4996)
#include <WinSock2.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define DATA_SIZE 1024 * 1024 * 10 //10MB
#define MAXLINE 409600

char* servIP = "";   // 서버ip
int servPort = 0;    // 포트번호

int makeSocket() {
    
    WSADATA wsaData;
    SOCKADDR_IN servAddr;
    int sock;
    char ip[20];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        printf("WSAStartup() error!\n");
    
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket create failed\n");

    struct hostent* host_entry;
    host_entry = gethostbyname(servIP);
    for (int ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++) {
        strcpy(ip, inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[ndx]));
    }

    memset(&servAddr, 0, sizeof(servAddr));     //Zero ou structure
    servAddr.sin_family = AF_INET;              //Internet address family
    servAddr.sin_addr.s_addr = inet_addr(ip);   //Server IP address
    servAddr.sin_port = htons(servPort);        //Server port

    if (connect(sock, (struct sockaddr*) & servAddr, sizeof(servAddr)) < 0)
        printf("connect() failed\n");

    return sock;
}

int send_file(char* fileDir, char* filename)
{
    char* packet;
    char pre_body[1024], post_body[1024];
    char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
    char boundary[] = "----WebKitFormBoundaryu8FzpUGNDgydoA4z";

    char* bodyline = calloc(sizeof(char), DATA_SIZE);
    char* fileBuf = calloc(sizeof(char), DATA_SIZE);
    int sock = makeSocket();

    printf("send file name : %s\n", filename);

    FILE* fp = fopen(fileDir, "r");
    if (fp == NULL) {
        printf("file can't read\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (fread(fileBuf, 1, file_size, fp) == -1) {
        printf("fread error\n");
    }

    sprintf(pre_body, "--%s\r\nContent-Disposition: form-data; name=\"pdata\"\r\n\r\n"
        "%s\r\n--%s\r\nContent-Disposition: form-data; name=\"flag\"\r\n\r\n"
        "%s\r\n--%s\r\nContent-Disposition: form-data; name=\"upfile\";filename=\"%s\"\r\n"
        "Content-Type:application/octect-stream\r\n\r\n"
        , boundary, "postdata", boundary, "no", boundary, filename);

    sprintf(post_body, "\r\n--%s--\r\n", boundary);

    int pre_len = strlen(pre_body);
    int post_len = strlen(post_body);
    int body_len = pre_len + file_size + post_len;

    memcpy(bodyline, pre_body, pre_len);
    memcpy(bodyline + pre_len, fileBuf, file_size);
    memcpy(bodyline + pre_len + file_size, post_body, post_len);

    sprintf(sendline, "POST /upload.php HTTP/1.1\r\n"
        "Host: %s\r\nConnection: keep-alive\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: multipart/form-data; boundary=%s\r\n\r\n", servIP, body_len, boundary);
    int head_len = strlen(sendline);
    int packet_len = head_len + body_len;

    packet = calloc(1, head_len + body_len + 1);
    memcpy(packet, sendline, head_len);
    memcpy(packet + head_len, bodyline, body_len);

    send(sock, packet, packet_len, 0);

    int n;
    while ((n = recv(sock, recvline, MAXLINE, 0)) > 0) {
        recvline[n] = '\0';
        printf("%s", recvline);
    }

    fclose(fp);
    free(packet);
    closesocket(sock);
    WSACleanup();
    return 0;
}

int main(int argc, char* argv[]) {
    char* fdir = "";    // 절대경로, 예) /home/user/path/filename.txt
    char* fname = "";   // 파일이름.확장자, 예) filename.txt
    send_file(fdir, fname);
    return 0;
}
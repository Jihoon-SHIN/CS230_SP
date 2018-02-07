#include "csapp.h"
#include <stdio.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (x11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

// Function prototypes
void doit(int fd_to_client);
int uri_parse(char *uri, char *target_addr, char *path, int *port);


/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen,flag=1;
    struct sockaddr_in clientaddr;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    if (port < 1024 || port > 65536) {
        fprintf(stderr, "Unable: port number '%d', must be 1024~65536\n", port);
        exit(1);
    }
    if ((listenfd = Open_listenfd(argv[1])) < 0) {
        fprintf(stderr, "Unable: open_listenfd(%d)\n", port);
        exit(1);
    }

    while (flag) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
		if(fork()==0){
        	doit(connfd);
        	Close(connfd);
		}
    }
	printf("%s", user_agent_hdr);
    return 0;
}

void doit(int fd_to_client) {
    int port = 0,flag=1;
    int fd2server;
    int bodyLength = 0, length;
    char buf[MAXLINE], request[MAXLINE], response[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], hostname[MAXLINE], pathname[MAXLINE];
    char error[MAXLINE];

    rio_t rio_client, rio_server;
    Rio_readinitb(&rio_client, fd_to_client);
    Rio_readlineb(&rio_client, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);

    if (uri_parse(uri, hostname, pathname, &port) == -1) {
        sprintf(error, "Wrong hostname : %s\r\n", hostname);
        Rio_writen(fd_to_client, error, strlen(error));
        return;
    }
    strcpy(request, method);
    strcat(request, " ");
    strcat(request, pathname);
    strcat(request, " ");
    strcat(request, version);
    strcat(request, "\r\n");

    while (flag) {
		if(strcmp(buf,"\r\n") !=0){
        	Rio_readlineb(&rio_client, buf, MAXLINE);
        	strcat(request, buf);
		}else break;
        
    }

	char temp[3];
	sprintf(temp,"%d",port);
    fd2server = Open_clientfd(hostname, temp);

    Rio_writen(fd2server, request, strlen(request));
    Rio_readinitb(&rio_server, fd2server);
    Rio_readlineb(&rio_server, response, MAXLINE);

    while (flag) {
        Rio_readlineb(&rio_server, buf, MAXLINE);
        strcat(response, buf);
		if(strcmp(buf,"\r\n") == 0) break;
    }

    Rio_writen(fd_to_client, response, strlen(response));

	while((length=Rio_readnb(&rio_server,buf,MAXLINE))>0 && bodyLength<=0){
		Rio_writen(fd_to_client,buf,length);
	}
	while(bodyLength>MAXLINE && bodyLength>0){
		bodyLength -= MAXLINE;
		Rio_readnb(&rio_server, buf, MAXLINE);
		Rio_writen(fd_to_client, buf, MAXLINE);
	}
    if(bodyLength !=0 || bodyLength>0 ){
        Rio_readnb(&rio_server,buf,bodyLength);
        Rio_writen(fd_to_client, buf, bodyLength);
    }

    return;
}


int uri_parse(char *uri, char *target_addr, char *path, int *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
   		target_addr[0] = '\0';
   		return -1;
    }

	hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    len = hostend - hostbegin;
    strncpy(target_addr, hostbegin, len);
    target_addr[len] = '\0';


    *port = 80;
    if (*hostend == ':')
        *port = atoi(hostend + 1);

    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
	   	path[0] = '\0';
    }
    else {
   		strcpy(path, pathbegin);
    }
    return 0;
}



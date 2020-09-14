#include <arpa/inet.h>          /* inet_ntoa */
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "http_handler/http_handler.h"

const int MAXLINE = 1024;   /* max length of a line */
const int RIO_BUFSIZE = 1024;
const int MAX_FD =  65536;


int main(int argc , char** argv){
	struct sockaddr_in clientaddr;
	int default_port = 9999,listenfd,connfd;
	char buf[256];
	//copy current work path to buf;
	char *path = getcwd(buf,256);

	//handle the input parameter
	socklen_t clientlen = sizeof(clientaddr);

	//judge the parameter
	if(argc == 2){
		if(argv[1][0] >= '0' && argv[1][0] <= '9'){
			default_port = atoi(argv[1]);
		}else{
			path  = argv[1];
			if(chdir(argv[1]) != 0){
				perror(argv[1]);
				exit(1);
			}
		}
	}
	listenfd = open_listenfd(default_port);

	// Ignore SIGPIPE signal, so if browser cancels the request, it
	// won't kill the whole process.
	signal(SIGPIPE, SIG_IGN);

	http_handler *users = new http_handler[MAX_FD];
	assert(users);
	
	for(int i = 0;i < 10;i ++){
		int pid = fork();
		if(pid == 0){   //child fd and this progress is blocked
			while(1){
				connfd = Accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
				users[connfd].init(connfd, clientaddr);
				users[connfd].process();
				Close(connfd);		
			}
		}else if(pid > 0){
			printf("child pid is %d\n",pid);		
		}else{
			perror("fork");		
		}
	}
	wait(NULL);
	return 0;
}

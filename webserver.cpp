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

#include <memory>
#include "CGImysql/sql_connection.h"
#include "http_handler/http_handler.h"


#define NODEBUG 0

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
	
	#ifdef NODEBUG
	    for(int i = 0;i < 2;i++){
        	close(i);
    	    }
	    //dev/null => 0
    	    open("/dev/null",O_RDWR);
    	    //dev/null => 1
    	    open("/dev/null",O_RDWR);
	#endif


	for(int i = 0;i < 100;i ++){
		int pid = fork();
		if(pid == 0){   //child fd and this progress is blocked
			//sql_connection *conn = new sql_connection();
			//use shared_ptr
			std::shared_ptr<sql_connection> conn =
 			std::shared_ptr<sql_connection>(std::make_shared<sql_connection>());
			conn->init("localhost", "root", "888888", "ttWebserver", 3306,1);
			while(1){
				connfd = Accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
				users[connfd].init(connfd, clientaddr);
				users[connfd].initmysql(conn);
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

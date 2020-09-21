#include "http_handler.h"

const int MAXLINE = 1024;
static const char *http_handler::default_mime_type = "text/html";

void http_handler::init(int sockfd,const sockaddr_in &addr){
	offset = 0;
	end = 0;
	m_content_length = 0;

	m_sockfd = sockfd;
	m_address = addr;
	m_method = GET;
	memset(m_read_buf, '\0', READ_BUFFER_SIZE);
	
	strcpy(m_filename,"root");
	filename_offset = strlen(m_filename);

	cgi = 0;
}

void http_handler::initmysql(std::shared_ptr<sql_connection> _mysql){
    mmysql = _mysql;
/*
    if (mysql_query(mmysql->GetConnection(), "SELECT username,passwd FROM user"))
    {
        printf("SELECT error:%s\n", mysql_error(mmysql->GetConnection()));
    }
*/
}

void http_handler::url_decode(char* src, char* dest, int max) {
    char *p = src;
    char code[3] = { 0 };
    while(*p && --max) {
        if(*p == '%') {
            memcpy(code, ++p, 2);
            *dest++ = (char)strtoul(code, NULL, 16);
            p += 2;
        } else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
}


http_handler::HTTP_CODE http_handler::process_read(){
	char* tmp = m_read_buf;
/*prase_requstline*/
	Readline(m_sockfd,m_read_buf,MAXLINE);
	printf("data is %s\n",m_read_buf);
	char* uri = strpbrk(tmp," \t");
	if(!uri){
		return BAD_REQUEST;	
	}
	//find uri	
	*uri ++ = '\0';
	char *s_method = tmp;
	if(strcasecmp(s_method,"GET") == 0){
		m_method = GET;
		printf("The request method is GET!");
	}else if (strcasecmp(s_method, "POST") == 0){
        	m_method = POST;
        	cgi = 1;
        }
	else{
		return BAD_REQUEST;	
	}

	uri += strspn(uri," \t");
	char *version = strpbrk(uri," \t");
	if(!version){
		return BAD_REQUEST;
	}
	*version ++ = '\0';
	//version is not cared
	


	//handle uri
	char* filename = uri;
	if(uri[0] == '/'){
		//filename = uri + 1;
		int length = strlen(filename);
		if (length == 0){
			filename = ".";
		} else {
			for (int i = 0; i < length; ++ i) {
				if (filename[i] == '?') {
					filename[i] = '\0';
					break;
				}
			}
		}
    	}
	url_decode(filename,m_filename + strlen(m_filename), MAXLINE);

	/*prase_headers*/
	// \n || \r\n
	while(m_read_buf[0] != '\n' && m_read_buf[1] != '\n'){
		int len = Readline(m_sockfd,m_read_buf,MAXLINE);
		char *text = m_read_buf;
        	if(strncasecmp(text, "Range:", 6) == 0){
            		sscanf(text, "Range: bytes=%lu-%lu", &offset, &end);
            		// Range: [start, end]
            		if(end != 0) end ++;
        	}else if(strncasecmp(text, "Content-length:", 15) == 0){// important ==> lens
			text[len] = '\0';
			text += 15;
			text += strspn(text, " \t");
			m_content_length = atol(text);
		}
	}
	
	/*prase_content*/
	if(m_content_length > 0){
		int len = Readn(m_sockfd,m_read_buf,m_content_length);
		m_read_buf[len] = '\0';
		printf("content is %s", m_read_buf);
	}

}


void http_handler::log_access(int status){
    printf("%s:%d %d - %s\n", inet_ntoa(m_address.sin_addr),
           ntohs(m_address.sin_port), status, m_filename);
}


const char* http_handler::get_mime_type(char *filename){
    char *dot = strrchr(filename, '.');
    if(dot){
	std::string tmp(dot,0,strlen(dot));
        if(meme_types.find(tmp) != meme_types.end()){
	    return meme_types[tmp].c_str();
	}
    }
    return default_mime_type;

}


void http_handler::serve_static(int out_fd, int in_fd,size_t total_size){
	char buf[256];
	if (offset > 0){
        	sprintf(buf, "HTTP/1.1 206 Partial\r\n");
        	sprintf(buf + strlen(buf), "Content-Range: bytes %lu-%lu/%lu\r\n",
                offset, end, total_size);
	} else {
        	sprintf(buf, "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\n");
	}
	
	sprintf(buf + strlen(buf), "Cache-Control: no-cache\r\n");
	sprintf(buf + strlen(buf), "Content-length: %lu\r\n",end - offset);
	sprintf(buf + strlen(buf), "Content-type: %s\r\n\r\n",get_mime_type(m_filename));

	Writen(out_fd, buf, strlen(buf));
	off_t t_offset = offset; /* copy */

	while(t_offset < end){
	    if(sendfile(out_fd, in_fd, &t_offset, end - offset) <= 0){
                break;
            }
            printf("offset: %d \n\n", t_offset);
            //Close(out_fd);
            break;
        }
}


void http_handler::process(){
	printf("accept request, fd is %d, pid is %d\n", m_sockfd, getpid());
	process_read();
	
	struct stat sbuf;
	    if(strlen(m_filename + filename_offset) == 1){
                strcat(m_filename, "index.html");
	    }else if(m_filename[filename_offset + 1] == '0'){
	        strcpy(m_filename + filename_offset, "/register.html");
	    }else if(m_filename[filename_offset + 1] == '1'){
	        strcpy(m_filename + filename_offset, "/log.html");
	    }else if(m_filename[filename_offset + 1] == '2'){
	    	strcpy(m_filename + filename_offset, "/judge.html");
	    }
	//cgi
	   

	   if(cgi == 1){
		if(m_filename[filename_offset + 1] == '3' || m_filename[filename_offset + 1] == '4'){
	        //code //user=123&passwd=123
                char name[100], password[100];
                int i;
                for (i = 5; m_read_buf[i] != '&'; ++i)
                name[i - 5] = m_read_buf[i];
                name[i - 5] = '\0';

                int j = 0;
                for (i = i + 10; m_read_buf[i] != '\0'; ++i, ++j)
                    password[j] = m_read_buf[i];
                password[j] = '\0';

		printf("\n name is %s\n password is %s",name,password);

		char *sql_insert = (char *)malloc(sizeof(char) * 200);
		    
               if (m_filename[filename_offset + 1] == '3')
               {
                strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
                strcat(sql_insert, "'");
                strcat(sql_insert, name);
    
                strcat(sql_insert, "', '");
                strcat(sql_insert, password);
                strcat(sql_insert, "')");
    
                   if (users.find(name) == users.end())
                   {
                       int res = mysql_query(mmysql->GetConnection(), sql_insert);
                       users.insert(make_pair(name, password));
    
                       if (!res)
		   	   strcpy(m_filename + filename_offset, "/log.html");
                       else
			   strcpy(m_filename + filename_offset, "/registerError.html");
                   }
                   else
                       strcpy(m_filename + filename_offset, "/registerError.html");
    		
                }else if (m_filename[filename_offset + 1] == '4'){
                    strcpy(sql_insert, "SELECT passwd FROM user where name = ");
		    strcat(sql_insert, name);
		    strcat(sql_insert, " limit 1;");


		
		    if (mysql_query(mmysql->GetConnection(), "SELECT username,passwd FROM user"))
    		    {
        		printf("SELECT error:%s\n", mysql_error(mmysql->GetConnection()));
    		    }

     		    MYSQL_RES *result = mysql_store_result(mmysql->GetConnection());

    		    if(MYSQL_ROW row = mysql_fetch_row(result)){
                        if (strcmp(row[0],name) == 0 && strcmp(row[1],password) == 0)
			    strcpy(m_filename + filename_offset, "/index.html");
                        else
		            strcpy(m_filename + filename_offset, "/logError.html");
		    }else
		        strcpy(m_filename + filename_offset, "/logError.html");
		
	
		}
	    	free(sql_insert);
	    }

      	}

	printf("\n filename is %s\n",m_filename);

	int status = 200, ffd = open(m_filename, O_RDONLY, 0);
	    if(ffd <= 0){
		status = 404;
		char *msg = "File not found";
		client_error(status, "Not found", msg);
	    } else {
		fstat(ffd, &sbuf);
        	if(S_ISREG(sbuf.st_mode)){
			if (end == 0){
                		end = sbuf.st_size;
            		}
            		if (offset > 0){
               			status = 206;
            		}
			serve_static(m_sockfd,ffd,sbuf.st_size);
            	}else if(S_ISDIR(sbuf.st_mode)){
			printf("visit dir\n");
		}else{
			status = 400;
			char *msg = "Unknow Error";
			client_error(status, "Error", msg);
            	}
		close(ffd);
	    }
	log_access(status);
}

void http_handler::client_error(int status, char *msg, char *longmsg){
    char buf[MAXLINE];
    sprintf(buf, "HTTP/1.1 %d %s\r\n", status, msg);
    sprintf(buf + strlen(buf),
            "Content-length: %lu\r\n\r\n", strlen(longmsg));
    sprintf(buf + strlen(buf), "%s", longmsg);
    Writen(m_sockfd, buf, strlen(buf));
}











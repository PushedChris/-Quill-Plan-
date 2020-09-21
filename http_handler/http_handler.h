#ifndef HTTPHAND_H
#define HTTPHAND_H
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/sendfile.h>

#include <mysql/mysql.h>
#include <unordered_map>
#include <map>
#include <string.h>
#include <memory>

#include "../socket_wrap/wrap.h"
#include "../CGImysql/sql_connection.h"

//static comprenhension >>> ?????


static const std::unordered_map<std::string,std::string> meme_types{
        {".css", "text/css"},
        {".gif", "image/gif"},
        {".htm", "text/html"},
        {".html", "text/html"},
        {".jpeg", "image/jpeg"},
        {".jpg", "image/jpeg"},
        {".ico", "image/x-icon"},
        {".js", "application/javascript"},
        {".pdf", "application/pdf"},
        {".mp4", "video/mp4"},
        {".png", "image/png"},
        {".svg", "image/svg+xml"},
        {".xml", "text/xml"}
};

class http_handler
{
public:
    static const int FILENAME_LEN = 512;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    static const char *default_mime_type;

    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
public:
    http_handler() {}
    ~http_handler() {}

public:
    void init(int sockfd, const sockaddr_in &addr);
    void initmysql(std::shared_ptr<sql_connection> _mysql);

    void process();
private:
    HTTP_CODE process_read();
    void url_decode(char* src, char* dest, int max);
    void log_access(int status);
    const char* get_mime_type(char *filename);
    void serve_static(int out_fd, int in_fd,size_t total_size);
    void client_error(int status, char *msg, char *longmsg);

    //only init once for saving time
    //static const std::unordered_map<std::string,std::string> meme_types;

private:
    int m_sockfd;
    sockaddr_in m_address;
    char m_read_buf[READ_BUFFER_SIZE];
    METHOD m_method;
    std::map<std::string, std::string> users;

    //use cgi
    int cgi;    
    std::shared_ptr<sql_connection> mmysql;

    char m_filename[FILENAME_LEN];
    
    int filename_offset;
    int m_content_length;
    //for support Range
    off_t offset;        
    size_t end;
};

#endif


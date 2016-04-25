#ifndef NETWORK_H_
#define NETWORK_H_

#define BYTE_NORMAL           0xff
#define BYTE_SPECIAL          0xfe
#define BYTE_OPEN             0x00
#define BYTE_READ             0x01
#define BYTE_CLOSE            0x02
#define BYTE_OK               0x03
#define BYTE_SETPOS           0x04
#define BYTE_STATFILE         0x05
#define BYTE_EOF              0x06
#define BYTE_GETPOS           0x07
#define BYTE_REQUEST          0x08
#define BYTE_REQUEST_SLOW     0x09
#define BYTE_HANDLE        	  0x0A
#define BYTE_DUMP          	  0x0B
#define BYTE_PING          	  0x0C
#define BYTE_G_MODE           0x0D
#define BYTE_MODE_D           0x0E
#define BYTE_MODE_I           0x0F
#define BYTE_CLOSE_DUMP       0x10
#define BYTE_LOG_STR          0xfb
#define BYTE_FILE          	  0xC0
#define BYTE_FOLDER           0xC1
#define BYTE_READ_DIR         0xCC
#define BYTE_INJECTSTART      0x40
#define BYTE_INJECTEND        0x41
#define BYTE_DUMPSTART        0x42
#define BYTE_DUMPEND          0x43
#define BYTE_END              0xfd

#define MASK_NORMAL           0x8000
#define MASK_USER             0x0100
#define MASK_COMMON           0x0200
#define MASK_COMMON_CLEAN     0x0400

void cafiine_connect(int *psock,u32 ip);
void cafiine_disconnect(int sock);
int cafiine_handshake(int sock);
int getMode(int sock,int * result);
int cafiine_fsetpos(int sock, int *result, int fd, int set);
int cafiine_send_handle(int sock, const char *path, int handle);
int cafiine_fopen(int sock, int *result, const char *path, const char *mode, int *handle);
void cafiine_send_file(int sock, char *file, int size, int fd);
int cafiine_fread(int sock, int *result, void *ptr, int size, int fd);
int cafiine_fclose(int sock, int *result, int fd,int dumpclose);
int saviine_start_injection(int sock, long persistentID,int * mask);
int saviine_end_injection(int sock);
int saviine_start_dump(int sock, long persistentID,int * mask);
int saviine_end_dump(int sock);
int saviine_readdir(int sock, char * path,char * resultname, int * resulttype, int * filesize);
void cafiine_send_ping(int sock, int val1, int val2);

 int recvwaitlen(int sock, void *buffer, int len);
 int recvbyte(int sock);
void log_string(int sock, const char* str, char flag_byte);
int sendbyte(int sock, unsigned char byte);

int server_connect(int *psock, unsigned int server_ip);
int recvwait(int sock, void *buffer, int len);
int sendwait(int sock, const void *buffer, int len);

#endif

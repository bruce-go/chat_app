#ifndef RDWRT_LIB_H
#define RDWRT_LIB_H


int readOnce(int fd, void *buf, unsigned int len);

int readn(int fd, void *buf, unsigned int len);

int writen(int fd, void *buf, unsigned int len);



#endif
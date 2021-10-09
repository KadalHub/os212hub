/*
 * Copyright (C) 2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: FORK
 * REV01: Sat 09 Oct 2021 20:28:04 WIB
 * START: Sat 03 Apr 2021 06:20:00 WIB
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE1        128
#define BUFSIZE2        256
#define BUFSIZE3        1024
typedef char*           String;
typedef char            Buf;
typedef int*            IntPtr;
typedef unsigned long   UL;
typedef void*           AnyAddrPtr;
typedef struct sysinfo  SYSINFO;

#define GETDATE "date +%s"
Buf    bufEpoch[BUFSIZE1];
String getEpoch(void) {
    FILE* filePtr = popen(GETDATE, "r");
    UL    tmpLong = atol(fgets(bufEpoch, BUFSIZE1, filePtr));
    pclose (filePtr);
    sprintf(bufEpoch, "%16.16lX", tmpLong);
    return (bufEpoch+8);
}

Buf    bufHostName[BUFSIZE1];
String getHost(void) {
    if (gethostname(bufHostName,BUFSIZE1)) strcpy(bufHostName,"HostNameError");
    return bufHostName;
}

String getUserName(void) {
    String userStr=getlogin();
    if   (userStr==NULL) userStr="UserNameError";
    return userStr;
}

#define DOSHASUM   "echo %s|sha1sum|tr '[a-z]' '[A-Z]'| cut -c1-8"
#define RESULT     8
Buf    bufSTAMP[BUFSIZE3];
String getStamp(void) {
    Buf    tmpSTR[BUFSIZE1];
    strcpy(tmpSTR,"XXXXXXXX");
    String tmpEpoch=getEpoch();
    if (!strcmp(getHost(), getUserName())) {
        strcpy(tmpSTR,getUserName());
        strcat(tmpSTR,tmpEpoch);
        Buf  tmpCMD[BUFSIZE2];
        sprintf(tmpCMD, DOSHASUM, tmpSTR);
        FILE* filePtr = popen(tmpCMD, "r");
        fgets(tmpSTR, RESULT+1, filePtr);
        tmpSTR[RESULT]=0;
        pclose(filePtr);
    }
    bufSTAMP[0]=0;
    strcpy(bufSTAMP,tmpEpoch);
    strcat(bufSTAMP," ");
    strcat(bufSTAMP,tmpSTR);
    strcat(bufSTAMP," ");
    return bufSTAMP;
}

int pcounter=1;
void printMyAddress (AnyAddrPtr address, String message) {
    printf("ZCZC ADDR %2.2d %#16.16lX %s\n", pcounter, (UL) address, message);
}

void report(IntPtr share) {
    sleep(1);
    printf("ZCZC NO %2d",   (*share)++);
    printf(" PID %5ld",     getpid());
    printf(" PPID %5ld",    getppid());
    printf(" HOST %s",      getHost());
    printf(" STAMP %s\n",   getStamp());
    sleep(1);
    fflush(NULL);
}

#define CHMOD          0666
#define MYFLAGS        O_CREAT|O_RDWR
#define MYPROTECTION   PROT_READ|PROT_WRITE
#define MYVISIBILITY   MAP_SHARED
#define SHAREFILE      "WEEK06-SHARE.bin"

int main(void) {
    int       fd  = open(SHAREFILE, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, sizeof(int));
    IntPtr sharePtr = mmap(NULL, sizeof(int), MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    *sharePtr=1;
    fork();
    fork();
    fork();
    report(sharePtr);
    report(sharePtr);
}


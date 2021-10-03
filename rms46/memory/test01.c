/*
 * Copyright (C) 2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: MYADDDRESSES
 * REV02: Sun 03 Oct 2021 04:22:27 WIB
 * REV01: Sat 02 Oct 2021 18:26:34 WIB
 * START: Sat 03 Apr 2021 06:20:43 WIB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#define BUFFERSIZE      256
typedef struct sysinfo* SYSINFO;
typedef char*           String;
typedef char            Buf;
typedef unsigned long   UL;
typedef void*           AnyAddrPtr;

#define GETDATE "date +%s"
Buf    bufEpoch[BUFFERSIZE];
String getEpoch(void) {
    FILE* filePtr = popen(GETDATE, "r");
    UL    tmpLong = atol(fgets(bufEpoch, BUFFERSIZE, filePtr));
    pclose (filePtr);
    sprintf(bufEpoch, "%16.16lX", tmpLong);
    return (bufEpoch+8);
}

Buf    bufHostName[BUFFERSIZE];
String getHostName(void) {
    if (gethostname(bufHostName,BUFFERSIZE)) strcpy(bufHostName,"HostNameError");
    return bufHostName;
}

String getUserName(void) {
    String userStr=getlogin();
    if   (userStr==NULL) userStr="UserNameError";
    return userStr;
}

#define DOSHASUM   "echo %s|sha1sum|tr '[a-z]' '[A-Z]'| cut -c1-8"
#define RESULT     8
Buf    bufSTAMP[BUFFERSIZE];
String getStamp(void) {
    Buf    tmpSTR[BUFFERSIZE];
    strcpy(tmpSTR,"XXXXXXXX");
    String tmpEpoch=getEpoch();
    if (!strcmp(getHostName(), getUserName())) {
        strcpy(tmpSTR,getUserName());
        strcat(tmpSTR,tmpEpoch);
        Buf  tmpCMD[BUFFERSIZE];
        sprintf(tmpCMD, DOSHASUM, tmpSTR);
        FILE* filePtr = popen(tmpCMD, "r");
        fgets(tmpSTR, RESULT+1, filePtr);
        tmpSTR[RESULT]=0;
        pclose(filePtr);
    }
    bufSTAMP[0]=0;
    strcpy(bufSTAMP,"ZCZC STAMP ");
    strcat(bufSTAMP,tmpEpoch);
    strcat(bufSTAMP," ");
    strcat(bufSTAMP,tmpSTR);
    strcat(bufSTAMP," ");
    return bufSTAMP;
}

int pcounter=1;
void printMyAddress (AnyAddrPtr address, String message) {
    printf("ZCZC ADDR %2.2d %#16.16lX %s\n", pcounter, address, message);
}

void myinfo(SYSINFO info) {
    printf("ZCZC HOST %s\n",  getHostName());
    printf("ZCZC USER %s\n",  getUserName());
    printf("ZCZC RAM  %lu MB\n", (info->totalram/1024/1024));
    printf("ZCZC FREE %lu MB\n", (info->freeram/1024/1024));
}

int main(void) {
    int  localdummy=0;
    printf("%s\n", getStamp());
    SYSINFO guestInfo;
    sysinfo(guestInfo);
    myinfo(guestInfo);
    printMyAddress(&guestInfo,     "&guestInfo");
    printMyAddress(&localdummy,    "&localdummy");
    printMyAddress(&pcounter,      "&pcounter");
    printMyAddress(bufEpoch,       "bufEpoch");
    printMyAddress(bufHostName,    "bufHostName");
    printMyAddress(bufSTAMP,       "bufSTAMP");
    printMyAddress(getEpoch,       "getEpoch()");
    printMyAddress(getHostName,    "getHostName()");
    printMyAddress(getStamp,       "getStamp()");
    printMyAddress(getUserName,    "getUserName()");
    printMyAddress(main,           "main()");
    printMyAddress(myinfo,         "myinfo()");
    printMyAddress(printf,         "printf()");
    printMyAddress(printMyAddress, "printMyAddress()");
    sleep(1);
    printf("%s\n", getStamp());
}


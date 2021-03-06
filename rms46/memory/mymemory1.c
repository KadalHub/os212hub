/*
 * Copyright (C) 2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: MYADDDRESSES
 * REV05: Mon 04 Oct 2021 16:58:33 WIB
 * REV04: Sun 03 Oct 2021 18:20:00 WIB
 * REV03: Sun 03 Oct 2021 16:10:00 WIB
 * REV02: Sun 03 Oct 2021 04:22:00 WIB
 * REV01: Sat 02 Oct 2021 18:26:00 WIB
 * START: Sat 03 Apr 2021 06:20:00 WIB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

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
    strcpy(bufSTAMP,"ZCZC STAMP ");
    strcat(bufSTAMP,tmpEpoch);
    strcat(bufSTAMP," ");
    strcat(bufSTAMP,tmpSTR);
    strcat(bufSTAMP," ");
    return bufSTAMP;
}

int pcounter=1;
void printMyAddress (AnyAddrPtr address, String message) {
    printf("ZCZC ADDR %2.2d %#16.16lX %s\n", pcounter, (UL) address, message);
}

#define ArraySize   1024*1024
int main(void) {
    SYSINFO guestInfo;
    int     localdummy=0;
    IntPtr  intArray=malloc((ArraySize+1) * sizeof(int));

    printf("%s\n", getStamp());
    for     (int ii=0; ii<ArraySize; ii++) *intArray=255;
    sysinfo(&guestInfo);
    printf("ZCZC HOST   %s\n",       getHost());
    printf("ZCZC USER   %s\n",       getUserName());
    printf("ZCZC RAM    %5lu MB\n",  guestInfo.totalram/1024/1024);
    printf("ZCZC FREE   %5lu MB\n",  guestInfo.freeram/1024/1024);
    printf("ZCZC BUFFER %5lu MB\n",  guestInfo.bufferram/1024/1024);
    printf("ZCZC SWAP   %5lu MB\n",  guestInfo.totalswap/1024/1024);
    printf("ZCZC FREESW %5lu MB\n",  guestInfo.freeswap/1024/1024);
    printMyAddress(&guestInfo,     "&guestInfo");
    printMyAddress(&intArray,      "&intArray");
    printMyAddress(&localdummy,    "&localdummy");
    printMyAddress(&pcounter,      "&pcounter");
    printMyAddress( atol,           "atol()");
    printMyAddress( bufEpoch,       "bufEpoch");
    printMyAddress( bufHostName,    "bufHostName");
    printMyAddress( bufSTAMP,       "bufSTAMP");
    printMyAddress( intArray,       "intArray");
    printMyAddress( fgets,          "fgets()");
    printMyAddress( getEpoch,       "getEpoch()");
    printMyAddress( getHost,        "getHost()");
    printMyAddress( gethostname,    "gethostname()");
    printMyAddress( getlogin,       "getlogin()");
    printMyAddress( getStamp,       "getStamp()");
    printMyAddress( getUserName,    "getUserName()");
    printMyAddress( main,           "main()");
    printMyAddress( malloc,         "malloc()");
    printMyAddress( pclose,         "pclose()");
    printMyAddress( popen,          "popen()");
    printMyAddress( printf,         "printf()");
    printMyAddress( printMyAddress, "printMyAddress()");
    printMyAddress( sprintf,        "sprintf()");
    printMyAddress( strcat,         "strcat()");
    printMyAddress( strcmp,         "strcmp()");
    printMyAddress( strcpy,         "strcpy()");
    printMyAddress( sysinfo,        "sysinfo()");
    sleep(1);
    printf("%s\n", getStamp());
}


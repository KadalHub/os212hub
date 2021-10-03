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
typedef char*           String;
typedef char            Buf;
typedef int*            IntPtr;
typedef unsigned long   UL;
typedef void*           AnyAddrPtr;
typedef struct sysinfo  SYSINFO;

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
    printf("ZCZC ADDR %2.2d %#16.16lX %s\n", pcounter++, address, message);
}

#define ArraySize   1024*1024
int main(void) {
    SYSINFO guestInfo;
    int     localdummy=0;
    IntPtr  intArray=malloc((ArraySize+1) * sizeof(int));

    printf("%s\n", getStamp());
    for     (int ii=0; ii<ArraySize; ii++) *intArray=255;
    sysinfo(&guestInfo);
    printf("ZCZC HOST   %s\n",       getHostName());
    printf("ZCZC USER   %s\n",       getUserName());
    printf("ZCZC RAM    %5lu MB\n",  guestInfo.totalram/1024/1024);
    printf("ZCZC FREE   %5lu MB\n",  guestInfo.freeram/1024/1024);
    printf("ZCZC BUFFER %5lu MB\n",  guestInfo.bufferram/1024/1024);
    printf("ZCZC SWAP   %5lu MB\n",  guestInfo.totalswap/1024/1024);
    printf("ZCZC FREESW %5lu MB\n",  guestInfo.freeswap/1024/1024);
    printMyAddress( getEpoch,       " getEpoch()");
    printMyAddress( getHostName,    " getHostName()");
    printMyAddress( getUserName,    " getUserName()");
    printMyAddress( getStamp,       " getStamp()");
    printMyAddress( printMyAddress, " printMyAddress()");
    printMyAddress( main,           " main()");
    printMyAddress(&pcounter,       "&pcounter");
    printMyAddress( bufEpoch,       " bufEpoch");
    printMyAddress( bufHostName,    " bufHostName");
    printMyAddress( bufSTAMP,       " bufSTAMP");
    printMyAddress( intArray,       " intArray");
    printMyAddress( atol,           " atol()");
    printMyAddress( printf,         " printf()");
    printMyAddress( sprintf,        " sprintf()");
    printMyAddress( fgets,          " fgets()");
    printMyAddress( popen,          " popen()");
    printMyAddress( pclose,         " pclose()");
    printMyAddress( gethostname,    " gethostname()");
    printMyAddress( getlogin,       " getlogin()");
    printMyAddress( strcmp,         " strcmp()");
    printMyAddress( strcat,         " strcat()");
    printMyAddress( strcpy,         " strcpy()");
    printMyAddress(&intArray,       "&intArray");
    printMyAddress(&localdummy,     "&localdummy");
    printMyAddress(&guestInfo,      "&guestInfo");
    sleep(1);
    printf("%s\n", getStamp());
}


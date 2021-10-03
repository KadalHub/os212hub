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

#define BUFFERSIZE 256
typedef struct     sysinfo* SYSINFO;
typedef char*      String;
typedef char       Chr;
typedef unsigned   long UL;
typedef void*      AnyAddrPtr;


#define GETDATE    "date +%s"
Chr gEpoch[BUFFERSIZE];
String getEpoch(void) {
    FILE* filePtr = popen(GETDATE, "r");
    UL    tmpLong = atol(fgets(gEpoch, BUFFERSIZE, filePtr));
    pclose (filePtr);
    sprintf(gEpoch, "%16.16lX", tmpLong);
    return (gEpoch+8);
}

Chr gHostName[BUFFERSIZE];
String getHostName(void) {
    if (gethostname(gHostName,BUFFERSIZE)) strcpy(gHostName,"HostNameError");
    return gHostName;
}

String getUserName(void) {
    String userStr=getlogin();
    if   (userStr==NULL) userStr="UserNameError";
    return userStr;
}

#define DOSHASUM   "echo %s|sha1sum|tr '[a-z]' '[A-Z]'| cut -c1-8"
#define RESULT     8
Chr gSTAMP[BUFFERSIZE];
String setStamp(void) {
    Chr    tmpSTR[BUFFERSIZE];
    strcpy(tmpSTR,"XXXXXXXX");
    String tmpEpoch=getEpoch();
    if (!strcmp(getHostName(), getUserName())) {
        strcpy(tmpSTR,getUserName());
        strcat(tmpSTR,tmpEpoch);
        Chr  tmpCMD[BUFFERSIZE];
        sprintf(tmpCMD, DOSHASUM, tmpSTR);
        FILE* filePtr = popen(tmpCMD, "r");
        fgets(tmpSTR, RESULT+1, filePtr);
        tmpSTR[RESULT]=0;
        pclose(filePtr);
    }
    gSTAMP[0]=0;
    strcpy(gSTAMP,"ZCZC STAMP ");
    strcat(gSTAMP,tmpEpoch);
    strcat(gSTAMP," ");
    strcat(gSTAMP,tmpSTR);
    strcat(gSTAMP," ");
    return gSTAMP;
}

int pcounter=1;
void printMyAddress (AnyAddrPtr address, String message) {
    printf("ZCZC ADDR %2.2d %#16.16X %s\n", pcounter, address, message);
}

int main(void) {
    printf("%s\n", setStamp());
    printMyAddress(&pcounter,      "&pcounter");
    printMyAddress(main,           "main()");
    printMyAddress(getEpoch,       "getEpoch()");
    printMyAddress(getHostName,    "getHostName()");
    printMyAddress(getUserName,    "getUserName()");
    printMyAddress(setStamp,       "setStamp()");
    printMyAddress(printf,         "printf");
    printMyAddress(printMyAddress, "printMyAddress");
    printMyAddress(gSTAMP,         "gSTAMP");
    printMyAddress(gEpoch,         "gEpoch");
    printMyAddress(gHostName,      "gHostName");
    sleep(1);
    printf("%s\n", setStamp());
}


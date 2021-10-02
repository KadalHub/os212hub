/*
 * Copyright (C) 2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: MYADDDRESSES
 * REV01: Sat 02 Oct 2021 18:26:34 WIB
 * START: Sat 03 Apr 2021 06:20:43 WIB
 */

#include <stdio.h>
#include <stdlib.h>

typedef void* AnyAddrPtr;
typedef char* ChrPtr;
typedef char  Chr;

void printMyAddress (AnyAddrPtr address, ChrPtr message) {
    printf("[%#16.16X] %s\n", address, message);
}

int main(void) {
    printMyAddress(&main,                    "main");
    printMyAddress(&printf,                  "printf");
    printMyAddress(&printMyAddress,          "printMyAddress");
}


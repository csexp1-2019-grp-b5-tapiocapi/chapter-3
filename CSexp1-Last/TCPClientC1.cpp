/*
   ============================================================================
Name        : TCPClientC1.c
Author      : Yasuhiro Noguchi
Version     : 0.0.1
Copyright   : Copyright (C) HEPT Consortium
Description : Simple TCP Client, only 1 connection
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "libclient.h"

#define HOSTNAME "192.168.0.30"
#define PORTNUM "10000"

bool start(const char* hostName, const char* portNum, const char* input) {

    int csocket;

    // サーバにソケット接続
    if ((csocket = clientTCPSocket(hostName, portNum)) == -1) {
        fprintf(stderr, "client_socket():error\n");
        return false;
    }

    // 送受信処理
    print_info(csocket, input);

    // ソケットクローズ
    close(csocket);
    return true;
}

int main (int argc, char *argv[]){
    start(HOSTNAME, PORTNUM, argv[1]);
    return EXIT_SUCCESS;
}


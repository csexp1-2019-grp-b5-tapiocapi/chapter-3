/*
   ============================================================================
Name        : TCPServerMT.c
Author      : Yasuhiro Noguchi
Version     : 0.0.1
Copyright   : Copyright (C) HEPT Consortium
Description : TCP Server implemented by pthread
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "libserver.h"
#include "lastA_server.h"

#define PORT "10000"

unordered_map<string, vector<unsigned long >> tag_map;
unordered_map<unsigned long, info*> info_map;

bool start(const char* portNum) {

    int ssocket = serverTCPSocket(portNum);
    if (ssocket == -1) {
        fprintf(stderr, "server_socket(%s):error\n", portNum);
        return false;
    }
    // fprintf(stderr, "ready for accept\n");

    create_tagDB(tag_map);
    create_geotagDB(info_map);
    cout << "ready" << endl;
    acceptLoop(ssocket);
    close(ssocket);
    return true;
}

int main(void) {
    start(PORT);
    return EXIT_SUCCESS;
}

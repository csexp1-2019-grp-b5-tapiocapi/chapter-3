/*
 * libclient.c
 *
 *  Created on: 2016/06/13
 *      Author: yasuh
 */

#include "libclient.h"

#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>

int clientTCPSocket(const char *hostName, const char *portNum) {

    // アドレス情報のヒントをゼロクリア
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // アドレス情報の登録
    struct addrinfo* res = NULL;
    int errcode = 0;
    errcode = getaddrinfo(hostName, portNum, &hints, &res);
    if (errcode != 0) {
        fprintf(stderr, "getaddrinfo():%s\n", gai_strerror(errcode));
        return (-1);
    }

    // ホスト名・ポート番号変換（接続に必須ではない）
    char nbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    if ((errcode = getnameinfo(res->ai_addr, res->ai_addrlen, nbuf,
                    sizeof(nbuf), sbuf, sizeof(sbuf),
                    NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
        fprintf(stderr, "getnameinfo():%s\n", gai_strerror(errcode));
        freeaddrinfo(res);
        return (-1);
    }
    // fprintf(stderr, "addr=%s\n", nbuf);
    // fprintf(stderr, "port=%s\n", sbuf);

    // ソケットの生成
    int soc = 0;
    soc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (soc == -1) {
        perror("socket");
        freeaddrinfo(res);
        return (-1);
    }

    // サーバに接続
    int connerr = 0;
    connerr = connect(soc, res->ai_addr, res->ai_addrlen);
    if (connerr == -1) {
        perror("connect");
        close(soc);
        freeaddrinfo(res);
        return (-1);
    }

    freeaddrinfo(res);
    return (soc);
}

void print_info(int sock, const char* msg) {
    char buf[100000];


    // サーバへ送信
    ssize_t slen;
    slen = send(sock, msg, strlen(msg), 0);
    if (slen == -1) {
        // エラー処理
        perror("send");
        return;
    }

    // サーバから受信
    ssize_t rlen;
    rlen = recv(sock, buf, sizeof(buf), 0);
    if (rlen == -1) {
        // エラー処理
        perror("recv");
        return;
    }

    if (rlen == 0) {
        // サーバ側からコネクション切断
        fprintf(stderr, "recv:EOF\n");
        return;
    }

    // コンソール表示
    buf[rlen] = '\0';
    printf("> %s", buf);
    fflush(stdout);

  //  sleep(1);	// 1秒待つ
}

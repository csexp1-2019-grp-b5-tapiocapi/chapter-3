/*
 * libserver.c
 *
 *  Created on: 2016/06/11
 *      Author: yasuh
 */

#include "libserver.h"
#include "lastA_server.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <pthread.h>

int serverTCPSocket(const char* port_num) {

    // アドレス情報を初期化
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // アドレス情報を設定
    int errcode = 0;
    struct addrinfo* res;
    if ((errcode = getaddrinfo(NULL, port_num, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo():%s\n", gai_strerror(errcode));
        return (-1);
    }

    char nbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    if ((errcode = getnameinfo(res->ai_addr, res->ai_addrlen, nbuf,
                    sizeof(nbuf), sbuf, sizeof(sbuf),
                    NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
        fprintf(stderr, "getnameinfo():%s\n", gai_strerror(errcode));
        freeaddrinfo(res);
        return (-1);
    }
    fprintf(stderr, "port=%s\n", sbuf);

    // ソケット生成
    int sock = 0;
    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol))
            == -1) {
        perror("socket");
        freeaddrinfo(res);
        return (-1);
    }

    // ソケットオプションの指定
    int socket_option = 1;
    socklen_t socket_option_size = sizeof(socket_option);
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &socket_option,
                socket_option_size) == -1) {
        perror("setsockopt");
        close(sock);
        freeaddrinfo(res);
        return (-1);
    }

    // ソケットにアドレスを指定
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sock);
        freeaddrinfo(res);
        return (-1);
    }

    // 接続要求の待ち受け
    if (listen(sock, SOMAXCONN) == -1) {
        perror("listen");
        close(sock);
        freeaddrinfo(res);
        return (-1);
    }
    freeaddrinfo(res);
    return (sock);
}

// thread function
void* thread_func(void *arg) {

    int acc = *((int*)arg);
    free(arg);

    pthread_detach(pthread_self());

    cout << "thread" << endl;
    echoBackLoop(acc);
    close(acc);

    pthread_exit(NULL);
}

void acceptLoop(int sock) {
    while (true) {
        struct sockaddr_storage from;
        socklen_t len = sizeof(from);
        int acc = 0;
        if ((acc = accept(sock, (struct sockaddr *) &from, &len)) == -1) {
            // エラー処理
            if (errno != EINTR) {
                perror("accept");
            }
        } else {
            // クライアントからの接続が行われた場合
            char hbuf[NI_MAXHOST];
            char sbuf[NI_MAXSERV];
            getnameinfo((struct sockaddr *) &from, len, hbuf,
                    sizeof(hbuf), sbuf, sizeof(sbuf),
                    NI_NUMERICHOST | NI_NUMERICSERV);
            fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);

            // スレッド生成
            int* param = (int*)malloc(sizeof(int));
            *param = acc;
            pthread_t th;
            if ( pthread_create(&th, NULL, thread_func, param) != 0 ) {
                perror("pthread_create");
                close(acc);	// 切断
            }
        }
    }
}

void echoBackLoop(int acc) {
    char buf[512];
    // ソケットから入力を受け取る
    ssize_t len = 0;
    if ((len = recv(acc, buf, sizeof(buf), 0)) == -1) {
        perror("recv");
        return;
    }
    cout << buf << endl;

    if (len == 0) {
        // クライアント側から切断（正常に切断）
        fprintf(stderr, "recv:EOF\n");
        return;
    }

    // 改行コードを行末に差し替える
    buf[len] = '\0';
    char* retPtr = NULL;
    if ((retPtr = strpbrk(buf, "\r\n")) != NULL) {
     *retPtr = '\0';
     }
    /*

    // 入力された内容に ":OK" を付与して送信する
    fprintf(stderr, "[client]%s\n", buf);	// コンソールに出力
    strncat(buf, ":OK\r\n", sizeof(buf) - strlen(buf) - 1);
    len = strlen(buf);
    if ((len = send(acc, buf, len, 0)) == -1) {		// クライアントに送信
    perror("send");
    break;
    }
    */
    string input_tag = string(buf), send_str;
    // TODO 今の実装だと入力を受け取ってからsortしていて遅い
    send_str = fetch_info(info_map, tag_map, input_tag);
    len = send_str.size();
    cout << send_str;
    if ((len = send(acc, send_str.c_str(), len, 0)) == -1) {		// クライアントに送信
    perror("send");
    return;
    }
    // cout << "send" << endl;
}


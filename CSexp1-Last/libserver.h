/*
 * libserver.h
 *
 *  Created on: 2016/06/13
 *      Author: yasuh
 */

#ifndef LIBSERVER_H_
#define LIBSERVER_H_

#include <stdbool.h>
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

/**
 * @brief TCPサーバソケットを生成する
 * @param port_num 待ち受けポート番号
 * @return 生成したソケット
 */
int serverTCPSocket(const char* port_num);

/**
 * @brief ポート待ち受けを開始する
 * @param soc サーバソケット
 */
void acceptLoop(int soc);

/**
 * @brief クライアントから切断されるまで，クライアントの入力に対してエコーバックを続ける
 * @param acc ソケット
 */
void echoBackLoop(int acc);

#endif /* LIBSERVER_H_ */

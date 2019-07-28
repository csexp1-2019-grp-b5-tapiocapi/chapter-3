/*
 * libclient.h
 *
 *  Created on: 2016/06/13
 *      Author: yasuh
 */

#ifndef LIBCLIENT_H_
#define LIBCLIENT_H_

/**
 * @brief TCPクライアントソケットを生成しサーバに接続を行う
 * @param hostName ホスト名
 * @param portNum ポート番号
 * @return クライアントソケット
 */
int clientTCPSocket(const char *hostName, const char *portNum);

/**
 * @brief 送受信ループ
 * @param soc クライアントソケット
 * @param msg 送信メッセージ
 */
void print_info(int sock, const char* msg);

#endif /* LIBCLIENT_H_ */

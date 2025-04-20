#pragma once
#include "game_header.h"

// 전역 변수 선언
extern float my_x, my_y, my_z;
extern SOCKET sock;
extern std::atomic<bool> running;

// 함수 선언
bool ConnectAndLogin();
void recv_thread(SOCKET sock);
void pos_sender_thread(SOCKET sock);

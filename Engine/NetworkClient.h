#pragma once
#include "game_header.h"

// ���� ���� ����
extern float my_x, my_y, my_z;
extern SOCKET sock;
extern std::atomic<bool> running;

// �Լ� ����
bool ConnectAndLogin();
void recv_thread(SOCKET sock);
void pos_sender_thread(SOCKET sock);

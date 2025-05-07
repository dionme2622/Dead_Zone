#pragma once
#include "game_header.h"

// 전역 변수 선언
extern float my_x, my_y, my_z;
extern SOCKET sock;
extern std::atomic<bool> running;
extern bool g_receivedMyInfo;
extern stoc_packet_player_info g_myInfo;

extern std::unordered_map<long long, std::tuple<float, float, float, float, float, float>> g_otherPlayerPositions;
extern std::mutex g_posMutex;

// 함수 선언
bool ConnectAndLogin();
void recv_thread(SOCKET sock);
void pos_sender_thread(SOCKET sock);

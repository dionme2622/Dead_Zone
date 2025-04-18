#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"
#include "KeyInput.h"

#pragma once

constexpr short GAME_PORT = 3000;
//Packet Type - SToC
constexpr char SToC_PLAYER_INFO = 1;
constexpr char SToC_PLAYER_MOVE = 2;
constexpr char SToC_PLAYER_ENTER = 3;
constexpr char SToC_PLAYER_LEAVE = 4;
constexpr char SToC_ALL_POSITION = 9;

//Packet Type - CToS
constexpr char CToS_PLAYER_LOGIN = 5;
constexpr char CToS_PLAYER_POS = 6;


constexpr char MAX_ID_LENGTH = 20;

constexpr char MOVE_UP = 1;
constexpr char MOVE_DOWN = 2;
constexpr char MOVE_LEFT = 3;
constexpr char MOVE_RIGHT = 4;

#pragma pack (push, 1)
//Packet - stoc
struct stoc_packet_player_info {
    unsigned char size;
    char type;
    long long  id;
    short x, y;
};

struct stoc_packet_enter {
    unsigned char size;
    char type;
    long long  id;
    char name[MAX_ID_LENGTH];
    char o_type;
    short x, y;
};

struct stoc_packet_leave {
    unsigned char size;
    char type;
    long long  id;
};

struct stoc_packet_move {
    unsigned char size;
    char type;
    long long id;
    short x, y;
};

struct stoc_packet_all_position {
    unsigned char size;
    char type;
    short count;
    struct {
        long long id;
        short x, y;
    } players[10];  // 최대 10명
};

//Packet - ctos
struct ctos_packet_login {
    unsigned char  size;
    char  type;
    char  name[MAX_ID_LENGTH];
};

struct ctos_packet_position {
    unsigned char size;
    char type;
    short x, y;
};

#pragma pack (pop)



short my_x = 4, my_y = 4;
SOCKET sock;
std::atomic<bool> running{ true };

void recv_thread(SOCKET sock) {
    char buffer[1024];
    while (running) {
        int ret = recv(sock, buffer, sizeof(buffer), 0);
        if (ret <= 0) {
            std::cout << "서버 연결 끊김.\n";
            running = false;
            break;
        }

        int offset = 0;
        while (offset < ret) {
            unsigned char size = buffer[offset];
            char type = buffer[offset + 1];

            switch (type) {
            case SToC_PLAYER_INFO: {
                stoc_packet_player_info* p = reinterpret_cast<stoc_packet_player_info*>(&buffer[offset]);
                std::cout << "[내 정보] ID: " << p->id << " 위치: (" << p->x << ", " << p->y << ")\n";
                break;
            }
            case SToC_PLAYER_ENTER: {
                stoc_packet_enter* p = reinterpret_cast<stoc_packet_enter*>(&buffer[offset]);
                std::cout << "[입장] ID: " << p->id << " 이름: " << p->name << " 위치: (" << p->x << ", " << p->y << ")\n";
                break;
            }
            case SToC_PLAYER_LEAVE: {
                stoc_packet_leave* p = reinterpret_cast<stoc_packet_leave*>(&buffer[offset]);
                std::cout << "[퇴장] ID: " << p->id << "\n";
                break;
            }
            case SToC_ALL_POSITION: {
                auto* p = reinterpret_cast<stoc_packet_all_position*>(&buffer[offset]);
                std::cout << "[전체 위치 업데이트] 인원수: " << p->count << "\n";
                for (int i = 0; i < p->count; ++i) {
                    std::cout << "- ID: " << p->players[i].id << " 위치: ("
                        << p->players[i].x << ", " << p->players[i].y << ")\n";
                }
                break;
            }
            default:
                std::cout << "[알 수 없는 패킷 타입]\n";
                break;
            }
            offset += size;
        }
    }
}

void pos_sender_thread() {
    while (running) {
        ctos_packet_position pos{};
        pos.size = sizeof(pos);
        pos.type = CToS_PLAYER_POS;
        pos.x = my_x;
        pos.y = my_y;

        send(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초마다 전송
    }
}

void Game::Init(const WindowInfo& info)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(GAME_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "서버 연결 실패\n";
        return ;
    }

    std::cout << "서버에 접속되었습니다.\n";

    // 로그인
    ctos_packet_login login{};
    login.size = sizeof(login);
    login.type = CToS_PLAYER_LOGIN;
    std::cout << "닉네임 입력: ";
    std::cin.getline(login.name, MAX_ID_LENGTH);
    send(sock, reinterpret_cast<char*>(&login), sizeof(login), 0);

    // 수신 쓰레드 시작
    std::thread(recv_thread, sock).detach();
    // 위치 송신 쓰레드 시작
    std::thread(pos_sender_thread).detach();

	GEngine->Init(info);
	
	GET_SINGLE(SceneManager)->LoadScene(SCENETYPE::EBATTLESCENE);
}

void Game::Update()
{
	/* ------------------------------ 키 입력 Debug -----------------------------------------*/
	if (INPUT->GetButtonDown(KEY_TYPE::ESC)) exit(0);

	if (INPUT->GetButtonDown(KEY_TYPE::B))
		_WIRE_FRAME_MODE = !_WIRE_FRAME_MODE;
	/* --------------------------------------------------------------------------------------*/

	GEngine->Update();
}

#include "pch.h"
#include "NetworkClient.h"

SOCKET sock;
std::atomic<bool> running{ true };
bool g_receivedMyInfo = false;
stoc_packet_player_info g_myInfo;

std::unordered_map<long long, std::tuple<float, float, float, float, float, float, float, bool, bool, bool>> g_otherPlayerPositions;
std::mutex g_posMutex;

bool ConnectAndLogin()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup ����\n";
        return false;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "���� ���� ����\n";
        return false;
    }

    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(GAME_PORT);

    inet_pton(AF_INET, "172.30.1.26", &serverAddr.sin_addr);

    if (connect(sock, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "���� ���� ����\n";
        closesocket(sock);
        return false;
    }

    std::cout << "Connected to the server.\n";

    ctos_packet_login login{};
    login.size = sizeof(login);
    login.type = CToS_PLAYER_LOGIN;
    std::cout << "ID : ";
    std::cin.getline(login.name, MAX_ID_LENGTH);
    send(sock, reinterpret_cast<char*>(&login), sizeof(login), 0);

    return true;
}

void recv_thread(SOCKET sock) {
    unsigned char buffer[4096];
    int data_size = 0;

    while (running) {
        int ret = recv(sock, reinterpret_cast<char*>(buffer) + data_size, sizeof(buffer) - data_size, 0);
        if (ret <= 0) {
            std::cout << "���� ���� ����.\n";
            running = false;
            break;
        }

        data_size += ret;
        int offset = 0;

        while (true) {
            if (data_size - offset < 2)  // �ּ��� size�� type�� �־�� ��
                break;

            unsigned char size = buffer[offset];
            if (data_size - offset < size)
                break;

            unsigned char type = buffer[offset + 1];

            switch (type) {
            case SToC_PLAYER_INFO: {
                auto* p = reinterpret_cast<stoc_packet_player_info*>(&buffer[offset]);
                std::cout << "[MY INFORMATION] ID: " << p->id << " ��ġ: (" << p->x << ", " << p->y << ", " << p->z << ")\n";
                g_myInfo = *p;
                g_receivedMyInfo = true;
                break;
            }
            case SToC_PLAYER_ENTER: {
                auto* p = reinterpret_cast<stoc_packet_enter*>(&buffer[offset]);
                std::cout << "[ENTER] ID: " << p->id << " �̸�: " << p->name
                    << " ��ġ: (" << p->x << ", " << p->y << ", " << p->z << ")\n";
                break;
            }
            case SToC_PLAYER_LEAVE: {
                auto* p = reinterpret_cast<stoc_packet_leave*>(&buffer[offset]);
                std::cout << "[LEAVE] ID: " << p->id << "\n";
                break;
            }
            case SToC_ALL_POSITION: {
                auto* p = reinterpret_cast<stoc_packet_all_position*>(&buffer[offset]);

                std::lock_guard<std::mutex> lock(g_posMutex);
                for (int i = 0; i < p->count; ++i) {
                    g_otherPlayerPositions[p->players[i].id] = std::make_tuple(
                        p->players[i].x,
                        p->players[i].y,
                        p->players[i].z,
                        p->players[i].rx,
                        p->players[i].ry,
                        p->players[i].rz,
                        p->players[i].speed,
                        p->players[i].isJumping,
                        p->players[i].isAiming,
                        p->players[i].isShooting
                    );
                }
                break;
            }
            default:
                std::cout << "[�� �� ���� ��Ŷ Ÿ��: " << static_cast<int>(type) << "]\n";
                break;
            }

            offset += size;
        }

        if (offset > 0) {
            memmove(buffer, buffer + offset, data_size - offset);
            data_size -= offset;
        }
    }
}
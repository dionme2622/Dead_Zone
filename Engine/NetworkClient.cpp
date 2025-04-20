#include "pch.h"
#include "NetworkClient.h"

float my_x = 4.001f, my_y = 4.002f, my_z = 4.003f;
SOCKET sock;
std::atomic<bool> running{ true };

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
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "���� ���� ����\n";
        closesocket(sock);
        return false;
    }

    std::cout << "������ ���ӵǾ����ϴ�.\n";

    ctos_packet_login login{};
    login.size = sizeof(login);
    login.type = CToS_PLAYER_LOGIN;
    std::cout << "�г��� �Է�: ";
    std::cin.getline(login.name, MAX_ID_LENGTH);
    send(sock, reinterpret_cast<char*>(&login), sizeof(login), 0);

    return true;
}

void recv_thread(SOCKET sock) {
    char buffer[1024];
    while (running) {
        int ret = recv(sock, buffer, sizeof(buffer), 0);
        if (ret <= 0) {
            std::cout << "���� ���� ����.\n";
            running = false;
            break;
        }

        int offset = 0;
        while (offset < ret) {
            unsigned char size = buffer[offset];
            char type = buffer[offset + 1];

            switch (type) {
            case SToC_PLAYER_INFO: {
                auto* p = reinterpret_cast<stoc_packet_player_info*>(&buffer[offset]);
                std::cout << "[�� ����] ID: " << p->id << " ��ġ: (" << p->x << ", " << p->y << ", " << p->z << ")\n";
                break;
            }
            case SToC_PLAYER_ENTER: {
                auto* p = reinterpret_cast<stoc_packet_enter*>(&buffer[offset]);
                std::cout << "[����] ID: " << p->id << " �̸�: " << p->name << " ��ġ: (" << p->x << ", " << p->y << ", " << p->z << ")\n";
                break;
            }
            case SToC_PLAYER_LEAVE: {
                auto* p = reinterpret_cast<stoc_packet_leave*>(&buffer[offset]);
                std::cout << "[����] ID: " << p->id << "\n";
                break;
            }
            case SToC_ALL_POSITION: {
                auto* p = reinterpret_cast<stoc_packet_all_position*>(&buffer[offset]);
                std::cout << "[��ü ��ġ ������Ʈ] �ο���: " << p->count << "\n";
                for (int i = 0; i < p->count; ++i) {
                    std::cout << "- ID: " << p->players[i].id << " ��ġ: (" << p->players[i].x << ", " << p->players[i].y << ", " << p->players[i].z << ")\n";
                }
                break;
            }
            default:
                std::cout << "[�� �� ���� ��Ŷ Ÿ��]\n";
                break;
            }
            offset += size;
        }
    }
}

void pos_sender_thread(SOCKET sock) {
    while (running) {
        ctos_packet_position pos{};
        pos.size = sizeof(pos);
        pos.type = CToS_PLAYER_POS;
        pos.x = my_x;
        pos.y = my_y;
        pos.z = my_z;
        send(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
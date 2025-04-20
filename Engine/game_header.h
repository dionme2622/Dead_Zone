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
	float x, y, z;
};

struct stoc_packet_enter {
	unsigned char size;
	char type;
	long long  id;
	char name[MAX_ID_LENGTH];
	char o_type;
	float x, y, z;
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
	float x, y, z;
};

struct stoc_packet_all_position {
	unsigned char size;
	char type;
	short count;
	struct {
		long long id;
		float x, y, z;
	} players[64];  // √÷¥Î 64∏Ì
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
	float x, y, z;
};


#pragma pack (pop)


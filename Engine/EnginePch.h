#pragma once

// std::byte ������� ����
#define _HAS_STD_BYTE 0

#define NOMINMAX       // �̹� ������ OK
#define byte _win_byte //byte �̸� �浹 ����
// ���� Winsock2 ��� ����
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

// ���� include
#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "d3dx12.h"
#include "SimpleMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// ���� lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

//#define _DEBUG_COLLIDER
#define _INSTANCING_MODE
extern bool _WIRE_FRAME_MODE;

// ���� typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,

	END
};

enum class UAV_REGISTER : uint8
{
	u0 = static_cast<uint8>(SRV_REGISTER::END),
	u1,
	u2,
	u3,
	u4,

	END,
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	CBV_SRV_REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
	UAV_REGISTER_COUNT = static_cast<uint8>(UAV_REGISTER::END) - CBV_SRV_REGISTER_COUNT,
	TOTAL_REGISTER_COUNT = CBV_SRV_REGISTER_COUNT + UAV_REGISTER_COUNT
};

struct WindowInfo
{
	HWND	hwnd; // ��� ������
	int32	width; // �ʺ�
	int32	height; // ����
	bool	windowed; // â��� or ��üȭ��
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec3 bitangent;
	Vec4 weights;
	Vec4 indices;
};

#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLE(type)	type::GetInstance()

#define DEVICE				GEngine->GetDevice()->GetDevice()
#define WINDOW				GEngine->GetWindow()
#define GRAPHICS_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()
#define RESOURCE_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetResourceCmdList()
#define COMPUTE_CMD_LIST	GEngine->GetComputeCmdQueue()->GetComputeCmdList()

#define GRAPHICS_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetGraphicsRootSignature()
#define COMPUTE_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetComputeRootSignature()

#define INPUT				GET_SINGLE(KeyInput)
#define DELTA_TIME			GET_SINGLE(Timer)->GetDeltaTime()

#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
	Matrix matViewInv;
};

struct AnimFrameParams
{
	//wstring boneName;
	Vec4	scale;
	Vec4	rotation; // Quaternion
	Vec4	translation;
};

// Utils
wstring s2ws(const string& s);
string ws2s(const wstring& s);
int ReadIntegerFromFile(FILE* pInFile);
float ReadFloatFromFile(FILE* pInFile);
BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);

extern unique_ptr<class Engine> GEngine;



//Vec3 QuaternionToEulerAngles(const & Quatern) {
//	Vector3 euler;
//
//	// roll (x-axis rotation)
//	float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
//	float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
//	euler.x = std::atan2(sinr_cosp, cosr_cosp);
//
//	// pitch (y-axis rotation)
//	float sinp = 2 * (q.w * q.y - q.z * q.x);
//	if (std::abs(sinp) >= 1)
//		euler.y = std::copysign(DirectX::XM_PIDIV2, sinp); // 90�� Ŭ����
//	else
//		euler.y = std::asin(sinp);
//
//	// yaw (z-axis rotation)
//	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
//	float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
//	euler.z = std::atan2(siny_cosp, cosy_cosp);
//
//	return euler;
//}
//
//void DecomposeMatrix(const Matrix& mat, Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation, Vector3& outEulerAngles) {
//	// 1. Translation
//	outTranslation = Vector3(mat._41, mat._42, mat._43);
//
//	// 2. Scale
//	outScale.x = Vector3(mat._11, mat._12, mat._13).Length();
//	outScale.y = Vector3(mat._21, mat._22, mat._23).Length();
//	outScale.z = Vector3(mat._31, mat._32, mat._33).Length();
//
//	// 3. Rotation Matrix (������ ����)
//	Matrix rotMat = mat;
//	rotMat._11 /= outScale.x; rotMat._12 /= outScale.x; rotMat._13 /= outScale.x;
//	rotMat._21 /= outScale.y; rotMat._22 /= outScale.y; rotMat._23 /= outScale.y;
//	rotMat._31 /= outScale.z; rotMat._32 /= outScale.z; rotMat._33 /= outScale.z;
//
//	// Quaternion���� ȸ�� ����
//	outRotation = Quaternion::CreateFromRotationMatrix(rotMat);
//
//	// Euler ����(Yaw, Pitch, Roll)�� ��ȯ
//	outEulerAngles = QuaternionToEulerAngles(outRotation);
//}
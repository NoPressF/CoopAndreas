#pragma once

#include "stdafx.h"

class CPacketListener
{
public:
	unsigned short m_iPacketID;
	void(*m_callback)(void*, int);

	CPacketListener(unsigned short id, void(*callback)(void*, int))
	{
		m_iPacketID = id;
		m_callback = callback;
	}
};

class CNetwork
{
public:
	static std::vector<CPacketListener*> m_packetListeners;
	static ENetHost* m_pClient;
	static ENetPeer* m_pPeer;
	static bool m_bConnected;
	static char m_IpAddress[128 + 1];
	static unsigned short m_nPort;

	static DWORD WINAPI InitAsync(LPVOID);
	static void Disconnect();
	static void SendPacket(unsigned short id, void* data, size_t dataSize, ENetPacketFlag flag);
	static void InitListeners();
	static void HandlePacketReceive(ENetEvent& event);
	static void AddListener(unsigned short id, void(*callback)(void*, int));
};
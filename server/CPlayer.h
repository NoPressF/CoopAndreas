#pragma once
class CPlayer
{
public:
	ENetPeer* m_pPeer;
	int m_iPlayerId;
	char m_Name[32 + 1] = { 0 };
	bool m_bIsHost;
	unsigned char m_nSeatId;
	int m_nVehicleId;
	std::vector<CPlayer*> m_streamedFor;

	CPlayer(ENetPeer* peer, int playerid);

};


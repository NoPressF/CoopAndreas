#include "stdafx.h"
#include "CNetworkVehicle.h"

std::vector<CNetworkVehicle*> CNetworkVehicleManager::m_pVehicles;

CNetworkVehicle* CNetworkVehicleManager::GetVehicle(int vehicleid)
{
	for (int i = 0; i != m_pVehicles.size(); i++)
	{
		if (m_pVehicles[i]->m_nVehicleId == vehicleid)
		{
			return m_pVehicles[i];
		}
	}
	return nullptr;
}

CNetworkVehicle* CNetworkVehicleManager::GetVehicle(CVehicle* vehicle)
{
	for (int i = 0; i != m_pVehicles.size(); i++)
	{
		if (m_pVehicles[i]->m_pVehicle == vehicle)
		{
			return m_pVehicles[i];
		}
	}
	
	return nullptr;
}

CNetworkVehicle* CNetworkVehicleManager::GetVehicle(CEntity* vehicle)
{
	for (int i = 0; i != m_pVehicles.size(); i++)
	{
		if (m_pVehicles[i]->m_pVehicle == vehicle)
		{
			return m_pVehicles[i];
		}
	}

	return nullptr;
}

int CNetworkVehicleManager::GetFreeID()
{
	if (!CLocalPlayer::m_bIsHost)
		return -1;

	for (int i = 0; i != MAX_SERVER_VEHICLES; i++)
	{
		if (CNetworkVehicleManager::GetVehicle(i) == nullptr)
			return i;
	}

	return -1;
}

void CNetworkVehicleManager::Add(CNetworkVehicle* vehicle)
{
	CNetworkVehicleManager::m_pVehicles.push_back(vehicle);
}

void CNetworkVehicleManager::Remove(CNetworkVehicle* vehicle)
{
	auto it = std::find(m_pVehicles.begin(), m_pVehicles.end(), vehicle);
	if (it != m_pVehicles.end())
	{
		m_pVehicles.erase(it);
	}
}

void CNetworkVehicleManager::UpdateDriver(CVehicle* vehicle)
{
	CNetworkVehicle* networkVehicle = CNetworkVehicleManager::GetVehicle(vehicle);
	CPackets::VehicleDriverUpdate* packet = CPacketHandler::VehicleDriverUpdate__Collect(networkVehicle);
	CNetwork::SendPacket(ePacketType::VEHICLE_DRIVER_UPDATE, packet, sizeof *packet, ENET_PACKET_FLAG_UNSEQUENCED);
	delete packet;
}

void CNetworkVehicleManager::UpdateIdle()
{
	for (int i = 0; i != m_pVehicles.size(); i++)
	{
		if (m_pVehicles[i]->m_pVehicle == nullptr)
			continue;

		if (CLocalPlayer::m_bIsHost && !m_pVehicles[i]->HasDriver())
		{
			CPackets::VehicleIdleUpdate* packet = CPacketHandler::VehicleIdleUpdate__Collect(m_pVehicles[i]);
			CNetwork::SendPacket(ePacketType::VEHICLE_IDLE_UPDATE, packet, sizeof * packet, ENET_PACKET_FLAG_UNSEQUENCED);
			delete packet;
		}
	}
}

void CNetworkVehicleManager::UpdatePassenger(CVehicle* vehicle, CPlayerPed* localPlayer)
{
	CNetworkVehicle* networkVehicle = CNetworkVehicleManager::GetVehicle(vehicle);
	CPackets::VehiclePassengerUpdate* packet = CPacketHandler::VehiclePassengerUpdate__Collect(networkVehicle, localPlayer);
	CNetwork::SendPacket(ePacketType::VEHICLE_PASSENGER_UPDATE, packet, sizeof * packet, ENET_PACKET_FLAG_UNSEQUENCED);
	delete packet;
}
// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

#include "rpc/server.h"

#include "PlayerData.h"

#define PORT 8080

unsigned int NEXT_PLAYER_ID = 0;

PlayerData firstPlayer;
PlayerData secondPlayer;

unsigned int getNextAvailablePlayerID()
{
	return ++NEXT_PLAYER_ID;
}

unsigned int createPlayer()
{
	unsigned int result = getNextAvailablePlayerID();
	if (firstPlayer.id == 0) firstPlayer.id = result;
	else secondPlayer.id = result;
	return result;
}

PlayerData& getServerPlayer(PlayerData& playerData)
{
	if (playerData.id == firstPlayer.id) return firstPlayer;
	else return secondPlayer;
}

void updateServerPlayer(PlayerData& playerData)
{
	if (playerData.id == firstPlayer.id) firstPlayer = playerData;
	else secondPlayer = playerData;
}

PlayerData& getOtherPlayer(unsigned int playerID)
{
	if (playerID == firstPlayer.id) return secondPlayer;
	else return firstPlayer;
}

int main()
{
	// Set up rpc server and listen to PORT
	rpc::server srv(PORT);
	std::cout << "Listening to port: " << PORT << std::endl;

	srv.bind("connect", []() {
		unsigned int playerID = createPlayer();
		std::cout << "Player connected! PlayerID: " << playerID << std::endl;
		return playerID;
	});

	srv.bind("sendUpdate", [](PlayerData& playerData) {
		updateServerPlayer(playerData);
		std::cout << firstPlayer.rightHandPosition[0] << "," << firstPlayer.rightHandPosition[1] << "," << firstPlayer.rightHandPosition[2] << std::endl;
	});

	srv.bind("requestUpdate", [](unsigned int playerID) {
		PlayerData& otherPlayer = getOtherPlayer(playerID);
		return otherPlayer;
	});

	// Blocking call to start the server: non-blocking call is srv.async_run(threadsCount);
	srv.run();
	return 0;
}

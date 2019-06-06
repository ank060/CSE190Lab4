// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

#include "rpc/server.h"

#include "PlayerData.h"
#include "SceneData.h"

#define PORT 8080

const unsigned int GRID_SIZE = 5;
const unsigned int MAX_DANGER_BALLS = 50;
unsigned int NEXT_PLAYER_ID = 0;

// std::vector<PlayerData> players;
PlayerData firstPlayer;
PlayerData secondPlayer;
SceneData sceneData;

unsigned int getNextAvailablePlayerID()
{
	return ++NEXT_PLAYER_ID;
}

unsigned int createPlayer()
{
	unsigned int result = getNextAvailablePlayerID();
	if (firstPlayer.id == 0)
	{
		firstPlayer.id = result;
		firstPlayer.score = 0;
	}
	else
	{
		secondPlayer.id = result;
		secondPlayer.score = 0;
	}
	return result;
}

bool isDangerBall(int index)
{
	return sceneData.dangerBallBits[index] == '1';
}

int pickRandomBallIndex()
{
	return rand() % (GRID_SIZE * GRID_SIZE * GRID_SIZE);
}

int pickRandomEmptyBallIndex()
{
	unsigned int result;
	do
	{
		result = pickRandomBallIndex();
	} while (result == sceneData.goalBallIndex || isDangerBall(result));
	return result;
}

void pickNewGoal()
{
	int ballIndex = pickRandomEmptyBallIndex();
	sceneData.goalBallIndex = ballIndex;
}

void pickNewDanger()
{
	int ballIndex = pickRandomEmptyBallIndex();
	sceneData.dangerBallBits[ballIndex] = '1';
	sceneData.dangerBallCount++;
}

void resetDangerBalls()
{
	sceneData.dangerBallCount = 0;
	sceneData.dangerBallBits = std::string(GRID_SIZE * GRID_SIZE * GRID_SIZE, '0');
}

void resetGame()
{
	sceneData.gameStart = false;
	resetDangerBalls();
	pickNewGoal();

	firstPlayer.score = 0;
	secondPlayer.score = 0;
}

PlayerData& getServerPlayer(unsigned int playerID)
{
	if (playerID == firstPlayer.id) return firstPlayer;
	else return secondPlayer;
}

PlayerData& getOtherServerPlayer(unsigned int playerID)
{
	if (playerID == firstPlayer.id) return secondPlayer;
	else return firstPlayer;
}

void updateServerPlayer(PlayerData& playerData)
{
	if (playerData.id == firstPlayer.id) firstPlayer = playerData;
	else secondPlayer = playerData;
}

int main()
{
	// Initialize the random seed
	srand(time(NULL));

	// Set up game...
	resetDangerBalls();
	pickNewGoal();

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
	});

	srv.bind("requestUpdate", [](unsigned int playerID) {
		PlayerData& otherPlayer = getOtherServerPlayer(playerID);
		return std::pair<PlayerData, SceneData>(otherPlayer, sceneData);
	});

	srv.bind("touchBall", [](unsigned int playerID, unsigned int ballTouched) {
		if (ballTouched == sceneData.goalBallIndex)
		{
			// Player touched the ball!
			pickNewGoal();
			
			// Pick a danger ball if able to!
			if (sceneData.dangerBallCount < MAX_DANGER_BALLS)
			{
				pickNewDanger();
			}

			// +1 points for player with playerID
			getServerPlayer(playerID).score++;
		}
	});

	srv.bind("touchDanger", [](unsigned int playerID, unsigned int ballTouched) {
		if (isDangerBall(ballTouched))
		{
			resetGame();
		}
	});

	srv.bind("touchStart", [](unsigned int playerID) {
		sceneData.gameStart = true;
	});

	// Blocking call to start the server: non-blocking call is srv.async_run(threadsCount);
	srv.run();
	return 0;
}

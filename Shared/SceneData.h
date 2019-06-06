#include "rpc/client.h"

struct SceneData
{
	unsigned int goalBallIndex;
	std::string dangerBallBits;
	unsigned int dangerBallCount;
	bool gameStart;

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(goalBallIndex, dangerBallBits, dangerBallCount, gameStart)
};
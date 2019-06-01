#include "rpc/client.h"
#include "PlayerData.h"

struct SceneData
{
	PlayerData otherPlayer;

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(otherPlayer)
};

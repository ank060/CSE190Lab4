#include "rpc/client.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct BallData
{
	unsigned int ownerID;
	unsigned int hitID;

	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 velocity;

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(
		ownerID, hitID,
		color.x, color.y, color.z,
		position.x, position.y, position.z,
		velocity.x, velocity.y, velocity.z
	)
};
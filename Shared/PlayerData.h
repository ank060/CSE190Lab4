#include "rpc/client.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct PlayerData
{
	unsigned int id;
	unsigned int score;

	bool leftHandClosed;
	bool rightHandClosed;

	glm::vec3 leftHandPosition;
	glm::vec3 rightHandPosition;
	glm::vec3 headPosition;

	glm::vec3 leftHandVelocity;
	glm::vec3 rightHandVelocity;

	glm::quat leftHandRotation;
	glm::quat rightHandRotation;
	glm::quat headRotation;

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(id, score, leftHandClosed, rightHandClosed,
		leftHandPosition.x, leftHandPosition.y, leftHandPosition.z,
		rightHandPosition.x, rightHandPosition.y, rightHandPosition.z,
		headPosition.x, headPosition.y, headPosition.z,
		leftHandVelocity.x, leftHandVelocity.y, leftHandVelocity.z,
		rightHandVelocity.x, rightHandVelocity.y, rightHandVelocity.z,
		leftHandRotation.x, leftHandRotation.y, leftHandRotation.z, leftHandRotation.w,
		rightHandRotation.x, rightHandRotation.y, rightHandRotation.z, rightHandRotation.w,
		headRotation.x, headRotation.y, headRotation.z, headRotation.w)
};
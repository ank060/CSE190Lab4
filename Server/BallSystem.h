#include "rpc/server.h"

#include "SceneData.h"
#include "BallData.h"

namespace BallSystem
{
	void createBall(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration)
	{

	}

	void bindBallHandlers(rpc::server& server, SceneData& scene)
	{
		server.bind("createBall", [scene](unsigned int ownerID, BallData& ballData) {
			scene.balls.push_back(ballData);
		});
	}
}
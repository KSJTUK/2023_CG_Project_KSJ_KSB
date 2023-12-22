#pragma once

glm::mat4 animatToGLM(aiMatrix4x4& aiMatr);
aiQuaternion nlerp(aiQuaternion& q1, aiQuaternion& q2, float blend);
bool triangleRayCasting(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
glm::vec3 vectorProcection(const glm::vec3& A, const glm::vec3& B);
float distanceRayBetweenPoint(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& point);

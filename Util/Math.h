#pragma once

glm::mat4 AimatTOGlm(aiMatrix4x4& ai_matr);
aiQuaternion nlerp(aiQuaternion& q1, aiQuaternion& q2, float blend);
bool TriangleRayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
glm::vec3 VectorProcection(const glm::vec3& A, const glm::vec3& B);
float DistanceRayBetweenPoint(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& Point);

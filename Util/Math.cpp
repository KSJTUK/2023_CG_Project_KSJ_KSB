#include "pch.h"
#include "Math.h"

glm::mat4 AimatTOGlm(aiMatrix4x4& ai_matr) {

	glm::mat4 result{};
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;


	return result;
}


aiQuaternion nlerp(aiQuaternion& q1, aiQuaternion& q2, float blend) {
	//cout << a.w + a.x + a.y + a.z << endl;
	q1.Normalize();
	q2.Normalize();

	aiQuaternion result;
	float dot_product = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = q1.x * one_minus_blend + blend * -q2.x;
		result.y = q1.y * one_minus_blend + blend * -q2.y;
		result.z = q1.z * one_minus_blend + blend * -q2.z;
		result.w = q1.w * one_minus_blend + blend * -q2.w;
	}
	else
	{
		result.x = q1.x * one_minus_blend + blend * q2.x;
		result.y = q1.y * one_minus_blend + blend * q2.y;
		result.z = q1.z * one_minus_blend + blend * q2.z;
		result.w = q1.w * one_minus_blend + blend * q2.w;
	}

	return result.Normalize();
}


bool TriangleRayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {

	glm::vec3 edge1, edge2;
	glm::vec3 pvec, tvec, qvec;
	float det, invDet;

	float u, v;

	// Compute vectors along two edges of the triangle
	edge1 = v1 - v0;
	edge2 = v2 - v0;

	// Compute the determinant
	pvec = glm::cross(RayDirection, edge2);
	det = glm::dot(edge1, pvec);


	// If the determinant is near zero, the ray lies in the plane of the triangle
	if (fabs(det) < FLT_EPSILON) return false;

	invDet = 1 / det;

	// Compute the u parameter of the intersection point
	tvec = RayOrigin - v0;
	u = glm::dot(tvec, pvec) * invDet;
	if (u < 0.f or u > 1.f) return false;

	// Compute the v parameter of the intersection point
	qvec = glm::cross(tvec, edge1);
	v = glm::dot(RayDirection, qvec) * invDet;
	if (v < 0.f or u + v > 1.f) return false;



	return true;

}

float DistanceRayBetweenPoint(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& Point){
	
	return
		std::fabs((Point.x - RayOrigin.x) * RayDirection.y - (Point.y - RayOrigin.y) * RayDirection.x + (Point.z - RayOrigin.z) * RayDirection.z)
		/
		glm::length(RayDirection) * 0.1f;

}


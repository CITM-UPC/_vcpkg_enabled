#include "BoundingBox.h"

BoundingBox operator*(const mat4& mat, const BoundingBox& bbox) {
	auto vertices = bbox.vertices();
	for (auto& v : vertices) v = mat * vec4(v, 1);

	BoundingBox result = { vertices.front(), vertices.front() };
	for (const auto& v : vertices) {
		result.min = glm::min(result.min, v);
		result.max = glm::max(result.max, v);
	}

	return result;
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const {
	BoundingBox result;
	result.min = glm::min(min, other.min);
	result.max = glm::max(max, other.max);
	return result;
}
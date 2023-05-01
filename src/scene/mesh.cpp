#include "scene/mesh.h"

#include <utility>

namespace gore
{
	mesh::mesh(std::vector<vertex> &&vertices, std::vector<unsigned int> &&indices) :
		vertices_{std::move(vertices)}, indices_{std::move(indices)}
	{
	}
}
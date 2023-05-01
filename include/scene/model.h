#ifndef GORE_MODEL_H
#define GORE_MODEL_H

#include "assimp/scene.h"	

#include "scene/aabb.h"
#include "scene/mesh.h"
#include "gl/material.h"
#include "gl/static_buffer.h"
#include "memory/texture_storage.h"
#include "memory/renderable_storage.h"
#include "util/platform.h"

#include <vector>
#include <string_view>
#include <string>
#include <array>

namespace gore
{
	class model
	{
	public:
		model();
		model(std::string_view path, rid id);

		unsigned int vertex_size() const;
		unsigned int index_size() const;
		aabb aabb_;
	private:
		mesh process_mesh(const aiScene *scene, aiMesh *aimesh);
		void process_node(const aiScene *scene, aiNode *node);

		unsigned int vertex_size_ = 0;
		unsigned int index_size_ = 0;

		std::vector<mesh> meshes_;
	};

	__FORCE_INLINE unsigned int model::vertex_size() const
	{
		return vertex_size_;
	}
	__FORCE_INLINE unsigned int model::index_size() const
	{
		return index_size_;
	}
}

#endif
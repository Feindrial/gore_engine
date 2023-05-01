#include "scene/model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/vec3.hpp"

#include "util/file_system.h"
#include "util/error_handler.h"
#include "scene/mesh.h"
#include "memory/texture_storage.h"
#include "memory/buffer_storage.h"
#include "memory/renderable_storage.h"

#include <string_view>
#include <vector>
#include <utility>
#include <iostream>

namespace gore
{
	model::model() :
		aabb_{ glm::vec3(1.0f), glm::vec3(-1.0f) }
	{

	}
	model::model(std::string_view path, rid id) : 
		aabb_{ glm::vec3(1.0f), glm::vec3(-1.0f) }
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path.data(),
			aiProcess_JoinIdenticalVertices |	
			aiProcess_PreTransformVertices |	
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			//aiProcess_FlipUVs |
			aiProcess_SplitLargeMeshes | 
			aiProcess_ImproveCacheLocality |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_OptimizeMeshes);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			error_handler::error(non_fatal::load_model);

		meshes_.reserve(scene->mNumMeshes);	
		process_node(scene, scene->mRootNode);		

		texture_storage::get()->reserve_material_data(id);
		const aiMaterial *aimaterial = scene->mMaterials[0];

		aiString diffuse;
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse);
		texture_storage::get()->push_material_data(id, diffuse.C_Str(), gl::material_type::diffuse);

		aiString normal;
		aimaterial->GetTexture(aiTextureType_NORMALS, 0, &normal);
		texture_storage::get()->push_material_data(id, normal.C_Str(), gl::material_type::normal);

		aiString specular;
		aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &specular);
		texture_storage::get()->push_material_data(id, specular.C_Str(), gl::material_type::specular);

		aiString roughness;
		aimaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughness);
		texture_storage::get()->push_material_data(id, roughness.C_Str(), gl::material_type::roughness);

		aiString ao;
		aimaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ao);
		texture_storage::get()->push_material_data(id, ao.C_Str(), gl::material_type::ao);

		
		buffer_storage::get()->reserve_buffer_data(id, vertex_size_, index_size_);
		for (const mesh &msh : meshes_)
		{
			buffer_storage::get()->push_mesh_data(id, msh.vertex_data(), msh.vertex_size(),
													  msh.index_data(), msh.index_size());
		}
	}

	mesh model::process_mesh(const aiScene *scene, aiMesh *aimesh)
	{
		glm::vec3 min(1.0f), max(-1.0f);
	
		mesh msh;
		msh.default_reserve()

		vertices.reserve(aimesh->mNumVertices);
		for (size_t i = 0; i < aimesh->mNumVertices; i++)
		{
			mesh_vertex vertex;
			if (aimesh->HasPositions())
			{
				vertex.position_.x = aimesh->mVertices[i].x;
				vertex.position_.y = aimesh->mVertices[i].y;
				vertex.position_.z = aimesh->mVertices[i].z;

				if (vertex.position_.x < min.x) min.x = vertex.position_.x;
				if (vertex.position_.x > max.x) max.x = vertex.position_.x;
				if (vertex.position_.y < min.y) min.y = vertex.position_.y;
				if (vertex.position_.y > max.y) max.y = vertex.position_.y;
				if (vertex.position_.z < min.z) min.z = vertex.position_.z;
				if (vertex.position_.z > max.z) max.z = vertex.position_.z;
			}
			if (aimesh->HasNormals())
			{
				vertex.normal_.x = aimesh->mNormals[i].x;
				vertex.normal_.y = aimesh->mNormals[i].y;
				vertex.normal_.z = aimesh->mNormals[i].z;
			}
			if (aimesh->HasTangentsAndBitangents())
			{
				vertex.tangent_.x = aimesh->mTangents[i].x;
				vertex.tangent_.y = aimesh->mTangents[i].y;
				vertex.tangent_.z = aimesh->mTangents[i].z;
				vertex.bitangent_.x = aimesh->mBitangents[i].x;
				vertex.bitangent_.y = aimesh->mBitangents[i].y;
				vertex.bitangent_.z = aimesh->mBitangents[i].z;
			}
			if (aimesh->HasTextureCoords(0))
			{
				vertex.texture_coord_.x = aimesh->mTextureCoords[0][i].x;
				vertex.texture_coord_.y = aimesh->mTextureCoords[0][i].y;
			}
			else
				vertex.texture_coord_ = glm::vec2(0.0f, 0.0f);

			vertices.emplace_back(vertex);
		}
		vertex_size_ += vertices.size() * sizeof(mesh::vertex);
		aabb_.try_to_extend(min, max);

		std::vector<unsigned int> indices;
		indices.reserve(aimesh->mNumFaces * aimesh->mFaces[0].mNumIndices);
		for (size_t i = 0; i < aimesh->mNumFaces; i++)
		{
			const aiFace face = aimesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}
		index_size_ += indices.size() * sizeof(mesh::index);

		return mesh(std::move(vertices), std::move(indices));		
	}
	void model::process_node(const aiScene *scene, aiNode *node)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes_.emplace_back(process_mesh(scene, mesh));
		}
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			process_node(scene, node->mChildren[i]);
		}
	}
}
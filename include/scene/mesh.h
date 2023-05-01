#ifndef GORE_MESH_H
#define GORE_MESH_H

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "core/error_macros.h"
#include "core/platform_macros.h"
#include "gl/vertex_array.h"
#include "util/error_handler.h"
#include "util/platform.h"
#include "util/util.h"

#include <limits>
#include <vector>
#include <variant>
#include <utility>
#include <limits>

namespace gore
{
	enum class default_vertex_attribute : char
	{
		position = util::bit(0),
		color    = util::bit(1),
		normal   = util::bit(2),
		tangent  = util::bit(3),
		uv       = util::bit(4)		
	};
	enum class index_attribute : char
	{
		index16  = util::bit(0),
		index32  = util::bit(1)
	};
	enum class vertex_format : char
	{
		f16,
		f32,
		f64,
		i8,
		i16,
		i32,
		ui8,
		ui16,
		ui32
	};
	enum class primitive_type : char
	{
		points,
		lines,
		line_strips,
		triangles
	};

    struct custom_vertex_attribute
    {
        vertex_format format_;
        unsigned char dimension_;
		unsigned char attribute_index = std::numeric_limits<char>::max();
    };
	struct mesh_vertex
	{	
		glm::vec3 position_;
		glm::vec3 normal_;
		glm::vec3 tangent_;
		glm::vec3 bitangent_;
		glm::vec2 texture_coord_;
	};
	
	class mesh
	{
	public:
		typedef std::vector<std::variant<std::pair<default_vertex_attribute, bool>,
							std::pair<custom_vertex_attribute, bool>>> vertex_layout;
		using vertex = mesh_vertex;
		using index = unsigned int;

		mesh(std::vector<vertex> &&vertices, std::vector<unsigned int> &&indices);

		unsigned int vertex_size() const;
		unsigned int index_size() const;
		const vertex* vertex_data() const;
		const unsigned int* index_data() const; 
		void set_vertex_layout(const vertex_layout &attributes, unsigned int vertex_count = 0);
		void set_index_layout(index_attribute index_attribute, unsigned int index_count = 0);
	private:
		std::vector<glm::vec3> positions_;
		std::vector<glm::vec4> colors_;
		std::vector<glm::vec3> normals_;
		std::vector<glm::vec3> tangents_;
		std::vector<glm::vec2> uvs_;
		std::vector<unsigned int> indices32_;
		std::vector<short> indices16_;
	};

#define elseif(frmt, rtrn) 				  \
		else if constexpr(format == frmt) \
			return rtrn;

	template<vertex_format format>
	constexpr unsigned int vformat_to_gl()
	{

		if constexpr(format == vertex_format::f16)
			return GL_HALF_FLOAT;
		elseif(vertex_format::f32, GL_FLOAT)
		elseif(vertex_format::f64, GL_DOUBLE)
		elseif(vertex_format::i8, GL_BYTE)
		elseif(vertex_format::i16, GL_SHORT)
		elseif(vertex_format::i32, GL_INT)
		elseif(vertex_format::ui8, GL_UNSIGNED_BYTE)
		elseif(vertex_format::ui16, GL_UNSIGNED_SHORT)
		elseif(vertex_format::ui32, GL_UNSIGNED_INT)
		else
			__GERR_UNEXPECTED(util::error_type::warning);
			return -1;

	}
	template<vertex_format format>
	constexpr unsigned int vformat_to_glsize()
	{
		if constexpr(format == vertex_format::f16)
			return 2u;
		elseif(vertex_format::f32, 4u)
		elseif(vertex_format::f64, 8u)
		elseif(vertex_format::i8, 1u)
		elseif(vertex_format::i16, 2u)
		elseif(vertex_format::i32, 4u)
		elseif(vertex_format::ui8, 1u)
		elseif(vertex_format::ui16, 2u)
		elseif(vertex_format::ui32, 4u)
		else
			__GERR_UNEXPECTED(util::error_type::warning);
			return -1;
	}

#undef elseif

	__FORCE_INLINE void mesh::set_vertex_layout(const vertex_layout &attributes, unsigned int vertex_count)
	{
	    gl::vertex_array vao;

		unsigned int acindex = 5;
		for (const auto &layout : attributes)
		{
			unsigned int dimension;
			unsigned int type;
			unsigned int size;
			if (std::holds_alternative<std::pair<default_vertex_attribute, bool>>(layout))
			{
				unsigned int adindex;
				unsigned int bindex = std::get<0>(layout).second ? 1 : 0;
				type = vformat_to_gl<vertex_format::f32>();
				size = vformat_to_glsize<vertex_format::f32>();
				switch (std::get<0>(layout).first)
				{
					case default_vertex_attribute::position: adindex = 0; dimension = 3; positions_.reserve(vertex_count); break;
					case default_vertex_attribute::color: adindex = 1; dimension = 4; colors_.reserve(vertex_count); break;
					case default_vertex_attribute::normal: adindex = 2; dimension = 3; normals_.reserve(vertex_count); break;
					case default_vertex_attribute::tangent: adindex = 3; dimension = 3; tangents_.reserve(vertex_count); break;
					case default_vertex_attribute::uv: adindex = 4; dimension = 2; uvs_.reserve(vertex_count); break;
				}

				vao.push_back(adindex, bindex, dimension, type, size);
			}
			else if (std::holds_alternative<std::pair<custom_vertex_attribute, bool>>(layout))
			{
				acindex = std::get<1>(layout).first.attribute_index == std::numeric_limits<char>::max() ? acindex : std::get<1>(layout).first.attribute_index;
				unsigned int bindex = std::get<1>(layout).second ? 1 : 0;
				type = vformat_to_gl<std::get<1>(layout).first.format_>();
				size = vformat_to_gl<std::get<1>(layout).first.format_>();
				dimension = std::get<1>(layout).first.dimension_;

				vao.push_back(acindex, bindex, dimension, type, size);

				acindex++;
			}

		}
	}
	void mesh::set_index_layout(index_attribute index_attribute, unsigned int index_count)
	{
		switch (index_attribute)
		{
			case index_attribute::index16: indices16_.reserve(index_count); break;
			case index_attribute::index32: indices32_.reserve(index_count); break;
		}
	}

	inline unsigned int mesh::vertex_size() const
	{
		return vertices_.size() * sizeof(mesh::vertex);
	}
	inline unsigned int mesh::index_size() const
	{
		return indices_.size() * sizeof(mesh::index);
	}
	inline const mesh::vertex* mesh::vertex_data() const
	{
		return vertices_.data();
	}
	inline const unsigned int* mesh::index_data() const
	{
		return indices_.data();
	}
}

#endif
#ifndef GORE_PROGRAM_H
#define GORE_PROGRAM_H

#include "glad/glad.h"
#include "glm/mat4x4.hpp"

#include "gl/shader.h"
#include "gl/globject.h"
#include "util/error_handler.h"
#include "util/platform.h"

#include <cstddef>
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <cstddef>

namespace gore
{
	namespace gl
	{
		class program : public globject
		{
			using program_type = GLenum;

			struct uniform_data
			{
				const std::string type_;
				const unsigned int arr_size_;
				const unsigned int location_;
			};
			struct block_data
			{
				struct variable
				{
					const std::string name_;
					const std::string type_;
					const unsigned int arr_size_;
					const unsigned int offset_;
				};

				std::vector<variable> variables_;
				unsigned int location_;
			};

			enum high_level_interface
			{
				uniform_block        = GL_UNIFORM_BLOCK,
				shader_storage_block = GL_SHADER_STORAGE_BLOCK
			};
			enum low_level_interface
			{
				uniform = GL_UNIFORM,
				buffer  = GL_BUFFER_VARIABLE,
				input 	= GL_PROGRAM_INPUT,
				output  = GL_PROGRAM_OUTPUT
			};
		public:
			program();
			~program();

			friend bool operator==(const program &prg1, const program &prg2);
			friend bool operator!=(const program &prg1, const program &prg2);

			program_type get_program_type() const;
			void attach(shader *shdr);
			void finalize();
			void bind() const;
			void unbind() const;
			void mat4(std::string_view name, const glm::mat4 &mat);
			void vec3(std::string_view name, const glm::vec3 &vec);
		private:	
			inline static std::map<shader_type, unsigned int> shader_type_lookup_ = {
				{ shader_type::vertex, GL_VERTEX_SHADER_BIT },
				{ shader_type::tess_control, GL_TESS_CONTROL_SHADER_BIT},
				{ shader_type::tess_eval, GL_TESS_EVALUATION_SHADER_BIT},
				{ shader_type::geometry, GL_GEOMETRY_SHADER_BIT},
				{ shader_type::fragment, GL_FRAGMENT_SHADER_BIT},
				{ shader_type::compute, GL_COMPUTE_SHADER_BIT}
			};

			void parse_interface(high_level_interface high, low_level_interface low);
			void parse_interface(low_level_interface low);
			void parse_program();

			bool is_final_ = false;

			std::vector<shader*> shaders_;
			std::unordered_map<std::string, const uniform_data> uniforms_;
			std::vector<block_data> ubos_;
			std::vector<block_data> ssbos_;
		};

		__FORCE_INLINE program::program_type program::get_program_type() const
		{
			program_type type = 0;
			for (const shader *shdr : shaders_)
			{
				type |= shader_type_lookup_[shdr->get_shader_type()];
			}

			return type;
		}
		__FORCE_INLINE void program::attach(shader *shdr)
		{
			if (!is_final_)
			{
				glAttachShader(id_, shdr->id());
				shaders_.emplace_back(shdr);
			}
		}
		

		inline bool operator==(const program &prg1, const program &prg2)
		{
			return prg1.id_ == prg2.id_;
		}
		inline bool operator!=(const program &prg1, const program &prg2)
		{
			return !(operator==(prg1, prg2));
		}
		inline void program::mat4(std::string_view name, const glm::mat4 &mat)
		{
			glUniformMatrix4fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &mat[0][0]);
		}
		inline void program::vec3(std::string_view name, const glm::vec3 &vec)
		{
			glUniform3fv(glGetUniformLocation(id_, name.data()), 1, &vec[0]); 
		}
		inline void program::bind() const
		{
			glUseProgram(id_);
		}
		inline void program::unbind() const
		{
			glUseProgram(0);
		}
	}
}

#endif
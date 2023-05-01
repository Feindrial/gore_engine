#include "gl/program.h"

#include "glad/glad.h"

#include "util/file_system.h"
#include "util/error_handler.h"
#include "gl/uniform_map.h"
#include "memory/buffer_storage.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

namespace gore
{
	namespace gl
	{
		program::program() : globject{ glCreateProgram() }
		{
			//glProgramParameteri(id_ , GL_PROGRAM_SEPARABLE, GL_TRUE);
		}
		program::~program()
		{
		}
	
		void program::parse_interface(low_level_interface low)
		{
			std::vector<unsigned int> lowlevelprops;
			const std::vector<unsigned int> unifprops = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_LOCATION
			};
			const std::vector<unsigned int> ioprops = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_LOCATION,
			};

			if (low == low_level_interface::uniform)
				lowlevelprops = unifprops;
			else if (low == low_level_interface::input || low == low_level_interface::output)
				lowlevelprops = ioprops;
			else
			{
				// incorrect parameter
				return;
			}

			int numlow;
			glGetProgramInterfaceiv(id_, low, GL_ACTIVE_RESOURCES, &numlow);

			for (size_t i = 0; i < numlow; i++) 
			{
				int lowprop[lowlevelprops.size()];
				glGetProgramResourceiv(id_, low, i, lowlevelprops.size(), lowlevelprops.data(), lowlevelprops.size(), nullptr, lowprop);

				std::vector<char> name_data(lowprop[0]);
    			glGetProgramResourceName(id_, low, i, name_data.size(), nullptr, &name_data[0]);
    			std::string name(name_data.begin(), name_data.end() - 1);

				if (low == low_level_interface::uniform)
				{
					if (lowprop[3] == -1)
						continue;
					else
						uniforms_.try_emplace(name, uniform_map[lowprop[1]], lowprop[2], lowprop[7]);
				}
				else if (low == low_level_interface::input)
				{
					// create vao
				}
				else if (low == low_level_interface::output)
				{
					// create framebuffer
				}
				
				std::cout << name << '\n';
			}
		}
		void program::parse_interface(high_level_interface high, low_level_interface low)
		{
			std::vector<unsigned int> lowlevelprops;
			const std::vector<unsigned int> unifprops = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE
			};
			const std::vector<unsigned int> buffprops = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_TOP_LEVEL_ARRAY_SIZE,
				GL_TOP_LEVEL_ARRAY_STRIDE
			};
			const std::vector<unsigned int> highlevelprop = { 
				GL_BUFFER_BINDING,
				GL_NUM_ACTIVE_VARIABLES,
			    GL_BUFFER_DATA_SIZE
			};
			const std::vector<unsigned int> active = { GL_ACTIVE_VARIABLES };

			if (low == low_level_interface::uniform)
				lowlevelprops = unifprops;
			else if (low == low_level_interface::buffer)
				lowlevelprops = buffprops;
			else
			{
				// incorrect parameter
				return;
			}

			int numhigh;
			glGetProgramInterfaceiv(id_, high, GL_ACTIVE_RESOURCES, &numhigh);

			for (size_t i = 0; i < numhigh; i++)
			{
				int highprop[highlevelprop.size()];
				glGetProgramResourceiv(id_, high, i, highlevelprop.size(), highlevelprop.data(), highlevelprop.size(), nullptr, highprop);

				std::vector<int> lowindices(highprop[1]);
				glGetProgramResourceiv(id_, high, i, active.size(), active.data(), highprop[1], nullptr, &lowindices[0]);

				block_data data;

				int lowprop[lowlevelprops.size()];
				for (size_t j = 0; j < highprop[1]; j++) 
				{
					glGetProgramResourceiv(id_, low, lowindices[j], lowlevelprops.size(), lowlevelprops.data(), lowlevelprops.size(), nullptr, lowprop);

					std::vector<char> name_data(lowprop[0]);
    				glGetProgramResourceName(id_, low, lowindices[j], name_data.size(), nullptr, &name_data[0]);
    				std::string name(name_data.begin(), name_data.end() - 1);

					data.variables_.push_back({
						name,
						uniform_map[lowprop[1]],
						static_cast<unsigned int>(lowprop[2]),
						static_cast<unsigned int>(lowprop[3])
					});
					
					std::cout << name << '\n';
				}

				if (high == high_level_interface::uniform_block)
				{
					ubos_.emplace_back(data);
					buffer_storage::get()->reserve_ubo_data()
				}
				else if (high == high_level_interface::shader_storage_block)
				{
					//no decleared size array
					if (!lowprop[7] || !lowprop[2])
					{
						
					}
					
					ssbos_.emplace_back(data);
				}
			}
		}
		
		void program::parse_program()
		{
			const unsigned int bufferprops[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_TOP_LEVEL_ARRAY_SIZE,
				GL_TOP_LEVEL_ARRAY_STRIDE
			};
			const unsigned int unifprops[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_LOCATION
			};
			const unsigned int iprops[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_LOCATION,
			};
			const unsigned int oprops[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_ARRAY_SIZE,
				GL_LOCATION,
				GL_LOCATION_INDEX,
				GL_LOCATION_COMPONENT
			};
			const unsigned int bufferprop[] = { GL_BUFFER_BINDING, GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_DATA_SIZE };
			const unsigned int active[] = { GL_ACTIVE_VARIABLES };
			
			auto fnc = [](auto pntr, int length){
				std::cout << "{";
				for (size_t i = 0; i < length; i++)
				{
					std::cout << pntr[i] << ", ";
				}
				std::cout << "}\n";
			};
			

			// parse_interface(high_level_interface::shader_storage_block, low_level_interface::buffer);
			// parse_interface(high_level_interface::uniform_block, low_level_interface::uniform);
			// parse_interface(low_level_interface::uniform);
			// return;
			int oldunifnnum;
			glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &oldunifnnum);
			int oldubonum;
			glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCKS, &oldubonum);
			int atomic;
			glGetProgramInterfaceiv(id_, GL_ATOMIC_COUNTER_BUFFER, GL_ACTIVE_RESOURCES, &atomic);
			int vrtx;
			glGetProgramInterfaceiv(id_, GL_VERTEX_SUBROUTINE, GL_ACTIVE_RESOURCES, &vrtx);
			int frgmnt;
			glGetProgramInterfaceiv(id_,  GL_FRAGMENT_SUBROUTINE, GL_ACTIVE_RESOURCES, &frgmnt);
			int numinput;
			glGetProgramInterfaceiv(id_, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numinput);
			std::cout << "------INPUT-------\n\n";
			for (size_t i = 0; i < numinput; i++) 
			{
				int unifprop[5];
				glGetProgramResourceiv(id_, GL_PROGRAM_INPUT, i, 5, iprops, 5, nullptr, unifprop);
				fnc(unifprop, 5);

				std::vector<char> nameData(unifprop[0]);
    			glGetProgramResourceName(id_, GL_PROGRAM_INPUT, i, nameData.size(), NULL, &nameData[0]);
    			std::string name(nameData.begin(), nameData.end() - 1);
				std::cout << name << '\n';
				std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_PROGRAM_INPUT, name.c_str()) << '\n';
			}
			int numoutput;
			glGetProgramInterfaceiv(id_, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numoutput);
			std::cout << "------OUTPUT-------\n\n";
			for (size_t i = 0; i < numoutput; i++) 
			{
				int unifprop[6];
				glGetProgramResourceiv(id_, GL_PROGRAM_OUTPUT, i, 6, oprops, 6, nullptr, unifprop);
				fnc(unifprop, 6);

				std::vector<char> nameData(unifprop[0]);
    			glGetProgramResourceName(id_, GL_PROGRAM_OUTPUT, i, nameData.size(), NULL, &nameData[0]);
    			std::string name(nameData.begin(), nameData.end() - 1);
				std::cout << name << '\n';
				std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_PROGRAM_OUTPUT, name.c_str()) << '\n';
			}
			int numbuffervar;
			glGetProgramInterfaceiv(id_, GL_BUFFER_VARIABLE, GL_ACTIVE_RESOURCES, &numbuffervar);
			std::cout << "------BUFFER VAR-------\n\n";
			for (size_t i = 0; i < numbuffervar; i++) 
			{
				int unifprop[9];
				glGetProgramResourceiv(id_, GL_BUFFER_VARIABLE, i, 9, bufferprops, 9, nullptr, unifprop);
				fnc(unifprop, 9);

				std::vector<char> nameData(unifprop[0]);
    			glGetProgramResourceName(id_, GL_BUFFER_VARIABLE, i, nameData.size(), NULL, &nameData[0]);
    			std::string name(nameData.begin(), nameData.end() - 1);
				std::cout << name << '\n';
				std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_BUFFER_VARIABLE, name.c_str()) << '\n';
			}
			int numunif;
			glGetProgramInterfaceiv(id_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numunif);
			std::cout << "----UNIFORMS----\n\n";
			for (size_t i = 0; i < numunif; i++) 
			{
				int unifprop[8];
				glGetProgramResourceiv(id_, GL_UNIFORM, i, 8, unifprops, 8, nullptr, unifprop);
				fnc(unifprop, 8);

				std::vector<char> nameData(unifprop[0]);
    			glGetProgramResourceName(id_, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
    			std::string name(nameData.begin(), nameData.end() - 1);
				std::cout << name << '\n';
				std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_UNIFORM, name.c_str()) << '\n';
				std::cout << "oldapiloc: " << glGetUniformLocation(id_, name.data()) << '\n';
			}
			int numubo;
			glGetProgramInterfaceiv(id_, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numubo);
			std::cout << "------UBOS-------\n\n";
			for (size_t i = 0; i < numubo; i++)
			{
				int ssboprop[3];
				glGetProgramResourceiv(id_, GL_UNIFORM_BLOCK, i, 3, bufferprop, 3, nullptr, ssboprop);
				std::cout << "ubo infos";
				fnc(ssboprop, 3);

				std::vector<int> ssbos(ssboprop[1]);
				glGetProgramResourceiv(id_, GL_UNIFORM_BLOCK, i, 1, active, ssboprop[1], nullptr, &ssbos[0]);

				for (size_t j = 0; j < ssboprop[1]; j++) 
				{
					int unifprop[8];
					glGetProgramResourceiv(id_, GL_UNIFORM, ssbos[j], 8, unifprops, 8, nullptr, unifprop);
					fnc(unifprop, 8);

					std::vector<char> nameData(unifprop[0]);
    				glGetProgramResourceName(id_, GL_UNIFORM, ssbos[j], nameData.size(), NULL, &nameData[0]);
    				std::string name(nameData.begin(), nameData.end() - 1);
					std::cout << name << '\n';
					std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_UNIFORM, name.c_str()) << '\n';
					std::cout << "oldapiloc: " << glGetUniformLocation(id_, name.data()) << '\n';
				}
			}
			int numssbo;
			glGetProgramInterfaceiv(id_, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numssbo);
			std::cout << "------SSBOS----\n\n";
			for (size_t i = 0; i < numssbo; i++)
			{
				int ssboprop[3];
				glGetProgramResourceiv(id_, GL_SHADER_STORAGE_BLOCK, i, 3, bufferprop, 3, nullptr, ssboprop);
				std::cout << "ssbo infos";
				fnc(ssboprop, 3);

				std::vector<int> ssbos(ssboprop[1]);
				glGetProgramResourceiv(id_, GL_SHADER_STORAGE_BLOCK, i, 1, active, ssboprop[1], nullptr, &ssbos[0]);

				for (size_t j = 0; j < ssboprop[1]; j++) 
				{
					int unifprop[9];
					glGetProgramResourceiv(id_, GL_BUFFER_VARIABLE, ssbos[j], 9, bufferprops, 9, nullptr, unifprop);
					fnc(unifprop, 9);

					std::vector<char> nameData(unifprop[0]);
    				glGetProgramResourceName(id_, GL_BUFFER_VARIABLE, ssbos[j], nameData.size(), NULL, &nameData[0]);
    				std::string name(nameData.begin(), nameData.end() - 1);
					std::cout << name << '\n';
					std::cout << "loc: " << glGetProgramResourceIndex(id_, GL_BUFFER_VARIABLE, name.c_str()) << '\n';
				}
			}
			std::cout << "\n\n***************END*************";
		}
		void program::finalize() 
		{
			//TODO check if shaders is compat  

			if (!is_final_)
			{
				glLinkProgram(id_);

				int success;
				glGetProgramiv(id_, GL_LINK_STATUS, &success);
				if (!success)
					error_handler::error(non_fatal::program_link);

				parse_program();

				for (shader *shdr : shaders_)
				{
					shdr->finalize();
				}
			}
		}
	}
}
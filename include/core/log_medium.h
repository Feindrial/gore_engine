#pragma once

#include <string_view>
#include <fstream>

namespace gore
{
	namespace error
	{
		class log_medium 
		{
		public:
			virtual ~log_medium() = default;

			virtual void write(const char *message) = 0;
			virtual void flush() = 0;
		};

		class filelog_medium: public log_medium 
		{
		public:
			filelog_medium(std::string_view filename);

			void write(const char *message) override;
			void flush() override;
		private:
			std::ofstream out_file_;
		};
		class consolelog_medium : public log_medium 
		{
		public:
			void write(const char *message) override;
			void flush() override;
		};
	}
}
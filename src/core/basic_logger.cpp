#include "core/basic_logger.h"

#include <memory>

namespace gore
{
	namespace error
	{
		extern std::shared_ptr<filelog_medium> basic_filelog = std::make_shared<filelog_medium>("gerr_main.txt");
		extern std::shared_ptr<consolelog_medium> basic_consolelog = std::make_shared<consolelog_medium>();

		extern logger f_logger({ basic_filelog });
		extern logger c_logger({ basic_consolelog });
		extern logger fc_logger({ basic_filelog, basic_consolelog });
	}
}
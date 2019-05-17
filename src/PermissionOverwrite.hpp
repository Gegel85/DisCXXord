#ifndef DISCXXORD_PERMISSIONOVERWRITE_HPP
#define DISCXXORD_PERMISSIONOVERWRITE_HPP


#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class PermissionOverwrite {
	public:
		PermissionOverwrite(json val);
	};
}


#endif //DISCXXORD_PERMISSIONOVERWRITE_HPP

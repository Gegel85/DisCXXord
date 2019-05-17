#ifndef DISCXXORD_ROLE_HPP
#define DISCXXORD_ROLE_HPP


#include "Permissions.hpp"
#include "Snowflake.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class Client;

	class Role : public Snowflake {
	public:
		bool hoist;
		bool managed;
		bool mentionable;
		unsigned pos;
		unsigned color;
		std::string name;
		Permissions permissions;

		Role(Client &client, json obj);
		bool hasPermission(unsigned perm);
	};
}


#endif //DISCXXORD_ROLE_HPP

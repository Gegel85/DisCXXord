#ifndef DISCXXORD_ROLE_HPP
#define DISCXXORD_ROLE_HPP


#include <JsonParser.hpp>
#include "Permissions.hpp"
#include "Snowflake.hpp"

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

		Role(Client &client, JsonObject &obj);
		bool hasPermission(unsigned perm);
	};
}


#endif //DISCXXORD_ROLE_HPP

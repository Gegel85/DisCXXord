#ifndef DISCXXORD_SNOWFLAKE_HPP
#define DISCXXORD_SNOWFLAKE_HPP


#include <string>
#include <JsonObject.hpp>
#include "Date.hpp"

namespace DisCXXord
{
	class Client;

	class Snowflake {
	protected:
		Client &_parent;

	public:
		const std::string id;
		const Date createdAt;

		Snowflake(Client &client, JsonObject &obj);
		std::string timestamp() const;
		bool operator==(Snowflake &);
	};
}


#endif //DISCXXORD_SNOWFLAKE_HPP

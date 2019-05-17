#ifndef DISCXXORD_MEMBER_HPP
#define DISCXXORD_MEMBER_HPP


#include <string>
#include <vector>
#include "Date.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class Guild;
	class User;
	class Role;

	class Member {
	public:
		Guild &guild;
		User &user;
		bool muted;
		bool deafen;
		Date joinedAt;
		std::string nick;
		std::vector<Role *> roles;

		Member(Guild &guild, User &user, json object);
	};
}


#endif //DISCXXORD_MEMBER_HPP

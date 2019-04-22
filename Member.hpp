#ifndef DISCXXORD_MEMBER_HPP
#define DISCXXORD_MEMBER_HPP


#include "User.hpp"
#include "Role.hpp"

namespace DisCXXord
{
	class Guild;

	class Member {
	public:
		Guild &guild;
		const User &user;
		bool muted;
		bool deafen;
		Date joinedAt;
		std::string nick;
		std::vector<Role *> roles;

		Member(Guild &guild, const User &user, JsonObject &object);
	};
}


#endif //DISCXXORD_MEMBER_HPP

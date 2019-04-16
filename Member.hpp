#ifndef DISCXXORD_MEMBER_HPP
#define DISCXXORD_MEMBER_HPP


#include "User.hpp"

namespace DisCXXord {
	class Member {
	private:
		User &_user;

	public:
		Member(Client &client, User &user, JsonObject &object);
		const User &user() const;
		User &user();
	};
}


#endif //DISCXXORD_MEMBER_HPP

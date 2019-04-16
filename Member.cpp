#include "Member.hpp"

namespace DisCXXord {
	Member::Member(DisCXXord::Client &client, User &user, JsonObject &object) :
		_user(user)
	{}

	const User &Member::user() const
	{
		return this->_user;
	}

	User &Member::user()
	{
		return this->_user;
	}
}
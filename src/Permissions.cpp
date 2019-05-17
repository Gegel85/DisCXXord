//
// Created by Gegel85 on 22/04/2019.
//

#include "Permissions.hpp"

namespace DisCXXord
{
	Permissions::Permissions(unsigned int val) :
		value(val)
	{
	}

	bool Permissions::has(unsigned perm)
	{
		return (this->value & ADMINISTRATOR) || (this->value & perm);
	}
}

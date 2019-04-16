/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Date.cpp
*/

#include "Date.hpp"

namespace DisCXXord
{
	Date::Date(time_t time)
	{
		time /= 1000;
		this->_timestamp = *gmtime(&time);
	}

	Date::Date(std::string time)
	{
		int y,M,d,h,m;
		float s;

		sscanf(time.c_str(), "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);
		this->_timestamp.tm_year = y - 1900;
		this->_timestamp.tm_mon = M - 1;
		this->_timestamp.tm_mday = d;
		this->_timestamp.tm_hour = h;
		this->_timestamp.tm_min = m;
		this->_timestamp.tm_sec = (int)s;
	}

	std::string Date::toISO() const
	{
		char buffer[22];

		std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &this->_timestamp);
		return buffer;
	}
}
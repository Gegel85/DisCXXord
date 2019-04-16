/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Date.hpp
*/

#ifndef DISCXXORD_DATE_HPP
#define DISCXXORD_DATE_HPP


#include <ctime>
#include <string>

namespace DisCXXord
{
	class Date {
	private:
		tm _timestamp;

	public:
		explicit Date(time_t time);
		explicit Date(std::string time);
		std::string toISO() const;
	};
}


#endif //DISCXXORD_DATE_HPP

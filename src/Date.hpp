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
		static Date now();

		explicit Date(time_t time = 0);
		explicit Date(std::string time);
		std::string toISO() const;
	};
}


#endif //DISCXXORD_DATE_HPP

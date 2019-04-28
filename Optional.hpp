#ifndef DISCXXORD_OPTIONAL_HPP
#define DISCXXORD_OPTIONAL_HPP


#include "Exceptions.hpp"

namespace DisCXXord
{
	template <typename type>
	class Optional {
	private:
		type	*_value;

	public:
		Optional() : _value(nullptr) {};
		Optional(type &&val) : _value(new type(val)) {};
		~Optional() {
			this->destroy();
		};

		type &operator*()
		{
			if (this->_value)
				return *this->_value;
			throw EmptyValueException("operator*");
		};

		type *operator->()
		{
			if (this->_value)
				return this->_value;
			throw EmptyValueException("operator->");
		};

		const type &operator*() const
		{
			if (this->_value)
				return *this->_value;
			throw EmptyValueException("operator*");
		};

		const type *operator->() const
		{
			if (this->_value)
				return this->_value;
			throw EmptyValueException("operator->");
		};

		Optional<type> &operator=(const type &val)
		{
			this->destroy();
			this->_value = new type(val);
			return *this;
		};

		void destroy()
		{
			if (!*this)
				return;
			delete this->_value;
			this->_value = nullptr;
		};

		template<typename ...args>
		void emplace(args&& ...val)
		{
			this->destroy();
			this->_value = new type(std::forward<args>(val)...);
		};

		bool operator!() const
		{
			return !this->_value;
		};

		explicit operator bool() const
		{
			return static_cast<bool>(this->_value);
		};

		template<typename type2>
		bool operator==(Optional<type2> &val)
		{
			return (!*this && !val) || **this == *val;
		};
	};
}


#endif //DISCXXORD_OPTIONAL_HPP

#ifndef DISCXXORD_OPTIONAL_HPP
#define DISCXXORD_OPTIONAL_HPP


#include "Exceptions.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	template <typename type>
	class Optional {
	private:
		type	*_value;
		bool	_destroy = true;

	public:
		Optional() : _value(nullptr) {};
		Optional(json val) : _value(val.is_null() ? nullptr : new type(val)) {};
		Optional(type *val) : _value(val), _destroy(false) {};
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

		Optional<type> &operator=(const type &new_obj)
		{
			this->destroy();
			this->_value = new type(new_obj);
			this->_destroy = true;
			return *this;
		};

		Optional<type> &operator=(type *new_ptr)
		{
			this->destroy();
			this->_value = new_ptr;
			this->_destroy = false;
			return *this;
		};

		Optional<type> &operator=(json new_json)
		{
			this->destroy();
			if (new_json.is_null())
				this->_value = nullptr;
			else
				this->_value = new type(new_json);
			return *this;
		};

		void destroy()
		{
			if (!*this)
				return;
			if (this->_destroy)
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

#ifndef DISCXXORD_OPTIONAL_HPP
#define DISCXXORD_OPTIONAL_HPP


#include <memory>
#include "Exceptions.hpp"
#include "SharedPtr.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	template <typename type>
	class Optional {
	private:
		SharedPtr<type>	_value;

		void destroy()
		{
			this->_value = nullptr;
		};

	public:
		Optional(json val) : _value(val.is_null() ? nullptr : new type(val)) {};
		Optional(type *val = nullptr) : _value(val) {};
		Optional(type &val) : _value(new type(val)) {};
		Optional(const type &val) : _value(new type(val)) {};
		Optional(Optional<type> &other) = default;
		Optional(Optional<type> &&other) = default;
		Optional(const Optional<type> &other) : _value(other ? new type(*other) : nullptr) {}
		Optional(const Optional<type> &&other) : _value(other ? new type(*other) : nullptr) {}

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
				return &*this->_value;
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
				return &*this->_value;
			throw EmptyValueException("operator->");
		};

		template<typename assignType>
		Optional<type> &operator=(assignType &new_obj)
		{
			this->_value.reset(new type(new_obj));
			return *this;
		};

		Optional<type> &operator=(type &&new_obj)
		{
			this->_value.reset(new type(new_obj));
			return *this;
		};

		Optional<type> &operator=(type *new_ptr)
		{
			this->_value.reset(new_ptr);
			return *this;
		};

		Optional<type> &operator=(json new_json)
		{
			if (new_json.is_null())
				this->_value = nullptr;
			else
				this->_value.reset(new type(new_json));
			return *this;
		};

		bool operator!() const
		{
			return !this->_value;
		};

		template<typename type2>
		bool operator==(Optional<type2> &val)
		{
			return (!*this && !val) || **this == *val;
		};

		explicit operator bool() const
		{
			return static_cast<bool>(this->_value);
		};

		template<typename ...args>
		void emplace(args&& ...val)
		{
			this->destroy();
			this->_value.reset(new type(std::forward<args>(val)...));
		};

		void reset(type *new_ptr, bool destroyable = false)
		{
			this->_value.reset(new_ptr, destroyable);
		};
	};
}


#endif //DISCXXORD_OPTIONAL_HPP

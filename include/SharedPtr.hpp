#ifndef DISCXXORD_SHAREDPTR_HPP
#define DISCXXORD_SHAREDPTR_HPP


#include <vector>
#include <algorithm>
#include <iostream>

namespace DisCXXord
{
	extern std::vector<void *> __allocated_memory;

	template<typename type>
	class SharedPtr {
	private:
		type *_value;
		bool _destroyable = true;

		void _destroy()
		{
			if (!this->_value || !this->_destroyable)
				return;

			auto it = std::find(__allocated_memory.begin(), __allocated_memory.end(), _value);

			if (it == __allocated_memory.end()) {
				std::cerr << "DisCXXord::SharedPtr::_destroy: Corrupted allocated memory list" << std::endl;
				abort();
			}
			__allocated_memory.erase(it);
			if (std::find(__allocated_memory.begin(), __allocated_memory.end(), _value) == __allocated_memory.end())
				delete _value;
			this->_value = nullptr;
		}

	public:
		SharedPtr(type *elem, bool destroyable = true) :
			_value(elem),
			_destroyable(destroyable)
		{
			if (elem)
				__allocated_memory.emplace_back(elem);
		};

		SharedPtr(const SharedPtr<type> &val) :
			_value(const_cast<type *>(val.operator->())),
			_destroyable(val.isDestroyable())
		{
			if (this->_value)
				__allocated_memory.emplace_back(this->_value);
		}

		SharedPtr<type> &operator=(type *elem)
		{
			if (elem == this->_value)
				return *this;
			this->_destroy();
			this->_value = elem;
			if (elem) {
				__allocated_memory.emplace_back(elem);
				this->_destroyable = true;
			}
			return *this;
		}

		~SharedPtr() {
			this->_destroy();
		}

		void reset(type *ptr = nullptr, bool destroyable = true)
		{
			this->_destroy();
			this->_destroyable = destroyable;
			if (destroyable)
				__allocated_memory.emplace_back(ptr);
			this->_value = ptr;
		}

		bool isDestroyable() const
		{
			return this->_destroyable;
		}

		type *operator->()
		{
			return _value;
		}

		type &operator*()
		{
			return *_value;
		}

		bool operator!() const
		{
			return !this->_value;
		}

		const type *operator->() const
		{
			return _value;
		}

		const type &operator*() const
		{
			return *_value;
		}

		explicit operator type*()
		{
			return this->_value;
		}

		explicit operator bool() const
		{
			return this->_value != nullptr;
		}
	};
}


#endif //DISCXXORD_SHAREDPTR_HPP

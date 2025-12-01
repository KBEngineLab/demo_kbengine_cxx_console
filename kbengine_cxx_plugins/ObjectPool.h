#pragma once
#include <vector>

namespace KBEngine
{

	template<typename T>
	class ObjectPool
	{
	public:
		~ObjectPool()
		{
			for (T* t : objects_)
				delete t;
		}

		T* createObject()
		{
			if (!objects_.empty())
			{
				T* t = objects_.back();
				objects_.pop_back();
				return t;
			}
			return new T();
		}

		void reclaimObject(T* obj)
		{
			objects_.push_back(obj);
		}

	private:
		std::vector<T*> objects_;
	};

}

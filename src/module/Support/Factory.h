#pragma once

#include <map>

namespace Skuld
{
	template<typename T, typename Key, typename Func>
	class Factory
	{
	public:
		static Factory& Instance()
		{
			static Factory instance;
			return instance;
		}

		class Register
		{
		private:
			Key k;
		public:
			Register(std::function<Func> func, Key key) : k(key)
			{
				Factory::Instance().mReg[key] = func;
			}
			Key GetKey()
			{
				return k;
			}
		};

		template<typename ... Args>
		T* Create(Key key, Args... args)
		{
			return mReg[key](args...);
		}
	private:
		std::map<Key, std::function<Func>> mReg;
	};
}
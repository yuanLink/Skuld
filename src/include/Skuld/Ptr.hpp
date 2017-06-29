#pragma once
#include "IDisposable.h"
#include <type_traits>

namespace Skuld
{
	template<typename V>
	class HideDisposeAndAddRef : public V
	{
	private:
		void AddRef();
		void Dispose();
	};

	template<typename V>
	class Ptr
	{
	private:
		V* ptr;
	public:
		Ptr() {
			ptr = nullptr;
		}
		Ptr(Ptr<V>&& ptr) {
			this->ptr = ptr.ptr;
			ptr.ptr = nullptr;
		}
		Ptr(const Ptr<V>& ptr) {
			this->ptr = ptr.ptr;
			this->ptr->AddRef();
		}
		Ptr(V* _ptr) : ptr(_ptr) { }
		void Attach(V* v) {
			if (ptr) ptr->Dispose();
			ptr = v;
		}
		V* Detach() {
			V* v = ptr;
			ptr = nullptr;
			return v;
		}
		V** operator&() {
			return &ptr;
		}
		Ptr<V>& operator=(const Ptr<V>& m) {
			if (m.ptr) m.ptr->AddRef();
			if (ptr) ptr->Dispose();
			ptr = m.ptr;
			return *this;
		}
		Ptr<V>& operator=(Ptr<V>&& m) {
			if (ptr) ptr->Dispose();
			m.ptr = nullptr;
			ptr = m.ptr;
			return *this;
		}
		Ptr<V>& operator=(V* m) {
			if (ptr) ptr->Dispose();
			ptr = m;
			return *this;
		}
		operator V*() {
			return ptr;
		}
		HideDisposeAndAddRef<V>* operator->() {
			return static_cast<HideDisposeAndAddRef<V>*>(ptr);
		}
		const HideDisposeAndAddRef<V>* operator->() const {
			return static_cast<HideDisposeAndAddRef<V>*>(ptr);
		}
		template<typename T>
		T* As() {
			static_assert(std::is_base_of<IDisposable, T>::value, "T needs to be IDisposable based.");
			return dynamic_cast<T*>(ptr);
		}
		~Ptr() {
			static_assert(std::is_base_of<IDisposable, V>::value, "V needs to be IDisposable based.");
			if (ptr) ptr->Dispose();
		}
	};

	template<typename V>
	inline static Ptr<V> MakePtr(V* v) {
		return Ptr<V>(v);
	}
}
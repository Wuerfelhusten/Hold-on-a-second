#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "REX/REX/Singleton.h"

using namespace std::literals;

#include <xbyak/xbyak.h>
#include "Plugin.h"


namespace stl
{
	using namespace SKSE::stl;

	template <class T, size_t size = 5>
	void write_thunk_call(std::uintptr_t a_src)
	{
		T::func = SKSE::GetTrampoline().write_call<size>(a_src, T::thunk);
	}
}
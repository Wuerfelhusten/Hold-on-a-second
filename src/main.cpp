#include "PCH.h"
#include "DialogueEscBlocker.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
	{
		SKSE::PluginVersionData v;
		v.PluginName(Plugin::NAME);
		v.AuthorName("Wuerfelhusten"sv);
		v.PluginVersion(Plugin::VERSION);
		v.UsesAddressLibrary();
		v.UsesNoStructs();
		return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse, true);

	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v"s);
#ifndef NDEBUG
	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);
#endif

	SKSE::log::info("Game version: {}", skse->RuntimeVersion());

	// Install input handler after input is initialized
	if (auto* messaging = SKSE::GetMessagingInterface()) {
		const bool ok = messaging->RegisterListener([](SKSE::MessagingInterface::Message* msg) {
			if (!msg) return;
			if (msg->type == SKSE::MessagingInterface::kInputLoaded) {
				SKSE::log::trace("[Messaging] kInputLoaded -> installing handler");
				HOAS::Install();
			}
		});
		SKSE::log::info("Registered SKSE messaging listener: {}", ok);
	} else {
		SKSE::log::warn("SKSE messaging interface not available; installing handler immediately");
		HOAS::Install();
	}

	return true;
}

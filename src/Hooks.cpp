#include "Hooks.h"
#include "DialogueEscBlocker.h"

namespace Hooks
{
	struct ProcessInputQueue
	{
		static void thunk(RE::BSTEventSource<RE::InputEvent*>* dispatcher, RE::InputEvent* const* events)
		{
			const auto blocker = DialogueEscBlocker::GetSingleton();

			if (events && blocker->ProcessInputEvents(events))
			{
				constexpr RE::InputEvent* const dummy[] = { nullptr };
				func(dispatcher, dummy);

				blocker->OpenJournalMenu();
			}
			else
			{
				func(dispatcher, events);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct GetSpeakerAsActor
	{
		static bool thunk(RE::MenuTopicManager* topic, RE::Actor*& actor)
		{
			if (DialogueEscBlocker::GetSingleton()->m_blockVoiceSkip)
				return false;

			return func(topic, actor);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct JournalMenuProcessMessage
	{
		static RE::UI_MESSAGE_RESULTS thunk(RE::JournalMenu* a_menu, RE::UIMessage& a_message)
		{
			const auto type = a_message.type;
			if (type == RE::UI_MESSAGE_TYPE::kHide || type == RE::UI_MESSAGE_TYPE::kForceHide)
			{
				DialogueEscBlocker::GetSingleton()->m_blockVoiceSkip = false;
			}

			return func(a_menu, a_message);;
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_JournalMenu[0] };
			func = Vtbl.write_vfunc(0x4, &thunk);
		}
	};


	void Install()
	{
		SKSE::AllocTrampoline(28);

		JournalMenuProcessMessage::Install();

		REL::Relocation<std::uintptr_t> inputUnk(RELOCATION_ID(67315, 68617), 0x7B);
		stl::write_thunk_call<ProcessInputQueue>(inputUnk.address());

		constexpr auto offset = RELOCATION_ID(50618, 51512);

		REL::Relocation<std::uintptr_t> actor1(offset, REL::Relocate(0x188, 0x145));
		stl::write_thunk_call<GetSpeakerAsActor>(actor1.address());

		REL::Relocation<std::uintptr_t> actor2(offset, REL::Relocate(0x1CD, 0x185));
		stl::write_thunk_call<GetSpeakerAsActor>(actor2.address());

		SKSE::log::info("Installed Hooks!");
	}
}
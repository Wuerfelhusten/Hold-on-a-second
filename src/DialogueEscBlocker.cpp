#include "PCH.h"
#include "DialogueEscBlocker.h"

namespace
{
	// Open the Journal menu using the native callback (same path as the map uses).
	static void OpenJournalViaCallback()
	{
		using func_t = void(*)(bool);
		static REL::Relocation<func_t> toggleOpenJournal{ RELOCATION_ID(52428, 53327) };
		toggleOpenJournal(true);
	}

	class DialogueEscBlocker final : public RE::MenuEventHandler
	{
	public:
		static DialogueEscBlocker* GetSingleton()
		{
			static DialogueEscBlocker instance;
			return std::addressof(instance);
		}

		// Process keyboard ESC (Cancel) or gamepad Menu while DialogueMenu is open.
		bool CanProcess(RE::InputEvent* a_event) override
		{
			if (!a_event) {
				return false;
			}
			const auto ui = RE::UI::GetSingleton();
			if (!ui || !ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {
				return false;
			}
			if (ui->IsMenuOpen(RE::JournalMenu::MENU_NAME)) {
				return false;
			}
			if (a_event->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
				return false;
			}

			auto* btn = static_cast<RE::ButtonEvent*>(a_event);
			const auto device = btn->GetDevice();

			// Keyboard: ESC (Cancel)
			if (device == RE::INPUT_DEVICE::kKeyboard) {
				const auto id = btn->GetIDCode();
				const bool isEsc = id == static_cast<std::uint32_t>(REX::W32::DIK_ESCAPE);
				return isEsc && btn->QUserEvent() == RE::BSFixedString("Cancel");
			}

			// Gamepad: specific button id (16)
			if (device == RE::INPUT_DEVICE::kGamepad) {
				return btn->GetIDCode() == 16;
			}

			return false;
		}

		bool ProcessButton(RE::ButtonEvent* a_event) override
		{
			const auto ui = RE::UI::GetSingleton();
			if (ui && ui->IsMenuOpen(RE::JournalMenu::MENU_NAME)) {
				return false;
			}
			if (!a_event || !a_event->IsDown() || !ui || ui->IsPauseMenuDisabled()) {
				return true;
			}

			auto* btn = static_cast<RE::ButtonEvent*>(a_event);
			const auto device = btn->GetDevice();

			if (device == RE::INPUT_DEVICE::kKeyboard) {
				const auto id = btn->GetIDCode();
				const bool isEsc = id == static_cast<std::uint32_t>(REX::W32::DIK_ESCAPE);
				if (isEsc && btn->QUserEvent() == RE::BSFixedString("Cancel")) {
					SKSE::log::info("[DialogueEscBlocker] Open Journal (keyboard ESC)");
					OpenJournalViaCallback();
				}
			}
			else if (device == RE::INPUT_DEVICE::kGamepad) {
				if (btn->GetIDCode() == 16) {
					SKSE::log::info("[DialogueEscBlocker] Open Journal (gamepad id=16)");
					OpenJournalViaCallback();
				}
			}

			return true;
		}
	};
}

void HOAS::Install()
{
	if (const auto controls = RE::MenuControls::GetSingleton()) {
		auto* h = DialogueEscBlocker::GetSingleton();
		if (!h->registered) {
			controls->AddHandler(h);
			SKSE::log::info("[DialogueEscBlocker] Handler added");
		}
		controls->RegisterHandler(h);
		SKSE::log::info("[DialogueEscBlocker] Handler registered");
	} else {
		SKSE::log::warn("[DialogueEscBlocker] MenuControls not available at install time");
	}
}

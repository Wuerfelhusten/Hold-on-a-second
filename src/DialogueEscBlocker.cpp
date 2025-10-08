#include "DialogueEscBlocker.h"

bool DialogueEscBlocker::ProcessInputEvents(RE::InputEvent* const* a_events) {

	static constexpr std::array badMenus{
		RE::BarterMenu::MENU_NAME,
		RE::JournalMenu::MENU_NAME,
		RE::GiftMenu::MENU_NAME
	};

	const auto ui = RE::UI::GetSingleton();
	if (!ui || !ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME) || std::ranges::any_of(badMenus, [&](const auto& menuName) { return ui->IsMenuOpen(menuName); })) {
		return false;
	}

	for (auto event = *a_events; event; event = event->next) {

		if (event->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
			continue;
		}

		const auto button = event->AsButtonEvent();
		if (!button)
			continue;

		if (!button->IsDown()) {
			continue;
		}

		const auto device = button->GetDevice();

		// Keyboard: ESC (Cancel)
		if (device == RE::INPUT_DEVICE::kKeyboard) {
			const auto id = button->GetIDCode();
			const bool isEsc = id == static_cast<std::uint32_t>(REX::W32::DIK_ESCAPE);

			return isEsc && button->QUserEvent() == RE::UserEvents::GetSingleton()->cancel;
		}
		else if (device == RE::INPUT_DEVICE::kGamepad) {
			return button->GetIDCode() == 16;
		}
	}

	return false;
}

void DialogueEscBlocker::OpenJournalMenu()
{
	m_blockVoiceSkip = true;
	ToggleOpenJournal(true);
}
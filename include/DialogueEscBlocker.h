#pragma once

class DialogueEscBlocker : public REX::Singleton<DialogueEscBlocker>
{
public:

	bool ProcessInputEvents(RE::InputEvent* const* a_events);
	void OpenJournalMenu();

	bool m_blockVoiceSkip = false;

private:
	// Open the Journal menu using the map menu callback
	static void ToggleOpenJournal(bool unk1)
	{
		using func_t = decltype(&ToggleOpenJournal);
		static REL::Relocation<func_t> func{ RELOCATION_ID(52428, 53327) };
		func(unk1);
	}
};

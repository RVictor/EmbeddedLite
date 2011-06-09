#include "editorsettingscaret.h"
#include "editor_config.h"

EditorSettingsCaret::EditorSettingsCaret( wxWindow* parent )
		:EditorSettingsCaretBase( parent )
		, TreeBookNode<EditorSettingsCaret>()
{
	OptionsConfigPtr options = EditorConfigST::Get()->GetOptions();
    m_spinCtrlBlinkPeriod->SetValue(options->GetCaretBlinkPeriod());
	m_spinCtrlCaretWidth->SetValue(options->GetCaretWidth());
}

void EditorSettingsCaret::Save(OptionsConfigPtr options)
{
	options->SetCaretBlinkPeriod(m_spinCtrlBlinkPeriod->GetValue());
	options->SetCaretWidth(m_spinCtrlCaretWidth->GetValue());
}

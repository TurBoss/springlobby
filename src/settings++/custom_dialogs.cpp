#include "custom_dialogs.h"
#include <wx/icon.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/defs.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/event.h>
#include <wx/stattext.h>

#include "../images/springsettings.xpm"
#include "../images/springlobby.xpm"


wxWindow* CustomMessageBox::m_settingsWindow = 0;
wxWindow* CustomMessageBox::m_lobbyWindow = 0;

//CustomMessageBox* GetNonModalMsgBox()
//{
//
//}

CustomMessageBox::CustomMessageBox(wxIcon* icon ,wxWindow *parent, const wxString& message,
        const wxString& caption ,
        long style, const wxPoint& pos )
			: wxDialog(parent,-1,caption,pos,wxDefaultSize,style|wxFRAME_FLOAT_ON_PARENT|wxDEFAULT_DIALOG_STYLE)
{
	SetIcon(*icon);
	wxBoxSizer* box = new wxBoxSizer(wxVERTICAL);
	box->Add(0,15,1,wxEXPAND|wxALL);
	box->Add(new wxStaticText(this, -1, message), 0, wxALL| wxEXPAND, 20);
	box->Add(0,15,1,wxEXPAND|wxALL);
	box->Add(CreateButtonSizer(style));

	SetSizer(box);
    box->SetSizeHints( this );
}

CustomMessageBox::~CustomMessageBox()
{
}


void CustomMessageBox::OnCloseWindow(wxCloseEvent& event)
{
    //wxDialogBase::AcceptAndClose();
}

void CustomMessageBox::setLobbypointer(wxWindow* arg)
{
	m_lobbyWindow = arg;
}

 void CustomMessageBox::setSettingspointer(wxWindow* arg)
{
	m_settingsWindow = arg;
}

 wxWindow* CustomMessageBox::getLobbypointer()
{
//	 if (m_lobbyWindow==0)
//			 wxLogWarning(_T("null parent window in custom message dialog"));
	return m_lobbyWindow;
}

 wxWindow* CustomMessageBox::getSettingspointer()
{
//	 if (m_settingsWindow==0)
//		 wxLogWarning(_T("null parent window in custom message dialog"));
	return m_settingsWindow;
}

int customMessageBox( int whichIcon , const wxString& message,const wxString& caption,
		long style , int x, int y )
{
		wxWindow* parent;
		wxIcon* icon;
		switch (whichIcon)
		{
			case SL_MAIN_ICON:
				icon = new wxIcon(springlobby_xpm);
				parent = CustomMessageBox::getLobbypointer();
				break;
			case SS_MAIN_ICON:
				icon = new wxIcon(springsettings_xpm);
				parent = CustomMessageBox::getSettingspointer();
				break;
			default:
				icon = new wxIcon(wxNullIcon);
				parent = 0;
				break;

		}
		CustomMessageBox dlg(icon,parent,message,caption,style,wxPoint(x,y));
		int re = dlg.ShowModal();
		switch (re)
		{
			case wxID_OK: return wxOK;
			case wxID_CANCEL: return wxCANCEL;
			case wxID_YES: return wxYES;
			case wxID_NO: return wxNO;
		}
		return -1;
}

void customMessageBoxNoModal( int whichIcon , const wxString& message,const wxString& caption,
		long style , int x, int y )
{
		wxWindow* parent;
		wxIcon* icon;
		switch (whichIcon)
		{
			case SL_MAIN_ICON:
				icon = new wxIcon(springlobby_xpm);
				parent = CustomMessageBox::getLobbypointer();
				break;
			case SS_MAIN_ICON:
				icon = new wxIcon(springsettings_xpm);
				parent = CustomMessageBox::getSettingspointer();
				break;
			default:
				icon = new wxIcon(wxNullIcon);
				parent = 0;
				break;

		}
		static CustomMessageBox* s_nonModalMsgBox;
		s_nonModalMsgBox = new  CustomMessageBox(icon,parent,message,caption,style,wxPoint(x,y));

		s_nonModalMsgBox->Show(true);
}

CreditsDialog::CreditsDialog(wxWindow* parent,wxString title,int whichIcon) : wxDialog(parent,-1,title,wxDefaultPosition,
		wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxFRAME_FLOAT_ON_PARENT)
{
	wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
	text_ctrl = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);
	container->Add(text_ctrl,1,wxEXPAND);
	//container->Add(new wxButton(this,wxID_CLOSE),0);

	 container->Add(wxDialog::CreateButtonSizer(wxOK));
	SetSizer(container);
	wxIcon* icon;
	switch (whichIcon)
	{
		case SL_MAIN_ICON:
			icon = new wxIcon(springlobby_xpm);
			break;
		case SS_MAIN_ICON:
			icon = new wxIcon(springsettings_xpm);
			break;
		default:
			icon = new wxIcon(wxNullIcon);
			break;

	}
	SetIcon(*icon);
}

void CreditsDialog::AddCredit(wxString person,wxString message)
{
	wxFont* heavyFont = new wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD);
	text_ctrl->SetDefaultStyle(wxTextAttr(wxNullColour,wxNullColour,*heavyFont));
	text_ctrl->AppendText(person + _T(": "));
	text_ctrl->SetDefaultStyle(wxTextAttr(wxNullColour,wxNullColour,*wxNORMAL_FONT));
	text_ctrl->AppendText(message + _T("\n\n"));
}

CreditsDialog::~CreditsDialog()
{
}


#pragma once
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/listctrl.h>
#include <wx/listbook.h>
#include <wx/sckipc.h>
#include <wx/socket.h>
#include <wx/textdlg.h>
#include "Kuznyechik.hpp"
#include "mycrypto.hpp"
#endif 

void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history);
BYTE *wxSToStr(wxString _buffer, BYTE *_nullptr);
ByteBlock Encrypt(BYTE *_buffer);
ByteBlock Decrypt(BYTE *_buffer);

class MyThread : public wxThread
{
public:
	MyThread(wxTextCtrl *textbox, wxSocketClient *client) :  _textbox(textbox), _client(client)
	{
	}
	
	virtual void *Entry();
protected:
	wxTextCtrl *_textbox;
	wxSocketClient *_client;
};

class GUI_CL : public wxFrame
{
public:

	GUI_CL();
	void ConnectToServer(wxCommandEvent& event);
	void SendMsgToServer(wxCommandEvent& event);
	void ShutClient(wxCommandEvent& event);

private:
	wxSocketClient *_client;
	wxIPV4address addr;
	wxPanel *_MainPanel;
	wxButton *_sendButton;
	wxBitmapButton *_conButton;
	wxButton *_disButton;
	wxTextCtrl *_textbox;
	wxTextCtrl *_sendbox;
	wxString buffer;
	char *_buffer;
	MyThread *thread;
	wxCriticalSection m_pThreadCS;   
	//friend class MyThread;	
	wxString _name;
	wxBoxSizer *_sizer1;
	wxBoxSizer *_sizer2;
	wxBoxSizer *_sizer3;
	wxTextEntryDialog *_dial1;
	wxTextEntryDialog *_dial2;
	wxTextEntryDialog *_dial3;
	wxString ip;
	wxString service;
};


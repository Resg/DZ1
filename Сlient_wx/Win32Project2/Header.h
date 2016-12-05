#pragma once
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/listctrl.h>
#include <wx/listbook.h>
#include <wx/sckipc.h>
#include <wx/socket.h>
#include <wx/textdlg.h>
#include <wx/menu.h>
#include "Kuznyechik.hpp"
#include "mycrypto.hpp"
#endif 



void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history, BYTE *pass);
BYTE *wxSToStr(wxString _buffer, BYTE *_nullptr);
ByteBlock Encrypt(BYTE *_buffer, BYTE * key);
ByteBlock Decrypt(BYTE *_buffer, BYTE * key);
int BYTEstrlen(BYTE *str);
void BYTEstrcat(BYTE *str1, BYTE *str2);
void BYTEstrcat(BYTE *str1, char *str2);

class MyThread : public wxThread
{
public:
	MyThread(wxTextCtrl *textbox, wxSocketClient *client, wxString pass) :  _textbox(textbox), _client(client), _pass(pass)
	{
	}
	
	virtual void *Entry();
private:
	wxTextCtrl *_textbox;
	wxSocketClient *_client;
	wxString _pass;
};
class MyDialog : public wxDialog
{
public:
	MyDialog(wxFrame *a);
	void _okclick(wxCommandEvent& event);
	wxString &IP() { return _ip; }
	wxString &NAME() { return _name; }
	wxString &SERV() { return _serv; }
	wxString &PASS() { return _pass; }
private:
	wxPanel *_dPanel;
	wxButtonBase *_okButton;
	wxTextCtrl *_ipText;
	wxTextCtrl *_servText;
	wxTextCtrl *_nickText;
	wxTextCtrl *_passText;
	wxString _ip;
	wxString _name;
	wxStaticText *_EIp;
	wxStaticText *_ENm;
	wxStaticText *_ESrv;
	wxString _serv;
	wxString _pass;
	DECLARE_EVENT_TABLE()
};
class GUI_CL : public wxFrame
{
public:

	GUI_CL();
	void ConnectToServer(wxCommandEvent& event);
	void SendMsgToServer(wxCommandEvent& event);
	void ShutClient(wxCommandEvent& event);
	void ChangeSettings(wxCommandEvent& event);
	void ClearScreen(wxCommandEvent& event);

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
	MyDialog *_MyDialog;
	wxTextEntryDialog *_dial3;
	wxString ip;
	wxString service;
	wxString pass;
	wxMenuBar *_menubar;
	wxMenu *_menu;
	wxMenu *_clear;
	
};



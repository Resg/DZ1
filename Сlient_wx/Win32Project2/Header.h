#pragma once
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/listctrl.h>
#include <wx/listbook.h>
//#include <wx\base64.h>
//#include <boost\asio.hpp>
#include <wx/sckipc.h>
#include <wx/socket.h>
//#include <boost/thread/thread.hpp>
#include <thread>
#endif 



class MyThread : public wxThread
{
public:
	MyThread(wxTextCtrl *textbox, wxSocketClient *client) :  _textbox(textbox), _client(client)
	{
	}
	//~MyThread();
	virtual void *Entry();
protected:
	//GUI_CL *m_pHandler;
	wxTextCtrl *_textbox;
	wxSocketClient *_client;
};

class GUI_CL : public wxFrame
{
public:

	GUI_CL(const wxString& title);
	void ConnectToServer(wxCommandEvent& event);
	void SendMsgToServer(wxCommandEvent& event);
	//void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox);
	//static void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox);

private:
	wxSocketClient *_client;
	wxIPV4address addr;
	wxPanel *_MainPanel;
	wxButton *_sendButton;
	wxBitmapButton *_conButton;
	wxTextCtrl *_textbox;
	wxTextCtrl *_sendbox;
	wxString buffer;
	char *_buffer;
	MyThread *thread;
	wxCriticalSection m_pThreadCS;   
	friend class MyThread;	
	char *_name;
	wxBoxSizer *_sizer1;
	wxBoxSizer *_sizer2;
	wxBoxSizer *_sizer3;
};




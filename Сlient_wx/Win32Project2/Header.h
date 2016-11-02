#pragma once
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/listctrl.h>
#include <wx/listbook.h>
//#include <boost\asio.hpp>
#include <wx/sckipc.h>
#include <wx/socket.h>
#include <thread>
#endif 

class GUI_CL : public wxFrame
{
public:

	GUI_CL(); 
	void ConnectToServer(wxCommandEvent& event);
	void SendMsgToServer(wxCommandEvent& event);
	//void GetMsgFServer(wxCommandEvent& event);
	void GetMsgFServer();
	void test()
	{
		*_textbox << "sada";
	}

private:
	wxSocketClient *_client;
	wxIPV4address addr;
	wxPanel *_MainPanel;
	wxButton *_sendButton;
	wxBitmapButton *_conButton;
	wxTextCtrl *_textbox;
	wxTextCtrl *_sendbox;
	wxString buffer;
	char * _buffer;
	
};
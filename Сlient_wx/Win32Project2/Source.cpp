
#include "Header.h"
//#include "CL_CL.h"

void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox);

GUI_CL::GUI_CL(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(700, 600))
{
	
	_MainPanel = new wxPanel(this,- 1);
	wxIcon _conIcon(wxT("connect"));
	wxBitmap conIcon;
	conIcon.CopyFromIcon(_conIcon);
	_conButton = new wxBitmapButton(_MainPanel,wxID_UP,conIcon,wxPoint(600,100));
	_disButton = new wxButton(_MainPanel, wxID_APPLY, "disconnect", wxPoint(600, 400), wxDefaultSize);
	Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ConnectToServer));
	//Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::GetMsgFServer));
	_textbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(0, 0),	wxSize(500, 150), wxTE_MULTILINE | wxTE_READONLY);
	_sendbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(250, 250),wxSize(500, 150), wxTE_PROCESS_ENTER | wxTE_MULTILINE);
	_sendButton = new wxButton(_MainPanel, wxID_ABOUT, "send", wxPoint(600, 500), wxDefaultSize);
	//_sendbox->Disable();
	_textbox->Disable();
	_sendButton->Disable();
	_disButton->Disable();
	Connect(-1, wxEVT_TEXT_ENTER, wxCommandEventHandler(GUI_CL::SendMsgToServer));
	Connect(wxID_ABOUT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::SendMsgToServer));
	Connect(wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ShutClient));
	_sizer1 = new wxBoxSizer(wxVERTICAL);
	_sizer2 = new wxBoxSizer(wxHORIZONTAL);
	//_sizer3 = new wxBoxSizer(wxHORIZONTAL);
	//_sizer3->Add(new wxPanel(_MainPanel,-1));
	_sizer2->Add(_conButton, 0, wxALL, 1);
	_sizer2->Add(_disButton, 0, wxALL, 1);
	_sizer2->AddStretchSpacer(1);
	_sizer2->Add(_sendButton, 0, wxALL, 1);
	_sizer1->Add(_textbox, 2, wxALL | wxEXPAND, 1);
	_sizer1->Add(_sizer2, 0, wxALIGN_TOP | wxEXPAND);
	_sizer1->Add(_sendbox, 1, wxALL | wxEXPAND, 1);
	_MainPanel->SetSizer(_sizer1);
	_sizer1->Fit(this);
	_sizer1->SetSizeHints(this);
	Centre();
}

void GUI_CL::ShutClient(wxCommandEvent& event)
{
	_client->ShutdownOutput();
	_client->Discard();
	thread->Kill();
	_client->Destroy();
	_disButton->Disable();
	_sendbox->Disable();
	_textbox->Disable();
	_sendButton->Disable();
	_conButton->Enable();
}

void GUI_CL::SendMsgToServer(wxCommandEvent& event)
{
	buffer = _sendbox->GetValue();
	//_client->Write(_name, strlen(_name));
	_client->Write(buffer,strlen(buffer));
	_sendbox->Clear();
}

void GUI_CL::ConnectToServer(wxCommandEvent& event)
{
	wxString ip = "192.168.14.131";
	wxString service = "7770";
	addr.Hostname(ip);
	addr.Service(service);
	_client = new wxSocketClient(wxSOCKET_NONE);
	_client->Connect(addr, true);
	_name = wxT("default");
	_name = _sendbox->GetValue();
	_sendbox->Clear();
	_client->Write(_name, strlen(_name));
	_conButton->Enable(false);
	_sendbox->Enable();
	_textbox->Enable();
	_sendButton->Enable();
	_disButton->Enable();
	thread = new MyThread(_textbox, _client);
	if (thread->Create(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can’t create thread!"));
	}
	thread->SetPriority(WXTHREAD_DEFAULT_PRIORITY);
	thread->Run();
	//thread->Wait();
	//wxThread *t1 = new wxThread();
	//test();
}
void *MyThread::Entry()
{
	GetMsgFServer(_client, _textbox);
	return 0;
}

void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox)
{
	char *_buffer = new char[4096];
	//*_textbox << ("00000");
	wxString *wxBuff;
	for (;; Sleep(100))
	{
		//*_textbox << ("11111");
		memset(_buffer, 0, 4096);
		_client->Read(_buffer, 4096);
		//wxBuff = _buffer;
		*_textbox << (_buffer);
		*_textbox << ("\n");
		//_client->WaitForRead()
	}
	delete _buffer;
}
/*void Client::GetMFServer()
{
	buffer = new char[4096];
	for (;; Sleep(100))
	{
		memset(buffer, 0, strlen(buffer));
		//cout << "123" << endl;
		if (recv(client, buffer, 4096, NULL))
		{
			//std::cout << buffer << "\n";
		}
		if (_end == 1)
			break;
	}
	delete buffer;
}

void Client::SendMToServer(char* _text)
{
	//char *buffer = new char[4096];
	for (;; Sleep(100))
	{
		//memset(buffer, 0, 10);
		//cout << buffer << endl;
		//std::cin >> buffer;
		//send(client, name, strlen(buffer), NULL);
		send(client, _text, strlen(_text), NULL);
	}
	//delete buffer;
}

void Client::connectToServer(char *_name)
{
	connect(client, (sockaddr *)&hints, sizeof(hints));
	//send(client, name, strlen(name), NULL);
}*/
/*void GUI_CL::GetMsgFServer(wxCommandEvent& event)
{
char *_buffer = new char[4096];
for (;; Sleep(100))
{
memset(_buffer, 1, strlen(buffer));
//_client->Read(_buffer, strlen(buffer));
*_textbox << (_buffer);

}
delete _buffer;
}
*/
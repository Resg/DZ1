
#include "Header.h"
//#include "CL_CL.h"


GUI_CL::GUI_CL() : wxFrame(NULL, wxID_ANY, "Client", wxDefaultPosition, wxSize(700, 600))
{
	_MainPanel = new wxPanel(this,- 1);
	
	wxIcon _conIcon(wxT("connect"));
	wxBitmap conIcon;
	conIcon.CopyFromIcon(_conIcon);
	_conButton = new wxBitmapButton(_MainPanel,wxID_UP,conIcon,wxPoint(600,100));
	Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ConnectToServer));
	//Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::GetMsgFServer));
	_textbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(-1, -1),	wxSize(250, 150), wxTE_READONLY);
	_sendbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(250, 250),wxSize(250, 150), wxTE_PROCESS_ENTER);
	_sendbox->Disable();
	_sendButton = new wxButton(_MainPanel, wxID_ABOUT, "send", wxPoint(600, 500), wxDefaultSize);
	Connect(-1, wxEVT_TEXT_ENTER, wxCommandEventHandler(GUI_CL::SendMsgToServer));
	
}

void GUI_CL::SendMsgToServer(wxCommandEvent& event)
{
	//buffer = "dsad";
	buffer = _sendbox->GetValue();
	
	//buffer = "wdqw";
	//*_sendbox << (buffer);
	//Sleep(1000);
	//_sendbox->Copy();
	_client->Write(buffer,strlen(buffer));
	_sendbox->Clear();
}

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
}*/
void GUI_CL::GetMsgFServer()
{
	char *_buffer = new char[4096];
	for (;; Sleep(100))
	{
		memset(_buffer, 0, strlen(_buffer));
		_client->Read(_buffer, strlen(_buffer));
		*_textbox << (_buffer);

	}
	delete _buffer;
}
void GUI_CL::ConnectToServer(wxCommandEvent& event)
{
	wxString ip = "192.168.14.131";
	wxString service = "7770";
	addr.Hostname(ip);
	addr.Service(service);
	_client = new wxSocketClient(wxSOCKET_NONE);
	_client->Connect(addr, true);
	_conButton->Enable(false);
	_sendbox->Enable();
	//test();
	
	std::thread t(GetMsgFServer);
	t.join;
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
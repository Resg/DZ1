
#include "Header.h"
FILE *history;
void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history);

GUI_CL::GUI_CL() : wxFrame(NULL, wxID_ANY, wxT("Client"), wxDefaultPosition, wxSize(700, 600))
{
	
	_MainPanel = new wxPanel(this,- 1);
	_dial1 = new wxTextEntryDialog(this, wxT("Enter ip"), wxT("IP"), wxT("127.0.0.1"), wxOK |wxCENTRE, wxPoint(780, 440));
	_dial1->ShowModal();
	ip = _dial1->GetValue();
	_dial1->Destroy();
	_dial2 = new wxTextEntryDialog(this, wxT("Enter service"), wxT("Service"), wxT("7770"), wxOK , wxPoint(780, 440));
	_dial2->ShowModal();
	service = _dial2->GetValue();
	_dial2->Destroy();
	_dial3 = new wxTextEntryDialog(this, wxT("Enter Nickname"), wxT("Nickname"), wxT("default"), wxOK , wxPoint(780, 440));
	_dial3->ShowModal();
	_name = _dial3->GetValue();
	_dial3->Destroy();
	wxIcon _conIcon(wxT("connect"));
	wxBitmap conIcon;
	conIcon.CopyFromIcon(_conIcon);
	_conButton = new wxBitmapButton(_MainPanel,wxID_UP,conIcon,wxPoint(600,100));
	_disButton = new wxButton(_MainPanel, wxID_APPLY, "disconnect", wxPoint(600, 400), wxDefaultSize);
	Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ConnectToServer));
	_textbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(0, 0),	wxSize(500, 150), wxTE_MULTILINE | wxTE_READONLY);
	_sendbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(250, 250),wxSize(500, 150), wxTE_PROCESS_ENTER | wxTE_MULTILINE);
	_sendButton = new wxButton(_MainPanel, wxID_ABOUT, "send", wxPoint(600, 500), wxDefaultSize);
	_textbox->Disable();
	_sendButton->Disable();
	_disButton->Disable();
	Connect(-1, wxEVT_TEXT_ENTER, wxCommandEventHandler(GUI_CL::SendMsgToServer));
	Connect(wxID_ABOUT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::SendMsgToServer));
	Connect(wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ShutClient));
	_sizer1 = new wxBoxSizer(wxVERTICAL);
	_sizer2 = new wxBoxSizer(wxHORIZONTAL);
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
	_client->Shutdown();
	_client->Discard();
	thread->Kill();
	_client->Destroy();
	_disButton->Disable();
	//_sendbox->Disable();
	_textbox->Disable();
	_sendButton->Disable();
	_conButton->Enable();
	fclose(history);
}

void GUI_CL::SendMsgToServer(wxCommandEvent& event)
{
	buffer = _sendbox->GetValue();
	BYTE *_strbuff=new BYTE[4096];
	/*if (buffer.size() >= 16)
		_strbuff = new BYTE[buffer.size() + 1];
	else
		_strbuff = new BYTE[16];*/
	wxSToStr(buffer, _strbuff);
	ByteBlock asd = Encrypt(_strbuff);
	_strbuff = asd.byte_ptr();
	_client->Write(_strbuff, 4096);
	_sendbox->Clear();
	//delete _strbuff;
}

void GUI_CL::ConnectToServer(wxCommandEvent& event)
{
	//wxString ip = _sendbox->GetValue();
	//_sendbox->Clear();
	//wxString service = "7770";
	addr.Hostname(ip);
	addr.Service(service);
	_client = new wxSocketClient(wxSOCKET_NONE);
	_client->Connect(addr, true);
	//_name = wxT("default");
	//_name = _sendbox->GetValue();
	//_sendbox->Clear();
	_client->Write(_name, strlen(_name));
	_conButton->Enable(false);
	_sendbox->Enable();
	_textbox->Enable();
	_sendButton->Enable();
	_disButton->Enable();
	history = fopen("history.txt", "a+" );
	thread = new MyThread(_textbox, _client);
	if (thread->Create(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can’t create thread!"));
	}
	thread->SetPriority(WXTHREAD_DEFAULT_PRIORITY);
	thread->Run();
}
void *MyThread::Entry()
{
	GetMsgFServer(_client, _textbox, history);
	return 0;
}

void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history)
{
	char *_buffer = new char[4096];
	for (;; Sleep(100))
	{
		memset(_buffer, 0, 4096);
		_client->Read(_buffer, 4096);
		fwrite(_buffer, sizeof(char), strlen(_buffer), history);
		fwrite("\n", sizeof(char), 1, history);
		*_textbox << (_buffer);
		*_textbox << ("\n");
	}
	delete _buffer;
}


BYTE *wxSToStr(wxString _buffer, BYTE *_nullptr)
{
	//_nullptr = new char[_buffer.size()+1];
	for (int i = 0; i <= _buffer.size(); i++)
	{
		if (i == _buffer.size())
			_nullptr[i] = 0;
		else
			_nullptr[i] = _buffer.c_str()[i];
	}
	return _nullptr;
}


ByteBlock Encrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 4096);
	ByteBlock _bytebstr2;
	hex_representation(_bytebstr1);
	std::vector<ByteBlock> bytevect=split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter= bytevect.begin();
	//ByteBlock _bytebstr2(_buffer, sizeof(_buffer));
	BYTE key[] = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
	ByteBlock _key(key, 32);
	Kuznyechik kuz(_key);
	kuz.encrypt(*_iter, _bytebstr2);
	//_buffer = _bytebstr2.byte_ptr();
	return  _bytebstr2;
	//_buffer = _bytebstr1.byte_ptr();
}

ByteBlock Decrypt(BYTE *_buffer)
{

}
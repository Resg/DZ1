
#include "Header.h"
FILE *history;
void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history);
bool handle = 1;

GUI_CL::GUI_CL() : wxFrame(NULL, wxID_ANY, wxT("Client"), wxDefaultPosition, wxSize(700, 600))
{
	_MyDialog = new MyDialog(this);
	_MyDialog->ShowModal();
	_MainPanel = new wxPanel(this,- 1);
	ip = _MyDialog->IP();
	service = _MyDialog->SERV();
	_name = _MyDialog->NAME();
	_MyDialog->Destroy();
	_menubar = new wxMenuBar;
	_menu = new wxMenu;
	_menu->Append(wxID_CONVERT, wxT("Settings"));
	_menubar->Append(_menu, wxT("Menu"));
	SetMenuBar(_menubar);
	Connect(wxID_CONVERT, wxEVT_COMMAND_MENU_SELECTED,	wxCommandEventHandler(GUI_CL::ChangeSettings));
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
	_client->ShutdownOutput();
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

void GUI_CL::ChangeSettings(wxCommandEvent& event)
{
	_MyDialog = new MyDialog(this);
	_MyDialog->ShowModal();
	_MainPanel = new wxPanel(this, -1);
	ip = _MyDialog->IP();
	service = _MyDialog->SERV();
	_name = _MyDialog->NAME();
	_MyDialog->Destroy();
}

void GUI_CL::SendMsgToServer(wxCommandEvent& event)
{
	buffer = _sendbox->GetValue();
	BYTE *_strbuff=new BYTE[1024];
	/*if (buffer.size() >= 16)
		_strbuff = new BYTE[buffer.size() + 1];
	else
		_strbuff = new BYTE[16];*/
	wxSToStr(buffer, _strbuff);
	ByteBlock asd = Encrypt(_strbuff);
	_strbuff = asd.byte_ptr();
	_client->Write(_strbuff, 1024);
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
	BYTE *_buffer = new BYTE[1024];
	for (;; Sleep(100))
	{
		memset(_buffer, 0, 1024);
		_client->Read(_buffer, 1024);
		ByteBlock datablock = Decrypt(_buffer);
		/*fwrite(_buffer, sizeof(char), strlen(_buffer), history);
		fwrite("\n", sizeof(char), 1, history);*/
		*_textbox << (datablock.byte_ptr());
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
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	//hex_representation(_bytebstr1);
	std::vector<ByteBlock> bytevect=split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter= bytevect.begin();
	//ByteBlock _bytebstr2(_buffer, sizeof(_buffer));
	BYTE key[] = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
	ByteBlock _key(key, 32);
	Kuznyechik kuz(_key);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.encrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	//_buffer = _bytebstr2.byte_ptr();
	return  _bytebstr2;
	//_buffer = _bytebstr1.byte_ptr();
}

ByteBlock Decrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	//std::string a=_buffer;
	//hex_to_bytes(_bytebstr1.byte_ptr())
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	//ByteBlock _bytebstr2(_buffer, sizeof(_buffer));
	BYTE key[] = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
	ByteBlock _key(key, 32);
	Kuznyechik kuz(_key);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.decrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	//_buffer = _bytebstr2.byte_ptr();
	return  _bytebstr2;
}

MyDialog::MyDialog(wxFrame *a) : wxDialog(a, wxID_EDIT, wxT("Start"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	_dPanel = new wxPanel(this, wxID_EDIT);
	_ipText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 10), wxDefaultSize);
	_ipText->AppendText(wxT("127.0.0.1"));
	_servText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 50), wxDefaultSize);
	_servText->AppendText(wxT("7770"));
	_nickText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 90), wxDefaultSize);
	_nickText->AppendText(wxT("Default"));
	Centre();
	_okButton = new wxButton(_dPanel, wxID_EXECUTE, wxT("Ok"), wxPoint(290, 180), wxDefaultSize);
	
	//Connect(wxID_EXECUTE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialog::_okclick));
	
}

/*
MyDialog::MyDialog() : wxFrame(NULL, wxID_EDIT, wxT("Start"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	_dPanel = new wxPanel(this, -1);
	_ipText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 10), wxDefaultSize);
	_ipText->AppendText(wxT("127.0.0.1"));
	_servText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 50), wxDefaultSize);
	_servText->AppendText(wxT("7770"));
	_nickText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 90), wxDefaultSize);
	_nickText->AppendText(wxT("Default"));
	_okButton = new wxButton(_dPanel, wxID_EXECUTE, wxT("Äàëåå"), wxPoint(290, 180), wxDefaultSize, wxBORDER_NONE);
	Connect(wxID_EXECUTE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialog::_okclick));
	Centre();
}*/

void MyDialog::_okclick(wxCommandEvent& event)
{
	_ip = _ipText->GetValue();
	_name = _nickText->GetValue();
	_serv = _servText->GetValue();
	handle = 0;
	Close();
}
BEGIN_EVENT_TABLE(MyDialog, wxDialog)
EVT_BUTTON(wxID_EXECUTE, MyDialog::_okclick)

END_EVENT_TABLE()

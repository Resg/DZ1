
#include "Header.h"
FILE *history;
bool _live=0;

GUI_CL::GUI_CL() : wxFrame(NULL, wxID_ANY, wxT("Client"), wxDefaultPosition, wxSize(700, 600))
{
	_MyDialog = new MyDialog(this);
	_MyDialog->ShowModal();
	_MainPanel = new wxPanel(this, -1);
	ip = _MyDialog->IP();
	pass = _MyDialog->PASS();
	service = _MyDialog->SERV();
	_name = _MyDialog->NAME();
	addr.Hostname(ip);
	BYTE buff[1024];
	memset(buff, 0, 1024);
	wxSToStr(pass, buff);
	ByteBlock datablock(buff, 32);
	pass = hex_representation(datablock);
	_MyDialog->Destroy();
	_menubar = new wxMenuBar;
	_menu = new wxMenu;
	_clear = new wxMenu;
	_menu->Append(wxID_CONVERT, wxT("Settings"));
	_clear->Append(wxID_CLEAR, wxT("Clear screen"));
	_menubar->Append(_menu, wxT("Menu"));
	_menubar->Append(_clear, wxT("Screen"));
	SetMenuBar(_menubar);
	Connect(wxID_CONVERT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GUI_CL::ChangeSettings));
	Connect(wxID_CLEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GUI_CL::ClearScreen));
	wxIcon _conIcon(wxT("connect"));
	wxBitmap conIcon;
	conIcon.CopyFromIcon(_conIcon);
	_conButton = new wxBitmapButton(_MainPanel, wxID_UP, conIcon, wxPoint(600, 100));
	_disButton = new wxButton(_MainPanel, wxID_APPLY, "disconnect", wxPoint(600, 400), wxDefaultSize);
	Connect(wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GUI_CL::ConnectToServer));
	_textbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(0, 0), wxSize(500, 150), wxTE_MULTILINE | wxTE_READONLY);
	_sendbox = new wxTextCtrl(_MainPanel, -1, wxT(""), wxPoint(250, 250), wxSize(500, 150), wxTE_PROCESS_ENTER | wxTE_MULTILINE);
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

void GUI_CL::ClearScreen(wxCommandEvent& event)
{
	_textbox->Clear();
}

void GUI_CL::ShutClient(wxCommandEvent& event)
{
	_live = 0;
	_client->ShutdownOutput();
	_client->Discard();
	_client->Destroy();
	if (thread->IsAlive())
	{
		thread->Kill();
	}
	_disButton->Disable();
	//_sendbox->Disable();
	//_textbox->Disable(false);
	_sendButton->Disable();
	_conButton->Enable();
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
	BYTE _strbuff[1024];
	wxSToStr(buffer, _strbuff);
	BYTE _1passbuff[1024];
	wxSToStr(pass, _1passbuff);
	ByteBlock asd = Encrypt(_strbuff, _1passbuff);
	_client->Write(asd.byte_ptr(), 1024);
	_sendbox->Clear();
}

void GUI_CL::ConnectToServer(wxCommandEvent& event)
{
	_live = 1;
	addr.Hostname(ip);
	BYTE buff[1024];
	memset(buff, 0, 1024);
	addr.Service(service);
	_client = new wxSocketClient(wxSOCKET_NONE);
	_client->Connect(addr, true);
	*_textbox << ("Connected\n");
	buff[0] = '1';
	_client->Write(buff, 1024);
	Sleep(110);
	memset(buff, 0, 1024);
	wxSToStr(pass, buff);
	_client->Write(buff, 1024);
	memset(buff, 0, 1024);
	Sleep(110);
	wxSToStr(_name, buff);
	_client->Write(buff, 1024);
	_conButton->Enable(false);
	_sendbox->Enable();
	_textbox->Enable();
	_sendButton->Enable();
	_disButton->Enable();
	//history = fopen("history.txt", "a+" );
	thread = new MyThread(_textbox, _client, pass);
	if (thread->Create(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can’t create thread!"));
	}
	thread->SetPriority(WXTHREAD_DEFAULT_PRIORITY);
	thread->Run();
}
void *MyThread::Entry()
{
	BYTE _passbuff[33];
	wxSToStr(_pass, _passbuff);
	GetMsgFServer(_client, _textbox, history, _passbuff);
	this->Kill();
	return 0;
}

void GetMsgFServer(wxSocketClient *_client, wxTextCtrl *_textbox, FILE *history, BYTE *pass)
{
	BYTE *_buffer = new BYTE[1024];
	while(_live)
	{
		memset(_buffer, 0, 1024);
		_client->Read(_buffer, 1024);
		ByteBlock datablock = Decrypt(_buffer, pass);
		*_textbox << (datablock.byte_ptr());
		*_textbox << ("\n");
		Sleep(100);
	}
	delete _buffer;
}


BYTE *wxSToStr(wxString _buffer, BYTE *_nullptr)
{
	for (int i = 0; i <= _buffer.size(); i++)
	{
		if (i == _buffer.size())
			_nullptr[i] = 0;
		else
			_nullptr[i] = _buffer.c_str()[i];
	}
	return _nullptr;
}


ByteBlock Encrypt(BYTE *_buffer, BYTE * key)
{
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	ByteBlock _keyb(key, 32);
	Kuznyechik kuz(_keyb);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.encrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	std::cout << _bytebstr2.byte_ptr() << std::endl;
	return  _bytebstr2;
}

ByteBlock Decrypt(BYTE *_buffer, BYTE * key)
{
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	ByteBlock _keyb(key, 32);
	Kuznyechik kuz(_keyb);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.decrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	std::cout << _bytebstr2.byte_ptr() << std::endl;
	return  _bytebstr2;
}

MyDialog::MyDialog(wxFrame *a) : wxDialog(a, wxID_EDIT, wxT("Start"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	_dPanel = new wxPanel(this, wxID_EDIT);
	_ipText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 10), wxDefaultSize);
	_ipText->AppendText(wxT("Enter IP"));
	_servText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 50), wxDefaultSize);
	_servText->AppendText(wxT("Enter service"));
	_nickText = new wxTextCtrl(_dPanel, -1, wxT(""), wxPoint(30, 90), wxDefaultSize);
	_passText = new wxTextCtrl(_dPanel, -1, wxT("Enter pass"), wxPoint(30, 130), wxDefaultSize, wxTE_PASSWORD);
	_nickText->AppendText(wxT("Enter nickname"));
	Centre();
	_okButton = new wxButton(_dPanel, wxID_EXECUTE, wxT("Ok"), wxPoint(290, 180), wxDefaultSize);
}

void MyDialog::_okclick(wxCommandEvent& event)
{
	_ip = _ipText->GetValue();
	_name = _nickText->GetValue();
	_serv = _servText->GetValue();
	_pass = _passText->GetValue();
	Close();
}


int BYTEstrlen(BYTE *str)
{
	int i = 0;
	while (str[i] != 0)
	{
		i++;
	}
	return i;
}

void BYTEstrcat(BYTE *str1, BYTE *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + BYTEstrlen(str2))) && (i<1024)); i++, k++)
		str1[i] = str2[k];
}
void BYTEstrcat(BYTE *str1, char *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + strlen(str2))) && (i<1024)); i++, k++)
		str1[i] = str2[k];
}


BEGIN_EVENT_TABLE(MyDialog, wxDialog)
EVT_BUTTON(wxID_EXECUTE, MyDialog::_okclick)
END_EVENT_TABLE()

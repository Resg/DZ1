
#include "Header.h"
//#include "CL_CL.h"

class MyApp : public wxApp
{
public:
	bool OnInit()
	{
		GUI_CL* GUI = new GUI_CL(wxT("Client"));
		START_Frame *start = new START_Frame(wxT("Start"));
		start->Show(true);
		GUI->Show(true);
		return true;
	}
};

IMPLEMENT_APP(MyApp);

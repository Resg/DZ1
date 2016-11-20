
#include "Header.h"

class MyApp : public wxApp
{
public:
	bool OnInit()
	{
		GUI_CL* GUI = new GUI_CL();
		GUI->Show(true);
		return true;
	}
};

IMPLEMENT_APP(MyApp);

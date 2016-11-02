
#include "Header.h"
//#include "CL_CL.h"

class MyApp : public wxApp
{
public:
	bool OnInit()
	{
		GUI_CL* GUI = new GUI_CL;
		GUI->Show();
		return true;
	}
};

IMPLEMENT_APP(MyApp);

#include "Application.h"


Application::Application()
{
	char szBuff[256];
	wsprintf(szBuff, "Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("�쐬"), MB_OK);
}

Application::~Application()
{
	char szBuff[256];
	wsprintf(szBuff, "�J��Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("�J��"), MB_OK);
};

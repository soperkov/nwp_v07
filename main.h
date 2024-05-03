#include <windows.h>
#include <tchar.h>
#include "nwpwin.h"

#include <gdiplus.h>

class gdiplus_application : public vsite::nwp::application
{
	Gdiplus::GdiplusStartupInput startup_input;
	ULONG_PTR           token;
public:
	gdiplus_application() {
		Gdiplus::GdiplusStartup(&token, &startup_input, nullptr);
	}
	~gdiplus_application() {
		Gdiplus::GdiplusShutdown(token);
	}
};

class main_window : public vsite::nwp::window {
	HBITMAP img_bmp = nullptr;
	tstring text;
	int bmp_width;
	int bmp_height;
public:
	main_window() : img_bmp(nullptr), text(_T("")) {}
		~main_window() { if (img_bmp) DeleteObject(img_bmp); DeleteObject(&text); }
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
	bool on_erase_bkgnd(HDC hdc) override;
};

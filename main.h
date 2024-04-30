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
	Gdiplus::Image* img = nullptr;
	TCHAR* text;
	HDC hdc;
public:
		main_window() : img(nullptr), text(nullptr) {}
		~main_window() { delete img; delete[] text; }
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
	bool on_erase_bkgnd(HDC hdc) override;
};

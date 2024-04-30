#include "main.h"
#include "rc.h"
#include <filesystem>

using namespace Gdiplus;

void draw_string(HDC hdc, TCHAR* text, RECT rc) {

	Graphics g(hdc);
	Font my_font(L"Arial", 16);
	RectF layout_rect(static_cast<float>(rc.left), 0.0f,
		static_cast<float>(rc.right - rc.left),
		static_cast<float>(rc.bottom - rc.top));
	layout_rect.Y = static_cast<float>(rc.bottom) - my_font.GetHeight(&g);
	layout_rect.Height = my_font.GetHeight(&g);
	SolidBrush white_brush(Color(255, 255, 255, 255));

	RectF shadow_rect(layout_rect);
	shadow_rect.Offset(1.5, 1.5);
	SolidBrush black_brush(Color(128, 0, 0, 0));

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	
	g.DrawString(text, -1, &my_font, shadow_rect, &format, &black_brush);
	g.DrawString(text, -1, &my_font, layout_rect, &format, &white_brush);
}

Image* check_scaling(HWND parent, Image* img) {
	int img_width = img->GetWidth();
	int img_height = img->GetHeight();
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
	float scale_factor = 1.0f;

	if (img_width > desktop_width && img_height > desktop_height) {
		scale_factor = min(static_cast<float>(desktop_width) / img_width, static_cast<float>(desktop_height) / img_height);
	}
	else if (img_width > desktop_width) {
		scale_factor = static_cast<float>(desktop_width) / img_width;
	}
	else if (img_height > desktop_height) {
		scale_factor = static_cast<float>(desktop_width) / img_height;
	}

	img_width = static_cast<int>(img_width * scale_factor);
	img_height = static_cast<int>(img_height * scale_factor);
	return img->GetThumbnailImage(img_width, img_height);
}


void main_window::on_paint(HDC hdc) 
{
	RECT rc;
	GetClientRect(*this, &rc);
	Graphics g(hdc);
	g.DrawImage(img, 0, 0, rc.right, rc.bottom);
	draw_string(hdc, text, rc);
}

void main_window::on_command(int id) 
{
	switch (id)
	{
		case ID_OPEN:
		{
			TCHAR path[MAX_PATH]; *path = 0;
			TCHAR filter[] = _T("Image Files\0*.jpg;*.bmp;*.png;*.gif;*.tiff;*.emf\0All Files (*.*)\0*.*\0");
			OPENFILENAME ofn{ sizeof OPENFILENAME };
			ofn.hwndOwner = *this;
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter;
			ofn.Flags = OFN_HIDEREADONLY;
			if (::GetOpenFileName(&ofn)) {
				img = check_scaling(*this, Image::FromFile(path));

				std::wstring filePath = std::filesystem::path(path).filename().wstring();
				size_t len = filePath.length() + 1;
				TCHAR* filePathTChar = new TCHAR[len];
				wcscpy_s(filePathTChar, len, filePath.c_str());
				text = filePathTChar;
				SetWindowPos(*this, nullptr, 0, 0, img->GetWidth(), img->GetHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				InvalidateRect(*this, nullptr, true);
			}
			break;
		}
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}


int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
 
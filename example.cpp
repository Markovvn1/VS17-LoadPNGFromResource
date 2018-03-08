#include <Windows.h>
#include <cstdio>
#include <gdiplus.h>
#include <iostream>
#include "resource.h"

using namespace Gdiplus;

using namespace std;

Bitmap* LoadFromResource(HINSTANCE hInst, LPCTSTR pName, LPCTSTR pType)
{
	HRSRC hResource = FindResource(hInst, pName, pType);
	if (!hResource) return NULL;

	DWORD imageSize = SizeofResource(hInst, hResource);
	if (!imageSize) return NULL;

	const void* pResourceData = LockResource(LoadResource(hInst, hResource));
	if (!pResourceData) return NULL;

	Bitmap* image;

	HGLOBAL m_hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = GlobalLock(m_hBuffer);

		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				image = Bitmap::FromStream(pStream);
				pStream->Release();
				if (image)
				{
					if (image->GetLastStatus() == Gdiplus::Ok)
					{
						GlobalUnlock(m_hBuffer);
						GlobalFree(m_hBuffer);
						return image;
					}

					delete image;
					image = NULL;
				}
			}
			image = NULL;
			GlobalUnlock(m_hBuffer);
		}
		GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return false;
}


int main()
{
	ULONG_PTR m_gdiplusToken;

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);




	HINSTANCE hInstance = GetModuleHandle(NULL);

	pBitmapClose = LoadFromResource(hInstance, MAKEINTRESOURCE(IDB_PNG1), "PNG");

	Color color;
	pBitmapClose->GetPixel(8, 8, &color);

	cout << (int)color.GetR() << ' ' << (int)color.GetG() << ' ' << (int)color.GetB() << endl;



	GdiplusShutdown(m_gdiplusToken);

	return 0;
}

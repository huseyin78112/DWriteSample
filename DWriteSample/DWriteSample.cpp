#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
HWND hWnd = NULL;
ID2D1Factory* m_pD2D1Factory = NULL;
ID2D1SolidColorBrush* m_pSolidBrush = NULL;
ID2D1HwndRenderTarget* m_pRenderTarget = NULL;
IDWriteFactory* m_pDWriteFactory = NULL;
IDWriteTextFormat* m_pTextFormat = NULL;
HRESULT Render();
HRESULT Resize(DWORD, DWORD);
wchar_t text[] = L"Hello World";
LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (Msg == WM_CLOSE)
    {
        DestroyWindow(hWnd);
    }
    else if (Msg == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    else if (Msg == WM_PAINT)
    {
        Render();
        ValidateRect(hWnd, NULL);
    }
    else if (Msg == WM_SIZE)
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        Resize(width, height);
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
wchar_t className[] = L"DWSample";
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr))
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_pD2D1Factory));
    }
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);
    }
    if (SUCCEEDED(hr))
    {
        WNDCLASSEX wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.hInstance = hInstance;
        wcex.lpszClassName = className;
        wcex.lpfnWndProc = WndProc;
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClassEx(&wcex);
        hWnd = CreateWindowEx(0, className, L"DirectWrite Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pD2D1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd), &m_pRenderTarget);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pSolidBrush);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextFormat(L"Segoe UI", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100.0f, L"en-US", &m_pTextFormat);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    }
    if (SUCCEEDED(hr))
    {
        ShowWindow(hWnd, nShowCmd);
        UpdateWindow(hWnd);
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return (int)msg.wParam;
    }
    return hr;
}
HRESULT Render()
{
    if (m_pRenderTarget != NULL && m_pTextFormat != NULL)
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
        D2D1_SIZE_F size = m_pRenderTarget->GetSize();
        D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width, size.height);
        m_pRenderTarget->DrawTextW(text, lstrlen(text), m_pTextFormat, rect, m_pSolidBrush);
        return m_pRenderTarget->EndDraw();
    }
    return E_FAIL;
}
HRESULT Resize(DWORD w, DWORD h)
{
    if (m_pRenderTarget != NULL)
    {
        return m_pRenderTarget->Resize(D2D1::SizeU(w, h));
    }
    return E_FAIL;
}
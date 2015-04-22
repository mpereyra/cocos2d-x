/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCEGLView.h"
#include "cocoa/CCSet.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "touch_dispatcher/CCTouch.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "text_input_node/CCIMEDispatcher.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "support/CCPointExtension.h"
#include "CCApplication.h"
#include "CCWinRTUtils.h"

#include "DLog.h"

#if (_MSC_VER >= 1800)
#include <d3d11_2.h>
#endif


using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::System;
using namespace Windows::UI::ViewManagement;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
using namespace Windows::Phone::UI::Input;
#endif

NS_CC_BEGIN

static CCEGLView* s_pEglView = NULL;

//////////////////////////////////////////////////////////////////////////
// impliment CCEGLView
//////////////////////////////////////////////////////////////////////////

// Initialize the DirectX resources required to run.
void WinRTWindow::Initialize(CoreWindow^ window, SwapChainPanel^ panel)
{
	m_window = window;

 	esInitContext ( &m_esContext );

    ANGLE_D3D_FEATURE_LEVEL featureLevel = ANGLE_D3D_FEATURE_LEVEL::ANGLE_D3D_FEATURE_LEVEL_9_1;

#if (_MSC_VER >= 1800)
    // WinRT on Windows 8.1 can compile shaders at run time so we don't care about the DirectX feature level
    featureLevel = ANGLE_D3D_FEATURE_LEVEL::ANGLE_D3D_FEATURE_LEVEL_ANY;
#endif


    HRESULT result = CreateWinrtEglWindow(WINRT_EGL_IUNKNOWN(panel), featureLevel, m_eglWindow.GetAddressOf());
	
	if (!SUCCEEDED(result))
	{
		CCLOG("Unable to create Angle EGL Window: %d", result);
		return;
	}

	m_esContext.hWnd = m_eglWindow;
    // width and height are ignored and determined from the CoreWindow the SwapChainBackgroundPanel is in.
    esCreateWindow ( &m_esContext, TEXT("Cocos2d-x"), 0, 0, ES_WINDOW_RGB | ES_WINDOW_ALPHA | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL );

	m_pointerPressedEvent = m_window->PointerPressed += 
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinRTWindow::OnPointerPressed);
        
    m_window->PointerReleased +=
        ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinRTWindow::OnPointerReleased);
    m_window->PointerMoved +=
        ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinRTWindow::OnPointerMoved);
    m_window->PointerWheelChanged +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinRTWindow::OnPointerWheelChanged);

	m_window->VisibilityChanged +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &WinRTWindow::OnVisibilityChanged);

	m_dummy = ref new Button();
	m_dummy->Opacity = 0.0;
	m_dummy->Width=1;
	m_dummy->Height=1;
	m_dummy->IsEnabled = true;
	panel->Children->Append(m_dummy);

	m_textBox = ref new TextBox();
	m_textBox->Opacity = 0.0;
	m_textBox->Width=1;
	m_textBox->Height=1;
	m_textBox->MaxLength = 1;

	panel->Children->Append(m_textBox);
	m_textBox->AddHandler(UIElement::KeyDownEvent, ref new KeyEventHandler(this, &WinRTWindow::OnTextKeyDown), true);
	m_textBox->AddHandler(UIElement::KeyUpEvent, ref new KeyEventHandler(this, &WinRTWindow::OnTextKeyUp), true);
	m_textBox->IsEnabled = false;

	auto keyboard = InputPane::GetForCurrentView();
	keyboard->Showing += ref new TypedEventHandler<InputPane^, InputPaneVisibilityEventArgs^>(this, &WinRTWindow::ShowKeyboard);
	keyboard->Hiding += ref new TypedEventHandler<InputPane^, InputPaneVisibilityEventArgs^>(this, &WinRTWindow::HideKeyboard);
	setIMEKeyboardState(false);

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &WinRTWindow::OnBackButtonPressed);
#endif
}

WinRTWindow::WinRTWindow(CoreWindow^ window) :
	m_lastPointValid(false),
	m_textInputEnabled(false)
{
	window->SizeChanged += 
	ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &WinRTWindow::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &WinRTWindow::OnLogicalDpiChanged);

	DisplayProperties::OrientationChanged +=
        ref new DisplayPropertiesEventHandler(this, &WinRTWindow::OnOrientationChanged);

	DisplayProperties::DisplayContentsInvalidated +=
		ref new DisplayPropertiesEventHandler(this, &WinRTWindow::OnDisplayContentsInvalidated);

	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &WinRTWindow::OnRendering));
}

extern "C" bool familyguy_cocos_was_backbutton_consumed = false;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
void WinRTWindow::OnBackButtonPressed(Platform::Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	familyguy_cocos_was_backbutton_consumed = false;
	CCDirector::sharedDirector()->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked);
	args->Handled = familyguy_cocos_was_backbutton_consumed;
}
#endif

void WinRTWindow::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	if (CCApplication::sharedApplication())
	{
		if (args->Visible)
			CCApplication::sharedApplication()->applicationWillEnterForeground();
		else
			CCApplication::sharedApplication()->applicationDidEnterBackground();
	}
}

void WinRTWindow::swapBuffers()
{
	eglSwapBuffers(m_esContext.eglDisplay, m_esContext.eglSurface);  
}



void WinRTWindow::OnSuspending()
{
#if (_MSC_VER >= 1800)
    Microsoft::WRL::ComPtr<ID3D11Device> device = m_eglWindow->GetAngleD3DDevice();
    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    HRESULT result = device.As(&dxgiDevice);
    if (SUCCEEDED(result))
    {
        dxgiDevice->Trim();
    }
#endif
}


void WinRTWindow::ResizeWindow()
{
     CCEGLView::sharedOpenGLView()->UpdateForWindowSizeChange();
}

CCPoint WinRTWindow::GetCCPoint(PointerEventArgs^ args) {
	auto p = args->CurrentPoint;
	float x = getScaledDPIValue(p->Position.X);
	float y = getScaledDPIValue(p->Position.Y);
    CCPoint pt(x, y);

	float zoomFactor = CCEGLView::sharedOpenGLView()->getFrameZoomFactor();

	if(zoomFactor > 0.0f) {
		pt.x /= zoomFactor;
		pt.y /= zoomFactor;
	}
	return pt;
}

void WinRTWindow::ShowKeyboard(InputPane^ inputPane, InputPaneVisibilityEventArgs^ args)
{
    CCEGLView::sharedOpenGLView()->ShowKeyboard(args->OccludedRect);
}

void WinRTWindow::HideKeyboard(InputPane^ inputPane, InputPaneVisibilityEventArgs^ args)
{
    CCEGLView::sharedOpenGLView()->HideKeyboard(args->OccludedRect);
}

void WinRTWindow::setIMEKeyboardState(bool bOpen)
{
	m_textInputEnabled = bOpen;
	if(m_textInputEnabled)
	{
		m_textBox->IsEnabled = true;
		m_textBox->Focus(FocusState::Pointer);
	}
	else
	{
		m_dummy->Focus(FocusState::Pointer);
		m_textBox->IsEnabled = false;
	}
}



void WinRTWindow::OnTextKeyDown(Object^ sender, KeyRoutedEventArgs^ args)
{
#if 0
	if(!m_textInputEnabled)
	{
		return;
	}

    auto key = args->Key;

    switch(key)
    {
    default:
        break;
    }
#endif
}

void WinRTWindow::OnTextKeyUp(Object^ sender, KeyRoutedEventArgs^ args)
{
	if(!m_textInputEnabled)
	{
		return;
	}

	args->Handled = true;

    auto key = args->Key;

    switch(key)
    {
    case VirtualKey::Escape:
        CCDirector::sharedDirector()->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked);
		args->Handled = true;
        break;
	case VirtualKey::Back:
        CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
        break;
    case VirtualKey::Enter:
		setIMEKeyboardState(false);
        CCIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
        break;
    default:
        char szUtf8[8] = {0};
        int nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)m_textBox->Text->Data(), 1, szUtf8, sizeof(szUtf8), NULL, NULL);
        CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(szUtf8, nLen);
        break;
    }	
	m_textBox->Text = "";
}

// set this variable externally (or add function to the winrtwindow ... which then needs to be accessed in game code)
// to change pointerwheel behaviour. 0 = do nothing, 1 = zoom, 2 = scroll. Should be made into an enum...
extern "C" int pointerwheel_mode = 1;

void WinRTWindow::OnPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args)
{
	if (!pointerwheel_mode) return;

	float direction = (float)args->CurrentPoint->Properties->MouseWheelDelta;

	if (pointerwheel_mode == 1)
	{
		// emulate a pinch sequence, need to have one intermediate move for the game
		// to do it's thing
		float dirscale = 0.5f;   // change this to affect speed
		float movedist = direction*dirscale;

		if (movedist > 100.f)  movedist = 100.f;
		if (movedist < -100.f) movedist = -100.f;

		int id1 = 995;
		int id2 = 996;

		CCPoint p1(210.0f, 0.0f);
		CCPoint p2(420.0f, 0.0f);
		CCEGLView::sharedOpenGLView()->handleTouchesBegin(1, &id1, &p1.x, &p1.y);
		CCEGLView::sharedOpenGLView()->handleTouchesBegin(1, &id2, &p2.x, &p2.y);

		p1.x -= movedist * 0.5f;
		p2.x += movedist * 0.5f;
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id1, &p1.x, &p1.y);
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id2, &p2.x, &p2.y);
		p1.x -= movedist * 0.5f;
		p2.x += movedist * 0.5f;
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id1, &p1.x, &p1.y);
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id2, &p2.x, &p2.y);

		CCEGLView::sharedOpenGLView()->handleTouchesEnd(1, &id2, &p2.x, &p2.y);
		CCEGLView::sharedOpenGLView()->handleTouchesEnd(1, &id1, &p1.x, &p1.y);
	}
	else
	{
	    float width = ConvertDipsToPixels(m_window->Bounds.Width);
	    float height = ConvertDipsToPixels(m_window->Bounds.Height);

		int id = 0;
		CCPoint p(width/2, height/2);
		CCEGLView::sharedOpenGLView()->handleTouchesBegin(1, &id, &p.x, &p.y);
		p.y += (direction * 0.5);
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id, &p.x, &p.y);
		p.y += (direction * 0.5);
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id, &p.x, &p.y);
		CCEGLView::sharedOpenGLView()->handleTouchesEnd(1, &id, &p.x, &p.y);
	}
}


void WinRTWindow::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	int id = args->CurrentPoint->PointerId;
    CCPoint pt = GetCCPoint(args);
	CCEGLView::sharedOpenGLView()->handleTouchesBegin(1, &id, &pt.x, &pt.y);
	
	// TODO, Jani: this is a workaround for the windows phone 8.1 compositor issue where the CompositionTarget::Rendering callback gets removed
	// if touch events are too many
	CompositionTarget::Rendering::remove(m_eventToken);
	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &WinRTWindow::OnRendering));
}

void WinRTWindow::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	auto currentPoint = args->CurrentPoint;
	if (currentPoint->IsInContact)
	{
		if (m_lastPointValid)
		{
			int id = args->CurrentPoint->PointerId;
			CCPoint p = GetCCPoint(args);
			CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id, &p.x, &p.y);
		}
		m_lastPoint = currentPoint->Position;
		m_lastPointValid = true;
	}
	else
	{
		m_lastPointValid = false;
	}
}

void WinRTWindow::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	int id = args->CurrentPoint->PointerId;
    CCPoint pt = GetCCPoint(args);
    CCEGLView::sharedOpenGLView()->handleTouchesEnd(1, &id, &pt.x, &pt.y);
}

using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;

#if WINAPI_FAMILY!=WINAPI_FAMILY_PHONE_APP
  using namespace Windows::UI::ViewManagement;
  static bool arewefullscreen = true;
#endif

void WinRTWindow::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
#if WINAPI_FAMILY!=WINAPI_FAMILY_PHONE_APP
	static bool snapped = false;
	arewefullscreen = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->IsFullScreen;

	if (!arewefullscreen && !snapped)
	{
		// not the case -> show popup!
		snapped = true;
		MessageDialog^ msg = ref new MessageDialog("Only full screen mode is supported!");
		msg->ShowAsync();
	}
#endif

	ResizeWindow();
	//CCEGLView::sharedOpenGLView()->UpdateForWindowSizeChange();
}

void WinRTWindow::OnLogicalDpiChanged(Object^ sender)
{
  CCEGLView::sharedOpenGLView()->UpdateForWindowSizeChange();
}

void WinRTWindow::OnOrientationChanged(Object^ sender)
{
	ResizeWindow();
  //CCEGLView::sharedOpenGLView()->UpdateForWindowSizeChange();
}

void WinRTWindow::OnDisplayContentsInvalidated(Object^ sender)
{
  CCEGLView::sharedOpenGLView()->UpdateForWindowSizeChange();
}

void WinRTWindow::OnRendering(Object^ sender, Object^ args)
{
	CCEGLView::sharedOpenGLView()->OnRendering();
}

void WinRTWindow::ReleasePointerPressed()
{
	m_window->PointerPressed -= m_pointerPressedEvent;
}

void WinRTWindow::EnablePointerPressed()
{
	m_pointerPressedEvent = m_window->PointerPressed += 
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinRTWindow::OnPointerPressed);
}

IWinrtEglWindow* WinRTWindow::GetAngleWindow()
{
  return m_eglWindow.Get();
}

CCEGLView::CCEGLView()
	: m_window(nullptr)
	, m_fFrameZoomFactor(1.0f)
	, m_bSupportTouch(false)
	, m_lastPointValid(false)
	, m_running(false)
	, m_winRTWindow(nullptr)
	, m_initialized(false)
  , m_preventSwap(false)
{
	s_pEglView = this;
    strcpy(m_szViewName, "Cocos2dxWinRT");
}

CCEGLView::~CCEGLView()
{
	CC_ASSERT(this == s_pEglView);
    s_pEglView = NULL;

	// TODO: cleanup 
}

bool CCEGLView::Create(CoreWindow^ window, SwapChainPanel^ panel)
{
    bool bRet = false;
	m_window = window;
  m_panel = panel;

	m_bSupportTouch = true;
	m_winRTWindow = ref new WinRTWindow(window);
	m_winRTWindow->Initialize(window, panel);
    m_initialized = false;
	UpdateForWindowSizeChange();
    return bRet;
}

bool CCEGLView::isOpenGLReady()
{
	// TODO: need to revisit this
    return (m_window.Get() != nullptr);
}

void CCEGLView::end()
{
	// TODO: need to implement

}

void CCEGLView::swapBuffers()
{
  if (!m_preventSwap)
  {
    m_winRTWindow->swapBuffers();
  }
}


void CCEGLView::setIMEKeyboardState(bool bOpen)
{
	if(m_winRTWindow) 
	{
		m_winRTWindow->setIMEKeyboardState(bOpen);
	}
}


void CCEGLView::resize(int width, int height)
{

}

void CCEGLView::setFrameZoomFactor(float fZoomFactor)
{
    m_fFrameZoomFactor = fZoomFactor;
    resize((int)(m_obScreenSize.width * fZoomFactor), (int)(m_obScreenSize.height * fZoomFactor));
    centerWindow();
    CCDirector::sharedDirector()->setProjection(CCDirector::sharedDirector()->getProjection());
}


float CCEGLView::getFrameZoomFactor()
{
    return m_fFrameZoomFactor;
}

void CCEGLView::setFrameSize(float width, float height)
{
	// not implemented in WinRT. Window is always full screen
    // CCEGLViewProtocol::setFrameSize(width, height);
}

void CCEGLView::centerWindow()
{
	// not implemented in WinRT. Window is always full screen
}

void CCEGLView::OnSuspending()
{
    if (m_winRTWindow)
    {
        m_winRTWindow->OnSuspending();
    }
}

CCEGLView* CCEGLView::sharedOpenGLView()
{
    return s_pEglView;
}

int CCEGLView::Run() 
{
	m_running = true; 

	return 0;
};

void CCEGLView::OnRendering()
{
  if (m_running && m_initialized && !m_preventSwap)
	{
#if WINAPI_FAMILY!=WINAPI_FAMILY_PHONE_APP
		if (!arewefullscreen)
		{
			// don't run the game
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
			swapBuffers();
		}
		else
		{
			CCDirector::sharedDirector()->mainLoop();
		}
#else
		CCDirector::sharedDirector()->mainLoop();
#endif
	}
}

void CCEGLView::HideKeyboard(Rect r)
{
    return; // not implemented
	float height = m_keyboardRect.Height;
	float factor = m_fScaleY / CC_CONTENT_SCALE_FACTOR();
	height = (float)height / factor;

	CCRect rect_end(0, 0, 0, 0);
	CCRect rect_begin(0, 0, m_obScreenSize.width / factor, height);

    CCIMEKeyboardNotificationInfo info;
    info.begin = rect_begin;
    info.end = rect_end;
    info.duration = 0;
    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardWillHide(info);
    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardDidHide(info);
}

void CCEGLView::ShowKeyboard(Rect r)
{
    return; // not implemented
	float height = r.Height;
	float factor = m_fScaleY / CC_CONTENT_SCALE_FACTOR();
	height = (float)height / factor;

	CCRect rect_begin(0.0f, 0.0f - height, m_obScreenSize.width / factor, height);
	CCRect rect_end(0.0f, 0.0f, m_obScreenSize.width / factor, height);

    CCIMEKeyboardNotificationInfo info;
    info.begin = rect_begin;
    info.end = rect_end;
    info.duration = 0;
    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardWillShow(info);
    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardDidShow(info);
	m_keyboardRect = r;
}


void CCEGLView::UpdateForWindowSizeChange()
{

	float width = ConvertDipsToPixels(m_window->Bounds.Width);
	float height = ConvertDipsToPixels(m_window->Bounds.Height);

	if(!m_initialized)
    {
        m_initialized = true;
        CCEGLViewProtocol::setFrameSize(width, height);
    }
    else
    {
        m_obScreenSize = CCSizeMake(width, height);
        CCSize designSize = getDesignResolutionSize();
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
        CCDirector::sharedDirector()->setProjection(CCDirector::sharedDirector()->getProjection());
        glClearColor(51 / 255.f, 153 / 255.f, 51 / 255.f, 255 / 255.f);
   }
}

void CCEGLView::openEditBox(CCEditBoxParam^ param)
{
	m_winRTWindow->ReleasePointerPressed();

	if (m_editBoxhandler)
	{		
		m_editBoxhandler->Invoke(nullptr, param);
	}	
}

void CCEGLView::SetCocosEditBoxHandler( EventHandler<Object^>^ handler )
{
	m_editBoxhandler = handler;
}

void CCEGLView::OnCloseEditBox()
{
	m_winRTWindow->EnablePointerPressed();
}

void CCEGLView::PreventSwap()
{
  m_preventSwap = true;
}

void CCEGLView::EnableSwap()
{
  m_preventSwap = false;
}

NS_CC_END

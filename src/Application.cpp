/*
Copyright (c) 2008 Jacob Essex

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/


#include "Application.h"
#include "Control.h"
#include <cassert>
//----------------------------------------------------------------------------------------------
Application* Application::mApp = 0;
//----------------------------------------------------------------------------------------------
BOOL CALLBACK Application::_dlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ){
	switch(Message){
			case WM_INITDIALOG:
				{
					onInit(hwnd); 
				}
				break;
			case WM_COMMAND:
				{
					ControlMapItr find = mObjects.find(LOWORD(wParam));
					if ( find != mObjects.end() ){
						find->second->callEventFunction(hwnd, wParam);
					}

				}
				break;
			case WM_CLOSE:
				if ( onClose() ){
					EndDialog(hwnd, 0);
				}
				break;
			default:
				return false;
	}
	return true;
}
//----------------------------------------------------------------------------------------------
Application::Application(){
	assert(!mApp);
	mApp = this;
}
//----------------------------------------------------------------------------------------------
Application::~Application(){
	for ( ControlMapItr iter = mObjects.begin(); iter != mObjects.end(); ++iter ){
		delete iter->second;
	}
}
//----------------------------------------------------------------------------------------------
BOOL CALLBACK Application::dlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ){
	assert(mApp);
	return mApp->_dlgProc(hwnd, Message, wParam, lParam);
}
//----------------------------------------------------------------------------------------------
void Application::onInit( HWND hwnd ){

}
//----------------------------------------------------------------------------------------------
bool Application::onClose(){
	return true;
}
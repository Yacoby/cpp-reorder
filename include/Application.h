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


#include <map>
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0601
#endif
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <boost/function.hpp>

class Control;
class Application{
	typedef std::map<int, Control*> ControlMap;
	typedef ControlMap::iterator ControlMapItr;
	ControlMap mObjects;

	BOOL CALLBACK _dlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static Application* mApp;

protected:
	template <typename T>
	T* registerObject( T* c, boost::function< void ( HWND,WPARAM ) > f )
	{
		assert(mObjects.find(c->getIDC())==mObjects.end());
		mObjects[c->getIDC()] = c;
		c->setEventFunction(f);
		return c;
	}

public:
	Application();
	virtual ~Application();
	static BOOL CALLBACK dlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	virtual void onInit(HWND hwnd);
	virtual bool onClose();




};
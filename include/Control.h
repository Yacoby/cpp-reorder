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


#ifndef CONTROL_H__
#define CONTROL_H__

#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0601
#endif
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <boost/function.hpp>

class Application;

/**
* @brief super class for items. Items must be inherited from this
*/
class Control{
private:
	/**
	* @brief used by the app class to trigger the held event function
	*/
	void callEventFunction(HWND hwnd, WPARAM param);
protected:
	HWND mControl, mParent;
	int mIDC;

	//Application* mAppPtr;

	boost::function< void ( HWND,WPARAM ) > mFunc;
public:
	friend Application;
	Control(HWND parent,HWND cont, int idc);
	Control(HWND parent, int idc);

	inline int getIDC(){
		return mIDC;
	}

	/**
	* @brief sets the function that is called when an event is triggered
	*/
	void setEventFunction(boost::function< void ( HWND,WPARAM ) > f);
};
#endif //#define CONTROL_H__
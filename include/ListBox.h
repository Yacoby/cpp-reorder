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


#include "Control.h"
#include <string>
#include <vector>
/**
* @brief provides basic features for the list box. Is basically a wrapper around the win32 functions
*/
class ListBox : public Control{
public:

	ListBox(HWND parent,HWND cont, int idc) : Control(parent, cont, idc){}
	ListBox(HWND parent, int idc) : Control(parent, idc){}

	std::vector<int> getSelectedItems();

	std::string getItem(int i);

	int getSelectedCount();

	void setSelected(int i);

	void addSelected(int i);

	//inset item at the given point
	void insert(const std::string& text, int index);
	///add item to end of list box
	void append(const std::string& text);

	int getCount();

	void setFocus(int index);

	void clear();

	void deleteItem(int i);

};
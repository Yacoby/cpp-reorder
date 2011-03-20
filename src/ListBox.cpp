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


#include "ListBox.h"
#include <boost/scoped_array.hpp>
#include <string>

//----------------------------------------------------------------------------------------------
std::vector<int> ListBox::getSelectedItems(){
	int count = getSelectedCount();
	std::vector<int> ret;
	int* buf =  new int[count];
	SendMessage(mControl, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);
	for ( int i = 0; i < count; ++i){
		ret.push_back(buf[i]);
	}
	return ret;
}
//----------------------------------------------------------------------------------------------
std::string ListBox::getItem( int i ){
	boost::scoped_array<char> temp(
		new char[SendMessage(mControl, LB_GETTEXTLEN,(WPARAM) i,(LPARAM) NULL)+1]
	);
	SendMessage(mControl, LB_GETTEXT, (WPARAM)i, (LPARAM)temp.get());
	return std::string(temp.get());
}
//----------------------------------------------------------------------------------------------
int ListBox::getSelectedCount(){
	return (int)SendMessage(mControl, LB_GETSELCOUNT, 0, 0);
}
//----------------------------------------------------------------------------------------------
void ListBox::setSelected( int i ){
	SendMessage(mControl, LB_SETSEL,(WPARAM) FALSE,(LPARAM) i);
}
//----------------------------------------------------------------------------------------------
void ListBox::addSelected( int i ){
	SendMessage(mControl, LB_SETSEL,(WPARAM) TRUE,(LPARAM) i);
}
//----------------------------------------------------------------------------------------------
void ListBox::insert( const std::string& text, int index ){
	SendMessage(mControl, LB_INSERTSTRING,(WPARAM) index,(LPARAM) text.c_str());
}
//----------------------------------------------------------------------------------------------
void ListBox::append( const std::string& text ){
	SendMessage(mControl, LB_INSERTSTRING,(WPARAM) getCount(),(LPARAM) text.c_str());
}
//----------------------------------------------------------------------------------------------
int ListBox::getCount(){
	return (int) SendMessage(mControl, LB_GETCOUNT, (WPARAM) NULL, (LPARAM) NULL);
}
//----------------------------------------------------------------------------------------------
void ListBox::setFocus( int index ){
	SendMessage( mControl,LB_SETCARETINDEX, (WPARAM) index,(LPARAM) FALSE);
}
//----------------------------------------------------------------------------------------------
void ListBox::clear(){
	assert(mParent);
	SendDlgItemMessage(mParent, mIDC, LB_RESETCONTENT, 0, 0);
}
//----------------------------------------------------------------------------------------------
void ListBox::deleteItem( int i ){
	SendMessage(mControl, LB_DELETESTRING, (WPARAM)i, 0);
}
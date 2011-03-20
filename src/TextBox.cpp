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


#include "TextBox.h"
#include <boost/scoped_array.hpp>
#include <string>

//----------------------------------------------------------------------------------------------
TextBox::TextBox( HWND parent,HWND cont, int idc ) : Control(parent, cont, idc){

}
//----------------------------------------------------------------------------------------------
TextBox::TextBox( HWND parent, int idc ) : Control(parent, idc){

}
//----------------------------------------------------------------------------------------------
int TextBox::getLength(){
	return GetWindowTextLength(mControl)+1;
}
//----------------------------------------------------------------------------------------------
std::string TextBox::getText(){
	const int len = getLength();
	boost::scoped_array<char> text(new char[len]);
	GetDlgItemText(mParent, mIDC, text.get(), len);
	return std::string(text.get());
}
//----------------------------------------------------------------------------------------------
void TextBox::setText( const std::string& p ){
	SetDlgItemText(mParent, mIDC, p.c_str());
}
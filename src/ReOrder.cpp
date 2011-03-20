#include "ReOrder.h"
#include <shlobj.h>
#include <shellapi.h>
//----------------------------------------------------------------------------------------------
bool hasExt(const std::string& file, const std::string& ext){
	return boost::iends_with(file,ext);
}
//----------------------------------------------------------------------------------------------
bool FileData::compareDate(FileData& a, FileData& b){
	return ( a.lastModified < b.lastModified );
}
//----------------------------------------------------------------------------------------------
std::vector<FileData> getSortedFiles(const std::string& path){
	std::vector<FileData> value;
	fs::directory_iterator end;
	for ( fs::directory_iterator dir_itr( path ); dir_itr != end;  ++dir_itr ) {
		if ( !fs::is_directory( dir_itr->status() ) && 
			( hasExt(dir_itr->leaf(), "esp") || hasExt(dir_itr->leaf(), "esm") ) ){
				FileData fd;
				fd.isEsp = hasExt(dir_itr->leaf(), "esp");
				fd.fileName = dir_itr->leaf();
				fd.lastModified = fs::last_write_time(path + dir_itr->leaf());
				value.push_back(fd);
		}
	}
	std::sort(value.begin(),value.end(), FileData::compareDate);
	return value;
}
//----------------------------------------------------------------------------------------------
std::string getRegKey(std::string pos, std::string name){
	HKEY hKey;
	TCHAR szProductType[1024];
	DWORD dwBufLen=1024*sizeof(TCHAR);
	LONG lRet;
	lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, pos.c_str(), 0, KEY_QUERY_VALUE, &hKey );
	if( lRet != ERROR_SUCCESS )	return "";
	lRet = RegQueryValueEx( hKey, name.c_str(),	NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
	RegCloseKey( hKey );

	if (( lRet != ERROR_SUCCESS )	||	(dwBufLen > 1024*sizeof(TCHAR)) ){
		return "";
	}
	return std::string(szProductType);
}


//----------------------------------------------------------------------------------------------
void ReOrderApp::onInit( HWND hwnd ){
	mESP		= registerObject(new ListBox(hwnd, IDC_ESP),  boost::bind( &ReOrderApp::onESP, this, _1, _2) );
	mESM		= registerObject(new ListBox(hwnd, IDC_ESM), boost::bind( &ReOrderApp::onESM, this, _1, _2) );
	mLocation	= registerObject(new TextBox(hwnd, IDC_TEXT), 0);
	registerObject(new Button(hwnd, IDC_TOP), boost::bind( &ReOrderApp::onTop, this, _1, _2));
	registerObject(new Button(hwnd, IDC_BOTTOM), boost::bind( &ReOrderApp::onBottom, this, _1, _2));
	registerObject(new Button(hwnd, IDC_UP), boost::bind(&ReOrderApp::onUp, this, _1, _2));
	registerObject(new Button(hwnd, IDC_DOWN), boost::bind(&ReOrderApp::onDown, this, _1, _2));
	registerObject(new Button(hwnd, IDC_REORDER),boost::bind(&ReOrderApp::onReOrder, this, _1, _2));
	registerObject(new Button(hwnd, IDC_RELOAD), boost::bind(&ReOrderApp::onReload, this, _1, _2));
	registerObject(new Button(hwnd, IDC_FOLDER), boost::bind(&ReOrderApp::onSelectDir, this, _1, _2));

	registerObject(new Button(hwnd, IDC_PRNTORDER), boost::bind(&ReOrderApp::onPrint, this, _1, _2));

	registerObject(new Button(hwnd, IDC_SORT_AZ), boost::bind(&ReOrderApp::onSortAZ, this, _1, _2));
	registerObject(new Button(hwnd, IDC_SORT_ZA), boost::bind(&ReOrderApp::onSortZA, this, _1, _2));


	std::ifstream ifs ("settings.reorder");
	if ( !ifs.is_open()){
		std::string path = getRegKey("Software\\Bethesda Softworks\\Morrowind", "Installed Path");
		if ( path.length() == 0 ){
			path = getRegKey("Software\\Bethesda Softworks\\Oblivion", "Installed Path");
		}
		mLocation->setText(path);
	}else{
		std::string path;
		std::getline(ifs, path);
		mLocation->setText(path);
		ifs.close();
	}

	reload();
}
//----------------------------------------------------------------------------------------------
bool ReOrderApp::onClose(){
	std::ofstream ofs("settings.reorder");
	ofs << mLocation->getText() << "\n";
	return true;
}
//----------------------------------------------------------------------------------------------
bool ReOrderApp::zaStringSort( std::string a, std::string b ){
	std::transform(a.begin(), a.end(), a.begin(), tolower);
	std::transform(b.begin(), b.end(), b.begin(), tolower);
	return (a > b);
}
//----------------------------------------------------------------------------------------------
bool ReOrderApp::azStringSort( std::string a, std::string b ){
	std::transform(a.begin(), a.end(), a.begin(), tolower);
	std::transform(b.begin(), b.end(), b.begin(), tolower);
	return (a < b);
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onSortAZ( HWND hwnd, WPARAM param ){
	std::vector<std::string> items;
	for ( int i = 0; i < mESP->getCount(); ++i)
		items.push_back(mESP->getItem(i));

	std::sort(items.begin(), items.end(), ReOrderApp::azStringSort);

	mESP->clear();

	for ( size_t i = 0; i < items.size(); ++i){
		mESP->append(items[i]);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onSortZA( HWND hwnd, WPARAM param ){
	std::vector<std::string> items;
	for ( int i = 0; i < mESP->getCount(); ++i){
		items.push_back(mESP->getItem(i));
	}

	std::sort(items.begin(), items.end(), ReOrderApp::zaStringSort);

	mESP->clear();

	for ( size_t i = 0; i < items.size(); ++i){
		mESP->append(items[i]);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onSelectDir( HWND hwnd, WPARAM param ){
	TCHAR path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = ("Select your data files folder");
	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
	SHGetPathFromIDList ( pidl, path );
	if ( pidl != NULL ){
		mLocation->setText(path);
		reload();
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onPrint( HWND hwnd, WPARAM param ){
	std::set<std::string> activeMods;
	std::ifstream ifs(std::string(mLocation->getText() + "Morrowind.ini").c_str());

	{
		std::string line;
		bool isInGameFiles = false;
		while ( ifs.is_open() && !ifs.eof() ){
			std::getline(ifs, line);

			if ( isInGameFiles ){
				size_t index = line.find("=");
				if ( index == -1 ){
					continue;
				}
				activeMods.insert(line.substr(index+1, line.length()-index+1));
			}
			if ( line.find("[Game Files]") == 0 ){
				isInGameFiles = true;
			}
			else if ( line.find("[") == 0 ){
				isInGameFiles = false;
			}
		}
		if ( ifs.is_open() ){
			ifs.close();
		}
	}


	{
		std::ofstream ofs("ModList.txt");
		std::vector<FileData> fl = getSortedFiles(mDataFilesPath);
		int c = 0;
		for ( size_t i = 0; i < fl.size(); ++i){
			if ( activeMods.size() && activeMods.find(fl[i].fileName) == activeMods.end() ){
				continue;
			}
			c++;
			ofs << "Plugin" << c << "=" << fl[i].fileName << "\n";
		}
		ofs.flush();
		ofs.close();
		::ShellExecute(0,"open", "ModList.txt", NULL, NULL, SW_SHOW);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onESP( HWND hwnd, WPARAM param ){
	if ( mESM->getSelectedCount() ){
		mESM->setSelected(-1);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onESM( HWND hwnd, WPARAM param ){
	if ( mESP->getSelectedCount() ){
		mESP->setSelected(-1);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onReload( HWND hwnd, WPARAM param ){
	reload();
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onTop( HWND hwnd, WPARAM param ){
	ListBox* lb = mESP;
	if ( !lb->getSelectedCount() ){
		lb = mESM;
	}
	if ( !lb->getSelectedCount() ){
		return;
	}

	int pos = 0;
	std::vector<int> items = lb->getSelectedItems();
	lb->setSelected(-1);

	for(size_t i = 0; i < items.size(); i++ ){
		std::string itemValue = lb->getItem(items[i]);
		lb->deleteItem(items[i]);
		lb->insert(itemValue,pos);
		lb->addSelected(pos);	
		pos +=1;
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onBottom( HWND hwnd, WPARAM param ){
	ListBox* lb = mESP;
	if ( !lb->getSelectedCount() ){
		lb = mESM;
	}
	if ( !lb->getSelectedCount() ){
		return;
	}

	int pos = lb->getCount() - 1;

	std::vector<int> items = lb->getSelectedItems();
	lb->setSelected(-1);

	for(int i = (int)items.size()-1; i >= 0 ; --i ){
		std::string itemValue = lb->getItem(items[i]);
		lb->deleteItem(items[i]);
		lb->insert(itemValue,pos);
		lb->addSelected(pos);	
		pos -=1;
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onUp( HWND hwnd, WPARAM param ){
	ListBox* lb = mESP;
	if ( !lb->getSelectedCount() )lb = mESM;
	if ( !lb->getSelectedCount() ) return;

	std::vector<int> items = lb->getSelectedItems();
	lb->setSelected(-1);

	int top = 0;
	for(size_t i = 0; i < items.size(); i++ ){
		if ( items[i] > top ){
			std::string itemValue = lb->getItem(items[i]);
			lb->deleteItem(items[i]);
			lb->insert(itemValue, items[i]-1);	
			lb->addSelected(items[i]-1);			
		}else{
			lb->addSelected(top);
			top += 1;
		}
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onDown( HWND hwnd, WPARAM param ){
	ListBox* lb = mESP;
	if ( !lb->getSelectedCount() )lb = mESM;
	if ( !lb->getSelectedCount() ) return;

	std::vector<int> items = lb->getSelectedItems();
	lb->setSelected(-1);

	int bottom = lb->getCount();
	for(int i = (int)items.size() - 1; i >= 0; i-- ){
		if ( items[i] < bottom-1 ){
			std::string itemValue = lb->getItem(items[i]);
			lb->deleteItem(items[i]);
			lb->insert(itemValue, items[i]+1);	
			lb->addSelected(items[i]+1);			
		}else{
			bottom -= 1;
			lb->addSelected(bottom);
		}
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::onReOrder( HWND hwnd, WPARAM param ){
	time_t t = time(0);

	//try and base the time off a orignaal data file
	if ( fs::exists(mDataFilesPath + "Morrowind.esm" ))
		t = fs::last_write_time(mDataFilesPath + "Morrowind.esm");
	else if ( fs::exists(mDataFilesPath + "Oblivion.esm" ))
		t = fs::last_write_time(mDataFilesPath + "Oblivion.esm");

	ListBox* lb = mESM;
	for ( int i = 0; i < lb->getCount(); ++i ){
		fs::last_write_time(mDataFilesPath + lb->getItem(i), t++);
	}
	lb = mESP;
	for ( int i = 0; i < lb->getCount(); ++i ){
		fs::last_write_time(mDataFilesPath + lb->getItem(i), t++);
	}
}
//----------------------------------------------------------------------------------------------
void ReOrderApp::reload(){
	mESM->clear();
	mESP->clear();

	std::string loc = mLocation->getText();

	//make sure it ends with \ .
	if (loc[loc.length()-1] != '\\' )
		loc += "\\";

	//find either oblivion or morrowind data files
	if ( fs::exists(loc + "Data Files\\") ) 
		loc += "Data Files\\";
	else if ( fs::exists(loc + "Data\\") )
		loc += "Data\\";
	mDataFilesPath = loc;
	std::vector<FileData> fd = getSortedFiles(loc);

	for ( unsigned int i = 0; i < fd.size(); ++i)
		if ( fd[i].isEsp )	mESP->append(fd[i].fileName);
		else				mESM->append(fd[i].fileName);
}
//----------------------------------------------------------------------------------------------
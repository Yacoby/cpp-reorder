#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0601
#endif
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>



#include <vector>
#include <string>
#include <set>
#include <fstream>


#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;


#include "resource.h" 

#include "Application.h"
#include "Button.h"
#include "ListBox.h"
#include "TextBox.h"


/**
* @brief checks if the given file has the given extension
* @param file the file name to check
* @param ext the extenion
*/
bool hasExt(const std::string& file, const std::string& ext);

/**
* @breif struct to hold file data I need about an objhect
*/
struct FileData{
	bool isEsp;
	std::string fileName;
	time_t lastModified;
	static bool compareDate(FileData& a, FileData& b);
};

/**
* @brief returns a vector of files listed by date
*/
std::vector<FileData> getSortedFiles(const std::string& path);

/**
* @brief gets the given key from the registry
* @return the key on sucsess "" on fail
*/
std::string getRegKey(std::string pos, std::string name);

class ReOrderApp : public Application{


	//my vars

	ListBox *mESM;
	ListBox* mESP;
	TextBox* mLocation;

	std::string mDataFilesPath;

public:

	void onInit(HWND hwnd);

	bool onClose();

	static bool zaStringSort(std::string a, std::string b);
	static bool azStringSort(std::string a, std::string b);

	void onSortAZ(HWND hwnd, WPARAM param);



	void onSortZA(HWND hwnd, WPARAM param);

	void onSelectDir(HWND hwnd, WPARAM param);

	void onPrint(HWND hwnd, WPARAM param);

	/**
	* @brief called when an action takes place on the esp list box
	*
	* deselects everything on the esm list box
	*/
	void onESP(HWND hwnd, WPARAM param);
	void onESM(HWND hwnd, WPARAM param);
	void onReload(HWND hwnd, WPARAM param);

	/**
	* @brief moves the selected items to the top of the list
	*/
	void onTop(HWND hwnd, WPARAM param);

	void onBottom(HWND hwnd, WPARAM param);

	/**
	* @brief moves the selected items up one
	*/
	void onUp(HWND hwnd, WPARAM param);

	void onDown(HWND hwnd, WPARAM param);

	/**
	* @brief saves the changes
	*/
	void onReOrder(HWND hwnd, WPARAM param);



	void reload();

};
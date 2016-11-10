#include "FolderUtils.h"
#include <Windows.h>
#include <vector>
#include <string>

std::vector<std::string> getAllFilesInsideFolder(std::string folder)
{
	using namespace std;

	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

std::vector<std::string> getFilesInsideFolderNoExtension(std::string folder)
{
	std::vector<std::string> filesWithExtension = getAllFilesInsideFolder(folder);

	std::vector<std::string> filesWithoutExtension;

	for (int i = 0; i < filesWithExtension.size(); i++)
	{
		int index = filesWithExtension[i].find_last_of('.');

		filesWithoutExtension.push_back(filesWithExtension[i].substr(0, index));
	}

	return filesWithoutExtension;
}
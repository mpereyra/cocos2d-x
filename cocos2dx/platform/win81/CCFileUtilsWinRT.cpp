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
#include "CCFileUtilsWinRT.h"
#include "CCWinRTUtils.h"
#include "platform/CCCommon.h"
//#include <Shlobj.h>
#include <Windows.h>

using namespace std;

NS_CC_BEGIN

static std::string s_pszResourcePath;

static void _checkPath()
{
    if (s_pszResourcePath.empty())
    {
		// TODO: needs to be tested
		s_pszResourcePath = CCFileUtilsWinRT::getAppPath() + '\\' + "Assets\\Resources\\";
    }
}

static CCFileUtils* s_pFileUtils = nullptr;

CCFileUtils* CCFileUtils::sharedFileUtils()
{
    if (s_pFileUtils == NULL)
    {
		CCFileUtilsWinRT* p = new CCFileUtilsWinRT();
		p->init();
		s_pFileUtils = p;
    }
	return s_pFileUtils;
}

CCFileUtilsWinRT::CCFileUtilsWinRT()
{
}

bool CCFileUtilsWinRT::init()
{
    _checkPath();
    // m_strDefaultResRootPath = s_pszResourcePath;
	return true; // CCFileUtils::init();
}

#if 1
bool CCFileUtilsWinRT::isFileExist(const std::string& strFilePath)
{
    bool ret = false;
    FILE * pf = 0;

    std::string strPath = strFilePath;
    if (!isAbsolutePath(strPath))
    { // Not absolute path, add the default root path at the beginning.
		strPath.insert(0, s_pszResourcePath);
    }

    const char* path = strPath.c_str();

	if (path && strlen(path) && (pf = fopen(path, "rb")))
    {
        ret = true;
        fclose(pf);
    }
    return ret;
}
#else

bool CCFileUtilsWinRT::isFileExist(const std::string& strFilePath)
{
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	std::string strPath = strFilePath;
    if (!isAbsolutePath(strPath))
    { // Not absolute path, add the default root path at the beginning.
		strPath.insert(0, s_pszResourcePath);
    }

#if 0
    std::string pathKey = strPath;
    pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
    std::replace( pathKey.begin(), pathKey.end(), '/', '\\'); 

#endif // 0


	hFind = FindFirstFileEx(CCUtf8ToUnicode(strPath.c_str(), -1).c_str(), FindExInfoStandard, &FindFileData,
             FindExSearchNameMatch, NULL, 0);


    return hFind != INVALID_HANDLE_VALUE;
#if 0
    if (0 == strFilePath.length())
    {
        return false;
    }
    
    std::string strPath = strFilePath;
    if (!isAbsolutePath(strPath))
    { // Not absolute path, add the default root path at the beginning.
		strPath.insert(0, s_pszResourcePath);
    }
    return GetFileAttributesA(strPath.c_str()) != -1 ? true : false;
#endif
}
#endif

bool CCFileUtilsWinRT::isAbsolutePath(const std::string& strPath)
{
    if (   strPath.length() > 2 
        && ( (strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z') )
        && strPath[1] == ':')
    {
        return true;
    }
    return false;
}

string CCFileUtilsWinRT::getWriteablePath()
{
	auto localFolderPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	return std::string(PlatformStringToString(localFolderPath)) + '\\';
}

string CCFileUtilsWinRT::getAppPath()
{
	Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
	return std::string(PlatformStringToString(package->InstalledLocation->Path));
}


// BPC_PATCH needed for win support to compile
void CCFileUtils::addSearchPath(const char* path_)
{
	std::string strPrefix;
	std::string path(path_);

	bool isAbsolutePath = path_[0] == '/' ? true : false;
	if (!isAbsolutePath)
	{ // Not an absolute path
		//strPrefix = m_strDefaultResRootPath;
	}
	path = strPrefix + path;
	if (path.length() > 0 && path[path.length() - 1] != '/')
	{
		path += "/";
	}

	//m_searchPathArray.push_back(path);
}

const char* CCFileUtils::fullPathFromRelativePath(const char *pszRelativePath)
{
	return pszRelativePath;
}

const char* CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
	std::string relativeFile = pszRelativeFile;
	CCString *pRet = new CCString();
	pRet->autorelease();
	pRet->m_sString = relativeFile.substr(0, relativeFile.rfind('/') + 1);
	pRet->m_sString += pszFilename;
	return pRet->m_sString.c_str();
}


unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
	unsigned char * pBuffer = NULL;
	CCAssert(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
	*pSize = 0;
	do
	{
		// read the file from hardware
		std::string fullPath = fullPathFromRelativePath(pszFileName); // TODO wrong?
		FILE *fp = fopen(fullPath.c_str(), pszMode);
		CC_BREAK_IF(!fp);

		fseek(fp, 0, SEEK_END);
		*pSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pBuffer = new unsigned char[*pSize];
		*pSize = fread(pBuffer, sizeof(unsigned char), *pSize, fp);
		fclose(fp);
	} while (0);

	if (!pBuffer)
	{
		std::string msg = "Get data from file(";
		msg.append(pszFileName).append(") failed!");

		CCLOG("%s", msg.c_str());
	}
	return pBuffer;
}

static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
	s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
	return s_bPopupNotify;
}

void CCFileUtils::setResourceDirectory(const char *pszDirectoryName)
{
	m_obDirectory = pszDirectoryName;
	if (m_obDirectory.size() > 0 && m_obDirectory[m_obDirectory.size() - 1] != '/')
	{
		m_obDirectory.append("/");
	}
}

const char* CCFileUtils::getResourceDirectory()
{
	return m_obDirectory.c_str();
}

void CCFileUtils::purgeFileUtils()
{
	if (s_pFileUtils != NULL)
	{
		s_pFileUtils->purgeCachedEntries();
	}

	CC_SAFE_DELETE(s_pFileUtils);
}

void CCFileUtils::purgeCachedEntries(){

}

string CCFileUtils::getWriteablePath()
{
	assert(0); //not called; overidden above
	return "";
}


NS_CC_END

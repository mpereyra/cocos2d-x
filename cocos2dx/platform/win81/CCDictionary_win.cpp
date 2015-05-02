
/* dictionary and array parsing */


/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

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

#include "../CCFileUtils.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCString.h"
#include "../CCSAXParser.h"
#include "support/tinyxml2/tinyxml2.h"
#include "support/zip_support/unzip.h"
#include <stack>
#include <algorithm>

using namespace std;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

NS_CC_BEGIN

typedef enum
{
	SAX_NONE = 0,
	SAX_KEY,
	SAX_DICT,
	SAX_INT,
	SAX_REAL,
	SAX_STRING,
	SAX_ARRAY
}CCSAXState;

typedef enum
{
	SAX_RESULT_NONE = 0,
	SAX_RESULT_DICT,
	SAX_RESULT_ARRAY
}CCSAXResult;

class CCDictMaker : public CCSAXDelegator
{
public:
	CCSAXResult m_eResultType;
	CCArray* m_pRootArray;
	CCDictionary *m_pRootDict;
	CCDictionary *m_pCurDict;
	std::stack<CCDictionary*> m_tDictStack;
	std::string m_sCurKey;   ///< parsed key
	std::string m_sCurValue; // parsed value
	CCSAXState m_tState;
	CCArray* m_pArray;

	std::stack<CCArray*> m_tArrayStack;
	std::stack<CCSAXState>  m_tStateStack;

public:
	CCDictMaker()
		: m_eResultType(SAX_RESULT_NONE),
		m_pRootArray(NULL),
		m_pRootDict(NULL),
		m_pCurDict(NULL),
		m_tState(SAX_NONE),
		m_pArray(NULL)
	{
	}

	~CCDictMaker()
	{
	}

	CCDictionary* dictionaryWithContentsOfFile(const char *pFileName)
	{
		m_eResultType = SAX_RESULT_DICT;
		CCSAXParser parser;

		if (false == parser.init("UTF-8"))
		{
			return NULL;
		}
		parser.setDelegator(this);

		parser.parse(pFileName);
		return m_pRootDict;
	}

	CCArray* arrayWithContentsOfFile(const char* pFileName)
	{
		m_eResultType = SAX_RESULT_ARRAY;
		CCSAXParser parser;

		if (false == parser.init("UTF-8"))
		{
			return NULL;
		}
		parser.setDelegator(this);

		parser.parse(pFileName);
		return m_pArray;
	}

	void startElement(void *ctx, const char *name, const char **atts)
	{
		CC_UNUSED_PARAM(ctx);
		CC_UNUSED_PARAM(atts);
		std::string sName((char*)name);
		if (sName == "dict")
		{
			m_pCurDict = new CCDictionary();
			if (m_eResultType == SAX_RESULT_DICT && m_pRootDict == NULL)
			{
				// Because it will call m_pCurDict->release() later, so retain here.
				m_pRootDict = m_pCurDict;
				m_pRootDict->retain();
			}
			m_tState = SAX_DICT;

			CCSAXState preState = SAX_NONE;
			if (!m_tStateStack.empty())
			{
				preState = m_tStateStack.top();
			}

			if (SAX_ARRAY == preState)
			{
				// add the dictionary into the array
				m_pArray->addObject(m_pCurDict);
			}
			else if (SAX_DICT == preState)
			{
				// add the dictionary into the pre dictionary
				CCAssert(!m_tDictStack.empty(), "The state is wrong!");
				CCDictionary* pPreDict = m_tDictStack.top();
				pPreDict->setObject(m_pCurDict, m_sCurKey.c_str());
			}

			m_pCurDict->release();

			// record the dict state
			m_tStateStack.push(m_tState);
			m_tDictStack.push(m_pCurDict);
		}
		else if (sName == "key")
		{
			m_tState = SAX_KEY;
		}
		else if (sName == "integer")
		{
			m_tState = SAX_INT;
		}
		else if (sName == "real")
		{
			m_tState = SAX_REAL;
		}
		else if (sName == "string")
		{
			m_tState = SAX_STRING;
		}
		else if (sName == "array")
		{
			m_tState = SAX_ARRAY;
			m_pArray = new CCArray();
			if (m_eResultType == SAX_RESULT_ARRAY && m_pRootArray == NULL)
			{
				m_pRootArray = m_pArray;
				m_pRootArray->retain();
			}
			CCSAXState preState = SAX_NONE;
			if (!m_tStateStack.empty())
			{
				preState = m_tStateStack.top();
			}

			if (preState == SAX_DICT)
			{
				m_pCurDict->setObject(m_pArray, m_sCurKey.c_str());
			}
			else if (preState == SAX_ARRAY)
			{
				CCAssert(!m_tArrayStack.empty(), "The state is wrong!");
				CCArray* pPreArray = m_tArrayStack.top();
				pPreArray->addObject(m_pArray);
			}
			m_pArray->release();
			// record the array state
			m_tStateStack.push(m_tState);
			m_tArrayStack.push(m_pArray);
		}
		else
		{
			m_tState = SAX_NONE;
		}
	}

	void endElement(void *ctx, const char *name)
	{
		CC_UNUSED_PARAM(ctx);
		CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
		std::string sName((char*)name);
		if (sName == "dict")
		{
			m_tStateStack.pop();
			m_tDictStack.pop();
			if (!m_tDictStack.empty())
			{
				m_pCurDict = m_tDictStack.top();
			}
		}
		else if (sName == "array")
		{
			m_tStateStack.pop();
			m_tArrayStack.pop();
			if (!m_tArrayStack.empty())
			{
				m_pArray = m_tArrayStack.top();
			}
		}
		else if (sName == "true")
		{
			CCString *str = new CCString("1");
			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(str);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(str, m_sCurKey.c_str());
			}
			str->release();
		}
		else if (sName == "false")
		{
			CCString *str = new CCString("0");
			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(str);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(str, m_sCurKey.c_str());
			}
			str->release();
		}
		else if (sName == "string" || sName == "integer" || sName == "real")
		{
			CCString* pStrValue = new CCString(m_sCurValue);

			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(pStrValue);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(pStrValue, m_sCurKey.c_str());
			}

			pStrValue->release();
			m_sCurValue.clear();
		}

		m_tState = SAX_NONE;
	}

	void textHandler(void *ctx, const char *ch, int len)
	{
		CC_UNUSED_PARAM(ctx);
		if (m_tState == SAX_NONE)
		{
			return;
		}

		CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
		CCString *pText = new CCString(std::string((char*)ch, 0, len));

		switch (m_tState)
		{
		case SAX_KEY:
			m_sCurKey = pText->getCString();
			break;
		case SAX_INT:
		case SAX_REAL:
		case SAX_STRING:
		{
			if (curState == SAX_DICT)
			{
				CCAssert(!m_sCurKey.empty(), "key not found : <integer/real>");
			}

			m_sCurValue.append(pText->getCString());
		}
		break;
		default:
			break;
		}
		pText->release();
	}
};

CCDictionary* createCCDictionaryWithContentsOfFile(const std::string& fullPath)
{
	// std::string fullPath = CCFileUtils::sharedFileUtils()->getPathForFilename(filename.c_str());
	CCDictMaker tMaker;
	return tMaker.dictionaryWithContentsOfFile(fullPath.c_str());
}

CCArray* createCCArrayWithContentsOfFile(const std::string& fullPath)
{
	// std::string fullPath = CCFileUtils::sharedFileUtils()->getPathForFilename(filename.c_str());
	CCDictMaker tMaker;
	return tMaker.arrayWithContentsOfFile(fullPath.c_str());
}

/*
* forward statement
*/
static tinyxml2::XMLElement* generateElementForArray(cocos2d::CCArray *array, tinyxml2::XMLDocument *pDoc);
static tinyxml2::XMLElement* generateElementForDict(cocos2d::CCDictionary *dict, tinyxml2::XMLDocument *pDoc);

/*
* Generate tinyxml2::XMLElement for CCObject through a tinyxml2::XMLDocument
*/
static tinyxml2::XMLElement* generateElementForObject(cocos2d::CCObject *object, tinyxml2::XMLDocument *pDoc)
{
	// object is CCString
	if (CCString *str = dynamic_cast<CCString *>(object))
	{
		tinyxml2::XMLElement* node = pDoc->NewElement("string");
		tinyxml2::XMLText* content = pDoc->NewText(str->getCString());
		node->LinkEndChild(content);
		return node;
	}

	// object is CCArray
	if (CCArray *array = dynamic_cast<CCArray *>(object))
		return generateElementForArray(array, pDoc);

	// object is CCDictionary
	if (CCDictionary *innerDict = dynamic_cast<CCDictionary *>(object))
		return generateElementForDict(innerDict, pDoc);

	CCLOG("This type cannot appear in property list");
	return NULL;
}

/*
* Generate tinyxml2::XMLElement for CCDictionary through a tinyxml2::XMLDocument
*/
static tinyxml2::XMLElement* generateElementForDict(cocos2d::CCDictionary *dict, tinyxml2::XMLDocument *pDoc)
{
	tinyxml2::XMLElement* rootNode = pDoc->NewElement("dict");

	CCDictElement *dictElement = NULL;
	CCDICT_FOREACH(dict, dictElement)
	{
		tinyxml2::XMLElement* tmpNode = pDoc->NewElement("key");
		rootNode->LinkEndChild(tmpNode);
		tinyxml2::XMLText* content = pDoc->NewText(dictElement->getStrKey());
		tmpNode->LinkEndChild(content);

		CCObject *object = dictElement->getObject();
		tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
		if (element)
			rootNode->LinkEndChild(element);
	}
	return rootNode;
}

/*
* Generate tinyxml2::XMLElement for CCArray through a tinyxml2::XMLDocument
*/
static tinyxml2::XMLElement* generateElementForArray(cocos2d::CCArray *array, tinyxml2::XMLDocument *pDoc)
{
	tinyxml2::XMLElement* rootNode = pDoc->NewElement("array");

	CCObject *object = NULL;
	CCARRAY_FOREACH(array, object)
	{
		tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
		if (element)
			rootNode->LinkEndChild(element);
	}
	return rootNode;
}


#endif /* (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) */

CCDictionary* ccFileUtils_dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
	CCDictionary* pRet = createCCDictionaryWithContentsOfFile(pFileName);
	return pRet;
}

CCArray* ccFileUtils_arrayWithContentsOfFileThreadSafe(const char* pFileName)
{
	CCArray* pRet = createCCArrayWithContentsOfFile(pFileName);
	return pRet;
}


NS_CC_END

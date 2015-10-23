/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef XMLTOOLS_H
#define XMLTOOLS_H

class QStringList;
class QString;

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "xmlParser.h"

class XMLTools
{
	public:
		friend class Font;
		static bool nodeExists(XMLNode nodeToCheck, const char *nodeName);
		static bool nodesExist(XMLNode nodeToCheck, const char *nodeName, int min, int max);
		static bool attributeNameValid(XMLNode nodeToCheck, int attribute, QStringList validNames);
		static bool attributeExists(XMLNode nodeToCheck, const char *attributeName);
		static bool attributeStringValid(XMLNode nodeToCheck, const char *attributeName, QStringList expectedValues, int defaultValue=-1);
		static bool attributeIntRangeValid(XMLNode nodeToCheck, const char *attributeName, int min, int max, int base=10);
		static bool attributeFloatRangeValid(XMLNode nodeToCheck, const char *attributeName, float min, float max);
		static void printXMLContent(XMLNode node, int indentLevel=0);
		static void embedResource(XMLNode ResourceNode, QString fileLocation);
		static void extractResource(XMLNode ResourceNode, QString fileLocation);
		static void setCurrentFileName(QString name);
		static void base64Encode(unsigned char *input, size_t &inputLength, char *output, size_t &outputLength);
		static void base64Decode(char *input, size_t &inputLength, unsigned char *output, size_t &outputLength);
	private:
		static QString getNodeTree(XMLNode node);
		static QString currentFileName;
		static const char *base64EncodeTable;
		static const unsigned char base64DecodeTable[];
};

#endif // XMLTOOLS_H

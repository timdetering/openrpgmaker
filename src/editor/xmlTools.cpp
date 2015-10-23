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

#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <cstdio>
#include "projectdata.h"
#include "xmlParser.h"
#include "xmlTools.h"

QString XMLTools::currentFileName = "";
const char *XMLTools::base64EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const unsigned char XMLTools::base64DecodeTable[] =
{
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, //   0 -  31
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0,62, 0, 0, 0,63,  52,53,54,55,56,57,58,59,  60,61, 0, 0, 0, 0, 0, 0, //  32 -  63
	0, 0, 1, 2, 3, 4, 5, 6,   7, 8, 9,10,11,12,13,14,  15,16,17,18,19,20,21,22,  23,24,25, 0, 0, 0, 0, 0, //  64 -  95
	0,26,27,28,29,30,31,32,  33,34,35,36,37,38,39,40,  41,42,43,44,45,46,47,48,  49,50,51, 0, 0, 0, 0, 0, //  96 - 127
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, // 128 - 159
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, // 160 - 191
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, // 191 - 223
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0  // 223 - 255
};

bool XMLTools::nodeExists(XMLNode nodeToCheck, const char *nodeName)
{
	int num = nodeToCheck.nChildNode(nodeName);
	if (num < 1)
	{
		QMessageBox::critical(0, "Error: Missing Data", QString("Missing '%1' node inside '%2' node.\n\nNode Tree: %3\nFile: %4").arg(nodeName).arg(nodeToCheck.getName()).arg(getNodeTree(nodeToCheck) + nodeName).arg(currentFileName));
		return false;
	}
	else if (num > 1)
		QMessageBox::warning(0, "Warning: To Much Data", QString("Multiple '%1' nodes found inside '%2' node. Only the first one will be used.\n\nNode Tree: %3\nFile: %4").arg(nodeName).arg(nodeToCheck.getName()).arg(getNodeTree(nodeToCheck) + nodeName).arg(currentFileName));
	return true;
}

bool XMLTools::nodesExist(XMLNode nodeToCheck, const char *nodeName, int min, int max)
{
	int num = nodeToCheck.nChildNode(nodeName);
	if (num < min)
	{
		QMessageBox::critical(0, "Error: Missing Data", QString("There should be at least %1 '%2' nodes inside '%3' node, but only %4 were found.\n\nNode Tree: %5\nFile: %6").arg(min).arg(nodeName).arg(nodeToCheck.getName()).arg(num).arg(getNodeTree(nodeToCheck) + nodeName).arg(currentFileName));
		return false;
	}
	else if (num > max)
		QMessageBox::critical(0, "Warning: To Much Data", QString("More than %1 '%2' nodes found inside '%3' node, only the first %4 will be used.\n\nNode Tree: %5\nFile: %6").arg(max).arg(nodeName).arg(nodeToCheck.getName()).arg(max).arg(getNodeTree(nodeToCheck) + nodeName).arg(currentFileName));
	return true;
}

bool XMLTools::attributeNameValid(XMLNode nodeToCheck, int attribute, QStringList validNames)
{
	QString attributeName = nodeToCheck.getAttributeName(attribute);
	QString errorString = QString("Attribute #%1's name in '%2' node should be either ").arg(attribute).arg(nodeToCheck.getName());
	for (int i = 0; i < validNames.size(); ++i)
	{
		if (i == validNames.size() - 1)
			errorString += QString("or \"%1\".").arg(validNames[i]);
		else
			errorString += QString("\"%1\", ").arg(validNames[i]);
	}
	if (!validNames.contains(attributeName))
	{
		errorString += QString("\n\nNode Tree: %1\nFile: %2").arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName);
		QMessageBox::critical(0, "Error: Invalid Data", errorString);
		return false;
	}
	return true;
}

bool XMLTools::attributeExists(XMLNode nodeToCheck, const char *attributeName)
{
	if (nodeToCheck.isAttributeSet(attributeName) == FALSE)
	{
		QMessageBox::critical(0, "Error: Missing Data", QString("Attribute '%1' is missing inside node '%2'.\n\nNode Tree: %3\nFile: %4").arg(attributeName).arg(nodeToCheck.getName()).arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName));
		return false;
	}
	return true;
}

bool XMLTools::attributeStringValid(XMLNode nodeToCheck, const char *attributeName, QStringList expectedValues, int defaultValue)
{
	QString attributeString = nodeToCheck.getAttribute(attributeName);
	QString errorString = QString("Attribute '%1' in '%2' node should be either ").arg(attributeName).arg(nodeToCheck.getName());
	int matches = 0;
	for (int i = 0; i < expectedValues.size(); ++i)
	{
		if (i == expectedValues.size() - 1)
			errorString += QString("or \"%1\".").arg(expectedValues[i]);
		else
			errorString += QString("\"%1\", ").arg(expectedValues[i]);
		if (attributeString == expectedValues[i])
			++matches;
	}
	if (matches == 0)
	{
		if (defaultValue == -1)
		{
			errorString += QString("\n\nNode Tree: %1\nFile: %2").arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName);
			QMessageBox::critical(0, "Error: Invalid Data", errorString);
			return false;
		}
		else
		{
			errorString += QString("\nDefaulting to \"%1\".").arg(expectedValues[defaultValue]);
			errorString += QString("\n\nNode Tree: %1\nFile: %2").arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName);
			QMessageBox::warning(0, "Warning: Invalid Data", errorString);
		}
	}
	return true;
}

bool XMLTools::attributeIntRangeValid(XMLNode nodeToCheck, const char *attributeName, int min, int max, int base)
{
	int attributeValue = QString(nodeToCheck.getAttribute(attributeName)).toInt(0, base);
	if (attributeValue < min || attributeValue > max)
	{
		QMessageBox::critical(0, "Error: Invalid Data", QString("Attribute '%1' in '%2' node should be between %3 and %4 (value=%5)\n\nNode Tree: %6\nFile: %7").arg(attributeName).arg(nodeToCheck.getName()).arg(min, 0, base).arg(max, 0, base).arg(attributeValue, 0, base).arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName));
		return false;
	}
	return true;
}

bool XMLTools::attributeFloatRangeValid(XMLNode nodeToCheck, const char *attributeName, float min, float max)
{
	float attributeValue = QString(nodeToCheck.getAttribute(attributeName)).toFloat();
	if (attributeValue < min || attributeValue > max)
	{
		QMessageBox::critical(0, "Error: Invalid Data", QString("Attribute '%1' in '%2' node should be between %3 and %4 (value=%5)\n\nNode Tree: %6\nFile: %7").arg(attributeName).arg(nodeToCheck.getName()).arg(min).arg(max).arg(attributeValue).arg(getNodeTree(nodeToCheck) + attributeName).arg(currentFileName));
		return false;
	}
	return true;
}

void XMLTools::printXMLContent(XMLNode node, int indentLevel)
{
	for (int i = 0; i < indentLevel; ++i)
		printf("  ");
	printf("<%s", node.getName());
	for (int i = 0; i < node.nAttribute(); ++i)
	{
		XMLAttribute attribute = node.getAttribute(i);
		printf(" %s=\"%s\"", attribute.lpszName, attribute.lpszValue);
	}
	if (node.nChildNode() == 0)
		printf("/>\n");
	else
	{
		printf(">\n");
		for (int i = 0; i < node.nChildNode(); ++i)
			printXMLContent(node.getChildNode(i), indentLevel + 1);
		for (int i = 0; i < indentLevel; ++i)
			printf("  ");
		printf("</%s>\n", node.getName());
	}
}

void XMLTools::embedResource(XMLNode ResourceNode, QString fileLocation)
{
	FILE *file;
	QFileInfo info = QFileInfo(fileLocation);
	size_t inputLength;
	size_t outputLength;
	unsigned char buffer[54];
	char base64[73];
	if (!info.exists())
	{
		QMessageBox::critical(0, "Invalid Resource File", QString("Error: Resource to embed doesn't exists.\nResource: \"%1\"").arg(fileLocation));
		return;
	}
	file = fopen(fileLocation.toUtf8().data(), "rb");
	while (!feof(file))
	{
		inputLength = fread(buffer, 1, sizeof(buffer), file);
		base64Encode(buffer, inputLength, base64, outputLength);
		base64[outputLength] = '\0';
		ResourceNode.addText(base64);
	}
	fclose(file);
}

void XMLTools::extractResource(XMLNode ResourceNode, QString fileLocation)
{
	QStringList base64List;
	QDir directory("/");
	FILE *file;
	size_t inputLength;
	size_t outputLength;
	unsigned char buffer[54];
	directory.mkpath(QFileInfo(fileLocation).path());
	file = fopen(fileLocation.toUtf8().data(), "wb");
	if (!file)
	{
		printf("failed to open file \"%s\" in write mode\n", fileLocation.toUtf8().data());
		perror("reason");
		return;
	}
	base64List = QString(ResourceNode.getText()).split(QChar('\n'), QString::SkipEmptyParts);
	for (int i = 0; i < base64List.size(); ++i)
	{
		base64List[i].remove(QRegExp("[^A-Za-z0-9+/=]"));
		inputLength = base64List[i].size();
		base64Decode(base64List[i].toUtf8().data(), inputLength, buffer, outputLength);
		fwrite(buffer, 1, outputLength, file);
	}
	fclose(file);
}

void XMLTools::setCurrentFileName(QString name)
{
	currentFileName = name;
}

void XMLTools::base64Encode(unsigned char *input, size_t &inputLength, char *output, size_t &outputLength)
{
	unsigned long combinedValue;
	size_t i, j;
	int paddingAmount = 0;
	for (i = 0, j = 0; i < inputLength; i += 3, j += 4)
	{
		combinedValue = input[i] << 16;
		if (i + 1 < inputLength)
		{
			combinedValue += input[i + 1] << 8;
			if (i + 2 < inputLength)
				combinedValue += input[i + 2];
			else
				paddingAmount = 1;
		}
		else
			paddingAmount = 2;
		output[j] = base64EncodeTable[(combinedValue >> 18) % 64];
		output[j + 1] = base64EncodeTable[(combinedValue >> 12) % 64];
		output[j + 2] = base64EncodeTable[(combinedValue >> 6) % 64];
		output[j + 3] = base64EncodeTable[combinedValue % 64];
		if (paddingAmount == 2)
		{
			output[j + 2] = '=';
			output[j + 3] = '=';
		}
		else if (paddingAmount == 1)
			output[j + 3] = '=';
	}
	outputLength = (inputLength / 3) * 4;
	if (paddingAmount != 0)
		outputLength += 4;
}

void XMLTools::base64Decode(char *input, size_t &inputLength, unsigned char *output, size_t &outputLength)
{
	unsigned long combinedValue;
	size_t i, j;
	int paddingAmount = 0;
	for (i = 0, j = 0; i < inputLength; i += 4, j += 3)
	{
		combinedValue = base64DecodeTable[(int)input[i]] << 18;
		combinedValue += base64DecodeTable[(int)input[i + 1]] << 12;
		combinedValue += base64DecodeTable[(int)input[i + 2]] << 6;
		combinedValue += base64DecodeTable[(int)input[i + 3]];
		if (input[i + 2] == '=')
			paddingAmount = 2;
		else if (input[i + 3] == '=')
			paddingAmount = 1;
		output[j] = (unsigned char)((combinedValue >> 16) % 256);
		if (paddingAmount < 2)
			output[j + 1] = (unsigned char)((combinedValue >> 8) % 256);
		if (paddingAmount < 1)
			output[j + 2] = (unsigned char)(combinedValue % 256);
	}
	outputLength = (inputLength / 4) * 3 - paddingAmount;
}

QString XMLTools::getNodeTree(XMLNode node)
{
	QString nodeTree = "";
	XMLNode::XMLNodeDataTag *data = node.d;
	while (data != NULL)
	{
		nodeTree.prepend(QString("%1->").arg(data->lpszName));
		data = data->pParent;
	}
	return nodeTree;
}

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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_attribute.h"
#include "xmlParser.h"
#include "xmlTools.h"

Attribute::Attribute()
{
	name = "Untitled";
	type = TYPE_WEAPON;
	multiplierA = 200;
	multiplierB = 150;
	multiplierC = 100;
	multiplierD = 50;
	multiplierE = 0;
}

Attribute::Attribute(Attribute &other)
{
	name = other.name;
	type = other.type;
	multiplierA = other.multiplierA;
	multiplierB = other.multiplierB;
	multiplierC = other.multiplierC;
	multiplierD = other.multiplierD;
	multiplierE = other.multiplierE;
}

Attribute::Attribute(const Attribute &other)
{
	name = other.name;
	type = other.type;
	multiplierA = other.multiplierA;
	multiplierB = other.multiplierB;
	multiplierC = other.multiplierC;
	multiplierD = other.multiplierD;
	multiplierE = other.multiplierE;
}

Attribute::Attribute(XMLNode AttributeNode)
{
	QString temp_string;
	name = AttributeNode.getAttribute("name");
	temp_string = AttributeNode.getAttribute("type");
	if (temp_string == "weapon")
		type = TYPE_WEAPON;
	else
		type = TYPE_MAGIC;
	multiplierA = QString(AttributeNode.getAttribute("multiplierA")).toInt();
	multiplierB = QString(AttributeNode.getAttribute("multiplierB")).toInt();
	multiplierC = QString(AttributeNode.getAttribute("multiplierC")).toInt();
	multiplierD = QString(AttributeNode.getAttribute("multiplierD")).toInt();
	multiplierE = QString(AttributeNode.getAttribute("multiplierE")).toInt();
}

Attribute::Attribute(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	type = storageFile.getSignedInt();
	multiplierA = storageFile.getSignedInt();
	multiplierB = storageFile.getSignedInt();
	multiplierC = storageFile.getSignedInt();
	multiplierD = storageFile.getSignedInt();
	multiplierE = storageFile.getSignedInt();
}

Attribute::~Attribute()
{
}

void Attribute::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(type);
	storageFile.putSignedInt(multiplierA);
	storageFile.putSignedInt(multiplierB);
	storageFile.putSignedInt(multiplierC);
	storageFile.putSignedInt(multiplierD);
	storageFile.putSignedInt(multiplierE);
}

QStringList Attribute::getResources()
{
	return QStringList();
}

XMLNode Attribute::getAttributeNode()
{
	XMLNode AttributeNode = XMLNode::createXMLTopNode("attribute");
	AttributeNode.addAttribute("name", name.toUtf8().data());
	AttributeNode.addAttribute("type", (type == TYPE_WEAPON) ? "weapon":"magic");
	AttributeNode.addAttribute("multiplierA", QString::number(multiplierA).toUtf8().data());
	AttributeNode.addAttribute("multiplierB", QString::number(multiplierB).toUtf8().data());
	AttributeNode.addAttribute("multiplierC", QString::number(multiplierC).toUtf8().data());
	AttributeNode.addAttribute("multiplierD", QString::number(multiplierD).toUtf8().data());
	AttributeNode.addAttribute("multiplierE", QString::number(multiplierE).toUtf8().data());
	return AttributeNode;
}

QString Attribute::getName()
{
	return name;
}

void Attribute::setName(QString value)
{
	name = value;
}

int Attribute::getType()
{
	return type;
}

void Attribute::setType(int value)
{
	type = value;
}

int Attribute::getMultiplierA()
{
	return multiplierA;
}

void Attribute::setMultiplierA(int value)
{
	multiplierA = value;
}

int Attribute::getMultiplierB()
{
	return multiplierB;
}

void Attribute::setMultiplierB(int value)
{
	multiplierB = value;
}

int Attribute::getMultiplierC()
{
	return multiplierC;
}

void Attribute::setMultiplierC(int value)
{
	multiplierC = value;
}

int Attribute::getMultiplierD()
{
	return multiplierD;
}

void Attribute::setMultiplierD(int value)
{
	multiplierD = value;
}

int Attribute::getMultiplierE()
{
	return multiplierE;
}

void Attribute::setMultiplierE(int value)
{
	multiplierE = value;
}

bool Attribute::tryLoad(XMLNode AttributeNode)
{
	if (!XMLTools::attributeExists(AttributeNode, "name"))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(AttributeNode, "type", QStringList() << "weapon" << "magic", 1))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "multiplierA"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AttributeNode, "multiplierA", 0, 1000))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "multiplierB"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AttributeNode, "multiplierB", 0, 1000))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "multiplierC"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AttributeNode, "multiplierC", 0, 1000))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "multiplierD"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AttributeNode, "multiplierD", 0, 1000))
		return false;
	if (!XMLTools::attributeExists(AttributeNode, "multiplierE"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AttributeNode, "multiplierE", 0, 1000))
		return false;
	return true;
}

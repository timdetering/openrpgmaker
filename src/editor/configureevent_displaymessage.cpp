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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QMessageBox>
#include <QMenu>
#include "configureevent_displaymessage.h"
#include "configureevent_wait.h"
#include "configuregradient.h"
#include "configuremenutext.h"
#include "getitemdialog.h"
#include "getnumberdialog.h"
#include "imagebrowser.h"
#include "menutext.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_DisplayMessage::ConfigureEvent_DisplayMessage(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	QMenu *insertMenu;
	QMenu *changeMenu;
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bXSideButton->setSpinBox(sbX);
	bYSideButton->setSpinBox(sbY);
	bWidthSideButton->setSpinBox(sbWidth);
	bHeightSideButton->setSpinBox(sbHeight);
	bXPaddingSideButton->setSpinBox(sbXPadding);
	bYPaddingSideButton->setSpinBox(sbYPadding);
	bOpacitySideButton->setSpinBox(sbOpacity);
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	lblWidthVariable->setText(varText);
	lblHeightVariable->setText(varText);
	lblOpacityVariable->setText(varText);
	wCoordinateVariables->setEnabled(false);
	wSizeVariables->setEnabled(false);
	wOpacityVariable->setEnabled(false);
	insertMenu = new QMenu(this);
	insertMenu->addAction(aInsertSpecialText);
	insertMenu->addAction(aInsertBitmap);
	insertMenu->addAction(aInsertPause);
	insertMenu->addAction(aInsertDollarSign);
	insertMenu->addAction(aCloseWindow);
	bInsert->setMenu(insertMenu);
	changeMenu = new QMenu(this);
	changeMenu->addAction(aChangeColor);
	changeMenu->addAction(aChangeFaceGraphic);
	changeMenu->addAction(aChangeDisplaySpeed);
	bChange->setMenu(changeMenu);
	QObject::connect(bTopColor, SIGNAL(colorChanged(QColor color)), this, SLOT(updateGradientPreview()));
	QObject::connect(bBottomColor, SIGNAL(colorChanged(QColor color)), this, SLOT(updateGradientPreview()));
}

ConfigureEvent_DisplayMessage::~ConfigureEvent_DisplayMessage()
{
}

QString ConfigureEvent_DisplayMessage::getFaceLocation()
{
	return faceLocation;
}

void ConfigureEvent_DisplayMessage::setFaceLocation(QString value)
{
	faceLocation = value;
	if (cbFlipFaceHorizontaly->isChecked())
		lblFaceGraphic->setPixmap(QPixmap::fromImage(QImage(faceLocation).mirrored(true, false)));
	else
		lblFaceGraphic->setPixmap(QPixmap(faceLocation));
}

QString ConfigureEvent_DisplayMessage::getMessage()
{
	return teMessage->toPlainText();
}

void ConfigureEvent_DisplayMessage::setMessage(QString value)
{
	teMessage->setPlainText(value);
}

int ConfigureEvent_DisplayMessage::getX()
{
	if (rbSpecificCoordinates->isChecked())
		return sbX->value();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

void ConfigureEvent_DisplayMessage::setX(int value)
{
	if (rbSpecificCoordinates->isChecked())
		sbX->setValue(value);
	else
		lblXVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_DisplayMessage::getY()
{
	if (rbSpecificCoordinates->isChecked())
		return sbY->value();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

void ConfigureEvent_DisplayMessage::setY(int value)
{
	if (rbSpecificCoordinates->isChecked())
		sbY->setValue(value);
	else
		lblYVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_DisplayMessage::getWidth()
{
	if (rbSpecificSize->isChecked())
		return sbWidth->value();
	return ProjectData::numberVariableIdFromString(lblWidthVariable->text());
}

void ConfigureEvent_DisplayMessage::setWidth(int value)
{
	if (rbSpecificSize->isChecked())
		sbWidth->setValue(value);
	else
		lblWidthVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_DisplayMessage::getHeight()
{
	if (rbSpecificSize->isChecked())
		return sbHeight->value();
	return ProjectData::numberVariableIdFromString(lblHeightVariable->text());
}

void ConfigureEvent_DisplayMessage::setHeight(int value)
{
	if (rbSpecificSize->isChecked())
		sbHeight->setValue(value);
	else
		lblHeightVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_DisplayMessage::getXPadding()
{
	return sbXPadding->value();
}

void ConfigureEvent_DisplayMessage::setXPadding(int value)
{
	sbXPadding->setValue(value);
}

int ConfigureEvent_DisplayMessage::getYPadding()
{
	return sbYPadding->value();
}

void ConfigureEvent_DisplayMessage::setYPadding(int value)
{
	sbYPadding->setValue(value);
}

int ConfigureEvent_DisplayMessage::getOpacity()
{
	if (rbSpecificOpacity->isChecked())
		return sbOpacity->value();
	return ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
}

void ConfigureEvent_DisplayMessage::setOpacity(int value)
{
	if (rbSpecificOpacity->isChecked())
		sbOpacity->setValue(value);
	else
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_DisplayMessage::getCoordinatesStoredInVariables()
{
	return rbStoredCoordinates->isChecked();
}

void ConfigureEvent_DisplayMessage::setCoordinatesStoredInVariables(bool stored)
{
	if (stored)
		rbStoredCoordinates->setChecked(true);
	else
		rbSpecificCoordinates->setChecked(true);
}

bool ConfigureEvent_DisplayMessage::getSizeStoredInVariables()
{
	return rbStoredSize->isChecked();
}

void ConfigureEvent_DisplayMessage::setSizeStoredInVariables(bool stored)
{
	if (stored)
		rbStoredSize->setChecked(true);
	else
		rbSpecificSize->setChecked(true);
}

bool ConfigureEvent_DisplayMessage::getOpacityStoredInVariable()
{
	return rbStoredOpacity->isChecked();
}

void ConfigureEvent_DisplayMessage::setOpacityStoredInVariable(bool stored)
{
	if (stored)
		rbStoredOpacity->setChecked(true);
	else
		rbSpecificOpacity->setChecked(true);
}

bool ConfigureEvent_DisplayMessage::getBlockEvents()
{
	return cbBlockEvents->isChecked();
}

void ConfigureEvent_DisplayMessage::setBlockEvents(bool blockEvents)
{
	cbBlockEvents->setChecked(blockEvents);
}

bool ConfigureEvent_DisplayMessage::getFlipFaceHorizontaly()
{
	return cbFlipFaceHorizontaly->isChecked();
}

void ConfigureEvent_DisplayMessage::setFlipFaceHorizontaly(bool flip)
{
	cbFlipFaceHorizontaly->setChecked(flip);
}

bool ConfigureEvent_DisplayMessage::getFaceDisplayedOnLeft()
{
	return (cboxFaceLocation->currentIndex() == 0) ? true:false;
}

void ConfigureEvent_DisplayMessage::setFaceDisplayedOnLeft(bool displayOnLeft)
{
	cboxFaceLocation->setCurrentIndex((displayOnLeft) ? 0:1);
}

int ConfigureEvent_DisplayMessage::getFontID()
{
	return cboxFont->currentIndex();
}

void ConfigureEvent_DisplayMessage::setFontID(int font)
{
	cboxFont->setCurrentIndex(font);
}

QColor ConfigureEvent_DisplayMessage::getTopColor()
{
	return bTopColor->getColor();
}

QColor ConfigureEvent_DisplayMessage::getBottomColor()
{
	return bBottomColor->getColor();
}

void ConfigureEvent_DisplayMessage::setColor(QColor top, QColor bottom)
{
	bTopColor->setColor(top);
	bBottomColor->setColor(bottom);
	updateGradientPreview();
}

void ConfigureEvent_DisplayMessage::on_aChangeColor_triggered()
{
	ConfigureGradient *configGradient = new ConfigureGradient();
	configGradient->setColors(bTopColor->getColor(), bBottomColor->getColor());
	if (configGradient->exec())
	{
		QColor topColor = configGradient->getTopColor();
		QColor bottomColor = configGradient->getBottomColor();
		QString colorText = QString("#%1%2%3").arg(topColor.red(), 2, 16, QChar('0')).arg(topColor.green(), 2, 16, QChar('0')).arg(topColor.blue(), 2, 16, QChar('0'));
		colorText += QString(", #%1%2%3").arg(bottomColor.red(), 2, 16, QChar('0')).arg(bottomColor.green(), 2, 16, QChar('0')).arg(bottomColor.blue(), 2, 16, QChar('0'));
		teMessage->insertPlainText(QString("$changeColor(%1)").arg(colorText));
	}
	delete configGradient;
}

void ConfigureEvent_DisplayMessage::on_aChangeDisplaySpeed_triggered()
{
	GetNumberDialog *dialog = new GetNumberDialog("Display Speed", 0, 20, this);
	if (dialog->exec())
	{
		QString text = "$changeDisplaySpeed(";
		int value = dialog->getValue();
		bool storedValue = dialog->isValueStoredInVariable();
		if (storedValue)
			teMessage->insertPlainText(QString("$changeDisplaySpeed($variable(%1))").arg(ProjectData::numberVariableIdToString(value, false)));
		else
			teMessage->insertPlainText(QString("$changeDisplaySpeed(%1)").arg(value));
	}
	delete dialog;
}

void ConfigureEvent_DisplayMessage::on_aChangeFaceGraphic_triggered()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/faces", ImageBrowser::BlockNone, QList<QSize>() << QSize(96, 96));
	if (imageBrowser->exec())
		teMessage->insertPlainText(QString("$changeFaceGraphic(%1)").arg(ProjectData::getRelativeResourcePath(imageBrowser->getFileLocation())));
	delete imageBrowser;
}

void ConfigureEvent_DisplayMessage::on_aCloseWindow_triggered()
{
	teMessage->insertPlainText("$closeWindow()");
}

void ConfigureEvent_DisplayMessage::on_aInsertBitmap_triggered()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	int blockedTypes = 0;
	blockedTypes += ImageBrowser::BlockBMP;
	blockedTypes += ImageBrowser::BlockGIF;
	blockedTypes += ImageBrowser::BlockJPEG;
	blockedTypes += ImageBrowser::BlockPBM;
	blockedTypes += ImageBrowser::BlockPCX;
	blockedTypes += ImageBrowser::BlockPGM;
	blockedTypes += ImageBrowser::BlockPNM;
	blockedTypes += ImageBrowser::BlockPPM;
	blockedTypes += ImageBrowser::BlockTGA;
	blockedTypes += ImageBrowser::BlockTIFF;
	blockedTypes += ImageBrowser::BlockXPM;
	imageBrowser->setupBrowser("/", blockedTypes, QList<QSize>() << QSize(32, 32));
	if (imageBrowser->exec())
		teMessage->insertPlainText(QString("$bitmap(%1)").arg(ProjectData::getRelativeResourcePath(imageBrowser->getFileLocation())));
	delete imageBrowser;
}

void ConfigureEvent_DisplayMessage::on_aInsertDollarSign_triggered()
{
	teMessage->insertPlainText("$$");
}

void ConfigureEvent_DisplayMessage::on_aInsertPause_triggered()
{
	ConfigureEvent_Wait *configWait = new ConfigureEvent_Wait(this);
	if (configWait->exec())
	{
		int time = configWait->getTime();
		bool timeStoredInVariable = configWait->isStoredTime();
		if (timeStoredInVariable)
			teMessage->insertPlainText(QString("$pause($variable(%1))").arg(ProjectData::numberVariableIdToString(time, false)));
		else
			teMessage->insertPlainText(QString("$pause(%1)").arg(time));
	}
}

void ConfigureEvent_DisplayMessage::on_aInsertSpecialText_triggered()
{
	MenuText *text = new MenuText;
	ConfigureMenuText *configText = new ConfigureMenuText(text, this, false);
	if (configText->exec())
	{
		QString finalText = "";
		int textType = text->getTextType();
		if (textType == MenuText::TYPE_PARTYMEMBER)
		{
			MenuText::PartyMemberText textData;
			finalText += "$partyMember(";
			text->getText(&textData);
			if (textData.memberID >= 0 && textData.memberID <= 3)
				finalText += QString("%1, ").arg(textData.memberID + 1);
			else
				finalText += QString("$variable(%1), ").arg(ProjectData::numberVariableIdToString(textData.memberID - 4, false));
			if (textData.textType == MenuText::PARTYMEMBER_NAME)
				finalText += "name)";
			else if (textData.textType == MenuText::PARTYMEMBER_CLASS)
				finalText += "class)";
			else if (textData.textType == MenuText::PARTYMEMBER_CONDITION)
				finalText += "condition)";
			else if (textData.textType == MenuText::PARTYMEMBER_LEVEL)
				finalText += "level)";
			else if (textData.textType == MenuText::PARTYMEMBER_HP)
				finalText += "hp)";
			else if (textData.textType == MenuText::PARTYMEMBER_MAXHP)
				finalText += "maxHP)";
			else if (textData.textType == MenuText::PARTYMEMBER_MP)
				finalText += "mp)";
			else if (textData.textType == MenuText::PARTYMEMBER_MAXMP)
				finalText += "maxMP)";
			else if (textData.textType == MenuText::PARTYMEMBER_EXPERIENCE)
				finalText += "experience)";
			else if (textData.textType == MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL)
				finalText += "experienceToNextLevel)";
			else if (textData.textType == MenuText::PARTYMEMBER_BASEATTACK)
				finalText += "baseAttack)";
			else if (textData.textType == MenuText::PARTYMEMBER_TOTALATTACK)
				finalText += "totalAttack)";
			else if (textData.textType == MenuText::PARTYMEMBER_BASEDEFENSE)
				finalText += "baseDefense)";
			else if (textData.textType == MenuText::PARTYMEMBER_TOTALDEFENSE)
				finalText += "totalDefense)";
			else if (textData.textType == MenuText::PARTYMEMBER_BASEWISDOM)
				finalText += "baseWisdom)";
			else if (textData.textType == MenuText::PARTYMEMBER_TOTALWISDOM)
				finalText += "totalWisdom)";
			else if (textData.textType == MenuText::PARTYMEMBER_BASEAGILITY)
				finalText += "baseAgility)";
			else if (textData.textType == MenuText::PARTYMEMBER_TOTALAGILITY)
				finalText += "totalAgility)";
			else if (textData.textType == MenuText::PARTYMEMBER_EQUIPPEDWEAPON)
				finalText += "equippedWeapon, ";
			else if (textData.textType == MenuText::PARTYMEMBER_EQUIPPEDSHIELD)
				finalText += "equippedShield, ";
			else if (textData.textType == MenuText::PARTYMEMBER_EQUIPPEDARMOR)
				finalText += "equippedArmor, ";
			else if (textData.textType == MenuText::PARTYMEMBER_EQUIPPEDHELMET)
				finalText += "equippedHelmet, ";
			else if (textData.textType == MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
				finalText += "equippedAccessory, ";
			if (textData.textType >= MenuText::PARTYMEMBER_EQUIPPEDWEAPON && textData.textType <= MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
			{
				if (textData.equipmentTextType == MenuText::EQUIPMENT_NAME)
					finalText == "name)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
					finalText == "description)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_ATTACK)
					finalText == "attack)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
					finalText == "defense)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_WISDOM)
					finalText == "wisdom)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_AGILITY)
					finalText == "agility)";
			}
		}
		else if (textType == MenuText::TYPE_MONSTER)
		{
			MenuText::MonsterText textData;
			finalText += "$monster(";
			text->getText(&textData);
			if (textData.idIsReferencedInVariable)
				finalText += QString("$variable(%1), ").arg(ProjectData::numberVariableIdToString(textData.monsterID, false));
			else
				finalText += QString("%1, ").arg(textData.monsterID);
			if (textData.textType == MenuText::MONSTER_NAME)
				finalText += "name)";
			else if (textData.textType == MenuText::MONSTER_MONEY)
				finalText += "money)";
			else if (textData.textType == MenuText::MONSTER_HP)
				finalText += "hp)";
			else if (textData.textType == MenuText::MONSTER_MP)
				finalText += "mp)";
			else if (textData.textType == MenuText::MONSTER_ATTACK)
				finalText += "attack)";
			else if (textData.textType == MenuText::MONSTER_DEFENSE)
				finalText += "defense)";
			else if (textData.textType == MenuText::MONSTER_WISDOM)
				finalText += "wisdom)";
			else if (textData.textType == MenuText::MONSTER_AGILITY)
				finalText += "agility)";
			else if (textData.textType == MenuText::MONSTER_STRONGAGAINST)
				finalText += "strongAgainst)";
			else if (textData.textType == MenuText::MONSTER_WEAKAGAINST)
				finalText += "weakAgainst)";
			else if (textData.textType == MenuText::MONSTER_EXPERIENCE)
				finalText += "experience)";
		}
		else if (textType == MenuText::TYPE_ITEM)
		{
			MenuText::ItemText textData;
			finalText += "$item(";
			text->getText(&textData);
			if (textData.idIsReferencedInVariable)
				finalText += QString("$variable(%1), ").arg(ProjectData::numberVariableIdToString(textData.itemID, false));
			else
				finalText += QString("%1, ").arg(textData.itemID);
			if (textData.textType == MenuText::ITEM_NAME)
				finalText += "name)";
			else if (textData.textType == MenuText::ITEM_DESCRIPTION)
				finalText += "description)";
			else if (textData.textType == MenuText::ITEM_PRICE)
				finalText += "price)";
		}
		else if (textType == MenuText::TYPE_CHARACTER)
		{
			MenuText::CharacterText textData;
			finalText += "$character(";
			text->getText(&textData);
			if (textData.idIsReferencedInVariable)
				finalText += QString("$variable(%1), ").arg(ProjectData::numberVariableIdToString(textData.characterID, false));
			else
				finalText += QString("%1, ").arg(textData.characterID);
			if (textData.textType == MenuText::CHARACTER_NAME)
				finalText += "name)";
			else if (textData.textType == MenuText::CHARACTER_CLASS)
				finalText += "class)";
			else if (textData.textType == MenuText::CHARACTER_CONDITION)
				finalText += "condition)";
			else if (textData.textType == MenuText::CHARACTER_LEVEL)
				finalText += "level)";
			else if (textData.textType == MenuText::CHARACTER_HP)
				finalText += "hp)";
			else if (textData.textType == MenuText::CHARACTER_MAXHP)
				finalText += "maxHP)";
			else if (textData.textType == MenuText::CHARACTER_MP)
				finalText += "mp)";
			else if (textData.textType == MenuText::CHARACTER_MAXMP)
				finalText += "maxMP)";
			else if (textData.textType == MenuText::CHARACTER_EXPERIENCE)
				finalText += "experience)";
			else if (textData.textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
				finalText += "experienceToNextLevel)";
			else if (textData.textType == MenuText::CHARACTER_BASEATTACK)
				finalText += "baseAttack)";
			else if (textData.textType == MenuText::CHARACTER_TOTALATTACK)
				finalText += "totalAttack)";
			else if (textData.textType == MenuText::CHARACTER_BASEDEFENSE)
				finalText += "baseDefense)";
			else if (textData.textType == MenuText::CHARACTER_TOTALDEFENSE)
				finalText += "totalDefense)";
			else if (textData.textType == MenuText::CHARACTER_BASEWISDOM)
				finalText += "baseWisdom)";
			else if (textData.textType == MenuText::CHARACTER_TOTALWISDOM)
				finalText += "totalWisdom)";
			else if (textData.textType == MenuText::CHARACTER_BASEAGILITY)
				finalText += "baseAgility)";
			else if (textData.textType == MenuText::CHARACTER_TOTALAGILITY)
				finalText += "totalAgility)";
			else if (textData.textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
				finalText += "equippedWeapon, ";
			else if (textData.textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
				finalText += "equippedShield, ";
			else if (textData.textType == MenuText::CHARACTER_EQUIPPEDARMOR)
				finalText += "equippedArmor, ";
			else if (textData.textType == MenuText::CHARACTER_EQUIPPEDHELMET)
				finalText += "equippedHelmet, ";
			else if (textData.textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
				finalText += "equippedAccessory, ";
			if (textData.textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData.textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
			{
				if (textData.equipmentTextType == MenuText::EQUIPMENT_NAME)
					finalText == "name)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
					finalText == "description)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_ATTACK)
					finalText == "attack)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
					finalText == "defense)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_WISDOM)
					finalText == "wisdom)";
				else if (textData.equipmentTextType == MenuText::EQUIPMENT_AGILITY)
					finalText == "agility)";
			}
		}
		else if (textType == MenuText::TYPE_VARIABLE)
		{
			MenuText::VariableText textData;
			text->getText(&textData);
			finalText += QString("$variable(%1").arg(ProjectData::numberVariableIdToString(textData.variableID, false));
			if (textData.alwaysShowPlusSign)
				finalText += ", alwaysShowPlusSign";
			finalText += ")";
		}
		else if (textType == MenuText::TYPE_SWITCH)
		{
			MenuText::SwitchText textData;
			text->getText(&textData);
			finalText += QString("$switch(%1, %2, %3)").arg(ProjectData::switchVariableIdToString(textData.switchID, false)).arg(textData.onText).arg(textData.offText);
		}
		else if (textType == MenuText::TYPE_STRING)
		{
			MenuText::StringText textData;
			text->getText(&textData);
			finalText += QString("$string(%1, ").arg(ProjectData::stringVariableIdToString(textData.stringID, false));
			finalText += (textData.systemString) ? "true":"false";
			finalText += ")";
		}
		if (!finalText.isEmpty())
			teMessage->insertPlainText(finalText);
	}
	delete configText;
	delete text;
}

void ConfigureEvent_DisplayMessage::on_bBrowseHeightVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblHeightVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblHeightVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_DisplayMessage::on_bBrowseOpacityVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_DisplayMessage::on_bBrowseWidthVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblWidthVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblWidthVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_DisplayMessage::on_bBrowseXVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblXVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblXVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_DisplayMessage::on_bBrowseYVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblYVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblYVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_DisplayMessage::on_bClearFaceGraphic_clicked()
{
	faceLocation = "";
	lblFaceGraphic->setPixmap(QPixmap());
}

void ConfigureEvent_DisplayMessage::on_bPreviewMessage_clicked()
{
	QMessageBox::information(this, "Feature Not Implemented", "Previewing of messages is not yet implemented.");
}

void ConfigureEvent_DisplayMessage::on_bSetFaceGraphic_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/faces", ImageBrowser::BlockNone, QList<QSize>() << QSize(96, 96));
	if (imageBrowser->exec())
		setFaceLocation(imageBrowser->getFileLocation());
	delete imageBrowser;
}

void ConfigureEvent_DisplayMessage::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Events::DisplayMessage");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void ConfigureEvent_DisplayMessage::on_cbFlipFaceHorizontaly_toggled(bool checked)
{
	if (checked)
		lblFaceGraphic->setPixmap(QPixmap::fromImage(QImage(faceLocation).mirrored(true, false)));
	else
		lblFaceGraphic->setPixmap(QPixmap(faceLocation));
}

void ConfigureEvent_DisplayMessage::on_rbSpecificCoordinates_toggled(bool checked)
{
	wCoordinates->setEnabled(checked);
	wCoordinateVariables->setEnabled(!checked);
}

void ConfigureEvent_DisplayMessage::on_rbSpecificOpacity_toggled(bool checked)
{
	wOpacity->setEnabled(checked);
	wOpacityVariable->setEnabled(!checked);
}

void ConfigureEvent_DisplayMessage::on_rbSpecificSize_toggled(bool checked)
{
	wSize->setEnabled(checked);
	wSizeVariables->setEnabled(!checked);
}

void ConfigureEvent_DisplayMessage::on_sbHeight_valueChanged(int value)
{
	lblHeightPixels->setText(QString("%1 pixels").arg((value * 32) + (sbYPadding->value() * 2)));
}

void ConfigureEvent_DisplayMessage::on_sbYPadding_valueChanged(int value)
{
	lblHeightPixels->setText(QString("%1 pixels").arg((sbHeight->value() * 32) + (value * 2)));
}

void ConfigureEvent_DisplayMessage::updateGradientPreview()
{
	QImage gradient = QImage(lblGradientPreview->width() - 4, lblGradientPreview->height() - 4, QImage::Format_RGB32);
	QColor topColor = bTopColor->getColor();
	QColor bottomColor = bBottomColor->getColor();
	float tR = (float)topColor.red() / 255.0;
	float tG = (float)topColor.green() / 255.0;
	float tB = (float)topColor.blue() / 255.0;
	float bR = (float)bottomColor.red() / 255.0;
	float bG = (float)bottomColor.green() / 255.0;
	float bB = (float)bottomColor.blue() / 255.0;
	float rangeR = bR - tR;
	float rangeG = bG - tG;
	float rangeB = bB - tB;
	float fR, fG, fB;
	for (int x = 0; x < gradient.width(); ++x)
	{
		float loc = (float)x / (float)(gradient.width() - 1);
		fR = (loc * rangeR) + tR;
		fG = (loc * rangeG) + tG;
		fB = (loc * rangeB) + tB;
		for (int y = 0; y < gradient.height(); ++y)
			gradient.setPixel(x, y, qRgb((int)(fR * 255), (int)(fG * 255), (int)(fB * 255)));
	}
	lblGradientPreview->setPixmap(QPixmap::fromImage(gradient));
}

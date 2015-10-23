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

#include "configureevent_changemenuimage.h"
#include "imagebrowser.h"
#include "menuobject.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeMenuImage::ConfigureEvent_ChangeMenuImage(int saveSlotID, int imageType, MenuImage::ImageLocation *location, int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	QString varText = ProjectData::numberVariableIdToString(0);
	imageLocation = location;
	type = imageType;
	setupUi(this);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_IMAGE);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuImage->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_IMAGE);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuImage->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		menuObjects = layout->getObjectsByType(MenuObject::TYPE_IMAGE);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuImage->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		layout = NULL;
	}
	if (cboxMenuImage->count() == 0)
	{
		cboxMenuImage->addItem("No Images To Configure");
		frmSaveSlot->setEnabled(false);
		wConfigureImage->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	cbUseValueInSaveSlot->setChecked((saveSlotID != 0));
	if (saveSlotID > 0)
		sbSaveSlot->setValue(saveSlotID);
	cboxCharacter->addItems(ProjectData::characterNames);
	cboxClass->addItems(ProjectData::classNames);
	cboxMonster->addItems(ProjectData::monsterNames);
	cboxVehicle->addItems(ProjectData::vehicleNames);
	bPartyMemberPoseSideButton->setSpinBox(sbPartyMemberPose);
	bCharacterPoseSideButton->setSpinBox(sbCharacterPose);
	bClassPoseSideButton->setSpinBox(sbClassPose);
	bMonsterPoseSideButton->setSpinBox(sbMonsterPose);
	bVehiclePoseSideButton->setSpinBox(sbVehiclePose);
	lblPartyMember_Variable->setText(varText);
	lblCharacter_Variable->setText(varText);
	lblClass_Variable->setText(varText);
	lblMonster_Variable->setText(varText);
	lblVehicle_Variable->setText(varText);
	lblTileset_Variable->setText(varText);
	lblTile_Variable->setText(varText);
	wPartyMember_Variable->setEnabled(false);
	cboxPartyMember_Direction->setEnabled(false);
	frmPartyMemberDirection->setEnabled(false);
	frmPartyMemberPose->setEnabled(false);
	wCharacter_Variable->setEnabled(false);
	frmCharacterDirection->setEnabled(false);
	frmCharacterPose->setEnabled(false);
	wClass_Variable->setEnabled(false);
	wMonster_Variable->setEnabled(false);
	wVehicle_Variable->setEnabled(false);
	wTile_Variables->setEnabled(false);
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		rbPartyMember->setChecked(true);
		if (imageLocation->partyMemberImage->memberID == 0)
			rbPartyMember1->setChecked(true);
		else if (imageLocation->partyMemberImage->memberID == 1)
			rbPartyMember2->setChecked(true);
		else if (imageLocation->partyMemberImage->memberID == 2)
			rbPartyMember3->setChecked(true);
		else if (imageLocation->partyMemberImage->memberID == 3)
			rbPartyMember4->setChecked(true);
		else
		{
			int varID = imageLocation->partyMemberImage->memberID - 4;
			rbPartyMemberVar->setChecked(true);
			lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (imageLocation->partyMemberImage->graphic == MenuImage::GRAPHIC_FACE)
			rbPartyMember_FaceGraphic->setChecked(true);
		else if (imageLocation->partyMemberImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbPartyMember_MapSprite->setChecked(true);
			cboxPartyMember_Direction->setCurrentIndex(imageLocation->partyMemberImage->direction);
			sbPartyMemberPose->setValue(imageLocation->partyMemberImage->pose + 1);
		}
		else
		{
			rbPartyMember_BattleSprite->setChecked(true);
			sbPartyMemberPose->setValue(imageLocation->partyMemberImage->pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		rbCharacter->setChecked(true);
		if (imageLocation->characterImage->idIsReferencedInVariable)
		{
			rbCharacter_Variable->setChecked(true);
			lblCharacter_Variable->setText(ProjectData::numberVariableIdToString(imageLocation->characterImage->characterID));
		}
		else
		{
			rbCharacter_Specific->setChecked(true);
			cboxCharacter->setCurrentIndex(imageLocation->characterImage->characterID);
		}
		if (imageLocation->characterImage->graphic == MenuImage::GRAPHIC_FACE)
			rbCharacter_FaceGraphic->setChecked(true);
		else if (imageLocation->characterImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbCharacter_MapSprite->setChecked(true);
			cboxCharacter_Direction->setCurrentIndex(imageLocation->characterImage->direction);
			sbCharacterPose->setValue(imageLocation->characterImage->pose + 1);
		}
		else
		{
			rbCharacter_BattleSprite->setChecked(true);
			sbCharacterPose->setValue(imageLocation->characterImage->pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		rbClass->setChecked(true);
		if (imageLocation->classImage->idIsReferencedInVariable)
		{
			rbClass_Variable->setChecked(true);
			lblClass_Variable->setText(ProjectData::numberVariableIdToString(imageLocation->classImage->classID));
		}
		else
		{
			rbClass_Specific->setChecked(true);
			cboxClass->setCurrentIndex(imageLocation->classImage->classID);
		}
		if (imageLocation->classImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbClass_MapSprite->setChecked(true);
			cboxClass_Direction->setCurrentIndex(imageLocation->classImage->direction);
			sbClassPose->setValue(imageLocation->classImage->pose + 1);
		}
		else
		{
			rbClass_BattleSprite->setChecked(true);
			sbClassPose->setValue(imageLocation->classImage->pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		rbMonster->setChecked(true);
		if (imageLocation->monsterImage->idIsReferencedInVariable)
		{
			rbMonster_Variable->setChecked(true);
			lblMonster_Variable->setText(ProjectData::numberVariableIdToString(imageLocation->monsterImage->monsterID));
		}
		else
		{
			rbMonster_Specific->setChecked(true);
			cboxMonster->setCurrentIndex(imageLocation->monsterImage->monsterID);
		}
		sbMonsterPose->setValue(imageLocation->monsterImage->pose + 1);
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		rbVehicle->setChecked(true);
		if (imageLocation->vehicleImage->idIsReferencedInVariable)
		{
			rbVehicle_Variable->setChecked(true);
			lblVehicle_Variable->setText(ProjectData::numberVariableIdToString(imageLocation->vehicleImage->vehicleID));
		}
		else
		{
			rbVehicle_Specific->setChecked(true);
			cboxVehicle->setCurrentIndex(imageLocation->vehicleImage->vehicleID);
		}
		cboxVehicle_Direction->setCurrentIndex(imageLocation->vehicleImage->direction);
		sbVehiclePose->setValue(imageLocation->vehicleImage->pose + 1);
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		int tilesetID, tileID;
		rbTile->setChecked(true);
		tilesetID = imageLocation->tileImage->tilesetID;
		tileID = imageLocation->tileImage->tileID;
		if (imageLocation->tileImage->idsReferencedInVariables)
		{
			rbTile_Variable->setChecked(true);
			lblTileset_Variable->setText(ProjectData::numberVariableIdToString(tilesetID));
			lblTile_Variable->setText(ProjectData::numberVariableIdToString(tileID));
		}
		else
		{
			rbTile_Specific->setChecked(true);
			bTile->setTile(((unsigned short)tilesetID << 8) + (unsigned short)tileID);
		}
	}
	else if (imageType == MenuImage::TYPE_FILE)
	{
		rbFile->setChecked(true);
		leFile->setText(*(imageLocation->fileLocation));
	}
}

ConfigureEvent_ChangeMenuImage::~ConfigureEvent_ChangeMenuImage()
{
}

void ConfigureEvent_ChangeMenuImage::setImageObject(QString object)
{
	int index = cboxMenuImage->findText(object);
	if (index != -1)
		cboxMenuImage->setCurrentIndex(index);
}

int ConfigureEvent_ChangeMenuImage::getImageType()
{
	if (rbPartyMember->isChecked())
		return MenuImage::TYPE_PARTYMEMBER;
	else if (rbCharacter->isChecked())
		return MenuImage::TYPE_CHARACTER;
	else if (rbClass->isChecked())
		return MenuImage::TYPE_CLASS;
	else if (rbMonster->isChecked())
		return MenuImage::TYPE_MONSTER;
	else if (rbVehicle->isChecked())
		return MenuImage::TYPE_VEHICLE;
	else if (rbTile->isChecked())
		return MenuImage::TYPE_TILE;
	return MenuImage::TYPE_FILE;
}

QString ConfigureEvent_ChangeMenuImage::getImageObject()
{
	return cboxMenuImage->currentText();
}

int ConfigureEvent_ChangeMenuImage::getSaveSlotID()
{
	if (cbUseValueInSaveSlot->isChecked())
		return sbSaveSlot->value();
	return 0;
}

void ConfigureEvent_ChangeMenuImage::on_buttonBox_accepted()
{
	if (type == MenuImage::TYPE_PARTYMEMBER)
		delete (imageLocation->partyMemberImage);
	else if (type == MenuImage::TYPE_CHARACTER)
		delete (imageLocation->characterImage);
	else if (type == MenuImage::TYPE_CLASS)
		delete (imageLocation->classImage);
	else if (type == MenuImage::TYPE_MONSTER)
		delete (imageLocation->monsterImage);
	else if (type == MenuImage::TYPE_VEHICLE)
		delete (imageLocation->vehicleImage);
	else if (type == MenuImage::TYPE_TILE)
		delete (imageLocation->tileImage);
	else if (type == MenuImage::TYPE_FILE)
		delete (imageLocation->fileLocation);
	if (rbPartyMember->isChecked())
	{
		imageLocation->partyMemberImage = new MenuImage::PartyMemberImage;
		if (rbPartyMember1->isChecked())
			imageLocation->partyMemberImage->memberID = 0;
		else if (rbPartyMember2->isChecked())
			imageLocation->partyMemberImage->memberID = 1;
		else if (rbPartyMember3->isChecked())
			imageLocation->partyMemberImage->memberID = 2;
		else if (rbPartyMember4->isChecked())
			imageLocation->partyMemberImage->memberID = 3;
		else
			imageLocation->partyMemberImage->memberID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text()) + 4;
		if (rbPartyMember_FaceGraphic->isChecked())
		{
			imageLocation->partyMemberImage->graphic = MenuImage::GRAPHIC_FACE;
			imageLocation->partyMemberImage->direction = 0;
			imageLocation->partyMemberImage->pose = 0;
		}
		else if (rbPartyMember_MapSprite->isChecked())
		{
			imageLocation->partyMemberImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation->partyMemberImage->direction = cboxPartyMember_Direction->currentIndex();
			imageLocation->partyMemberImage->pose = sbPartyMemberPose->value() - 1;
		}
		else
		{
			imageLocation->partyMemberImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation->partyMemberImage->direction = 0;
			imageLocation->partyMemberImage->pose = sbPartyMemberPose->value() - 1;
		}
	}
	else if (rbCharacter->isChecked())
	{
		imageLocation->characterImage = new MenuImage::CharacterImage;
		if (rbCharacter_Specific->isChecked())
		{
			imageLocation->characterImage->characterID = cboxCharacter->currentIndex();
			imageLocation->characterImage->idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation->characterImage->characterID = ProjectData::numberVariableIdFromString(lblCharacter_Variable->text());
			imageLocation->characterImage->idIsReferencedInVariable = true;
		}
		if (rbCharacter_FaceGraphic->isChecked())
		{
			imageLocation->characterImage->graphic = MenuImage::GRAPHIC_FACE;
			imageLocation->characterImage->direction = 0;
			imageLocation->characterImage->pose = 0;
		}
		else if (rbCharacter_MapSprite->isChecked())
		{
			imageLocation->characterImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation->characterImage->direction = cboxCharacter_Direction->currentIndex();
			imageLocation->characterImage->pose = sbCharacterPose->value() - 1;
		}
		else
		{
			imageLocation->characterImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation->characterImage->direction = 0;
			imageLocation->characterImage->pose = sbCharacterPose->value() - 1;
		}
	}
	else if (rbClass->isChecked())
	{
		imageLocation->classImage = new MenuImage::ClassImage;
		if (rbClass_Specific->isChecked())
		{
			imageLocation->classImage->classID = cboxClass->currentIndex();
			imageLocation->classImage->idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation->classImage->classID = ProjectData::numberVariableIdFromString(lblClass_Variable->text());
			imageLocation->classImage->idIsReferencedInVariable = true;
		}
		if (rbClass_MapSprite->isChecked())
		{
			imageLocation->classImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation->classImage->direction = cboxClass_Direction->currentIndex();
			imageLocation->classImage->pose = sbClassPose->value() - 1;
		}
		else if (rbClass_BattleSprite->isChecked())
		{
			imageLocation->classImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation->classImage->direction = 0;
			imageLocation->classImage->pose = sbClassPose->value() - 1;
		}
	}
	else if (rbMonster->isChecked())
	{
		imageLocation->monsterImage = new MenuImage::MonsterImage;
		if (rbMonster_Specific->isChecked())
		{
			imageLocation->monsterImage->monsterID = cboxMonster->currentIndex();
			imageLocation->monsterImage->idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation->monsterImage->monsterID = ProjectData::numberVariableIdFromString(lblMonster_Variable->text());
			imageLocation->monsterImage->idIsReferencedInVariable = true;
		}
		imageLocation->monsterImage->pose = sbMonsterPose->value() - 1;
	}
	else if (rbVehicle->isChecked())
	{
		imageLocation->vehicleImage = new MenuImage::VehicleImage;
		if (rbVehicle_Specific->isChecked())
		{
			imageLocation->vehicleImage->vehicleID = cboxVehicle->currentIndex();
			imageLocation->vehicleImage->idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation->vehicleImage->vehicleID = ProjectData::numberVariableIdFromString(lblVehicle_Variable->text());
			imageLocation->vehicleImage->idIsReferencedInVariable = true;
		}
		imageLocation->vehicleImage->direction = cboxVehicle_Direction->currentIndex();
		imageLocation->vehicleImage->pose = sbVehiclePose->value() - 1;
	}
	else if (rbTile->isChecked())
	{
		imageLocation->tileImage = new MenuImage::TileImage;
		if (rbTile_Specific->isChecked())
		{
			unsigned short tile = bTile->getTile();
			imageLocation->tileImage->tilesetID = (int)((tile >> 8) % 256);
			imageLocation->tileImage->tileID = (int)(tile % 256);
			imageLocation->tileImage->idsReferencedInVariables = false;
		}
		else
		{
			imageLocation->tileImage->tilesetID = ProjectData::numberVariableIdFromString(lblTileset_Variable->text());
			imageLocation->tileImage->tileID = ProjectData::numberVariableIdFromString(lblTile_Variable->text());
			imageLocation->tileImage->idsReferencedInVariables = true;
		}
	}
	else if (rbFile->isChecked())
		imageLocation->fileLocation = new QString(ProjectData::getAbsoluteResourcePath(leFile->text()));
}

void ConfigureEvent_ChangeMenuImage::on_bBrowseFiles_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser(this);
	imageBrowser->setupBrowser("/");
	if (imageBrowser->exec())
		leFile->setText(ProjectData::getRelativeResourcePath(imageBrowser->getFileLocation()));
	delete imageBrowser;
}

void ConfigureEvent_ChangeMenuImage::on_bCharacter_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblCharacter_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblCharacter_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bClass_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblClass_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblClass_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bMonster_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMonster_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMonster_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bTile_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTile_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTile_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bTileset_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTileset_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTileset_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_bVehicle_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVehicle_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVehicle_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuImage::on_rbFile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wFile);
}

void ConfigureEvent_ChangeMenuImage::on_rbPartyMember_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPartyMember);
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wCharacter);
}

void ConfigureEvent_ChangeMenuImage::on_rbClass_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wClass);
}

void ConfigureEvent_ChangeMenuImage::on_rbMonster_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wMonster);
}

void ConfigureEvent_ChangeMenuImage::on_rbVehicle_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wVehicle);
}

void ConfigureEvent_ChangeMenuImage::on_rbTile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wTile);
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(false);
		frmCharacterPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_FaceGraphic_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(false);
		frmCharacterPose->setEnabled(false);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_MapSprite_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(true);
		frmCharacterPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_Specific_toggled(bool checked)
{
	cboxCharacter->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbCharacter_Variable_toggled(bool checked)
{
	wCharacter_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbClass_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmClassDirection->setEnabled(false);
		frmClassPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbClass_MapSprite_toggleD(bool checked)
{
	if (checked)
	{
		frmClassDirection->setEnabled(true);
		frmClassPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbClass_Specific_toggled(bool checked)
{
	cboxClass->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbClass_Variable_toggled(bool checked)
{
	wClass_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbMonster_Specific_toggled(bool checked)
{
	cboxMonster->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbMonster_Variable_toggled(bool checked)
{
	wMonster_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbPartyMemberVar_toggled(bool checked)
{
	wPartyMember_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbPartyMember_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(false);
		frmPartyMemberPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbPartyMember_FaceGraphic_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(false);
		frmPartyMemberPose->setEnabled(false);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbPartyMember_MapSprite_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(true);
		frmPartyMemberPose->setEnabled(true);
	}
}

void ConfigureEvent_ChangeMenuImage::on_rbTile_Specific_toggled(bool checked)
{
	bTile->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbTile_Variable_toggled(bool checked)
{
	wTile_Variables->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbVehicle_Specific_toggled(bool checked)
{
	cboxVehicle->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_rbVehicle_Variable_toggled(bool checked)
{
	wVehicle_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuImage::on_cbUseValueInSaveSlot_toggled(bool checked)
{
	wSaveSlot->setEnabled(checked);
}

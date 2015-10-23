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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configuremenuimage.h"
#include "imagebrowser.h"
#include "menuimage.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureMenuImage::ConfigureMenuImage(MenuImage *imageToConfigure, QWidget *parent, bool enableSelectSaveSlot) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	int imageType;
	int saveSlotID = imageToConfigure->getSaveSlotID();
	image = imageToConfigure;
	setupUi(this);
	bSaveSlotSideButton->setSpinBox(sbSaveSlot);
	cboxCharacter->addItems(ProjectData::characterNames);
	cboxClass->addItems(ProjectData::classNames);
	cboxMonster->addItems(ProjectData::monsterNames);
	cboxVehicle->addItems(ProjectData::vehicleNames);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
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
	imageType = image->getImageType();
	cbUseValueInSaveSlot->setChecked((saveSlotID != 0));
	if (saveSlotID > 0)
		sbSaveSlot->setValue(saveSlotID);
	frmSaveSlot->setVisible(enableSelectSaveSlot);
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		MenuImage::PartyMemberImage imageLocation;
		image->getImageLocation(&imageLocation);
		rbPartyMember->setChecked(true);
		if (imageLocation.memberID == 0)
			rbPartyMember1->setChecked(true);
		else if (imageLocation.memberID == 1)
			rbPartyMember2->setChecked(true);
		else if (imageLocation.memberID == 2)
			rbPartyMember3->setChecked(true);
		else if (imageLocation.memberID == 3)
			rbPartyMember4->setChecked(true);
		else
		{
			int varID = imageLocation.memberID - 4;
			rbPartyMemberVar->setChecked(true);
			lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (imageLocation.graphic == MenuImage::GRAPHIC_FACE)
			rbPartyMember_FaceGraphic->setChecked(true);
		else if (imageLocation.graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbPartyMember_MapSprite->setChecked(true);
			cboxPartyMember_Direction->setCurrentIndex(imageLocation.direction);
			sbPartyMemberPose->setValue(imageLocation.pose + 1);
		}
		else
		{
			rbPartyMember_BattleSprite->setChecked(true);
			sbPartyMemberPose->setValue(imageLocation.pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		MenuImage::CharacterImage imageLocation;
		image->getImageLocation(&imageLocation);
		rbCharacter->setChecked(true);
		if (imageLocation.idIsReferencedInVariable)
		{
			rbCharacter_Variable->setChecked(true);
			lblCharacter_Variable->setText(ProjectData::numberVariableIdToString(imageLocation.characterID));
		}
		else
		{
			rbCharacter_Specific->setChecked(true);
			cboxCharacter->setCurrentIndex(imageLocation.characterID);
		}
		if (imageLocation.graphic == MenuImage::GRAPHIC_FACE)
			rbCharacter_FaceGraphic->setChecked(true);
		else if (imageLocation.graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbCharacter_MapSprite->setChecked(true);
			cboxCharacter_Direction->setCurrentIndex(imageLocation.direction);
			sbCharacterPose->setValue(imageLocation.pose + 1);
		}
		else
		{
			rbCharacter_BattleSprite->setChecked(true);
			sbCharacterPose->setValue(imageLocation.pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		MenuImage::ClassImage imageLocation;
		image->getImageLocation(&imageLocation);
		rbClass->setChecked(true);
		if (imageLocation.idIsReferencedInVariable)
		{
			rbClass_Variable->setChecked(true);
			lblClass_Variable->setText(ProjectData::numberVariableIdToString(imageLocation.classID));
		}
		else
		{
			rbClass_Specific->setChecked(true);
			cboxClass->setCurrentIndex(imageLocation.classID);
		}
		if (imageLocation.graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			rbClass_MapSprite->setChecked(true);
			cboxClass_Direction->setCurrentIndex(imageLocation.direction);
			sbClassPose->setValue(imageLocation.pose + 1);
		}
		else
		{
			rbClass_BattleSprite->setChecked(true);
			sbClassPose->setValue(imageLocation.pose + 1);
		}
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		MenuImage::MonsterImage imageLocation;
		image->getImageLocation(&imageLocation);
		rbMonster->setChecked(true);
		if (imageLocation.idIsReferencedInVariable)
		{
			rbMonster_Variable->setChecked(true);
			lblMonster_Variable->setText(ProjectData::numberVariableIdToString(imageLocation.monsterID));
		}
		else
		{
			rbMonster_Specific->setChecked(true);
			cboxMonster->setCurrentIndex(imageLocation.monsterID);
		}
		sbMonsterPose->setValue(imageLocation.pose + 1);
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		MenuImage::VehicleImage imageLocation;
		image->getImageLocation(&imageLocation);
		rbVehicle->setChecked(true);
		if (imageLocation.idIsReferencedInVariable)
		{
			rbVehicle_Variable->setChecked(true);
			lblVehicle_Variable->setText(ProjectData::numberVariableIdToString(imageLocation.vehicleID));
		}
		else
		{
			rbVehicle_Specific->setChecked(true);
			cboxVehicle->setCurrentIndex(imageLocation.vehicleID);
		}
		cboxVehicle_Direction->setCurrentIndex(imageLocation.direction);
		sbVehiclePose->setValue(imageLocation.pose + 1);
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		MenuImage::TileImage imageLocation;
		int tilesetID, tileID;
		image->getImageLocation(&imageLocation);
		rbTile->setChecked(true);
		tilesetID = imageLocation.tilesetID;
		tileID = imageLocation.tileID;
		if (imageLocation.idsReferencedInVariables)
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
		QString imageLocation;
		image->getImageLocation(&imageLocation);
		rbFile->setChecked(true);
		leFile->setText(imageLocation);
	}
}

ConfigureMenuImage::~ConfigureMenuImage()
{
}

void ConfigureMenuImage::on_bBrowseFiles_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser(this);
	imageBrowser->setupBrowser("/");
	if (imageBrowser->exec())
		leFile->setText(ProjectData::getRelativeResourcePath(imageBrowser->getFileLocation()));
	delete imageBrowser;
}

void ConfigureMenuImage::on_bCharacter_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bClass_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bMonster_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bPartyMember_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bTile_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bTileset_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_bVehicle_BrowseVariables_clicked()
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

void ConfigureMenuImage::on_rbFile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wFile);
}

void ConfigureMenuImage::on_rbPartyMember_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPartyMember);
}

void ConfigureMenuImage::on_rbCharacter_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wCharacter);
}

void ConfigureMenuImage::on_rbClass_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wClass);
}

void ConfigureMenuImage::on_rbMonster_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wMonster);
}

void ConfigureMenuImage::on_rbVehicle_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wVehicle);
}

void ConfigureMenuImage::on_rbTile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wTile);
}

void ConfigureMenuImage::on_rbCharacter_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(false);
		frmCharacterPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbCharacter_FaceGraphic_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(false);
		frmCharacterPose->setEnabled(false);
	}
}

void ConfigureMenuImage::on_rbCharacter_MapSprite_toggled(bool checked)
{
	if (checked)
	{
		frmCharacterDirection->setEnabled(true);
		frmCharacterPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbCharacter_Specific_toggled(bool checked)
{
	cboxCharacter->setEnabled(checked);
}

void ConfigureMenuImage::on_rbCharacter_Variable_toggled(bool checked)
{
	wCharacter_Variable->setEnabled(checked);
}

void ConfigureMenuImage::on_rbClass_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmClassDirection->setEnabled(false);
		frmClassPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbClass_MapSprite_toggleD(bool checked)
{
	if (checked)
	{
		frmClassDirection->setEnabled(true);
		frmClassPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbClass_Specific_toggled(bool checked)
{
	cboxClass->setEnabled(checked);
}

void ConfigureMenuImage::on_rbClass_Variable_toggled(bool checked)
{
	wClass_Variable->setEnabled(checked);
}

void ConfigureMenuImage::on_rbMonster_Specific_toggled(bool checked)
{
	cboxMonster->setEnabled(checked);
}

void ConfigureMenuImage::on_rbMonster_Variable_toggled(bool checked)
{
	wMonster_Variable->setEnabled(checked);
}

void ConfigureMenuImage::on_rbPartyMemberVar_toggled(bool checked)
{
	wPartyMember_Variable->setEnabled(checked);
}

void ConfigureMenuImage::on_rbPartyMember_BattleSprite_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(false);
		frmPartyMemberPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbPartyMember_FaceGraphic_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(false);
		frmPartyMemberPose->setEnabled(false);
	}
}

void ConfigureMenuImage::on_rbPartyMember_MapSprite_toggled(bool checked)
{
	if (checked)
	{
		frmPartyMemberDirection->setEnabled(true);
		frmPartyMemberPose->setEnabled(true);
	}
}

void ConfigureMenuImage::on_rbTile_Specific_toggled(bool checked)
{
	bTile->setEnabled(checked);
}

void ConfigureMenuImage::on_rbTile_Variable_toggled(bool checked)
{
	wTile_Variables->setEnabled(checked);
}

void ConfigureMenuImage::on_rbVehicle_Specific_toggled(bool checked)
{
	cboxVehicle->setEnabled(checked);
}

void ConfigureMenuImage::on_rbVehicle_Variable_toggled(bool checked)
{
	wVehicle_Variable->setEnabled(checked);
}

void ConfigureMenuImage::on_cbUseValueInSaveSlot_toggled(bool checked)
{
	wSaveSlot->setEnabled(checked);
}

void ConfigureMenuImage::on_buttonBox_accepted()
{
	if (cbUseValueInSaveSlot->isChecked())
		image->setSaveSlotID(sbSaveSlot->value());
	else
		image->setSaveSlotID(0);
	if (rbPartyMember->isChecked())
	{
		MenuImage::PartyMemberImage imageLocation;
		if (rbPartyMember1->isChecked())
			imageLocation.memberID = 0;
		else if (rbPartyMember2->isChecked())
			imageLocation.memberID = 1;
		else if (rbPartyMember3->isChecked())
			imageLocation.memberID = 2;
		else if (rbPartyMember4->isChecked())
			imageLocation.memberID = 3;
		else
			imageLocation.memberID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text()) + 4;
		if (rbPartyMember_FaceGraphic->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_FACE;
			imageLocation.direction = 0;
			imageLocation.pose = 0;
		}
		else if (rbPartyMember_MapSprite->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.direction = cboxPartyMember_Direction->currentIndex();
			imageLocation.pose = sbPartyMemberPose->value() - 1;
		}
		else
		{
			imageLocation.graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.direction = 0;
			imageLocation.pose = sbPartyMemberPose->value() - 1;
		}
		image->setImageType(MenuImage::TYPE_PARTYMEMBER);
		image->setImageLocation(&imageLocation);
	}
	else if (rbCharacter->isChecked())
	{
		MenuImage::CharacterImage imageLocation;
		if (rbCharacter_Specific->isChecked())
		{
			imageLocation.characterID = cboxCharacter->currentIndex();
			imageLocation.idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation.characterID = ProjectData::numberVariableIdFromString(lblCharacter_Variable->text());
			imageLocation.idIsReferencedInVariable = true;
		}
		if (rbCharacter_FaceGraphic->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_FACE;
			imageLocation.direction = 0;
			imageLocation.pose = 0;
		}
		else if (rbCharacter_MapSprite->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.direction = cboxCharacter_Direction->currentIndex();
			imageLocation.pose = sbCharacterPose->value() - 1;
		}
		else
		{
			imageLocation.graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.direction = 0;
			imageLocation.pose = sbCharacterPose->value() - 1;
		}
		image->setImageType(MenuImage::TYPE_CHARACTER);
		image->setImageLocation(&imageLocation);
	}
	else if (rbClass->isChecked())
	{
		MenuImage::ClassImage imageLocation;
		if (rbClass_Specific->isChecked())
		{
			imageLocation.classID = cboxClass->currentIndex();
			imageLocation.idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation.classID = ProjectData::numberVariableIdFromString(lblClass_Variable->text());
			imageLocation.idIsReferencedInVariable = true;
		}
		if (rbClass_MapSprite->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.direction = cboxClass_Direction->currentIndex();
			imageLocation.pose = sbClassPose->value() - 1;
		}
		else if (rbClass_BattleSprite->isChecked())
		{
			imageLocation.graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.direction = 0;
			imageLocation.pose = sbClassPose->value() - 1;
		}
		image->setImageType(MenuImage::TYPE_CLASS);
		image->setImageLocation(&imageLocation);
	}
	else if (rbMonster->isChecked())
	{
		MenuImage::MonsterImage imageLocation;
		if (rbMonster_Specific->isChecked())
		{
			imageLocation.monsterID = cboxMonster->currentIndex();
			imageLocation.idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation.monsterID = ProjectData::numberVariableIdFromString(lblMonster_Variable->text());
			imageLocation.idIsReferencedInVariable = true;
		}
		imageLocation.pose = sbMonsterPose->value() - 1;
		image->setImageType(MenuImage::TYPE_MONSTER);
		image->setImageLocation(&imageLocation);
	}
	else if (rbVehicle->isChecked())
	{
		MenuImage::VehicleImage imageLocation;
		if (rbVehicle_Specific->isChecked())
		{
			imageLocation.vehicleID = cboxVehicle->currentIndex();
			imageLocation.idIsReferencedInVariable = false;
		}
		else
		{
			imageLocation.vehicleID = ProjectData::numberVariableIdFromString(lblVehicle_Variable->text());
			imageLocation.idIsReferencedInVariable = true;
		}
		imageLocation.direction = cboxVehicle_Direction->currentIndex();
		imageLocation.pose = sbVehiclePose->value() - 1;
		image->setImageType(MenuImage::TYPE_VEHICLE);
		image->setImageLocation(&imageLocation);
	}
	else if (rbTile->isChecked())
	{
		MenuImage::TileImage imageLocation;
		if (rbTile_Specific->isChecked())
		{
			unsigned short tile = bTile->getTile();
			imageLocation.tilesetID = (int)((tile >> 8) % 256);
			imageLocation.tileID = (int)(tile % 256);
			imageLocation.idsReferencedInVariables = false;
		}
		else
		{
			imageLocation.tilesetID = ProjectData::numberVariableIdFromString(lblTileset_Variable->text());
			imageLocation.tileID = ProjectData::numberVariableIdFromString(lblTile_Variable->text());
			imageLocation.idsReferencedInVariables = true;
		}
		image->setImageType(MenuImage::TYPE_TILE);
		image->setImageLocation(&imageLocation);
	}
	else if (rbFile->isChecked())
	{
		image->setImageType(MenuImage::TYPE_FILE);
		image->setImageLocation(ProjectData::getAbsoluteResourcePath(leFile->text()));
	}
}

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

#include "configureevent_changefacegraphic.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeFaceGraphic::ConfigureEvent_ChangeFaceGraphic(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	wCharacterVariable->setEnabled(false);
}

ConfigureEvent_ChangeFaceGraphic::~ConfigureEvent_ChangeFaceGraphic()
{
}

void ConfigureEvent_ChangeFaceGraphic::setCharacterID(int characterID)
{
	ProjectData::DataReference<Character> *charRef;
	if (rbSpecificCharacter->isChecked())
	{
		cboxCharacters->setCurrentIndex(characterID);
		charRef = new ProjectData::DataReference<Character>(characterID, __FILE__, __LINE__);
	}
	else
	{
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
		charRef = new ProjectData::DataReference<Character>(0, __FILE__, __LINE__);
	}
	setFaceGraphicLocation((*charRef)->getFaceGraphicLocation());
	delete charRef;
}

void ConfigureEvent_ChangeFaceGraphic::setIsReferencedCharacter(bool referenced)
{
	if (referenced)
		rbReferencedCharacter->setChecked(true);
	else
		rbSpecificCharacter->setChecked(true);
}

void ConfigureEvent_ChangeFaceGraphic::setFaceGraphicLocation(QString location)
{
	if (!location.isEmpty())
	{
		lblFaceGraphic->setPixmap(QPixmap(location));
		faceLocation = location;
	}
}

int ConfigureEvent_ChangeFaceGraphic::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
}

bool ConfigureEvent_ChangeFaceGraphic::isReferencedCharacter()
{
	return rbReferencedCharacter->isChecked();
}

QString ConfigureEvent_ChangeFaceGraphic::getFaceGraphicLocation()
{
	return faceLocation;
}

void ConfigureEvent_ChangeFaceGraphic::on_bBrowseCharacterVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeFaceGraphic::on_bSetFaceGraphic_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/faces", ImageBrowser::BlockNone, QList<QSize>() << QSize(96, 96));
	if (imageBrowser->exec())
		setFaceGraphicLocation(imageBrowser->getFileLocation());
	delete imageBrowser;
}

void ConfigureEvent_ChangeFaceGraphic::on_cboxCharacters_currentIndexChanged(int index)
{
	ProjectData::DataReference<Character> *charRef = new ProjectData::DataReference<Character>(index, __FILE__, __LINE__);
	setFaceGraphicLocation((*charRef)->getFaceGraphicLocation());
	delete charRef;
}

void ConfigureEvent_ChangeFaceGraphic::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}

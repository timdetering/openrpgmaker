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

#include <QListWidgetItem>
#include <QVariant>
#include <QFileInfo>
#include "configuremapeventmovementpattern.h"
#include "configuremapeventpagespritegraphic.h"
#include "configuremovementpattern_wait.h"
#include "configuremovementpattern_increasedecreasetransparency.h"
#include "projectdata.h"
#include "projectdata_mapevent.h"
#include "soundeffectbrowser.h"
#include "variableselector.h"

QStringList ConfigureMapEventMovementPattern::patternNames = QStringList() \
	<< "Move Up-Left" \
	<< "Move Up" \
	<< "Move Up-Right" \
	<< "Move Right" \
	<< "Move Down-Right" \
	<< "Move Down" \
	<< "Move Down-Left" \
	<< "Move Left" \
	<< "Move Randomly" \
	<< "Increase Movement Speed" \
	<< "Decrease Movement Speed" \
	<< "Move Forward" \
	<< "Move Backward" \
	<< "Move Toward Hero" \
	<< "Move Away From Hero" \
	<< "Face Up" \
	<< "Face Down" \
	<< "Face Left" \
	<< "Face Right" \
	<< "Turn Left" \
	<< "Turn Right" \
	<< "U-Turn" \
	<< "Face Random Direction" \
	<< "Face Left Right Randomly" \
	<< "Face Hero" \
	<< "Face Away From Hero" \
	<< "Increase Movement Frequency" \
	<< "Decrease Movement Frequency" \
	<< "Begin Jump" \
	<< "End Jump" \
	<< "Lock Facing" \
	<< "Unlock Facing" \
	<< "Wait" \
	<< "Phasing Mode OFF" \
	<< "Phasing Mode ON" \
	<< "Stop Animation" \
	<< "Resume Animation" \
	<< "Change Graphic" \
	<< "Increase Transparency" \
	<< "Decrease Transparency" \
	<< "Switch ON" \
	<< "Switch OFF" \
	<< "Play Sound Effect";

ConfigureMapEventMovementPattern::ConfigureMapEventMovementPattern(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bDelete->setEnabled(false);
	bClearList->setEnabled(false);
	bMoveSelectedUp->setEnabled(false);
	bMoveSelectedDown->setEnabled(false);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureMapEventMovementPattern::~ConfigureMapEventMovementPattern()
{
	for (int i = 0; i < patterns.size(); ++i)
		delete (patterns[i]);
	patterns.clear();
}

void ConfigureMapEventMovementPattern::addPattern(MapEvent::MovementPattern *pattern)
{
	QListWidgetItem *item;
	int location = patterns.size();
	patterns.append(new MapEvent::MovementPattern(pattern));
	item = new QListWidgetItem(getPatternViewableText(pattern));
	item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
	item->setData(Qt::UserRole, QVariant(location));
	lwPattern->addItem(item);
	bClearList->setEnabled(true);
}

MapEvent::MovementPattern *ConfigureMapEventMovementPattern::getPattern(int location)
{
	return patterns[lwPattern->item(location)->data(Qt::UserRole).toInt()];
}

int ConfigureMapEventMovementPattern::numPatterns()
{
	return patterns.size();
}

bool ConfigureMapEventMovementPattern::getRepeatPattern()
{
	return cbRepeatPattern->isChecked();
}

void ConfigureMapEventMovementPattern::setRepeatPattern(bool value)
{
	cbRepeatPattern->setChecked(value);
}

bool ConfigureMapEventMovementPattern::getIgnoreImpossibleMoves()
{
	return cbIgnoreImpossibleMoves->isChecked();
}

void ConfigureMapEventMovementPattern::setIgnoreImpossibleMoves(bool value)
{
	cbIgnoreImpossibleMoves->setChecked(value);
}

void ConfigureMapEventMovementPattern::on_bDelete_clicked()
{
	QListWidgetItem *item;
	int patternID, row = lwPattern->currentRow();
	item = lwPattern->takeItem(row);
	patternID = item->data(Qt::UserRole).toInt();
	delete (patterns[patternID]);
	patterns.removeAt(patternID);
	delete item;
	for (int i = 0; i < lwPattern->count(); ++i)
	{
		item = lwPattern->item(i);
		patternID = item->data(Qt::UserRole).toInt();
		if (patternID >= row)
			--patternID;
		item->setData(Qt::UserRole, QVariant(patternID));
	}
	if (patterns.size() == 0)
		bClearList->setEnabled(false);
	row = lwPattern->currentRow();
	if (row > 0)
		bMoveSelectedUp->setEnabled(true);
	else
		bMoveSelectedUp->setEnabled(false);
	if (row < patterns.size() - 1)
		bMoveSelectedDown->setEnabled(true);
	else
		bMoveSelectedDown->setEnabled(false);
}

void ConfigureMapEventMovementPattern::on_bClearList_clicked()
{
	for (int i = 0; i < patterns.size(); ++i)
		delete (patterns[i]);
	patterns.clear();
	lwPattern->clear();
	bClearList->setEnabled(false);
	bMoveSelectedUp->setEnabled(false);
	bMoveSelectedDown->setEnabled(false);
}

void ConfigureMapEventMovementPattern::on_bMoveSelectedUp_clicked()
{
	QListWidgetItem *item;
	int row = lwPattern->currentRow();
	item = lwPattern->takeItem(row);
	--row;
	lwPattern->insertItem(row, item);
	item->setSelected(true);
	lwPattern->setCurrentRow(row);
	if (row > 0)
		bMoveSelectedUp->setEnabled(true);
	else
		bMoveSelectedUp->setEnabled(false);
	bMoveSelectedDown->setEnabled(true);
}

void ConfigureMapEventMovementPattern::on_bMoveSelectedDown_clicked()
{
	QListWidgetItem *item;
	int row = lwPattern->currentRow();
	item = lwPattern->takeItem(row);
	++row;
	lwPattern->insertItem(row, item);
	item->setSelected(true);
	lwPattern->setCurrentRow(row);
	if (row < patterns.size() - 1)
		bMoveSelectedDown->setEnabled(true);
	else
		bMoveSelectedDown->setEnabled(false);
	bMoveSelectedUp->setEnabled(true);
}

void ConfigureMapEventMovementPattern::on_bMoveUpLeft_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEUPLEFT);
}

void ConfigureMapEventMovementPattern::on_bMoveUp_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEUP);
}

void ConfigureMapEventMovementPattern::on_bMoveUpRight_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEUPRIGHT);
}

void ConfigureMapEventMovementPattern::on_bMoveRight_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVERIGHT);
}

void ConfigureMapEventMovementPattern::on_bMoveDownRight_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEDOWNRIGHT);
}

void ConfigureMapEventMovementPattern::on_bMoveDown_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEDOWN);
}

void ConfigureMapEventMovementPattern::on_bMoveDownLeft_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEDOWNLEFT);
}

void ConfigureMapEventMovementPattern::on_bMoveLeft_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVELEFT);
}

void ConfigureMapEventMovementPattern::on_bMoveRandomly_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVERANDOMLY);
}

void ConfigureMapEventMovementPattern::on_bIncreaseMoveSpeed_clicked()
{
	addNewPattern(MapEvent::PATTERN_INCREASEMOVEMENTSPEED);
}

void ConfigureMapEventMovementPattern::on_bDecreaseMoveSpeed_clicked()
{
	addNewPattern(MapEvent::PATTERN_DECREASEMOVEMENTSPEED);
}

void ConfigureMapEventMovementPattern::on_bMoveForward_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEFORWARD);
}

void ConfigureMapEventMovementPattern::on_bMoveBackward_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEBACKWARD);
}

void ConfigureMapEventMovementPattern::on_bMoveTowardHero_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVETOWARDHERO);
}

void ConfigureMapEventMovementPattern::on_bMoveAwayFromHero_clicked()
{
	addNewPattern(MapEvent::PATTERN_MOVEAWAYFROMHERO);
}

void ConfigureMapEventMovementPattern::on_bFaceUp_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACEUP);
}

void ConfigureMapEventMovementPattern::on_bFaceDown_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACEDOWN);
}

void ConfigureMapEventMovementPattern::on_bFaceLeft_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACELEFT);
}

void ConfigureMapEventMovementPattern::on_bFaceRight_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACERIGHT);
}

void ConfigureMapEventMovementPattern::on_bTurnLeft_clicked()
{
	addNewPattern(MapEvent::PATTERN_TURNLEFT);
}

void ConfigureMapEventMovementPattern::on_bTurnRight_clicked()
{
	addNewPattern(MapEvent::PATTERN_TURNRIGHT);
}

void ConfigureMapEventMovementPattern::on_bUTurn_clicked()
{
	addNewPattern(MapEvent::PATTERN_UTURN);
}

void ConfigureMapEventMovementPattern::on_bFaceRandomDirection_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACERANDOMDIRECTION);
}

void ConfigureMapEventMovementPattern::on_bFaceLeftRightRandomly_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACELEFTRIGHTRANDOMLY);
}

void ConfigureMapEventMovementPattern::on_bFaceHero_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACEHERO);
}

void ConfigureMapEventMovementPattern::on_bFaceAwayFromHero_clicked()
{
	addNewPattern(MapEvent::PATTERN_FACEAWAYFROMHERO);
}

void ConfigureMapEventMovementPattern::on_bIncreaseMovementFrequency_clicked()
{
	addNewPattern(MapEvent::PATTERN_INCREASEMOVEMENTFREQUENCY);
}

void ConfigureMapEventMovementPattern::on_bDecreaseMovementFrequency_clicked()
{
	addNewPattern(MapEvent::PATTERN_DECREASEMOVEMENTFREQUENCY);
}

void ConfigureMapEventMovementPattern::on_bBeginJump_clicked()
{
	addNewPattern(MapEvent::PATTERN_BEGINJUMP);
}

void ConfigureMapEventMovementPattern::on_bEndJump_clicked()
{
	addNewPattern(MapEvent::PATTERN_ENDJUMP);
}

void ConfigureMapEventMovementPattern::on_bLockFacing_clicked()
{
	addNewPattern(MapEvent::PATTERN_LOCKFACING);
}

void ConfigureMapEventMovementPattern::on_bUnlockFacing_clicked()
{
	addNewPattern(MapEvent::PATTERN_UNLOCKFACING);
}

void ConfigureMapEventMovementPattern::on_bWait_clicked()
{
	addNewPattern(MapEvent::PATTERN_WAIT);
}

void ConfigureMapEventMovementPattern::on_bPhasingModeOFF_clicked()
{
	addNewPattern(MapEvent::PATTERN_PHASINGMODEOFF);
}

void ConfigureMapEventMovementPattern::on_bPhasingModeON_clicked()
{
	addNewPattern(MapEvent::PATTERN_PHASINGMODEON);
}

void ConfigureMapEventMovementPattern::on_bStopAnimation_clicked()
{
	addNewPattern(MapEvent::PATTERN_STOPANIMATION);
}

void ConfigureMapEventMovementPattern::on_bResumeAnimation_clicked()
{
	addNewPattern(MapEvent::PATTERN_RESUMEANIMATION);
}

void ConfigureMapEventMovementPattern::on_bChangeGraphic_clicked()
{
	addNewPattern(MapEvent::PATTERN_CHANGEGRAPHIC);
}

void ConfigureMapEventMovementPattern::on_bIncreaseTransparency_clicked()
{
	addNewPattern(MapEvent::PATTERN_INCREASETRANSPARENCY);
}

void ConfigureMapEventMovementPattern::on_bDecreaseTransparency_clicked()
{
	addNewPattern(MapEvent::PATTERN_DECREASETRANSPARENCY);
}

void ConfigureMapEventMovementPattern::on_bSwitchON_clicked()
{
	addNewPattern(MapEvent::PATTERN_SWITCHON);
}

void ConfigureMapEventMovementPattern::on_bSwitchOFF_clicked()
{
	addNewPattern(MapEvent::PATTERN_SWITCHOFF);
}

void ConfigureMapEventMovementPattern::on_bPlaySoundEffect_clicked()
{
	addNewPattern(MapEvent::PATTERN_PLAYSOUNDEFFECT);
}

void ConfigureMapEventMovementPattern::on_lwPattern_itemSelectionChanged()
{
	int row = lwPattern->currentRow();
	if (row >= 0 && row < lwPattern->count())
		bDelete->setEnabled(true);
	else
		bDelete->setEnabled(false);
	if (row > 0)
		bMoveSelectedUp->setEnabled(true);
	else
		bMoveSelectedUp->setEnabled(false);
	if (row < patterns.size() - 1)
		bMoveSelectedDown->setEnabled(true);
	else
		bMoveSelectedDown->setEnabled(false);
}

QString ConfigureMapEventMovementPattern::getPatternViewableText(MapEvent::MovementPattern *pattern)
{
	QString viewableText = patternNames[pattern->type];
	if (pattern->type == MapEvent::PATTERN_WAIT)
		viewableText += QString(": %1 decisecond(s)").arg(pattern->intValue);
	else if (pattern->type == MapEvent::PATTERN_CHANGEGRAPHIC)
	{
		if (pattern->stringValue.startsWith("sprite:"))
			viewableText += QString(": %1").arg(QFileInfo(pattern->stringValue.mid(7)).baseName());
		else if (pattern->stringValue.startsWith("tile:"))
			viewableText += QString(": tile(0x%1)").arg(pattern->stringValue.mid(5).toUpper());
	}
	else if (pattern->type == MapEvent::PATTERN_INCREASETRANSPARENCY || pattern->type == MapEvent::PATTERN_DECREASETRANSPARENCY)
		viewableText += QString(": %1%").arg(pattern->intValue);
	else if (pattern->type == MapEvent::PATTERN_SWITCHON || pattern->type == MapEvent::PATTERN_SWITCHOFF)
		viewableText += QString(": [%1]").arg(ProjectData::switchVariableIdToString(pattern->intValue));
	else if (pattern->type == MapEvent::PATTERN_PLAYSOUNDEFFECT)
		viewableText += QString(": %1").arg(QFileInfo(pattern->soundEffect.location).fileName());
	return viewableText;
}

void ConfigureMapEventMovementPattern::addNewPattern(int type)
{
	MapEvent::MovementPattern *pattern = NULL;
	if (type == MapEvent::PATTERN_WAIT)
	{
		ConfigureMovementPattern_Wait *configWait = new ConfigureMovementPattern_Wait(this);
		if (configWait->exec())
		{
			pattern = new MapEvent::MovementPattern;
			pattern->type = type;
			pattern->intValue = configWait->getTime();
		}
		delete configWait;
	}
	else if (type == MapEvent::PATTERN_CHANGEGRAPHIC)
	{
		ConfigureMapEventPageSpriteGraphic *configGraphic = new ConfigureMapEventPageSpriteGraphic("sprite:", this);
		if (configGraphic->exec())
		{
			pattern = new MapEvent::MovementPattern;
			pattern->type = type;
			pattern->stringValue = configGraphic->getSpriteLocation();
		}
		delete configGraphic;
	}
	else if (type == MapEvent::PATTERN_INCREASETRANSPARENCY || type == MapEvent::PATTERN_DECREASETRANSPARENCY)
	{
		ConfigureMovementPattern_IncreaseDecreaseTransparency *configTransparency = new ConfigureMovementPattern_IncreaseDecreaseTransparency(this);
		if (configTransparency->exec())
		{
			pattern = new MapEvent::MovementPattern;
			pattern->type = type;
			pattern->intValue = configTransparency->getAmount();
		}
		delete configTransparency;
	}
	else if (type == MapEvent::PATTERN_SWITCHON || type == MapEvent::PATTERN_SWITCHOFF)
	{
		VariableSelector *variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, 0, this);
		if (variableSelector->exec())
		{
			pattern = new MapEvent::MovementPattern;
			pattern->type = type;
			pattern->intValue = variableSelector->getVariableID();
		}
		delete variableSelector;
	}
	else if (type == MapEvent::PATTERN_PLAYSOUNDEFFECT)
	{
		SoundEffectBrowser *soundBrowser = new SoundEffectBrowser(this);
		soundBrowser->setupBrowser();
		if (soundBrowser->exec())
		{
			pattern = new MapEvent::MovementPattern;
			pattern->type = type;
			soundBrowser->getSoundEffectInfo(&pattern->soundEffect);
		}
		delete soundBrowser;
	}
	else
	{
		pattern = new MapEvent::MovementPattern;
		pattern->type = type;
	}
	if (pattern != NULL)
	{
		QListWidgetItem *item;
		int location = patterns.size();
		patterns.append(pattern);
		item = new QListWidgetItem(getPatternViewableText(pattern));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
		item->setData(Qt::UserRole, QVariant(location));
		lwPattern->addItem(item);
		bClearList->setEnabled(true);
	}
}

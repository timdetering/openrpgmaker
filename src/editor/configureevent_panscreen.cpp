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

#include "configureevent_panscreen.h"
#include "selectmaplocation.h"

ConfigureEvent_PanScreen::ConfigureEvent_PanScreen(int map, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	frmPan->setEnabled(false);
	frmOption->setEnabled(false);
	frmTransitionSpeed->setEnabled(false);
	frmTiles->setEnabled(false);
	lblLocation->setText("(0, 0)");
	bTilesSideButton->setSpinBox(sbTiles);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	mapID = map;
	currentX = 0;
	currentY = 0;
	coordinatesStoredInVariables = false;
}

ConfigureEvent_PanScreen::~ConfigureEvent_PanScreen()
{
}

int ConfigureEvent_PanScreen::getOperation()
{
	if (rbLockPanState->isChecked())
		return LOCK_PAN_STATE;
	else if (rbUnlockPanState->isChecked())
		return UNLOCK_PAN_STATE;
	else if (rbPanScreen->isChecked())
		return PAN_SCREEN;
	else if (rbReturnToOrigin->isChecked())
		return RETURN_TO_ORIGIN;
	return -1;
}

void ConfigureEvent_PanScreen::setOperation(int value)
{
	if (value == LOCK_PAN_STATE)
		rbLockPanState->setChecked(true);
	else if (value == UNLOCK_PAN_STATE)
		rbUnlockPanState->setChecked(true);
	else if (value == PAN_SCREEN)
		rbPanScreen->setChecked(true);
	else if (value == RETURN_TO_ORIGIN)
		rbReturnToOrigin->setChecked(true);
}

int ConfigureEvent_PanScreen::getPan()
{
	if (rbDirection->isChecked())
		return DIRECTION;
	return LOCATION;
}

void ConfigureEvent_PanScreen::setPan(int value)
{
	if (value == DIRECTION)
		rbDirection->setChecked(true);
	else if (value == LOCATION)
		rbLocation->setChecked(true);
}

int ConfigureEvent_PanScreen::getDirection()
{
	return cboxDirection->currentIndex();
}

void ConfigureEvent_PanScreen::setDirection(int value)
{
	cboxDirection->setCurrentIndex(value);
}

int ConfigureEvent_PanScreen::getXLocation()
{
	return currentX;
}

void ConfigureEvent_PanScreen::setXLocation(int value)
{
	currentX = value;
	if (coordinatesStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2])").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)));
	else
		lblLocation->setText(QString("(%1, %2)").arg(currentX).arg(currentY));
}

int ConfigureEvent_PanScreen::getYLocation()
{
	return currentY;
}

void ConfigureEvent_PanScreen::setYLocation(int value)
{
	currentY = value;
	if (coordinatesStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2])").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)));
	else
		lblLocation->setText(QString("(%1, %2)").arg(currentX).arg(currentY));
}

bool ConfigureEvent_PanScreen::getCoordinatesStoredInVariables()
{
	return coordinatesStoredInVariables;
}

void ConfigureEvent_PanScreen::setCoordinatesStoredInVariables(bool stored)
{
	coordinatesStoredInVariables = stored;
	if (coordinatesStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2])").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)));
	else
		lblLocation->setText(QString("(%1, %2)").arg(currentX).arg(currentY));
}

int ConfigureEvent_PanScreen::getTransitionSpeed()
{
	return cboxTransitionSpeed->currentIndex();
}

void ConfigureEvent_PanScreen::setTransitionSpeed(int value)
{
	cboxTransitionSpeed->setCurrentIndex(value);
}

int ConfigureEvent_PanScreen::getTiles()
{
	return sbTiles->value();
}

void ConfigureEvent_PanScreen::setTiles(int value)
{
	sbTiles->setValue(value);
}

bool ConfigureEvent_PanScreen::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_PanScreen::setHaltOtherProcesses(bool value)
{
	cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_PanScreen::on_bSelectLocation_clicked()
{
	SelectMapLocation *selectLocation = new SelectMapLocation(mapID);
	selectLocation->setLocationStoredInVariables(coordinatesStoredInVariables);
	selectLocation->setLocation(currentX, currentY, 0);
	if (selectLocation->exec())
	{
		currentX = selectLocation->getXLocation();
		currentY = selectLocation->getYLocation();
		coordinatesStoredInVariables = selectLocation->isLocationStoredInVariables();
	}
	delete selectLocation;
	if (coordinatesStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2])").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)));
	else
		lblLocation->setText(QString("(%1, %2)").arg(currentX).arg(currentY));
}

void ConfigureEvent_PanScreen::on_rbDirection_toggled(bool checked)
{
	cboxDirection->setEnabled(checked);
	wLocation->setEnabled(!checked);
	frmTiles->setEnabled(checked);
}

void ConfigureEvent_PanScreen::on_rbLockPanState_toggled(bool checked)
{
	if (checked)
	{
		frmPan->setEnabled(false);
		frmOption->setEnabled(false);
		frmTransitionSpeed->setEnabled(false);
		frmTiles->setEnabled(false);
	}
}

void ConfigureEvent_PanScreen::on_rbPanScreen_toggled(bool checked)
{
	if (checked)
	{
		frmPan->setEnabled(true);
		frmOption->setEnabled(true);
		frmTransitionSpeed->setEnabled(true);
		if (rbDirection->isChecked())
			frmTiles->setEnabled(true);
		else
			frmTiles->setEnabled(false);
	}
}

void ConfigureEvent_PanScreen::on_rbReturnToOrigin_toggled(bool checked)
{
	if (checked)
	{
		frmPan->setEnabled(false);
		frmOption->setEnabled(false);
		frmTransitionSpeed->setEnabled(true);
		frmTiles->setEnabled(false);
	}
}

void ConfigureEvent_PanScreen::on_rbUnlockPanState_toggled(bool checked)
{
	if (checked)
	{
		frmPan->setEnabled(false);
		frmOption->setEnabled(false);
		frmTransitionSpeed->setEnabled(false);
		frmTiles->setEnabled(false);
	}
}

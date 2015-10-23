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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QTableWidget>
#include <QString>
#include "configurephases.h"
#include "projectdata.h"

ConfigurePhases::ConfigurePhases(QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	setupUi(this);
	for (int i = 0; i < NUM_PHASES; ++i)
		lwPhases->item(i)->setText(QString("%1: %2").arg(i + 1, 2, 10, QChar('0')).arg(ProjectData::phaseNames[i]));
	ignoreEvents = false;
}

ConfigurePhases::~ConfigurePhases()
{
}

void ConfigurePhases::on_lwPhases_itemDoubleClicked(QListWidgetItem *item)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		item->setText(item->text().mid(4));
		ignoreEvents = false;
	}
}

void ConfigurePhases::on_lwPhases_itemChanged(QListWidgetItem *item)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		item->setText(QString("%1: %2").arg(lwPhases->row(item) + 1, 2, 10, QChar('0')).arg(item->text()));
		ignoreEvents = false;
	}
}

void ConfigurePhases::on_buttonBox_accepted()
{
	for (int i = 0; i < NUM_PHASES; ++i)
		ProjectData::phaseNames[i] = lwPhases->item(i)->text().mid(4);
}

void ConfigurePhases::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Passability");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

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

#ifndef IMPORTER_H
#define IMPORTER_H

class Importer_ProjectData;

#include <QFileInfoList>
#include "ui_importer.h"

class Importer : public QDialog, public Ui::Importer
{
	Q_OBJECT
	public:
		Importer(QWidget *parent=0);
		~Importer();
	protected slots:
		void on_lwPluginList_currentRowChanged(int row);
		void on_bRunPlugin_clicked();
	private:
		QStringList pluginLocations;
		QStringList pluginNames;
		QStringList pluginDescriptions;
};

#endif // IMPORTER_H

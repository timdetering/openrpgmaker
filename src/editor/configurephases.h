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

#ifndef CONFIGUREPHASES_H
#define CONFIGUREPHASES_H

#include "ui_configurephases.h"

class ConfigurePhases : public QDialog, public Ui::ConfigurePhases
{
	Q_OBJECT
	public:
		ConfigurePhases(QWidget *parent=0);
		~ConfigurePhases();
	protected slots:
		void on_lwPhases_itemDoubleClicked(QListWidgetItem *item);
		void on_lwPhases_itemChanged(QListWidgetItem *item);
		void on_buttonBox_accepted();
		void on_buttonBox_helpRequested();
	private:
		bool ignoreEvents;
};

#endif // CONFIGUREPHASES_H

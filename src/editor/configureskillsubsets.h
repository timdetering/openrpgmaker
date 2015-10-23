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

#ifndef CONFIGURESKILLSUBSETS_H
#define CONFIGURESKILLSUBSETS_H

#include "ui_configureskillsubsets.h"

class ConfigureSkillSubsets : public QDialog, public Ui::ConfigureSkillSubsets
{
	Q_OBJECT
	public:
		ConfigureSkillSubsets(QWidget *parent=0);
		~ConfigureSkillSubsets();
	protected slots:
		void on_bAddSubset_clicked();
		void on_bDeleteSubset_clicked();
		void on_cboxType_currentIndexChanged(int index);
		void on_leName_textChanged(const QString &text);
		void on_lwSkillSubsets_itemSelectionChanged();
		void on_buttonBox_accepted();
		void on_buttonBox_helpRequested();
	private:
		void closeEvent(QCloseEvent *event);
		QList<int> currentSkillTypes;
		bool ignoreEvents;
};

#endif // CONFIGURESKILLSUBSETS_H

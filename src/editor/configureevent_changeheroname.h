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

#ifndef CONFIGUREEVENT_CHANGEHERONAME_H
#define CONFIGUREEVENT_CHANGEHERONAME_H

#include "ui_configureevent_changeheroname.h"

class ConfigureEvent_ChangeHeroName : public QDialog, public Ui::ConfigureEvent_ChangeHeroName
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeHeroName(QWidget *parent=0);
		~ConfigureEvent_ChangeHeroName();
		void setCharacterID(int characterID);
		void setIsReferencedCharacter(bool referenced);
		void setString(QString string);
		void setStringID(int stringID);
		int getCharacterID();
		bool isReferencedCharacter();
		QString getString();
		int getStringID();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_bBrowseStringVariables_clicked();
		void on_cboxCharacters_currentIndexChanged(int index);
		void on_rbSpecificCharacter_toggled(bool checked);
		void on_rbSpecificValue_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEHERONAME_H

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

#ifndef CHARACTERCLASSEXPERIENCECURVEEDITOR_H
#define CHARACTERCLASSEXPERIENCECURVEEDITOR_H

class Character;
class Class;

#include "ui_characterclassexperiencecurveeditor.h"
#include "projectdata.h"

class CharacterClassExperienceCurveEditor : public QDialog, public Ui::CharacterClassExperienceCurveEditor
{
	Q_OBJECT
	public:
		CharacterClassExperienceCurveEditor(int id, bool character, QWidget *parent=0);
		~CharacterClassExperienceCurveEditor();
		void setupEditor();
	protected slots:
		void updateExperienceCurve();
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		ProjectData::DataReference<Character> *charRef;
		ProjectData::DataReference<Class> *classRef;
		bool editingCharacter;
		bool ignoreEvents;
};

#endif // CHARACTERCLASSEXPERIENCECURVEEDITOR_H

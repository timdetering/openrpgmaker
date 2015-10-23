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

#ifndef CHARACTERCLASSSTATEDITOR_H
#define CHARACTERCLASSSTATEDITOR_H

class Character;
class Class;

#include "ui_characterclassstateditor.h"
#include "projectdata.h"
#include "randomnumbergenerator.h"

class CharacterClassStatEditor : public QDialog, public Ui::CharacterClassStatEditor
{
	Q_OBJECT
	public:
		enum Stat {STAT_MAXHP=0, STAT_MAXMP, STAT_ATTACK, STAT_DEFENSE, STAT_WISDOM, STAT_AGILITY};
		enum AccelerateGrowth {ACCELERATE_EARLY=0, ACCELERATE_EQUAL=5, ACCELERATE_LATE=10};
		CharacterClassStatEditor(int id, bool character, int initialStat=STAT_MAXHP, QWidget *parent=0);
		~CharacterClassStatEditor();
		void setupEditor();
	protected slots:
		void on_gvMaxHP_valueChanged(int level, int value);
		void on_sbMaxHPLevel_valueChanged(int level);
		void on_sbMaxHP_valueChanged(int value);
		void on_bMaxHPSpecifyEndpoints_clicked();
		void on_bMaxHPOutstanding_clicked();
		void on_bMaxHPAboveAverage_clicked();
		void on_bMaxHPAverage_clicked();
		void on_bMaxHPBelowAverage_clicked();
		void on_gvMaxMP_valueChanged(int level, int value);
		void on_sbMaxMPLevel_valueChanged(int level);
		void on_sbMaxMP_valueChanged(int value);
		void on_bMaxMPSpecifyEndpoints_clicked();
		void on_bMaxMPOutstanding_clicked();
		void on_bMaxMPAboveAverage_clicked();
		void on_bMaxMPAverage_clicked();
		void on_bMaxMPBelowAverage_clicked();
		void on_gvAttack_valueChanged(int level, int value);
		void on_sbAttackLevel_valueChanged(int level);
		void on_sbAttack_valueChanged(int value);
		void on_bAttackSpecifyEndpoints_clicked();
		void on_bAttackOutstanding_clicked();
		void on_bAttackAboveAverage_clicked();
		void on_bAttackAverage_clicked();
		void on_bAttackBelowAverage_clicked();
		void on_gvDefense_valueChanged(int level, int value);
		void on_sbDefenseLevel_valueChanged(int level);
		void on_sbDefense_valueChanged(int value);
		void on_bDefenseSpecifyEndpoints_clicked();
		void on_bDefenseOutstanding_clicked();
		void on_bDefenseAboveAverage_clicked();
		void on_bDefenseAverage_clicked();
		void on_bDefenseBelowAverage_clicked();
		void on_gvWisdom_valueChanged(int level, int value);
		void on_sbWisdomLevel_valueChanged(int level);
		void on_sbWisdom_valueChanged(int value);
		void on_bWisdomSpecifyEndpoints_clicked();
		void on_bWisdomOutstanding_clicked();
		void on_bWisdomAboveAverage_clicked();
		void on_bWisdomAverage_clicked();
		void on_bWisdomBelowAverage_clicked();
		void on_gvAgility_valueChanged(int level, int value);
		void on_sbAgilityLevel_valueChanged(int level);
		void on_sbAgility_valueChanged(int value);
		void on_bAgilitySpecifyEndpoints_clicked();
		void on_bAgilityOutstanding_clicked();
		void on_bAgilityAboveAverage_clicked();
		void on_bAgilityAverage_clicked();
		void on_bAgilityBelowAverage_clicked();
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		void applyEndpoints(int level1Endpoint, int level99Endpoint, int accelerateGrowth);
		ProjectData::DataReference<Character> *charRef;
		ProjectData::DataReference<Class> *classRef;
		RandomNumberGenerator numberGenerator;
		bool editingCharacter;
};

#endif // CHARACTERCLASSSTATEDITOR_H

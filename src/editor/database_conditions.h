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

#ifndef DATABASE_CONDITIONS_H
#define DATABASE_CONDITIONS_H

class Condition;

#include "ui_database_conditions.h"
#include "projectdata.h"

class Database_Conditions : public QWidget, public Ui::Database_Conditions
{
	Q_OBJECT
	public:
		Database_Conditions(QWidget *parent=0);
		~Database_Conditions();
		void setupConditions();
		void shutdownConditions();
		void applyConditions();
	protected slots:
		void on_lwConditionList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxExtent_currentIndexChanged(int index);
		void on_tbIcon_clicked();
		void on_sbGroup_valueChanged(int value);
		void on_sbPriority_valueChanged(int value);
		void on_cboxRestriction_currentIndexChanged(int index);
		void on_sbSusceptibilityA_valueChanged(int value);
		void on_sbSusceptibilityB_valueChanged(int value);
		void on_sbSusceptibilityC_valueChanged(int value);
		void on_sbSusceptibilityD_valueChanged(int value);
		void on_sbSusceptibilityE_valueChanged(int value);
		void on_sbNumTurns_valueChanged(int value);
		void on_sbPercentEachTurn_valueChanged(int value);
		void on_sbPhysicalDamageRecoveryChance_valueChanged(int value);
		void on_sbAttackAlteration_valueChanged(int value);
		void on_sbDefenseAlteration_valueChanged(int value);
		void on_sbWisdomAlteration_valueChanged(int value);
		void on_sbAgilityAlteration_valueChanged(int value);
		void on_sbHitPercentage_valueChanged(int value);
		void on_cbAttackInfluence_toggled(bool on);
		void on_sbAttackInfluence_valueChanged(int value);
		void on_cbWisdomInfluence_toggled(bool on);
		void on_sbWisdomInfluence_valueChanged(int value);
		void on_sbAnimation_valueChanged(int value);
		void on_hpEffect_rbNone_toggled(bool on);
		void on_hpEffect_rbRecovery_toggled(bool on);
		void on_hpEffect_rbDamage_toggled(bool on);
		void on_hpEffect_sbPercentPerTurn_valueChanged(int value);
		void on_hpEffect_sbAmountPerTurn_valueChanged(int value);
		void on_hpEffect_sbAmountPerNumSteps_valueChanged(int value);
		void on_hpEffect_sbNumSteps_valueChanged(int value);
		void on_mpEffect_rbNone_toggled(bool on);
		void on_mpEffect_rbRecovery_toggled(bool on);
		void on_mpEffect_rbDamage_toggled(bool on);
		void on_mpEffect_sbPercentPerTurn_valueChanged(int value);
		void on_mpEffect_sbAmountPerTurn_valueChanged(int value);
		void on_mpEffect_sbAmountPerNumSteps_valueChanged(int value);
		void on_mpEffect_sbNumSteps_valueChanged(int value);
		void on_cbReflectSkills_toggled(bool on);
		void on_cbLockEquipment_toggled(bool on);
		void on_rbEvadingNoChange_toggled(bool on);
		void on_rbEvadingAlways_toggled(bool on);
		void on_rbEvadingNever_toggled(bool on);
		void on_rbCriticalHitsNoChange_toggled(bool on);
		void on_rbCriticalHitsAlways_toggled(bool on);
		void on_rbCriticalHitsNever_toggled(bool on);
		void animateSprite();
	private:
		void updateConditionList();
		void loadAnimationImage();
		QGraphicsPixmapItem *animationItem;
		QTimer *animationTimer;
		ProjectData::DataReference<Condition> *conditionRef;
		int animationFrame;
		int animationFrameDirection;
		int animationCharacterSpriteID;
		bool ignoreEvents;
		bool animationPingPong;
};

#endif // DATABASE_CONDITIONS_H

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

#include <QDateTime>
#include "characterclassstateditor.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_class.h"
#include "specifyendpoints.h"

CharacterClassStatEditor::CharacterClassStatEditor(int id, bool character, int initialStat, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	charRef = NULL;
	classRef = NULL;
	if (character)
		charRef = new ProjectData::DataReference<Character>(id, __FILE__, __LINE__);
	else
		classRef = new ProjectData::DataReference<Class>(id, __FILE__, __LINE__);
	editingCharacter = character;
	tabWidget->setCurrentIndex(initialStat);
	numberGenerator.seedGenerator(QDateTime::currentDateTime().toTime_t());
	bMaxHPLevelSpinButton->setSpinBox(sbMaxHPLevel);
	bMaxHPSpinButton->setSpinBox(sbMaxHP);
	bMaxMPLevelSpinButton->setSpinBox(sbMaxMPLevel);
	bMaxMPSpinButton->setSpinBox(sbMaxMP);
	bAttackLevelSpinButton->setSpinBox(sbAttackLevel);
	bAttackSpinButton->setSpinBox(sbAttack);
	bDefenseLevelSpinButton->setSpinBox(sbDefenseLevel);
	bDefenseSpinButton->setSpinBox(sbDefense);
	bWisdomLevelSpinButton->setSpinBox(sbWisdomLevel);
	bWisdomSpinButton->setSpinBox(sbWisdom);
	bAgilityLevelSpinButton->setSpinBox(sbAgilityLevel);
	bAgilitySpinButton->setSpinBox(sbAgility);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

CharacterClassStatEditor::~CharacterClassStatEditor()
{
	if (charRef != NULL)
	{
		delete charRef;
		charRef = NULL;
	}
	if (classRef != NULL)
	{
		delete classRef;
		classRef = NULL;
	}
}

void CharacterClassStatEditor::setupEditor()
{
	gvMaxHP->setNumValues(99);
	gvMaxHP->setForegroundColor(QColor(255, 0, 0));
	gvMaxHP->setModifiable(true);
	gvMaxHP->setMaxValue(9999);
	gvMaxMP->setNumValues(99);
	gvMaxMP->setForegroundColor(QColor(0, 0, 255));
	gvMaxMP->setModifiable(true);
	gvMaxMP->setMaxValue(999);
	gvAttack->setNumValues(99);
	gvAttack->setForegroundColor(QColor(255, 192, 0));
	gvAttack->setModifiable(true);
	gvAttack->setMaxValue(999);
	gvDefense->setNumValues(99);
	gvDefense->setForegroundColor(QColor(0, 255, 0));
	gvDefense->setModifiable(true);
	gvDefense->setMaxValue(999);
	gvWisdom->setNumValues(99);
	gvWisdom->setForegroundColor(QColor(255, 0, 255));
	gvWisdom->setModifiable(true);
	gvWisdom->setMaxValue(999);
	gvAgility->setNumValues(99);
	gvAgility->setForegroundColor(QColor(0, 255, 255));
	gvAgility->setModifiable(true);
	gvAgility->setMaxValue(999);
	gvMaxHP->setupGraph();
	gvMaxMP->setupGraph();
	gvAttack->setupGraph();
	gvDefense->setupGraph();
	gvWisdom->setupGraph();
	gvAgility->setupGraph();
	for (int i = 0; i < 99; ++i)
	{
		if (editingCharacter)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		else
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
		}
	}
	gvMaxHP->setDisplayValue(-1);
	gvMaxMP->setDisplayValue(-1);
	gvAttack->setDisplayValue(-1);
	gvDefense->setDisplayValue(-1);
	gvWisdom->setDisplayValue(-1);
	gvAgility->setDisplayValue(-1);
	gvMaxHP->updateView();
	gvMaxMP->updateView();
	gvAttack->updateView();
	gvDefense->updateView();
	gvWisdom->updateView();
	gvAgility->updateView();
	sbMaxHP->setValue(gvMaxHP->getValue(0));
	sbMaxMP->setValue(gvMaxMP->getValue(0));
	sbAttack->setValue(gvAttack->getValue(0));
	sbDefense->setValue(gvDefense->getValue(0));
	sbWisdom->setValue(gvWisdom->getValue(0));
	sbAgility->setValue(gvAgility->getValue(0));
}

void CharacterClassStatEditor::on_gvMaxHP_valueChanged(int level, int value)
{
	sbMaxHPLevel->setValue(level);
	sbMaxHP->setValue(value);
}

void CharacterClassStatEditor::on_sbMaxHPLevel_valueChanged(int level)
{
	sbMaxMPLevel->setValue(level);
	sbAttackLevel->setValue(level);
	sbDefenseLevel->setValue(level);
	sbWisdomLevel->setValue(level);
	sbAgilityLevel->setValue(level);
	sbMaxHP->setValue(gvMaxHP->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbMaxHP_valueChanged(int value)
{
	gvMaxHP->changeValue(sbMaxHPLevel->value() - 1, value);
	gvMaxHP->updateView();
}

void CharacterClassStatEditor::on_bMaxHPSpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvMaxHP->getValue(0), gvMaxHP->getValue(98), 9999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bMaxHPOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(500ul, 599ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(8000ul, 9999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxHPAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 499ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(6000ul, 7999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxHPAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(300ul, 399ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(4000ul, 5999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxHPBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 299ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(2000ul, 3999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_gvMaxMP_valueChanged(int level, int value)
{
	sbMaxMPLevel->setValue(level);
	sbMaxMP->setValue(value);
}

void CharacterClassStatEditor::on_sbMaxMPLevel_valueChanged(int level)
{
	sbMaxHPLevel->setValue(level);
	sbAttackLevel->setValue(level);
	sbDefenseLevel->setValue(level);
	sbWisdomLevel->setValue(level);
	sbAgilityLevel->setValue(level);
	sbMaxMP->setValue(gvMaxMP->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbMaxMP_valueChanged(int value)
{
	gvMaxMP->changeValue(sbMaxMPLevel->value() - 1, value);
	gvMaxMP->updateView();
}

void CharacterClassStatEditor::on_bMaxMPSpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvMaxMP->getValue(0), gvMaxMP->getValue(98), 999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bMaxMPOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(50ul, 59ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(800ul, 999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxMPAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(40ul, 49ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(600ul, 799ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxMPAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(30ul, 39ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 599ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bMaxMPBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(20ul, 29ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 399ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_gvAttack_valueChanged(int level, int value)
{
	sbAttackLevel->setValue(level);
	sbAttack->setValue(value);
}

void CharacterClassStatEditor::on_sbAttackLevel_valueChanged(int level)
{
	sbMaxHPLevel->setValue(level);
	sbMaxMPLevel->setValue(level);
	sbDefenseLevel->setValue(level);
	sbWisdomLevel->setValue(level);
	sbAgilityLevel->setValue(level);
	sbAttack->setValue(gvAttack->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbAttack_valueChanged(int value)
{
	gvAttack->changeValue(sbAttackLevel->value() - 1, value);
	gvAttack->updateView();
}

void CharacterClassStatEditor::on_bAttackSpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvAttack->getValue(0), gvAttack->getValue(98), 999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bAttackOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(50ul, 59ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(800ul, 999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAttackAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(40ul, 49ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(600ul, 799ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAttackAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(30ul, 39ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 599ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAttackBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(20ul, 29ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 399ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_gvDefense_valueChanged(int level, int value)
{
	sbDefenseLevel->setValue(level);
	sbDefense->setValue(value);
}

void CharacterClassStatEditor::on_sbDefenseLevel_valueChanged(int level)
{
	sbMaxHPLevel->setValue(level);
	sbMaxMPLevel->setValue(level);
	sbAttackLevel->setValue(level);
	sbWisdomLevel->setValue(level);
	sbAgilityLevel->setValue(level);
	sbDefense->setValue(gvDefense->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbDefense_valueChanged(int value)
{
	gvDefense->changeValue(sbDefenseLevel->value() - 1, value);
	gvDefense->updateView();
}

void CharacterClassStatEditor::on_bDefenseSpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvDefense->getValue(0), gvDefense->getValue(98), 999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bDefenseOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(50ul, 59ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(800ul, 999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bDefenseAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(40ul, 49ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(600ul, 799ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bDefenseAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(30ul, 39ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 599ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bDefenseBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(20ul, 29ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 399ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_gvWisdom_valueChanged(int level, int value)
{
	sbWisdomLevel->setValue(level);
	sbWisdom->setValue(value);
}

void CharacterClassStatEditor::on_sbWisdomLevel_valueChanged(int level)
{
	sbMaxHPLevel->setValue(level);
	sbMaxMPLevel->setValue(level);
	sbAttackLevel->setValue(level);
	sbDefenseLevel->setValue(level);
	sbAgilityLevel->setValue(level);
	sbWisdom->setValue(gvWisdom->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbWisdom_valueChanged(int value)
{
	gvWisdom->changeValue(sbWisdomLevel->value() - 1, value);
	gvWisdom->updateView();
}

void CharacterClassStatEditor::on_bWisdomSpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvWisdom->getValue(0), gvWisdom->getValue(98), 999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bWisdomOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(50ul, 59ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(800ul, 999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bWisdomAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(40ul, 49ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(600ul, 799ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bWisdomAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(30ul, 39ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 599ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bWisdomBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(20ul, 29ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 399ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_gvAgility_valueChanged(int level, int value)
{
	sbAgilityLevel->setValue(level);
	sbAgility->setValue(value);
}

void CharacterClassStatEditor::on_sbAgilityLevel_valueChanged(int level)
{
	sbMaxHPLevel->setValue(level);
	sbMaxMPLevel->setValue(level);
	sbAttackLevel->setValue(level);
	sbDefenseLevel->setValue(level);
	sbWisdomLevel->setValue(level);
	sbAgility->setValue(gvAgility->getValue(level - 1));
}

void CharacterClassStatEditor::on_sbAgility_valueChanged(int value)
{
	gvAgility->changeValue(sbAgilityLevel->value() - 1, value);
	gvAgility->updateView();
}

void CharacterClassStatEditor::on_bAgilitySpecifyEndpoints_clicked()
{
	SpecifyEndpoints *specifyEndpoints = new SpecifyEndpoints(gvAgility->getValue(0), gvAgility->getValue(98), 999);
	if (specifyEndpoints->exec())
	{
		int level1Endpoint = specifyEndpoints->getLevel1Endpoint();
		int level99Endpoint = specifyEndpoints->getLevel99Endpoint();
		int accelerateGrowth = specifyEndpoints->getAccelerateGrowth();
		applyEndpoints(level1Endpoint, level99Endpoint, accelerateGrowth);
	}
	delete specifyEndpoints;
}

void CharacterClassStatEditor::on_bAgilityOutstanding_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(50ul, 59ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(800ul, 999ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAgilityAboveAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(40ul, 49ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(600ul, 799ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAgilityAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(30ul, 39ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(400ul, 599ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_bAgilityBelowAverage_clicked()
{
	int level1Endpoint = (int)(numberGenerator.getRandomNumber(20ul, 29ul));
	int level99Endpoint = (int)(numberGenerator.getRandomNumber(200ul, 399ul));
	int acceleration = (int)(numberGenerator.getRandomNumber(5000ul, 7000ul) / 1000ul);
	applyEndpoints(level1Endpoint, level99Endpoint, acceleration);
}

void CharacterClassStatEditor::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Ok)
	{
		for (int i = 0; i < 99; ++i)
		{
			if (editingCharacter)
			{
				(*charRef)->setMaxHP(i + 1, gvMaxHP->getValue(i));
				(*charRef)->setMaxMP(i + 1, gvMaxMP->getValue(i));
				(*charRef)->setAttack(i + 1, gvAttack->getValue(i));
				(*charRef)->setDefense(i + 1, gvDefense->getValue(i));
				(*charRef)->setWisdom(i + 1, gvWisdom->getValue(i));
				(*charRef)->setAgility(i + 1, gvAgility->getValue(i));
			}
			else
			{
				(*classRef)->setMaxHP(i + 1, gvMaxHP->getValue(i));
				(*classRef)->setMaxMP(i + 1, gvMaxMP->getValue(i));
				(*classRef)->setAttack(i + 1, gvAttack->getValue(i));
				(*classRef)->setDefense(i + 1, gvDefense->getValue(i));
				(*classRef)->setWisdom(i + 1, gvWisdom->getValue(i));
				(*classRef)->setAgility(i + 1, gvAgility->getValue(i));
			}
		}
		accept();
	}
	else if (buttonClicked == QDialogButtonBox::Cancel)
		reject();
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("Database::Characters::StatEditor");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void CharacterClassStatEditor::applyEndpoints(int level1Endpoint, int level99Endpoint, int accelerateGrowth)
{
	BarGraph *graph = NULL;
	float p1 = (float)level1Endpoint;
	float p3 = (float)level99Endpoint;
	float p2, t, y, decimalPart;
	int range = level99Endpoint - level1Endpoint + 1;
	int wholePart;
	if (tabWidget->currentIndex() == STAT_MAXHP)
		graph = gvMaxHP;
	else if (tabWidget->currentIndex() == STAT_MAXMP)
		graph = gvMaxMP;
	else if (tabWidget->currentIndex() == STAT_ATTACK)
		graph = gvAttack;
	else if (tabWidget->currentIndex() == STAT_DEFENSE)
		graph = gvDefense;
	else if (tabWidget->currentIndex() == STAT_WISDOM)
		graph = gvWisdom;
	else if (tabWidget->currentIndex() == STAT_AGILITY)
		graph = gvAgility;
	if (accelerateGrowth == ACCELERATE_EARLY)
		p2 = (range * 1.0) + (float)level1Endpoint;
	else if (accelerateGrowth == 1)
		p2 = (range * 0.9) + (float)level1Endpoint;
	else if (accelerateGrowth == 2)
		p2 = (range * 0.8) + (float)level1Endpoint;
	else if (accelerateGrowth == 3)
		p2 = (range * 0.7) + (float)level1Endpoint;
	else if (accelerateGrowth == 4)
		p2 = (range * 0.6) + (float)level1Endpoint;
	else if (accelerateGrowth == 5)
		p2 = (range * 0.5) + (float)level1Endpoint;
	else if (accelerateGrowth == 6)
		p2 = (range * 0.4) + (float)level1Endpoint;
	else if (accelerateGrowth == 7)
		p2 = (range * 0.3) + (float)level1Endpoint;
	else if (accelerateGrowth == 8)
		p2 = (range * 0.2) + (float)level1Endpoint;
	else if (accelerateGrowth == 9)
		p2 = (range * 0.1) + (float)level1Endpoint;
	else // (accelerateGrowth == ACCELERATE_LATE)
		p2 = (range * 0.0) + (float)level1Endpoint;
	for (int i = 0; i < 99; ++i)
	{
		t = (i / 98.0);
		y = ((1.0 - t) * (1.0 - t) * p1) + (2.0 * (1.0 -t) * t * p2) + (t * t * p3);
		wholePart = (int)y;
		decimalPart = y - (float)wholePart;
		if (decimalPart >= 0.5)
			++wholePart;
		graph->changeValue(i, wholePart);
	}
	graph->updateView();
	sbMaxHP->setValue(gvMaxHP->getValue(sbMaxHPLevel->value() - 1));
	sbMaxMP->setValue(gvMaxMP->getValue(sbMaxMPLevel->value() - 1));
	sbAttack->setValue(gvAttack->getValue(sbAttackLevel->value() - 1));
	sbDefense->setValue(gvDefense->getValue(sbDefenseLevel->value() - 1));
	sbWisdom->setValue(gvWisdom->getValue(sbWisdomLevel->value() - 1));
	sbAgility->setValue(gvWisdom->getValue(sbAgilityLevel->value() - 1));
}

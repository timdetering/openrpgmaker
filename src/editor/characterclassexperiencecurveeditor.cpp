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

#include <QLabel>
#include <QString>
#include <QObject>
#include "bargraph.h"
#include "characterclassexperiencecurveeditor.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_class.h"

CharacterClassExperienceCurveEditor::CharacterClassExperienceCurveEditor(int id, bool character, QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	setupUi(this);
	charRef = NULL;
	classRef = NULL;
	if (character)
		charRef = new ProjectData::DataReference<Character>(id, __FILE__, __LINE__);
	else
		classRef = new ProjectData::DataReference<Class>(id, __FILE__, __LINE__);
	editingCharacter = character;
	bPrimarySpinButton->setSpinBox(sbPrimary);
	bSecondarySpinButton->setSpinBox(sbSecondary);
	QObject::connect(sbPrimary, SIGNAL(valueChanged(int)), this, SLOT(updateExperienceCurve()));
	QObject::connect(sbSecondary, SIGNAL(valueChanged(int)), this, SLOT(updateExperienceCurve()));
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

CharacterClassExperienceCurveEditor::~CharacterClassExperienceCurveEditor()
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

void CharacterClassExperienceCurveEditor::setupEditor()
{
	gvExperienceGraph->setNumValues(99);
	gvExperienceGraph->setForegroundColor(QColor(255, 0, 255));
	gvExperienceGraph->setMaxValue(9999999);
	if (editingCharacter)
	{
		sbPrimary->setValue((*charRef)->getExperiencePrimary());
		sbSecondary->setValue((*charRef)->getExperienceSecondary());
	}
	else
	{
		sbPrimary->setValue((*classRef)->getExperiencePrimary());
		sbSecondary->setValue((*classRef)->getExperienceSecondary());
	}
	gvExperienceGraph->setupGraph();
	gvExperienceGraph->setDisplayValue(-1);
	ignoreEvents = false;
	updateExperienceCurve();
}

void CharacterClassExperienceCurveEditor::updateExperienceCurve()
{
	if (!ignoreEvents)
	{
		QString total1to25 = "";
		QString total26to50 = "";
		QString total51to75 = "";
		QString total76to99 = "";
		QString per1to25 = "";
		QString per26to50 = "";
		QString per51to75 = "";
		QString per76to99 = "";
		int primary = sbPrimary->value();
		int secondary = sbSecondary->value();
		int totalExperience = 0;
		int nextLevelExperience = 0;
		for (int i = 0; i < 99; ++i)
		{
			totalExperience += nextLevelExperience;
			nextLevelExperience = (primary * (i + 1)) + secondary;
			gvExperienceGraph->changeValue(i, totalExperience);
			if (i < 25)
			{
				if (totalExperience > 9999999)
					total1to25 += QString("overflow\n");
				else
					total1to25 += QString("%1\n").arg(totalExperience);
				per1to25 += QString("%1\n").arg(nextLevelExperience);
			}
			else if (i < 50)
			{
				if (totalExperience > 9999999)
					total26to50 += QString("overflow\n");
				else
					total26to50 += QString("%1\n").arg(totalExperience);
				per26to50 += QString("%1\n").arg(nextLevelExperience);
			}
			else if (i < 75)
			{
				if (totalExperience > 9999999)
					total51to75 += QString("overflow\n");
				else
					total51to75 += QString("%1\n").arg(totalExperience);
				per51to75 += QString("%1\n").arg(nextLevelExperience);
			}
			else
			{
				if (totalExperience > 9999999)
					total76to99 += QString("overflow\n");
				else
					total76to99 += QString("%1\n").arg(totalExperience);
				if (i == 98)
					per76to99 += "--------------\n";
				else
					per76to99 += QString("%1\n").arg(nextLevelExperience);
			}
		}
		total76to99 += " \n";
		per76to99 += " \n";
		total1to25.remove(total1to25.size() - 1, 1);
		total26to50.remove(total26to50.size() - 1, 1);
		total51to75.remove(total51to75.size() - 1, 1);
		total76to99.remove(total76to99.size() - 1, 1);
		per1to25.remove(per1to25.size() - 1, 1);
		per26to50.remove(per26to50.size() - 1, 1);
		per51to75.remove(per51to75.size() - 1, 1);
		per76to99.remove(per76to99.size() - 1, 1);
		lblTotal_1to25->setText(total1to25);
		lblTotal_26to50->setText(total26to50);
		lblTotal_51to75->setText(total51to75);
		lblTotal_76to99->setText(total76to99);
		lblPerLevel_1to25->setText(per1to25);
		lblPerLevel_26to50->setText(per26to50);
		lblPerLevel_51to75->setText(per51to75);
		lblPerLevel_76to99->setText(per76to99);
		gvExperienceGraph->updateView();
	}
}

void CharacterClassExperienceCurveEditor::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Ok)
	{
		if (editingCharacter)
		{
			(*charRef)->setExperiencePrimary(sbPrimary->value());
			(*charRef)->setExperienceSecondary(sbSecondary->value());
		}
		else
		{
			(*classRef)->setExperiencePrimary(sbPrimary->value());
			(*classRef)->setExperienceSecondary(sbSecondary->value());
		}
		accept();
	}
	else if (buttonClicked == QDialogButtonBox::Cancel)
		reject();
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("Database::Characters::ExperienceCurveEditor");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

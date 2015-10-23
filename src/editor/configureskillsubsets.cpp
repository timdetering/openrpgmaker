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

#include <QMessageBox>
#include "configureskillsubsets.h"
#include "projectdata.h"
#include "projectdata_skill.h"

ConfigureSkillSubsets::ConfigureSkillSubsets(QWidget *parent) : QDialog(parent)
{
	QListWidgetItem *item;
	ignoreEvents = true;
	for (int i = 0; i < ProjectData::skillNames.size(); ++i)
	{
		ProjectData::DataReference<Skill> *skillRef = new ProjectData::DataReference<Skill>(i, __FILE__, __LINE__);
		currentSkillTypes.append(ProjectData::skillSubsets[(*skillRef)->getType()]->type);
		delete skillRef;
	}
	setupUi(this);
	for (int i = 0; i < ProjectData::skillSubsets.size(); ++i)
	{
		item = new QListWidgetItem();
		item->setText(QString("%1: %2").arg(i, 2, 10, QChar('0')).arg(ProjectData::skillSubsets[i]->name));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		lwSkillSubsets->addItem(item);
	}
	bDeleteSubset->setEnabled((ProjectData::skillSubsets.size() > 4));
	bAddSubset->setEnabled((ProjectData::skillSubsets.size() < 100));
	lwSkillSubsets->setCurrentRow(0);
	lwSkillSubsets->item(0)->setSelected(true);
	ignoreEvents = false;
}

ConfigureSkillSubsets::~ConfigureSkillSubsets()
{
}

void ConfigureSkillSubsets::on_bAddSubset_clicked()
{
	QListWidgetItem *item = new QListWidgetItem(QString("%1: Untitled").arg(ProjectData::skillSubsets.size(), 2, 10, QChar(' ')));
	ProjectData::SkillSubset *subset = new ProjectData::SkillSubset;
	subset->name = "Untitled";
	subset->type = ProjectData::SKILLTYPE_NORMAL;
	ProjectData::skillSubsets.append(subset);
	ProjectData::saved = false;
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	lwSkillSubsets->addItem(item);
	lwSkillSubsets->setCurrentRow(lwSkillSubsets->count() - 1);
	item->setSelected(true);
	bDeleteSubset->setEnabled(true);
	leName->selectAll();
	leName->setFocus();
}

void ConfigureSkillSubsets::on_bDeleteSubset_clicked()
{
	int row = lwSkillSubsets->currentRow();
	int confirmation = QMessageBox::question(this, "Delete Skill Subset?", "Are you sure you want to delete the selected skill subset?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirmation == QMessageBox::Yes)
	{
		delete lwSkillSubsets->takeItem(row);
		delete (ProjectData::skillSubsets[row]);
		ProjectData::skillSubsets.removeAt(row);
		ProjectData::saved = false;
		bAddSubset->setEnabled(true);
	}
}

void ConfigureSkillSubsets::on_cboxType_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		int row = lwSkillSubsets->currentRow();
		ProjectData::skillSubsets[row]->type = index;
	}
}

void ConfigureSkillSubsets::on_leName_textChanged(const QString &text)
{
	if (!ignoreEvents)
	{
		int row = lwSkillSubsets->currentRow();
		ProjectData::skillSubsets[row]->name = text;
		lwSkillSubsets->item(row)->setText(QString("%1: %2").arg(row, 2, 10, QChar('0')).arg(text));
	}
}

void ConfigureSkillSubsets::on_lwSkillSubsets_itemSelectionChanged()
{
	int row = lwSkillSubsets->currentRow();
	ignoreEvents = true;
	leName->setText(ProjectData::skillSubsets[row]->name);
	cboxType->setCurrentIndex(ProjectData::skillSubsets[row]->type);
	ignoreEvents = false;
	if (row >= 0 && row < 4)
	{
		bDeleteSubset->setEnabled(false);
		frmName->setEnabled(false);
		frmType->setEnabled(false);
	}
	else
	{
		bDeleteSubset->setEnabled(true);
		frmName->setEnabled(true);
		frmType->setEnabled(true);
	}
}

void ConfigureSkillSubsets::on_buttonBox_accepted()
{
	for (int i = 0; i < currentSkillTypes.size(); ++i)
	{
		ProjectData::DataReference<Skill> *skillRef = new ProjectData::DataReference<Skill>(i, __FILE__, __LINE__);
		if ((*skillRef)->getType() >= ProjectData::skillSubsets.size())
			(*skillRef)->changeTypeRetainingSkillType(currentSkillTypes[i]);
		delete skillRef;
	}
}

void ConfigureSkillSubsets::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Database::Skills::ConfigureSkillSubsets");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void ConfigureSkillSubsets::closeEvent(QCloseEvent *event)
{
	for (int i = 0; i < currentSkillTypes.size(); ++i)
	{
		ProjectData::DataReference<Skill> *skillRef = new ProjectData::DataReference<Skill>(i, __FILE__, __LINE__);
		if ((*skillRef)->getType() >= ProjectData::skillSubsets.size())
			(*skillRef)->changeTypeRetainingSkillType(currentSkillTypes[i]);
		delete skillRef;
	}
	QDialog::closeEvent(event);
}

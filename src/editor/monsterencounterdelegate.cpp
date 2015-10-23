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

#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QIcon>
#include "monsterencounterdelegate.h"
#include "projectdata.h"
#include "spinboxsidebutton.h"

MonsterEncounterDelegate::MonsterEncounterDelegate(int groupIDColumn, int priorityColumn, QObject *parent) : QStyledItemDelegate(parent)
{
	this->groupIDColumn = groupIDColumn;
	this->priorityColumn = priorityColumn;
}

QWidget *MonsterEncounterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == groupIDColumn)
	{
		QComboBox *comboBox = new QComboBox(parent);
		comboBox->setDuplicatesEnabled(true);
		comboBox->setEditable(false);
		comboBox->setMaxVisibleItems(10);
		comboBox->addItems(ProjectData::monsterGroupNames);
		return comboBox;
	}
	else if (index.column() == priorityColumn)
	{
		QWidget *wPriority;
		QHBoxLayout *hblPriorityLayout;
		QSpinBox *sbPriority;
		SpinBoxSideButton *bPrioritySideButton;
		QIcon spinBoxSideButtonIcon;
		wPriority = new QWidget(parent);
		wPriority->setObjectName("wPriority");
		hblPriorityLayout = new QHBoxLayout(wPriority);
		hblPriorityLayout->setSpacing(0);
		hblPriorityLayout->setContentsMargins(0, 0, 0, 0);
		hblPriorityLayout->setObjectName("hblPriorityLayout");
		sbPriority = new QSpinBox(wPriority);
		sbPriority->setMinimum(1);
		sbPriority->setMaximum(100);
		sbPriority->setObjectName("sbPriority");
		bPrioritySideButton = new SpinBoxSideButton(wPriority);
		bPrioritySideButton->setMinimumSize(QSize(13, 21));
		bPrioritySideButton->setMaximumSize(QSize(13, 21));
		spinBoxSideButtonIcon.addFile(":/icons/spinBoxSideButton.png", QSize(), QIcon::Normal, QIcon::Off);
		bPrioritySideButton->setIcon(spinBoxSideButtonIcon);
		bPrioritySideButton->setObjectName("bPrioritySideButton");
		bPrioritySideButton->setSpinBox(sbPriority);
		hblPriorityLayout->addWidget(sbPriority);
		hblPriorityLayout->addWidget(bPrioritySideButton);
		return wPriority;
	}
	else
		return QStyledItemDelegate::createEditor(parent, option, index);
}

void MonsterEncounterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == groupIDColumn)
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		int groupID = index.model()->data(index, Qt::UserRole).toInt();
		comboBox->setCurrentIndex(groupID);
	}
	else if (index.column() == priorityColumn)
	{
		QSpinBox *spinBox = editor->findChild<QSpinBox*>("sbPriority");
		int priority = index.model()->data(index, Qt::UserRole).toInt();
		spinBox->setValue(priority);
	}
	else
		QStyledItemDelegate::setEditorData(editor, index);
}

void MonsterEncounterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == groupIDColumn)
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		int groupID = comboBox->currentIndex();
		model->setData(index, groupID, Qt::UserRole);
		model->setData(index, ProjectData::monsterGroupNames[groupID], Qt::DisplayRole);
	}
	else if (index.column() == priorityColumn)
	{
		QSpinBox *spinBox = editor->findChild<QSpinBox*>("sbPriority");
		spinBox->interpretText();
		int priority = spinBox->value();
		model->setData(index, priority, Qt::UserRole);
		model->setData(index, QString::number(priority), Qt::DisplayRole);
	}
	else
		QStyledItemDelegate::setModelData(editor, model, index);
}

void MonsterEncounterDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}

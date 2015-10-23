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

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QIcon>
#include <QSpinBox>
#include <QComboBox>
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuobjectdelegate.h"
#include "menuscrollarea.h"
#include "menuselectablearea.h"
#include "menutext.h"
#include "projectdata.h"
#include "spinboxsidebutton.h"

MenuObjectDelegate::MenuObjectDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
	objectType = -1;
}

MenuObjectDelegate::~MenuObjectDelegate()
{
	objectType = -1;
}

void MenuObjectDelegate::setCurrentObject(MenuObject *menuObject)
{
	if (menuObject != NULL)
	{
		objectType = menuObject->getType();
		if (objectType == MenuObject::TYPE_FRAME)
			object.frame = (MenuFrame*)menuObject;
		if (objectType == MenuObject::TYPE_TEXT)
			object.text = (MenuText*)menuObject;
		if (objectType == MenuObject::TYPE_IMAGE)
			object.image = (MenuImage*)menuObject;
		if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			object.selectableArea = (MenuSelectableArea*)menuObject;
		if (objectType == MenuObject::TYPE_SCROLLAREA)
			object.scrollArea = (MenuScrollArea*)menuObject;
		if (objectType == MenuObject::TYPE_ITEMLIST)
			object.itemList = (MenuItemTable*)menuObject;
	}
}

QWidget *MenuObjectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int row = index.row();
	int column = index.column();
	//printf("createEditor: %d, %d\n", row, column);
	if (row == 0)
	{
		if (column == 1)
		{
			QLineEdit *lineEdit = new QLineEdit(parent);
			lineEdit->setFrame(false);
			return lineEdit;
		}
	}
	else if (objectType == MenuObject::TYPE_FRAME)
	{
		if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.frame->getParent();
			if (row == 1) // X
			{
				spinBox->setMinimum(object.frame->getWidth() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(640);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
				else
					spinBox->setMaximum(parentObject->getWidth());
			}
			else if (row == 2) // Y
			{
				spinBox->setMinimum(object.frame->getHeight() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(480);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
				else
					spinBox->setMaximum(parentObject->getHeight());
			}
			else if (row == 3) // Width
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(640);
			}
			else if (row == 4) // Height
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(480);
			}
			return spinBox;
		}
	}
	else if (objectType == MenuObject::TYPE_TEXT)
	{
		if (column == 1 && (row == 1 || row == 2 || row == 7))
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.text->getParent();
			if (row == 1) // X
			{
				spinBox->setMinimum(object.text->getWidth() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(640);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
				else
					spinBox->setMaximum(parentObject->getWidth());
			}
			else if (row == 2) // Y
			{
				spinBox->setMinimum(object.text->getHeight() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(480);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
				else
					spinBox->setMaximum(parentObject->getHeight());
			}
			else if (row == 7) // Text Width
			{
				spinBox->setMinimum(0);
				spinBox->setMaximum(640);
			}
			return spinBox;
		}
		else if (row == 4 && column == 1) // Font
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItems(ProjectData::fontNames);
			return comboBox;
		}
		else if (row == 8 && column == 1) // Alignment
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItem("Left");
			comboBox->addItem("Center");
			comboBox->addItem("Right");
			return comboBox;
		}
	}
	else if (objectType == MenuObject::TYPE_IMAGE)
	{
		if (row == 1 && column == 1) // X
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.image->getParent();
			spinBox->setMinimum(object.image->getWidth() * -1);
			if (parentObject == NULL)
				spinBox->setMaximum(640);
			else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
				spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
			else
				spinBox->setMaximum(parentObject->getWidth());
			return spinBox;
		}
		else if (row == 2 && column == 1) // Y
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.image->getParent();
			spinBox->setMinimum(object.image->getHeight() * -1);
			if (parentObject == NULL)
				spinBox->setMaximum(480);
			else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
				spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
			else
				spinBox->setMaximum(parentObject->getHeight());
			return spinBox;
		}
		else if (row == 4 && column == 1) // Scaled
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItem("True");
			comboBox->addItem("False");
			return comboBox;
		}
		else if (row == 5 && column == 1) // Scaled to Width
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			spinBox->setMinimum(1);
			spinBox->setMaximum(640);
			return spinBox;
		}
		else if (row == 6 && column == 1) // Scaled to Height
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			spinBox->setMinimum(1);
			spinBox->setMaximum(480);
			return spinBox;
		}
	}
	else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
	{
		if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.selectableArea->getParent();
			if (row == 1) // X
			{
				spinBox->setMinimum(object.selectableArea->getWidth() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(640);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
				else
					spinBox->setMaximum(parentObject->getWidth());
			}
			else if (row == 2) // Y
			{
				spinBox->setMinimum(object.selectableArea->getHeight() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(480);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
				else
					spinBox->setMaximum(parentObject->getHeight());
			}
			else if (row == 3) // Width
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(640);
			}
			else if (row == 4) // Height
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(480);
			}
			return spinBox;
		}
	}
	else if (objectType == MenuObject::TYPE_SCROLLAREA)
	{
		if (column == 1 && row >= 1 && row <= 8)
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.scrollArea->getParent();
			if (row == 1) // X
			{
				spinBox->setMinimum(object.scrollArea->getWidth() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(640);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
				else
					spinBox->setMaximum(parentObject->getWidth());
			}
			else if (row == 2) // Y
			{
				spinBox->setMinimum(object.scrollArea->getHeight() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(480);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
				else
					spinBox->setMaximum(parentObject->getHeight());
			}
			else if (row == 3) // Width
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(640);
			}
			else if (row == 4) // Height
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(480);
			}
			else if (row == 5) // Internal Width
			{
				spinBox->setMinimum(object.scrollArea->getWidth());
				spinBox->setMaximum(9999);
			}
			else if (row == 6) // Internal Height
			{
				spinBox->setMinimum(object.scrollArea->getHeight());
				spinBox->setMaximum(9999);
			}
			else if (row == 7) // Current Scroll X
			{
				spinBox->setMinimum(0);
				spinBox->setMaximum(object.scrollArea->getInternalWidth() - object.scrollArea->getWidth());
			}
			else if (row == 8) // Current Scroll Y
			{
				spinBox->setMinimum(0);
				spinBox->setMaximum(object.scrollArea->getInternalHeight() - object.scrollArea->getHeight());
			}
			return spinBox;
		}
	}
	else if (objectType == MenuObject::TYPE_ITEMLIST)
	{
		if (column == 1 && (((row >= 1 && row <= 4)) || (row >= 10 && row <= 15)) )
		{
			QSpinBox *spinBox = new QSpinBox(parent);
			MenuObject *parentObject = object.itemList->getParent();
			if (row == 1) // X
			{
				spinBox->setMinimum(object.itemList->getWidth() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(640);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalWidth());
				else
					spinBox->setMaximum(parentObject->getWidth());
			}
			else if (row == 2) // Y
			{
				spinBox->setMinimum(object.itemList->getHeight() * -1);
				if (parentObject == NULL)
					spinBox->setMaximum(480);
				else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
					spinBox->setMaximum(((MenuScrollArea*)parentObject)->getInternalHeight());
				else
					spinBox->setMaximum(parentObject->getHeight());
			}
			else if (row == 3) // Width
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(640);
			}
			else if (row == 4) // Height
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(480);
			}
			else if (row == 10) // Number of Columns
			{
				spinBox->setMinimum(1);
				spinBox->setMaximum(8);
			}
			else if (row == 11) // Row Height
			{
				spinBox->setMinimum(32);
				spinBox->setMaximum(64);
			}
			else
			{
				spinBox->setMinimum(0);
				spinBox->setMaximum(32);
			}
			return spinBox;
		}
		else if (column == 1 && row == 6) // Font
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItems(ProjectData::fontNames);
			return comboBox;
		}
		else if (column == 1 && row == 9) // Text Alignment
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItem("Left");
			comboBox->addItem("Center");
			comboBox->addItem("Right");
			return comboBox;
		}
		else if (column == 1 && row == 17) // Sorting Enabled
		{
			QComboBox *comboBox = new QComboBox(parent);
			comboBox->addItem("True");
			comboBox->addItem("False");
			return comboBox;
		}
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void MenuObjectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	int row = index.row();
	int column = index.column();
	//printf("setEditorData: %d, %d\n", row, column);
	if (row == 0)
	{
		if (column == 1)
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			lineEdit->setText(index.data().toString());
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_FRAME)
	{
		if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				spinBox->setValue(object.frame->getX());
			else if (row == 2) // Y
				spinBox->setValue(object.frame->getY());
			else if (row == 3) // Width
				spinBox->setValue(object.frame->getWidth());
			else if (row == 4) // Height
				spinBox->setValue(object.frame->getHeight());
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_TEXT)
	{
		if (column == 1 && (row == 1 || row == 2 || row == 7))
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				spinBox->setValue(object.text->getX());
			else if (row == 2) // Y
				spinBox->setValue(object.text->getY());
			else if (row == 7) // Text Width
				spinBox->setValue(object.text->getTextWidth());
		}
		else if (row == 4 && column == 1) // Font
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			comboBox->setCurrentIndex(object.text->getFontID());
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else if (row == 8 && column == 1) // Alignment
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			comboBox->setCurrentIndex(object.text->getAlignment());
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_IMAGE)
	{
		if (row == 1 && column == 1) // X
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->setValue(object.image->getX());
		}
		else if (row == 2 && column == 1) // Y
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->setValue(object.image->getY());
		}
		else if (row == 4 && column == 1) // Scaled
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			if (object.image->isScaled())
				comboBox->setCurrentIndex(0);
			else
				comboBox->setCurrentIndex(1);
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else if (row == 5 && column == 1) // Scaled to Width
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->setValue(object.image->getScaledToWidth());
		}
		else if (row == 6 && column == 1) // Scaled to Height
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->setValue(object.image->getScaledToHeight());
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
	{
		if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				spinBox->setValue(object.selectableArea->getX());
			else if (row == 2) // Y
				spinBox->setValue(object.selectableArea->getY());
			else if (row == 3) // Width
				spinBox->setValue(object.selectableArea->getWidth());
			else if (row == 4) // Height
				spinBox->setValue(object.selectableArea->getHeight());
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_SCROLLAREA)
	{
		if (column == 1 && row >= 1 && row <= 8)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				spinBox->setValue(object.scrollArea->getX());
			else if (row == 2) // Y
				spinBox->setValue(object.scrollArea->getY());
			else if (row == 3) // Width
				spinBox->setValue(object.scrollArea->getWidth());
			else if (row == 4) // Height
				spinBox->setValue(object.scrollArea->getHeight());
			else if (row == 5) // Internal Width
				spinBox->setValue(object.scrollArea->getInternalWidth());
			else if (row == 6) // Internal Height
				spinBox->setValue(object.scrollArea->getInternalHeight());
			else if (row == 7) // Current Scroll X
				spinBox->setValue(object.scrollArea->getCurrentScrollX());
			else if (row == 8) // Current Scroll Y
				spinBox->setValue(object.scrollArea->getCurrentScrollY());
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else if (objectType == MenuObject::TYPE_ITEMLIST)
	{
		if (column == 1 && ((row >= 1 && row <= 4) || (row >= 10 && row <= 15)))
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				spinBox->setValue(object.itemList->getX());
			else if (row == 2) // Y
				spinBox->setValue(object.itemList->getY());
			else if (row == 3) // Width
				spinBox->setValue(object.itemList->getWidth());
			else if (row == 4) // Height
				spinBox->setValue(object.itemList->getHeight());
			else if (row == 10) // Number of Columns
				spinBox->setValue(object.itemList->getNumColumns());
			else if (row == 11) // Row Height
				spinBox->setValue(object.itemList->getRowHeight());
			else if (row == 12) // Column Spacing
				spinBox->setValue(object.itemList->getColumnSpacing());
			else if (row == 13) // Row Spacing
				spinBox->setValue(object.itemList->getRowSpacing());
			else if (row == 14) // Text X-Offset
				spinBox->setValue(object.itemList->getTextXOffset());
			else if (row == 15) // Text Y-Offset
				spinBox->setValue(object.itemList->getTextYOffset());
		}
		else if (column == 1 && row == 6) // Font
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			comboBox->setCurrentIndex(object.itemList->getFontID());
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else if (column == 1 && row == 9) // Text Alignment
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			comboBox->setCurrentIndex(object.itemList->getTextAlignment());
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else if (column == 1 && row == 17) // Sorting Enabled
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			if (object.itemList->isSortingEnabled())
				comboBox->setCurrentIndex(0);
			else
				comboBox->setCurrentIndex(1);
			QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), comboBox, SLOT(close()));
		}
		else
			QStyledItemDelegate::setEditorData(editor, index);
	}
	else
		QStyledItemDelegate::setEditorData(editor, index);
}

void MenuObjectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	int row = index.row();
	int column = index.column();
	//printf("setModelData: %d, %d\n", row, column);
	if (objectType == MenuObject::TYPE_FRAME)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.frame->setName(lineEdit->text());
			model->setData(index, object.frame->getName());
		}
		else if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				object.frame->setX(spinBox->value());
			else if (row == 2) // Y
				object.frame->setY(spinBox->value());
			else if (row == 3) // Width
				object.frame->setWidth(spinBox->value());
			else if (row == 4) // Height
				object.frame->setHeight(spinBox->value());
			model->setData(index, spinBox->value());
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else if (objectType == MenuObject::TYPE_TEXT)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.text->setName(lineEdit->text());
			model->setData(index, object.text->getName());
		}
		else if (column == 1 && (row == 1 || row == 2 || row == 7))
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				object.text->setX(spinBox->value());
			else if (row == 2) // Y
				object.text->setY(spinBox->value());
			else if (row == 7) // Text Width
				object.text->setTextWidth(spinBox->value());
			model->setData(index, QString::number(spinBox->value()));
		}
		else if (row == 4 && column == 1) // Font
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			object.text->setFontID(comboBox->currentIndex());
			model->setData(index, comboBox->currentText());
		}
		else if (row == 8 && column == 1) // Alignment
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			object.text->setAlignment(comboBox->currentIndex());
			model->setData(index, comboBox->currentText());
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else if (objectType == MenuObject::TYPE_IMAGE)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.image->setName(lineEdit->text());
			model->setData(index, object.image->getName());
		}
		else if (row == 1 && column == 1) // X
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			object.image->setX(spinBox->value());
			model->setData(index, QString::number(spinBox->value()));
		}
		else if (row == 2 && column == 1) // Y
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			object.image->setY(spinBox->value());
			model->setData(index, QString::number(spinBox->value()));
		}
		else if (row == 4 && column == 1) // Scaled
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			if (comboBox->currentIndex() == 0)
				object.image->setScaled(true);
			else
				object.image->setScaled(false);
			model->setData(index, comboBox->currentText());
		}
		else if (row == 5 && column == 1) // Scaled to Width
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			object.image->setScaledToWidth(spinBox->value());
			model->setData(index, QString::number(spinBox->value()));
		}
		else if (row == 6 && column == 1) // Scaled to Height
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			object.image->setScaledToHeight(spinBox->value());
			model->setData(index, QString::number(spinBox->value()));
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.selectableArea->setName(lineEdit->text());
			model->setData(index, object.selectableArea->getName());
		}
		else if (column == 1 && row >= 1 && row <= 4)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				object.selectableArea->setX(spinBox->value());
			else if (row == 2) // Y
				object.selectableArea->setY(spinBox->value());
			else if (row == 3) // Width
				object.selectableArea->setWidth(spinBox->value());
			else if (row == 4) // Height
				object.selectableArea->setHeight(spinBox->value());
			model->setData(index, spinBox->value());
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else if (objectType == MenuObject::TYPE_SCROLLAREA)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.scrollArea->setName(lineEdit->text());
			model->setData(index, object.scrollArea->getName());
		}
		else if (column == 1 && row >= 1 && row <= 8)
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				object.scrollArea->setX(spinBox->value());
			else if (row == 2) // Y
				object.scrollArea->setY(spinBox->value());
			else if (row == 3) // Width
				object.scrollArea->setWidth(spinBox->value());
			else if (row == 4) // Height
				object.scrollArea->setHeight(spinBox->value());
			else if (row == 5) // Internal Width
				object.scrollArea->setInternalWidth(spinBox->value());
			else if (row == 6) // Internal Height
				object.scrollArea->setInternalHeight(spinBox->value());
			else if (row == 7) // Current Scroll X
				object.scrollArea->scrollTo(spinBox->value(), object.scrollArea->getCurrentScrollY());
			else if (row == 8) // Current Scroll Y
				object.scrollArea->scrollTo(object.scrollArea->getCurrentScrollX(), spinBox->value());
			model->setData(index, spinBox->value());
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else if (objectType == MenuObject::TYPE_ITEMLIST)
	{
		if (row == 0 && column == 1) // Object Name
		{
			QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
			object.itemList->setName(lineEdit->text());
			model->setData(index, object.itemList->getName());
		}
		else if (column == 1 && ((row >= 1 && row <= 4) || (row >= 10 && row <= 15)))
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			if (row == 1) // X
				object.itemList->setX(spinBox->value());
			else if (row == 2) // Y
				object.itemList->setY(spinBox->value());
			else if (row == 3) // Width
				object.itemList->setWidth(spinBox->value());
			else if (row == 4) // Height
				object.itemList->setHeight(spinBox->value());
			else if (row == 10) // Number of Columns
				object.itemList->setNumColumns(spinBox->value());
			else if (row == 11) // Row Height
				object.itemList->setRowHeight(spinBox->value());
			else if (row == 12) // Column Spacing
				object.itemList->setColumnSpacing(spinBox->value());
			else if (row == 13) // Row Spacing
				object.itemList->setRowSpacing(spinBox->value());
			else if (row == 14) // Text X-Offset
				object.itemList->setTextXOffset(spinBox->value());
			else if (row == 15) // Text Y-Offset
				object.itemList->setTextYOffset(spinBox->value());
			model->setData(index, spinBox->value());
		}
		else if (column == 1 && row == 6) // Font
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			object.itemList->setFontID(comboBox->currentIndex());
			model->setData(index, comboBox->currentText());
		}
		else if (column == 1 && row == 9) // Text Alignment
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			object.itemList->setTextAlignment(comboBox->currentIndex());
			model->setData(index, comboBox->currentText());
		}
		else if (column == 1 && row == 17) // Sorting Enabled
		{
			QComboBox *comboBox = static_cast<QComboBox*>(editor);
			if (comboBox->currentIndex() == 0)
				object.itemList->setSortingEnabled(true);
			else
				object.itemList->setSortingEnabled(false);
			model->setData(index, comboBox->currentText());
		}
		else
			QStyledItemDelegate::setModelData(editor, model, index);
	}
	else
		QStyledItemDelegate::setModelData(editor, model, index);
}

void MenuObjectDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

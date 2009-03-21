/*
 * Copyright (C) Rudi Pettazzi <rudi.pettazzi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <string>
#include "path_editor.h"
#include "win_api.h"

/**
 * ui setup
 */
PathEditor::PathEditor(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	// toolbar
    QFrame *toolbar = new QFrame;
    toolbar->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    QPushButton *addBtn = initButton(":/images/add.png");
    QPushButton *deleteBtn = initButton(":/images/remove.png");
    QPushButton *upBtn = initButton(":/images/up.png");
    QPushButton *downBtn = initButton(":/images/down.png");
    QHBoxLayout *toolLayout = new QHBoxLayout;
    toolLayout->setMargin(0);
    toolLayout->setSpacing(0);
    toolLayout->addStretch(1);
    toolLayout->addWidget(addBtn);
    toolLayout->addWidget(deleteBtn);
    toolLayout->addWidget(upBtn);
    toolLayout->addWidget(downBtn);
    toolbar->setLayout(toolLayout);

    // path elements list
    listWidget = new QListWidget;
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QPushButton *saveBtn = new QPushButton(tr("&Save"));
    QPushButton *cancelBtn = new QPushButton(tr("&Cancel"));
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    // signals
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(commit()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(rollback()));
    connect(addBtn, SIGNAL(clicked()), this, SLOT(itemAdd()));
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(itemDelete()));
    connect(upBtn, SIGNAL(clicked()), this, SLOT(itemUp()));
    connect(downBtn, SIGNAL(clicked()), this, SLOT(itemDown()));

    QString path;
    int res = waLoadPath(&path);

    if (res == 0) {
    	pathData = SPLIT_PATH(path);
    	listWidget->addItems(pathData);
    } else {
    	showError(waFormatMessage(res));
    }

    QFontMetrics mtr(listWidget->font());
    int n = listWidget->count();
    int width = 0;
    for (int i = 0; i < n; i++) {
        int w = mtr.width(listWidget->item(i)->text());
        if (w > width)
            width = w;
    }
    QScrollBar *b = listWidget->verticalScrollBar();
    resize(b ? width + b->width() : width, 250);
    addBtn->setFocus();
}

QPushButton* PathEditor::initButton(const char *iconFilename)
{
	QPushButton *b = new QPushButton;
	QIcon icon(iconFilename);
	b->setIcon(icon);
	b->setFlat(true);
	b->setMinimumSize(18, 18);
	b->setMaximumSize(18, 18);
	return b;
}

void PathEditor::showError(const QString& msg)
{
	QMessageBox::critical(this, tr("Error"), msg);
}

void PathEditor::showMessage()
{
	QMessageBox::information(this, tr("Information"), tr("Environment updated"));
}

/**
 * Open a file dialog and add the selected item to the list widget.
 * The registry is not changed until the user commits.
 */
void PathEditor::itemAdd()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setModal(true);
	dialog.setDirectory("C:");

	if (dialog.exec()) {
        QStringList filenames = dialog.selectedFiles();
        listWidget->addItems(filenames);
	}
}

/**
 * Delete the selected item from the list widget. The registry
 * is not changed until the user commits.
 */
void PathEditor::itemDelete()
{
    int r = listWidget->row(listWidget->currentItem());
    QListWidgetItem *t = listWidget->takeItem(r);
    delete t;
}

void PathEditor::itemUp()
{
	QListWidgetItem *tmp = listWidget->currentItem();
	if (tmp == NULL)
        return;
	int r = listWidget->row(tmp);
	if (r == 0)
	    return;

	QListWidgetItem *other = listWidget->takeItem(r-1);
	listWidget->insertItem(r, other);
	listWidget->insertItem(r-1, tmp);
}

void PathEditor::itemDown()
{
	QListWidgetItem *tmp = listWidget->currentItem();
	if (tmp == NULL)
		return;

	int r = listWidget->row(tmp);
	if (r == listWidget->count()-1)
		return;

	QListWidgetItem *other = listWidget->takeItem(r+1);
	listWidget->insertItem(r, other);
	listWidget->insertItem(r+1, tmp);
}

/**
 * Save to the registry the path concatenating the list widget items.
 */
void PathEditor::commit()
{
    int count = listWidget->count();
	QString s;

    for (int i = 0; i < count; i++) {
    	if (i != 0) s.append(';');
    	s.append(listWidget->item(i)->text());
    }

    int res = waSavePath(s);

    if (res == 0) {
    	pathData = SPLIT_PATH(s);
    	showMessage();
    } else {
    	showError(waFormatMessage(res));
    }
}

/**
 * Undo any changes
 */
void PathEditor::rollback()
{
    listWidget->clear();
    listWidget->addItems(pathData);
}

PathEditor::~PathEditor()
{

}


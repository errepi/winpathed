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
#ifndef PATHEDITOR_H
#define PATHEDITOR_H

#include <QtGui>

#define SPLIT_PATH(s) s.split(";", QString::SkipEmptyParts);

class QPushButton;
class QLabel;
class QListWidget;
class QFileDialog;
class QLineEdit;

class PathEditor : public QDialog
{
    Q_OBJECT

public:
    PathEditor(QWidget *parent = 0, Qt::WindowFlags = Qt::WindowMinMaxButtonsHint);
    ~PathEditor();

private slots:
    void itemAdd();
    void itemDelete();
    void itemUp();
    void itemDown();
    void commit();
    void rollback();

private:

    QListWidget *listWidget;
    QStringList pathData;

    QPushButton* initButton(const char *iconFilename);
    void showError(const QString& msg);
    void showMessage();

};

#endif // PATHEDITOR_H

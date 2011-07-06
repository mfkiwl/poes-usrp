/*
    POES-USRP, a software for recording and decoding POES high resolution weather satellite images.
    Copyright (C) 2009-2011 Free Software Foundation, Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Email: <postmaster@poes-weather.com>
    Web: <http://www.poes-weather.com>
*/
//---------------------------------------------------------------------------
#ifndef SATPROPDIALOG_H
#define SATPROPDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

//---------------------------------------------------------------------------
namespace Ui {
    class SatPropDialog;
}

class QListWidget;
class PList;
class TSat;

//---------------------------------------------------------------------------
class SatPropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SatPropDialog(PList *satList, QWidget *parent = 0);
    ~SatPropDialog();

protected:
    void keyPressEvent(QKeyEvent *event);

    int countSelected(QListWidget *lw);
    int finditem(QListWidget *lw, const QString& str);

private slots:
    void on_satlistWidget_itemClicked(QListWidgetItem* item);
    void on_rgblv_itemClicked(QListWidgetItem* item);
    void on_applyRGB_clicked();
    void on_addRGBBtn_clicked();

    void on_defaultrgbBtn_clicked();

private:
    Ui::SatPropDialog *ui;

    PList *satlist;
    TSat  *selsat;
};

#endif // SATPROPDIALOG_H

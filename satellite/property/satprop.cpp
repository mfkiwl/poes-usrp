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

#include <stdlib.h>
#include <memory.h>
#include <QSettings>

#include "plist.h"
#include "satprop.h"

//---------------------------------------------------------------------------
TSatProp::TSatProp(void)
{
    // default RGB to NOAA
    _rgb_day_ch[0] = 1; _rgb_night_ch[0] = 3;
    _rgb_day_ch[1] = 2; _rgb_night_ch[1] = 4;
    _rgb_day_ch[2] = 4; _rgb_night_ch[2] = 5;

    rgblist = new PList;

}

//---------------------------------------------------------------------------
TSatProp::~TSatProp(void)
{
    clear_rgb();
    delete rgblist;
}

//---------------------------------------------------------------------------
TSatProp& TSatProp::operator = (TSatProp& src)
{
    if(this == &src)
        return *this;

    clear_rgb();
    for(int i=0; i<src.rgblist->Count; i++)
        rgblist->Add(new TRGBConf(*((TRGBConf *) src.rgblist->ItemAt(i))));

    return *this;
}

//---------------------------------------------------------------------------
void TSatProp::zero(void)
{
    clear_rgb();
}

//---------------------------------------------------------------------------
TRGBConf *TSatProp::add_rgb(const QString& name, int r, int g, int b)
{
    if(name.isEmpty())
        return NULL;

    TRGBConf *rc = get_rgb(name);

    if(rc)
        rc->rgb_ch(r, g, b);
    else {
        rc = new TRGBConf(name, r, g, b);
        rgblist->Add(rc);
    }

    return rc;
}

//---------------------------------------------------------------------------
void TSatProp::del_rgb(const QString& name)
{
    TRGBConf *rc = get_rgb(name);

    if(rc) {
        rgblist->Delete(rc);
        delete rc;
    }
}

//---------------------------------------------------------------------------
TRGBConf *TSatProp::get_rgb(QString name)
{
    TRGBConf *rc;
    int i;

    for(i=0; i<rgblist->Count; i++) {
        rc = (TRGBConf *) rgblist->ItemAt(i);
        if(rc->name() == name)
            return rc;
    }

    return NULL;
}

//---------------------------------------------------------------------------
void TSatProp::clear_rgb(void)
{
    TRGBConf *rgbconf;

    while((rgbconf = (TRGBConf *)rgblist->Last())) {
        rgblist->Delete(rgbconf);
        delete rgbconf;
    }
}







//---------------------------------------------------------------------------
void TSatProp::rgb_day(int r_ch, int g_ch, int b_ch)
{
    // default to NOAA: 5 channels
    _rgb_day_ch[0] = r_ch;
    _rgb_day_ch[1] = g_ch;
    _rgb_day_ch[2] = b_ch;
}

//---------------------------------------------------------------------------
void TSatProp::rgb_night(int r_ch, int g_ch, int b_ch)
{
    // default to NOAA: 5 channels
    _rgb_night_ch[0] = r_ch;
    _rgb_night_ch[1] = g_ch;
    _rgb_night_ch[2] = b_ch;
}

//---------------------------------------------------------------------------
void TSatProp::checkChannels(int max_ch)
{
    for(int i=0; i<3; i++) {
        _rgb_day_ch[i] = _rgb_day_ch[i] < 1 ? 1:_rgb_day_ch[i] > max_ch ? max_ch:_rgb_day_ch[i];
        _rgb_night_ch[i] = _rgb_night_ch[i] < 1 ? 1:_rgb_night_ch[i] > max_ch ? max_ch:_rgb_night_ch[i];
    }
}

//---------------------------------------------------------------------------
void TSatProp::readSettings(QSettings *reg)
{
    TRGBConf *rc;
    QString str;
    int i = 0;

    reg->beginGroup("RGB-Conf");

    rc = new TRGBConf;
    while(true) {
        str.sprintf("Conf-%d", i++);
        reg->beginGroup(str);

        if(!reg->contains("ID")) {
            reg->endGroup();
            break;
        }

        rc->readSettings(reg);

        if(!get_rgb(rc->name()))
            rgblist->Add(new TRGBConf(*rc));

        // int j = rgblist->Count;

        reg->endGroup();
    }

    reg->endGroup(); // RGB-Conf
    delete rc;
}

//---------------------------------------------------------------------------
void TSatProp::add_rgb_defaults(void)
{
    // add some NOAA defaults (applies also to Feng-Yun and Meteor M-N1 also)
    add_rgb("RGB Daytime", 1, 2, 4);
    add_rgb("RGB Nighttime", 3, 4, 5);
    add_rgb("RGB Daytime cyan snow", 3, 2, 1);
}

//---------------------------------------------------------------------------
void TSatProp::writeSettings(QSettings *reg)
{
    TRGBConf *rc;
    QString str;
    int i;

    reg->beginGroup("RGB-Conf");

    for(i=0; i<rgblist->Count; i++) {
        rc = (TRGBConf *) rgblist->ItemAt(i);
        str.sprintf("Conf-%d", i);

        reg->beginGroup(str);
        rc->writeSettings(reg);
        reg->endGroup();
    }

    reg->endGroup(); // RGB-Conf
}

//---------------------------------------------------------------------------
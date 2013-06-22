// =============================================================================
// This file is part of boinctui.
// http://boinctui.googlecode.com
// Copyright (C) 2012,2013 Sergey Suslov
//
// boinctui is free software; you can redistribute it and/or modify it  under
// the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// boinctui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details
// <http://www.gnu.org/licenses/>.
// =============================================================================

#include "mainwin.h"
#include "tuievent.h"


#define INFPANWIDTH  20 //ширина инф панели


MainWin::MainWin(NRect rect, Config* cfg) : NGroup(rect)
{
    colname.push_back("  #  ");
    colname.push_back("state ");
    colname.push_back("   done%%");
    colname.push_back("  project             ");
    colname.push_back("  est");
    colname.push_back("  d/l");
    colname.push_back("  application                   ");
    colname.push_back("  task");
    tablheader = new NStaticText(NRect(1, rect.cols -2-(INFPANWIDTH)-1, 1, 1));
    //tablheader->setstring(getcolorpair(COLOR_CYAN,COLOR_BLACK) | A_BOLD,"  #  state    done%%  project               est d/l   task");
    wtask = new TaskWin(NRect(getheight()/2, getwidth()-2-(INFPANWIDTH)-1, 2, 1), cfg); //создаем окно процессов внутри wmain
    setcoltitle();
    wmsg = new MsgWin(NRect(getheight()-wtask->getheight()-4, getwidth()-2-(INFPANWIDTH+1), wtask->getheight()+3, 1)); //создаем окно евентов
    hline = new NHLine(NRect(1, getwidth()-2-(INFPANWIDTH+1), wtask->getheight()+2, 1), NULL); //горизонтальная линия
    vline = new NVLine(NRect(getheight()-2, 1, 1 , getwidth()-INFPANWIDTH-2), NULL); //вертикальная линия

    panel1 = new InfoPanel(NRect(getheight()-2,INFPANWIDTH,1,getwidth()-INFPANWIDTH-1));
    caption = new NColorString(0,"");
    insert(tablheader);
    insert(wtask);
    insert(wmsg);
    insert(hline);
    insert(vline);
    insert(panel1);
}


void MainWin::resize(int rows, int cols)
{
    NGroup::resize(rows, cols);
    tablheader->resize(1, getwidth()-2-(INFPANWIDTH)-1);
    wtask->resize(getheight()/2, getwidth()-2-(INFPANWIDTH)-1); //размер окна задач
    wmsg->resize(getheight()-wtask->getheight()-4, getwidth()-2-(INFPANWIDTH+1));
    wmsg->move(wtask->getheight()+3, 1);
    hline->resize(1, getwidth()-2-(INFPANWIDTH+1)); //горизонтальная линия
    hline->move(wtask->getheight()+2, 1);
    vline->resize(getheight()-2, 1);
    vline->move(1 , getwidth()-INFPANWIDTH-2);
    panel1->resize(getheight()-2,INFPANWIDTH);
    panel1->move(1,getwidth()-INFPANWIDTH-1);
}


void 	MainWin::setserver(Srv* srv) //установить отображаемый сервер
{
    wmsg->setserver(srv);
    wtask->setserver(srv);
    panel1->setserver(srv);
}


void MainWin::setcoltitle()
{
    std::string s = "";
    for (size_t i = 0; i < colname.size(); i++)
    {
	if (wtask->iscolvisible(i))
	    s = s + colname[i];
    }
    tablheader->setstring(getcolorpair(COLOR_CYAN,COLOR_BLACK) | A_BOLD, s.c_str());
}


void MainWin::refresh()
{
    wattrset(win, getcolorpair(COLOR_WHITE,COLOR_BLACK) | A_BOLD);
    box(win, ACS_VLINE, ACS_HLINE);
    //рисуем заголовок
    wmove(win,0,(getwidth()/2)-(caption->getlen()+1)/2);
    std::list<NColorStringPart*>::iterator it;
    for (it = caption->parts.begin(); it != caption->parts.end(); it++) //цикл по частям тек строки
    {
	NColorStringPart* part = *it;
	wattrset(win,part->attr);
	wprintw(win,"%s",part->s.c_str());
	//wattrset(win,0);
    }
    //wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    //wattroff(win, getcolorpair(COLOR_WHITE,COLOR_BLACK) | A_BOLD);
    NGroup::refresh();
}


void MainWin::eventhandle(NEvent* ev) 	//обработчик событий
{
    NGroup::eventhandle(ev); //предок

    if ( ev->done )
	return;

    if (ev->type == NEvent::evPROG) //прграммные
    {
	switch(ev->cmdcode)
	{
	    case evCOLVIEWCH: //изменился набор колонок
	    {
		setcoltitle();
		tablheader->refresh();
		wtask->refresh();
	    }
	} //switch
    }
}


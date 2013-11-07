/****************************************************************************
** $Id: qt/examples/application/application.cpp   2.3.0   edited 2001-01-26 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

/* Here's the NOMAD license, since modifications were made to the original
   'application.cpp' file. */

/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* École Polytechnique de Montréal - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montréal (Québec) H3C 3A7 Canada             *
* Tel:  (514) 340-6053 x6928                                                  *
* Email: Gilles.Couture@gerad.ca                                              *
* Fax: (514) 340-5665                                                         *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation. You can find the              *
* GNU General Public License at:                                              *
*             http://www.gnu.org/copyleft/gpl.html                            *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program; if not, write to the Free Software                 *
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. *
*                                                                             *
* You can find information on the NOMAD software at:                          *
*             http://www.gerad.ca/NOMAD/                                      *
******************************************************************************/

#include <iostream>
#include <qaccel.h>
#include <qapplication.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qimage.h>
#include <qkeycode.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
using namespace std;
#include "LIBRARY/filesave.xpm"
#include "texteditor.h"

const char * fileSaveText = "Click this button to save the file you are "
"editing.\n\n"
"You can also select the Save command from the File menu.\n\n";

TextEditor::TextEditor(const char *file_name)
  : QMainWindow(0, "example application main window", 0)
{
  filename = file_name;
  int id;

  QPixmap saveIcon;

  fileTools = new QToolBar(this, "file operations");
  fileTools->setLabel(tr("File Operations"));

  saveIcon = QPixmap(filesave);
  QToolButton * fileSave
    = new QToolButton(saveIcon, "Save File", QString::null, this,
		      SLOT(save()), fileTools, "save file");

  (void)QWhatsThis::whatsThisButton(fileTools);

  QWhatsThis::add(fileSave, fileSaveText);

  QPopupMenu * file = new QPopupMenu(this);
  menuBar()->insertItem("&File", file);

  id = file->insertItem(saveIcon, "&Save", this, SLOT(save()), CTRL+Key_S);
  file->setWhatsThis(id, fileSaveText);
  id = file->insertItem("Save &as...", this, SLOT(saveAs()));
  file->setWhatsThis(id, fileSaveText);
  file->insertSeparator();
  file->insertItem("&Close", this, SLOT(close()), CTRL+Key_W);

  QPopupMenu * help = new QPopupMenu(this);
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", help);

  help->insertItem("&About", this, SLOT(about()), Key_F1);
  help->insertSeparator();
  help->insertItem("What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1);

  editor = new QTextEdit(this, "editor");
  editor->setTextFormat(Qt::PlainText);
  editor->setFont(QFont("Courier", 10));
  editor->setFocus();
  setCentralWidget(editor);
  statusBar()->message("Ready", 2000);
  resize(450, 600);
}

TextEditor::~TextEditor()
{}

void TextEditor::load()
{
  QString fn = QFileDialog::getOpenFileName(QString::null, QString::null,
					    this);
  if (!fn.isEmpty())
    load(fn);
  else
    statusBar()->message("Loading aborted", 2000);
}

void TextEditor::load(const char * fileName)
{
  QFile file(fileName);
  if (!file.open( IO_ReadOnly))
    return;

  editor->clear();
  QTextStream stream(&file);
  editor->setText(stream.read());
  file.close();

  editor->repaint();
  editor->setModified(false);
  setCaption(fileName);
  statusBar()->message(QString("Loaded document %1").arg(fileName), 2000);
}

void TextEditor::save()
{
  if (filename.isEmpty())
    {
      saveAs();
      return;
    }

  QFile file(filename);
  // A check to see if the file is open.
  if (!file.open(IO_WriteOnly))
    {
      statusBar()->message(QString("Could not write to %1").arg(filename),
			   2000);
      return;
    }

  // Else the file is open. The text is written in the file.
  QTextStream stream(&file);
  stream << editor->text();
  file.close();

  editor->setModified(false);
  setCaption(filename);
  statusBar()->message(QString("File %1 saved").arg(filename), 2000);
}

void TextEditor::saveAs()
{
  QString fn = QFileDialog::getSaveFileName(QString::null, QString::null,
					    this);
  if (!fn.isEmpty())
    {
      filename = fn;
      save();
    }
  else
    statusBar()->message("Saving aborted", 2000);
}

void TextEditor::closeEvent(QCloseEvent * ce)
{
  if (!editor->isModified())
    {
      ce->accept();
      return;
    }

  switch(QMessageBox::information(this, "Qt Application Example",
				  "The document has been changed since "
				  "the last save.",
				  "Save Now", "Cancel", "Leave Anyway",
				  0, 1))
    {
      case 0:   save();
                ce->accept();
		break;
      case 1:
      default:  // just for sanity
	        ce->ignore();
		break;
      case 2:   ce->accept();
	        break;
    }
}

void TextEditor::about()
{
  QMessageBox::about(this, "Text Editor",
		     "This simple text editor captures data\n"
		     "for Nomad (like the starting point).");
}

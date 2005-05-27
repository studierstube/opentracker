/****************************************************************************
** Form interface generated from reading ui file '.\settings.ui'
**
** Created: Thu Aug 5 01:29:06 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include "SoVacuumKit.h"

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;

class Settings : public QDialog
{ 
    Q_OBJECT

public:
    Settings( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Settings();

    QLabel* TextLabel1;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel2;
    QLabel* TextLabel5;
    QComboBox* ComboBox1;
    QComboBox* ComboBox2;
    QComboBox* ComboBox3;
    QComboBox* ComboBox4;
    QComboBox* ComboBox5;

    void setParent(SoVacuumKit *p) { parent = p; }

public slots:
    virtual void ModeActivated( int mode );
    virtual void TransferFunctionActivated( int funct );
    virtual void ColorMapActivated( int colorMap );
    virtual void AxisActivated( int axis );
    virtual void FileActivated( const QString & );

    SoVacuumKit *parent;
};

#endif // SETTINGS_H

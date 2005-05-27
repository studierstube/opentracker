/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

void Settings::ModeActivated( int mode)
{
    parent->switch_mode(mode+1);
}

void Settings::TransferFunctionActivated( int funct)
{
    parent->switch_predefColorMap(funct);
}

void Settings::ColorMapActivated( int colorMap)
{
    parent->switch_colorMapType(colorMap);
}

void Settings::AxisActivated( int axis)
{
    parent->switch_axis(axis);
}

void Settings::FileActivated( const QString &fileName )
{
    QString name=QString("../apps/vacuum/data/")+fileName+QString(".vol");
    parent->fileName.setValue(name.ascii());
}
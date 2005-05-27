/****************************************************************************
** Settings meta object code from reading C++ file 'settings.h'
**
** Created: Thu Aug 5 01:29:06 2004
**      by: The Qt MOC ($Id:  qt/moc_yacc.cpp   3.0.3   edited Mar 12 07:51 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "settings.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Settings::className() const
{
    return "Settings";
}

QMetaObject *Settings::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Settings;

#ifndef QT_NO_TRANSLATION
QString Settings::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Settings", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Settings::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Settings", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Settings::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "mode", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"ModeActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "funct", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"TransferFunctionActivated", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "colorMap", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"ColorMapActivated", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "axis", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"AxisActivated", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"FileActivated", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "ModeActivated(int)", &slot_0, QMetaData::Public },
	{ "TransferFunctionActivated(int)", &slot_1, QMetaData::Public },
	{ "ColorMapActivated(int)", &slot_2, QMetaData::Public },
	{ "AxisActivated(int)", &slot_3, QMetaData::Public },
	{ "FileActivated(const QString&)", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Settings", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Settings.setMetaObject( metaObj );
    return metaObj;
}

void* Settings::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Settings" ) ) return (Settings*)this;
    return QDialog::qt_cast( clname );
}

bool Settings::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ModeActivated(static_QUType_int.get(_o+1)); break;
    case 1: TransferFunctionActivated(static_QUType_int.get(_o+1)); break;
    case 2: ColorMapActivated(static_QUType_int.get(_o+1)); break;
    case 3: AxisActivated(static_QUType_int.get(_o+1)); break;
    case 4: FileActivated(static_QUType_QString.get(_o+1)); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Settings::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Settings::qt_property( int _id, int _f, QVariant* _v)
{
    return QDialog::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES

/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -c DesktopAdaptor -a desktopadaptor dde-desktop.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DESKTOPADAPTOR_H_1441881428
#define DESKTOPADAPTOR_H_1441881428

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "../dbustype.h"
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.deepin.dde.Desktop
 */
class DesktopAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.Desktop")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.dde.Desktop\">\n"
"    <method name=\"AskSkip\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"i\"/>\n"
"      <arg direction=\"out\" type=\"{ibs}\"/>\n"
"      <annotation value=\"ConflictInfo\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"    <method name=\"ConflictDialog\">\n"
"      <arg direction=\"out\" type=\"{ibs}\"/>\n"
"      <annotation value=\"ConflictInfo\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    DesktopAdaptor(QObject *parent);
    virtual ~DesktopAdaptor();

    static inline const char *staticServerPath()
    { return "com.deepin.dde.desktop"; }
    static inline const char *staticInterfacePath()
    { return "/com/deepin/dde/desktop"; }
    static inline const char *staticInterfaceName()
    { return "com.deepin.dde.desktop"; }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    ConflictInfo AskSkip(const QString &in0, const QString &in1, const QString &in2, int in3);
    ConflictInfo ConflictDialog();
Q_SIGNALS: // SIGNALS
};

#endif
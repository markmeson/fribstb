/****************************************************************************
** Meta object code from reading C++ file 'stbwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "stbwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stbwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_STBWidget_t {
    QByteArrayData data[13];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_STBWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_STBWidget_t qt_meta_stringdata_STBWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "STBWidget"
QT_MOC_LITERAL(1, 10, 11), // "updateTitle"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "cellChange"
QT_MOC_LITERAL(4, 34, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(5, 52, 4), // "item"
QT_MOC_LITERAL(6, 57, 12), // "colHdrChange"
QT_MOC_LITERAL(7, 70, 3), // "col"
QT_MOC_LITERAL(8, 74, 12), // "rowHdrChange"
QT_MOC_LITERAL(9, 87, 3), // "row"
QT_MOC_LITERAL(10, 91, 4), // "save"
QT_MOC_LITERAL(11, 96, 7), // "checked"
QT_MOC_LITERAL(12, 104, 6) // "saveAs"

    },
    "STBWidget\0updateTitle\0\0cellChange\0"
    "QTableWidgetItem*\0item\0colHdrChange\0"
    "col\0rowHdrChange\0row\0save\0checked\0"
    "saveAs"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_STBWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    1,   55,    2, 0x0a /* Public */,
       6,    1,   58,    2, 0x0a /* Public */,
       8,    1,   61,    2, 0x0a /* Public */,
      10,    1,   64,    2, 0x0a /* Public */,
      10,    0,   67,    2, 0x2a /* Public | MethodCloned */,
      12,    1,   68,    2, 0x0a /* Public */,
      12,    0,   71,    2, 0x2a /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,

       0        // eod
};

void STBWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<STBWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateTitle(); break;
        case 1: _t->cellChange((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->colHdrChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->rowHdrChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->save((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->save(); break;
        case 6: _t->saveAs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->saveAs(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject STBWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableWidget::staticMetaObject>(),
    qt_meta_stringdata_STBWidget.data,
    qt_meta_data_STBWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *STBWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *STBWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_STBWidget.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int STBWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'raytracer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../raytracer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raytracer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RayTracer_t {
    QByteArrayData data[12];
    char stringdata[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RayTracer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RayTracer_t qt_meta_stringdata_RayTracer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RayTracer"
QT_MOC_LITERAL(1, 10, 11), // "renderScene"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "drawShadowSet"
QT_MOC_LITERAL(4, 37, 14), // "drawReflectSet"
QT_MOC_LITERAL(5, 52, 9), // "loadScene"
QT_MOC_LITERAL(6, 62, 4), // "name"
QT_MOC_LITERAL(7, 67, 13), // "generateScene"
QT_MOC_LITERAL(8, 81, 18), // "setNormalRendering"
QT_MOC_LITERAL(9, 100, 16), // "setMCPathTracing"
QT_MOC_LITERAL(10, 117, 9), // "setScene1"
QT_MOC_LITERAL(11, 127, 9) // "setScene2"

    },
    "RayTracer\0renderScene\0\0drawShadowSet\0"
    "drawReflectSet\0loadScene\0name\0"
    "generateScene\0setNormalRendering\0"
    "setMCPathTracing\0setScene1\0setScene2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RayTracer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    0,   67,    2, 0x0a /* Public */,
       5,    1,   68,    2, 0x0a /* Public */,
       7,    0,   71,    2, 0x0a /* Public */,
       8,    0,   72,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x0a /* Public */,
      10,    0,   74,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RayTracer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RayTracer *_t = static_cast<RayTracer *>(_o);
        switch (_id) {
        case 0: _t->renderScene(); break;
        case 1: _t->drawShadowSet(); break;
        case 2: _t->drawReflectSet(); break;
        case 3: _t->loadScene(); break;
        case 4: _t->loadScene((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->generateScene(); break;
        case 6: _t->setNormalRendering(); break;
        case 7: _t->setMCPathTracing(); break;
        case 8: _t->setScene1(); break;
        case 9: _t->setScene2(); break;
        default: ;
        }
    }
}

const QMetaObject RayTracer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RayTracer.data,
      qt_meta_data_RayTracer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RayTracer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RayTracer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RayTracer.stringdata))
        return static_cast<void*>(const_cast< RayTracer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RayTracer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    checkConsistency.cpp \
    compression.cpp \
    indentation_remover.cpp \
    main.cpp \
    xml_editor.cpp \
    xml_to_json.cpp \
    xml_vector.cpp

HEADERS += \
    checkConsistency.h \
    compression.h \
    indentation_remover.h \
    xml_editor.h \
    xml_to_json.h \
    xml_vector.h

FORMS += \
    xml_editor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

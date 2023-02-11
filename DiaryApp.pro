QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Business/Business.cpp \
        Business/DiaryManager.cpp \
        Business/EntryManager.cpp \
        Business/UserManager.cpp \
        Common/Diary.cpp \
        Common/Entry.cpp \
        Common/User.cpp \
        Persistence/FilePersistence.cpp \
        Presentation/entryview.cpp \
        Presentation/exerciseview.cpp \
        Presentation/loginview.cpp \
        Presentation/mainwindow.cpp \
        Presentation/moodview.cpp \
        Presentation/registerview.cpp \
        Presentation/topicview.cpp \
        Presentation/travelmapview.cpp \
        Presentation/travelview.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Business/Business.h \
    Business/DiaryManager.h \
    Business/EntryManager.h \
    Business/IBusiness.h \
    Business/UserManager.h \
    Common/Diary.h \
    Common/Entry.h \
    Common/User.h \
    Persistence/FilePersistence.h \
    Persistence/IPersistence.h \
    Presentation/entryview.h \
    Presentation/exerciseview.h \
    Presentation/loginview.h \
    Presentation/mainwindow.h \
    Presentation/moodview.h \
    Presentation/registerview.h \
    Presentation/topicview.h \
    Presentation/travelmapview.h \
    Presentation/travelview.h

FORMS += \
    Presentation/entryview.ui \
    Presentation/exerciseview.ui \
    Presentation/loginview.ui \
    Presentation/mainwindow.ui \
    Presentation/moodview.ui \
    Presentation/registerview.ui \
    Presentation/topicview.ui \
    Presentation/travelmapview.ui \
    Presentation/travelview.ui



RESOURCES += \
    Images.qrc

DISTFILES += \
    Data/Diaries.bin \
    Data/Topics.bin \
    Data/User.bin \
    Data/angry.txt \
    Data/anxious.txt \
    Data/nervous.txt \
    Data/sad.txt \
    Data/stressed.txt \
    Data/tired.txt \
    Diaries.bin \
    Entries.bin \
    Images/desk.jpg \
    Images/drawnNotebook.jpg \
    Images/white.png \ \





copydata.commands = if [ ! -d $$OUT_PWD/Data ]; then $(COPY_DIR) $$PWD/Data $$OUT_PWD; fi
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


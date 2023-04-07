#DEFINES += QCUSTOMPLOT_USE_OPENGL

QT += widgets
QT += printsupport
RC_ICONS = icone.ico
QMAKE_CXXFLAGS_WARN_OFF += -Wunused-parameter
SOURCES += \
    ../../../../../../../../qcustomplot/qcustomplot.cpp \
    basic_func.cpp \
    clickable_label.cpp \
    compilateur.cpp \
    fenetre.cpp \
    graphique.cpp \
    loading_bar.cpp \
    main.cpp \
    map.cpp \
    parameter.cpp \
    parametre.cpp \
    perso.cpp \
    primary.cpp \
    simulateur.cpp \
    sliderfermeture.cpp \
    world.cpp

HEADERS += \
    ../../../../../../../../qcustomplot/qcustomplot.h \
    clickable_label.h \
    compilateur.h \
    constant.h \
    data_noms.h \
    fenetre.h \
    graphique.h \
    loading_bar.h \
    main.h \
    map.h \
    parameter.h \
    parametre.h \
    perso.h \
    primary.h \
    simulateur.h \
    sliderfermeture.h \
    world.h

FORMS += \
    form.ui \
    para.ui

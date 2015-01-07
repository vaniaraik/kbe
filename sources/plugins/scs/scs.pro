
TARGET        = $$qtLibraryTarget(scs)
TEMPLATE      = lib
INCLUDEPATH  += ../../kbe

DESTDIR = ../../bin/plugins

CONFIG       += plugin

OBJECTS_DIR = obj
MOC_DIR = moc

win32 {
    LIBS += -L$$PWD/win32
}

LIBS+= -lantlr3c

TRANSLATIONS += media/translations/scs_en_EN.ts \
                media/translations/scs_ru_RU.ts

lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.qm
lrelease.CONFIG       += no_link target_predeps
QMAKE_EXTRA_COMPILERS += lrelease

RESOURCES += scs.qrc

INCLUDEPATH += \
    scsparser \
    highlightingrules \
    include

HEADERS += \
    highlightingrules/scsstdhighlightingrule.h \
    highlightingrules/scsmultilinehighlightingrule.h \
    highlightingrules/scsmultilinecommenthighlightingrule.h \
    highlightingrules/scshighlightingrulespool.h \
    highlightingrules/scsabstracthighlightingrule.h \
    scsparser/scsparserexception.h \
    scsparser/scsparserwrapper.h \
    scsparser/SCsCLexer.h \
    scsparser/SCsCParser.h \
    scsparser/scscparserdefs.h \
	scsparser/scsasynchparser.h \
    scswindow.h \
    scsplugin.h \
    scscodeerroranalyzer.h \
    scserrortablewidget.h \
    scscodeeditor.h \
    scserrortablewidgetitem.h \
    scsfindwidget.h \
    scssyntaxhighlighter.h \
    scscodeanalyzer.h \
    scscodecompleter.h \
    include/antlr3.h \
    include/antlr3treeparser.h \
    include/antlr3tokenstream.h \
    include/antlr3string.h \
    include/antlr3rewritestreams.h \
    include/antlr3recognizersharedstate.h \
    include/antlr3parsetree.h \
    include/antlr3parser.h \
    include/antlr3memory.h \
    include/antlr3lexer.h \
    include/antlr3intstream.h \
    include/antlr3interfaces.h \
    include/antlr3input.h \
    include/antlr3filestream.h \
    include/antlr3exception.h \
    include/antlr3errors.h \
    include/antlr3encodings.h \
    include/antlr3defs.h \
    include/antlr3debugeventlistener.h \
    include/antlr3cyclicdfa.h \
    include/antlr3convertutf.h \
    include/antlr3commontreenodestream.h \
    include/antlr3commontreeadaptor.h \
    include/antlr3commontree.h \
    include/antlr3commontoken.h \
    include/antlr3collections.h \
    include/antlr3bitset.h \
    include/antlr3basetreeadaptor.h \
    include/antlr3basetree.h \
    include/antlr3baserecognizer.h \
    include/antlr3.h


SOURCES += \
    highlightingrules/scsstdhighlightingrule.cpp \
    highlightingrules/scsmultilinehighlightingrule.cpp \
    highlightingrules/scsmultilinecommenthighlightingrule.cpp \
    highlightingrules/scshighlightingrulespool.cpp \
    highlightingrules/scsabstracthighlightingrule.cpp \
    scsparser/scsparserwrapper.cpp \
    scsparser/scsparserexception.cpp \
    scsparser/SCsCLexer.c \
    scsparser/SCsCParser.c \
    scsparser/scscparserdefs.c \
	scsparser/scsasynchparser.cpp \
    scswindow.cpp \
    scserrortablewidget.cpp \
    scscodeeditor.cpp \
    scsfindwidget.cpp \
    scscodeerroranalyzer.cpp \
    scsplugin.cpp \
    scserrortablewidgetitem.cpp \
    scscodeanalyzer.cpp \
    scssyntaxhighlighter.cpp \
    scscodecompleter.cpp







TEMPLATE = subdirs 
CONFIG += ordered

win32-msvc* {
    MSVC_VER = $$(VisualStudioVersion)
    equals(MSVC_VER, 14.0){
        message("msvc 2015")
    }

    equals(MSVC_VER, 14.1){
        message("msvc 2017")
    }

    equals(MSVC_VER, 14.2){
        message("msvc 2019")
    }

    QMAKE_CXXFLAGS += /FS
}

TRANSLATIONS += \
    src/translation/abt_en.ts \
    src/translation/abt_ja.ts

SUBDIRS += \
          $$PWD/src/variantconverter \
#1
          $$PWD/src/xmlbuilder \
          $$PWD/src/searchxmlbuilder \
          $$PWD/src/processxmllistgenerator \
          $$PWD/src/processxmlbuilder \
          $$PWD/src/stringxmlbuilder \
          $$PWD/src/pluginsxmlbuilder \
          $$PWD/src/profilexmlbuilder \
#2
          $$PWD/src/filesearchloader \
          $$PWD/src/executor \
#3
          $$PWD/src/profilecombobox \
          $$PWD/src/basictable \
          $$PWD/src/consolebase \
#4
          $$PWD/src/fileinfodialog \
          $$PWD/src/filesearchdialog \
#5
          $$PWD/src/searchtable \
          $$PWD/src/stringtable \
          $$PWD/src/filequeuetable \
#          $$PWD/src/extrafunctionstable \
          $$PWD/src/pluginstree \
#6
          $$PWD/src/AutoBatchRunner \
          $$PWD/src/AutoBatchScheduler \
          $$PWD/src/ProfileEditor \

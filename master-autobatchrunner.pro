TEMPLATE = subdirs 
CONFIG += ordered

SUBDIRS += \
          $$PWD/src/variantconverter \
          $$PWD/src/convertchecker \
#1
          $$PWD/src/xmlbuilder \
          $$PWD/src/searchxmlbuilder \
          $$PWD/src/processxmlbuilder \
          $$PWD/src/processxmllistgenerator \
          $$PWD/src/stringxmlbuilder \
          $$PWD/src/extrafunctionsxmlbuilder \
          $$PWD/src/profilexmlbuilder \
#2
          $$PWD/src/filesearchloader \
          $$PWD/src/executor \
#3
          $$PWD/src/profilecombobox \
          $$PWD/src/basictable \
#4
          $$PWD/src/fileinfodialog \
          $$PWD/src/filesearchdialog \
#5
          $$PWD/src/searchtable \
          $$PWD/src/stringtable \
          $$PWD/src/filequeuetable \
          $$PWD/src/extrafunctionstable \
#6
          $$PWD/src/AutoBatchRunner \
          $$PWD/src/ProfileEditor \
          $$PWD/src/AutoBatchScheduler

RESOURCES += \
    src/res/resources.qrc

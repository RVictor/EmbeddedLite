OPT= -DREGEXP_UNICODE 
DEBUG= -g 
CMP=g++  $(DEBUG) $(OPT) 
GCC=gcc 
EXE_NAME=Runtime/codelite-emb
EXE_NAME_NO_PATH=codelite-emb
OBJ_DIR=Debug_gcc_unicode
EXT=ud
OUTPUT_DIR=lib
WXCFG=--unicode=yes --debug=yes
DESTDIR=

CCFLAGS= -Wall $(shell wx-config --cxxflags $(WXCFG)) -fno-strict-aliasing -DASTYLE_LIB -DYY_NEVER_INTERACTIVE=1 -DGTK -DSCI_LEXER -DLINK_LEXERS -D__WX__ -DINSTALL_DIR=\"/usr/share/codelite\" -DPLUGINS_DIR=\".\"
LINK_FLAGS =  -L../$(OUTPUT_DIR)  -lplugin$(EXT) -lcodelite$(EXT) -lwxsqlite3$(EXT) -lwxscintilla$(EXT) -L../sdk/sqlite3/lib $(shell wx-config --libs $(WXCFG))
PLUGIN_LINK_FLAGS =  -L../$(OUTPUT_DIR) $(shell wx-config --libs $(WXCFG))  
INCLUDES := $(INCLUDES)  

INCLUDES := $(INCLUDES) $(shell pkg-config --cflags gtk+-2.0)
LINK_FLAGS := $(LINK_FLAGS) $(shell pkg-config --libs gtk+-2.0)
PLUGIN_LINK_FLAGS := $(PLUGIN_LINK_FLAGS) $(shell pkg-config --libs gtk+-2.0)
INCLUDES := $(INCLUDES) -I. -I../sdk/codelite_indexer/network -I../sdk/wxsqlite3/include -I../sdk/wxsqlite3/sqlite3/include -I../sdk/wxscintilla/include -I../sdk/wxscintilla/src/scintilla/include -I../sdk/wxscintilla/src/scintilla/src -I../Interfaces -I../Debugger -I../Plugin -I../CodeLite 


GizmosObjects = \
	Debug_gcc_unicode/gizmos.o\
	Debug_gcc_unicode/newclassbasedlg.o\
	Debug_gcc_unicode/newclassdlg.o\
	Debug_gcc_unicode/newinheritancebasedlg.o\
	Debug_gcc_unicode/newinheritancedlg.o\
	Debug_gcc_unicode/newplugindata.o\
	Debug_gcc_unicode/newwxprojectbasedlg.o\
	Debug_gcc_unicode/newwxprojectdlg.o\
	Debug_gcc_unicode/newwxprojectinfo.o\
	Debug_gcc_unicode/pluginwizard.o\
	Debug_gcc_unicode/pluginwizard_pag2.o\
	Debug_gcc_unicode/pluginwizard_page1.o\

## build targets
all :  pre_build Gizmos post_build

pre_build:
	@echo
	@echo "----------Building project:[ Gizmos - Debug_gcc_unicode ]----------"
	@echo
	@test -d  $(OBJ_DIR) || mkdir $(OBJ_DIR)
	@test -d  ../$(OUTPUT_DIR) || mkdir ../$(OUTPUT_DIR)

clean:
	$(RM) ../$(OUTPUT_DIR)/Gizmos.so
	$(RM) -fr $(OBJ_DIR)
	$(RM) -fr ../Runtime/plugins/Gizmos.so

post_build:

Debug_gcc_unicode/gizmos.o: gizmos.cpp Debug_gcc_unicode/gizmos.o.d
	$(CMP) -c gizmos.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/gizmos.o
Debug_gcc_unicode/gizmos.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/gizmos.o -MFDebug_gcc_unicode/gizmos.o.d  -MM gizmos.cpp
Debug_gcc_unicode/gizmos.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/gizmos.o.i gizmos.cpp
Debug_gcc_unicode/newclassbasedlg.o: newclassbasedlg.cpp Debug_gcc_unicode/newclassbasedlg.o.d
	$(CMP) -c newclassbasedlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newclassbasedlg.o
Debug_gcc_unicode/newclassbasedlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newclassbasedlg.o -MFDebug_gcc_unicode/newclassbasedlg.o.d  -MM newclassbasedlg.cpp
Debug_gcc_unicode/newclassbasedlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newclassbasedlg.o.i newclassbasedlg.cpp
Debug_gcc_unicode/newclassdlg.o: newclassdlg.cpp Debug_gcc_unicode/newclassdlg.o.d
	$(CMP) -c newclassdlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newclassdlg.o
Debug_gcc_unicode/newclassdlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newclassdlg.o -MFDebug_gcc_unicode/newclassdlg.o.d  -MM newclassdlg.cpp
Debug_gcc_unicode/newclassdlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newclassdlg.o.i newclassdlg.cpp
Debug_gcc_unicode/newinheritancebasedlg.o: newinheritancebasedlg.cpp Debug_gcc_unicode/newinheritancebasedlg.o.d
	$(CMP) -c newinheritancebasedlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newinheritancebasedlg.o
Debug_gcc_unicode/newinheritancebasedlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newinheritancebasedlg.o -MFDebug_gcc_unicode/newinheritancebasedlg.o.d  -MM newinheritancebasedlg.cpp
Debug_gcc_unicode/newinheritancebasedlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newinheritancebasedlg.o.i newinheritancebasedlg.cpp
Debug_gcc_unicode/newinheritancedlg.o: newinheritancedlg.cpp Debug_gcc_unicode/newinheritancedlg.o.d
	$(CMP) -c newinheritancedlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newinheritancedlg.o
Debug_gcc_unicode/newinheritancedlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newinheritancedlg.o -MFDebug_gcc_unicode/newinheritancedlg.o.d  -MM newinheritancedlg.cpp
Debug_gcc_unicode/newinheritancedlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newinheritancedlg.o.i newinheritancedlg.cpp
Debug_gcc_unicode/newplugindata.o: newplugindata.cpp Debug_gcc_unicode/newplugindata.o.d
	$(CMP) -c newplugindata.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newplugindata.o
Debug_gcc_unicode/newplugindata.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newplugindata.o -MFDebug_gcc_unicode/newplugindata.o.d  -MM newplugindata.cpp
Debug_gcc_unicode/newplugindata.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newplugindata.o.i newplugindata.cpp
Debug_gcc_unicode/newwxprojectbasedlg.o: newwxprojectbasedlg.cpp Debug_gcc_unicode/newwxprojectbasedlg.o.d
	$(CMP) -c newwxprojectbasedlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newwxprojectbasedlg.o
Debug_gcc_unicode/newwxprojectbasedlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newwxprojectbasedlg.o -MFDebug_gcc_unicode/newwxprojectbasedlg.o.d  -MM newwxprojectbasedlg.cpp
Debug_gcc_unicode/newwxprojectbasedlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newwxprojectbasedlg.o.i newwxprojectbasedlg.cpp
Debug_gcc_unicode/newwxprojectdlg.o: newwxprojectdlg.cpp Debug_gcc_unicode/newwxprojectdlg.o.d
	$(CMP) -c newwxprojectdlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newwxprojectdlg.o
Debug_gcc_unicode/newwxprojectdlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newwxprojectdlg.o -MFDebug_gcc_unicode/newwxprojectdlg.o.d  -MM newwxprojectdlg.cpp
Debug_gcc_unicode/newwxprojectdlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newwxprojectdlg.o.i newwxprojectdlg.cpp
Debug_gcc_unicode/newwxprojectinfo.o: newwxprojectinfo.cpp Debug_gcc_unicode/newwxprojectinfo.o.d
	$(CMP) -c newwxprojectinfo.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newwxprojectinfo.o
Debug_gcc_unicode/newwxprojectinfo.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newwxprojectinfo.o -MFDebug_gcc_unicode/newwxprojectinfo.o.d  -MM newwxprojectinfo.cpp
Debug_gcc_unicode/newwxprojectinfo.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newwxprojectinfo.o.i newwxprojectinfo.cpp
Debug_gcc_unicode/pluginwizard.o: pluginwizard.cpp Debug_gcc_unicode/pluginwizard.o.d
	$(CMP) -c pluginwizard.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/pluginwizard.o
Debug_gcc_unicode/pluginwizard.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/pluginwizard.o -MFDebug_gcc_unicode/pluginwizard.o.d  -MM pluginwizard.cpp
Debug_gcc_unicode/pluginwizard.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/pluginwizard.o.i pluginwizard.cpp
Debug_gcc_unicode/pluginwizard_pag2.o: pluginwizard_pag2.cpp Debug_gcc_unicode/pluginwizard_pag2.o.d
	$(CMP) -c pluginwizard_pag2.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/pluginwizard_pag2.o
Debug_gcc_unicode/pluginwizard_pag2.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/pluginwizard_pag2.o -MFDebug_gcc_unicode/pluginwizard_pag2.o.d  -MM pluginwizard_pag2.cpp
Debug_gcc_unicode/pluginwizard_pag2.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/pluginwizard_pag2.o.i pluginwizard_pag2.cpp
Debug_gcc_unicode/pluginwizard_page1.o: pluginwizard_page1.cpp Debug_gcc_unicode/pluginwizard_page1.o.d
	$(CMP) -c pluginwizard_page1.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/pluginwizard_page1.o
Debug_gcc_unicode/pluginwizard_page1.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/pluginwizard_page1.o -MFDebug_gcc_unicode/pluginwizard_page1.o.d  -MM pluginwizard_page1.cpp
Debug_gcc_unicode/pluginwizard_page1.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/pluginwizard_page1.o.i pluginwizard_page1.cpp

Gizmos: $(GizmosObjects)
	$(CMP) -fPIC  -shared  -o ../$(OUTPUT_DIR)/Gizmos.so $(GizmosObjects) $(PLUGIN_LINK_FLAGS) 
	cp -f ../$(OUTPUT_DIR)/Gizmos.so ../Runtime/plugins/.

-include Debug_gcc_unicode/*.o.d


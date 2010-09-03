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


ExternalToolsObjects = \
	Debug_gcc_unicode/externaltooldlg.o\
	Debug_gcc_unicode/externaltools.o\
	Debug_gcc_unicode/externaltoolsbasedlg.o\
	Debug_gcc_unicode/externaltoolsdata.o\
	Debug_gcc_unicode/newtoolbase.o\
	Debug_gcc_unicode/newtooldlg.o\

## build targets
all :  pre_build ExternalTools post_build

pre_build:
	@echo
	@echo "----------Building project:[ ExternalTools - Debug_gcc_unicode ]----------"
	@echo
	@test -d  $(OBJ_DIR) || mkdir $(OBJ_DIR)
	@test -d  ../$(OUTPUT_DIR) || mkdir ../$(OUTPUT_DIR)

clean:
	$(RM) ../$(OUTPUT_DIR)/ExternalTools.so
	$(RM) -fr $(OBJ_DIR)
	$(RM) -fr ../Runtime/plugins/ExternalTools.so

post_build:

Debug_gcc_unicode/externaltooldlg.o: externaltooldlg.cpp Debug_gcc_unicode/externaltooldlg.o.d
	$(CMP) -c externaltooldlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/externaltooldlg.o
Debug_gcc_unicode/externaltooldlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/externaltooldlg.o -MFDebug_gcc_unicode/externaltooldlg.o.d  -MM externaltooldlg.cpp
Debug_gcc_unicode/externaltooldlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/externaltooldlg.o.i externaltooldlg.cpp
Debug_gcc_unicode/externaltools.o: externaltools.cpp Debug_gcc_unicode/externaltools.o.d
	$(CMP) -c externaltools.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/externaltools.o
Debug_gcc_unicode/externaltools.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/externaltools.o -MFDebug_gcc_unicode/externaltools.o.d  -MM externaltools.cpp
Debug_gcc_unicode/externaltools.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/externaltools.o.i externaltools.cpp
Debug_gcc_unicode/externaltoolsbasedlg.o: externaltoolsbasedlg.cpp Debug_gcc_unicode/externaltoolsbasedlg.o.d
	$(CMP) -c externaltoolsbasedlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/externaltoolsbasedlg.o
Debug_gcc_unicode/externaltoolsbasedlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/externaltoolsbasedlg.o -MFDebug_gcc_unicode/externaltoolsbasedlg.o.d  -MM externaltoolsbasedlg.cpp
Debug_gcc_unicode/externaltoolsbasedlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/externaltoolsbasedlg.o.i externaltoolsbasedlg.cpp
Debug_gcc_unicode/externaltoolsdata.o: externaltoolsdata.cpp Debug_gcc_unicode/externaltoolsdata.o.d
	$(CMP) -c externaltoolsdata.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/externaltoolsdata.o
Debug_gcc_unicode/externaltoolsdata.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/externaltoolsdata.o -MFDebug_gcc_unicode/externaltoolsdata.o.d  -MM externaltoolsdata.cpp
Debug_gcc_unicode/externaltoolsdata.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/externaltoolsdata.o.i externaltoolsdata.cpp
Debug_gcc_unicode/newtoolbase.o: newtoolbase.cpp Debug_gcc_unicode/newtoolbase.o.d
	$(CMP) -c newtoolbase.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newtoolbase.o
Debug_gcc_unicode/newtoolbase.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newtoolbase.o -MFDebug_gcc_unicode/newtoolbase.o.d  -MM newtoolbase.cpp
Debug_gcc_unicode/newtoolbase.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newtoolbase.o.i newtoolbase.cpp
Debug_gcc_unicode/newtooldlg.o: newtooldlg.cpp Debug_gcc_unicode/newtooldlg.o.d
	$(CMP) -c newtooldlg.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/newtooldlg.o
Debug_gcc_unicode/newtooldlg.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/newtooldlg.o -MFDebug_gcc_unicode/newtooldlg.o.d  -MM newtooldlg.cpp
Debug_gcc_unicode/newtooldlg.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/newtooldlg.o.i newtooldlg.cpp

ExternalTools: $(ExternalToolsObjects)
	$(CMP) -fPIC  -shared  -o ../$(OUTPUT_DIR)/ExternalTools.so $(ExternalToolsObjects) $(PLUGIN_LINK_FLAGS) 
	cp -f ../$(OUTPUT_DIR)/ExternalTools.so ../Runtime/plugins/.

-include Debug_gcc_unicode/*.o.d


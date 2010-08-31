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


Subversion2Objects = \
	Debug_gcc_unicode/commit_dialog.o\
	Debug_gcc_unicode/commitmessagescache.o\
	Debug_gcc_unicode/subversion2.o\
	Debug_gcc_unicode/subversion2_ui.o\
	Debug_gcc_unicode/subversion_password_db.o\
	Debug_gcc_unicode/subversion_view.o\
	Debug_gcc_unicode/svn_checkout_dialog.o\
	Debug_gcc_unicode/svn_command_handlers.o\
	Debug_gcc_unicode/svn_console.o\
	Debug_gcc_unicode/svn_copy_dialog.o\
	Debug_gcc_unicode/svn_default_command_handler.o\
	Debug_gcc_unicode/svn_login_dialog.o\
	Debug_gcc_unicode/svn_preferences_dialog.o\
	Debug_gcc_unicode/svnblamedialog.o\
	Debug_gcc_unicode/svnblameeditor.o\
	Debug_gcc_unicode/svncommand.o\
	Debug_gcc_unicode/svncommandhandler.o\
	Debug_gcc_unicode/svnstatushandler.o\
	Debug_gcc_unicode/svnxml.o\

## build targets
all :  pre_build Subversion2 post_build

pre_build:
	@echo
	@echo "----------Building project:[ Subversion2 - Debug_gcc_unicode ]----------"
	@echo
	@test -d  $(OBJ_DIR) || mkdir $(OBJ_DIR)
	@test -d  ../$(OUTPUT_DIR) || mkdir ../$(OUTPUT_DIR)

clean:
	$(RM) ../$(OUTPUT_DIR)/Subversion2.so
	$(RM) -fr $(OBJ_DIR)
	$(RM) -fr ../Runtime/plugins/Subversion2.so

post_build:

Debug_gcc_unicode/commit_dialog.o: commit_dialog.cpp Debug_gcc_unicode/commit_dialog.o.d
	$(CMP) -c commit_dialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/commit_dialog.o
Debug_gcc_unicode/commit_dialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/commit_dialog.o -MFDebug_gcc_unicode/commit_dialog.o.d  -MM commit_dialog.cpp
Debug_gcc_unicode/commit_dialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/commit_dialog.o.i commit_dialog.cpp
Debug_gcc_unicode/commitmessagescache.o: commitmessagescache.cpp Debug_gcc_unicode/commitmessagescache.o.d
	$(CMP) -c commitmessagescache.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/commitmessagescache.o
Debug_gcc_unicode/commitmessagescache.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/commitmessagescache.o -MFDebug_gcc_unicode/commitmessagescache.o.d  -MM commitmessagescache.cpp
Debug_gcc_unicode/commitmessagescache.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/commitmessagescache.o.i commitmessagescache.cpp
Debug_gcc_unicode/subversion2.o: subversion2.cpp Debug_gcc_unicode/subversion2.o.d
	$(CMP) -c subversion2.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/subversion2.o
Debug_gcc_unicode/subversion2.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/subversion2.o -MFDebug_gcc_unicode/subversion2.o.d  -MM subversion2.cpp
Debug_gcc_unicode/subversion2.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/subversion2.o.i subversion2.cpp
Debug_gcc_unicode/subversion2_ui.o: subversion2_ui.cpp Debug_gcc_unicode/subversion2_ui.o.d
	$(CMP) -c subversion2_ui.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/subversion2_ui.o
Debug_gcc_unicode/subversion2_ui.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/subversion2_ui.o -MFDebug_gcc_unicode/subversion2_ui.o.d  -MM subversion2_ui.cpp
Debug_gcc_unicode/subversion2_ui.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/subversion2_ui.o.i subversion2_ui.cpp
Debug_gcc_unicode/subversion_password_db.o: subversion_password_db.cpp Debug_gcc_unicode/subversion_password_db.o.d
	$(CMP) -c subversion_password_db.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/subversion_password_db.o
Debug_gcc_unicode/subversion_password_db.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/subversion_password_db.o -MFDebug_gcc_unicode/subversion_password_db.o.d  -MM subversion_password_db.cpp
Debug_gcc_unicode/subversion_password_db.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/subversion_password_db.o.i subversion_password_db.cpp
Debug_gcc_unicode/subversion_view.o: subversion_view.cpp Debug_gcc_unicode/subversion_view.o.d
	$(CMP) -c subversion_view.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/subversion_view.o
Debug_gcc_unicode/subversion_view.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/subversion_view.o -MFDebug_gcc_unicode/subversion_view.o.d  -MM subversion_view.cpp
Debug_gcc_unicode/subversion_view.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/subversion_view.o.i subversion_view.cpp
Debug_gcc_unicode/svn_checkout_dialog.o: svn_checkout_dialog.cpp Debug_gcc_unicode/svn_checkout_dialog.o.d
	$(CMP) -c svn_checkout_dialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_checkout_dialog.o
Debug_gcc_unicode/svn_checkout_dialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_checkout_dialog.o -MFDebug_gcc_unicode/svn_checkout_dialog.o.d  -MM svn_checkout_dialog.cpp
Debug_gcc_unicode/svn_checkout_dialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_checkout_dialog.o.i svn_checkout_dialog.cpp
Debug_gcc_unicode/svn_command_handlers.o: svn_command_handlers.cpp Debug_gcc_unicode/svn_command_handlers.o.d
	$(CMP) -c svn_command_handlers.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_command_handlers.o
Debug_gcc_unicode/svn_command_handlers.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_command_handlers.o -MFDebug_gcc_unicode/svn_command_handlers.o.d  -MM svn_command_handlers.cpp
Debug_gcc_unicode/svn_command_handlers.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_command_handlers.o.i svn_command_handlers.cpp
Debug_gcc_unicode/svn_console.o: svn_console.cpp Debug_gcc_unicode/svn_console.o.d
	$(CMP) -c svn_console.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_console.o
Debug_gcc_unicode/svn_console.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_console.o -MFDebug_gcc_unicode/svn_console.o.d  -MM svn_console.cpp
Debug_gcc_unicode/svn_console.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_console.o.i svn_console.cpp
Debug_gcc_unicode/svn_copy_dialog.o: svn_copy_dialog.cpp Debug_gcc_unicode/svn_copy_dialog.o.d
	$(CMP) -c svn_copy_dialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_copy_dialog.o
Debug_gcc_unicode/svn_copy_dialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_copy_dialog.o -MFDebug_gcc_unicode/svn_copy_dialog.o.d  -MM svn_copy_dialog.cpp
Debug_gcc_unicode/svn_copy_dialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_copy_dialog.o.i svn_copy_dialog.cpp
Debug_gcc_unicode/svn_default_command_handler.o: svn_default_command_handler.cpp Debug_gcc_unicode/svn_default_command_handler.o.d
	$(CMP) -c svn_default_command_handler.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_default_command_handler.o
Debug_gcc_unicode/svn_default_command_handler.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_default_command_handler.o -MFDebug_gcc_unicode/svn_default_command_handler.o.d  -MM svn_default_command_handler.cpp
Debug_gcc_unicode/svn_default_command_handler.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_default_command_handler.o.i svn_default_command_handler.cpp
Debug_gcc_unicode/svn_login_dialog.o: svn_login_dialog.cpp Debug_gcc_unicode/svn_login_dialog.o.d
	$(CMP) -c svn_login_dialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_login_dialog.o
Debug_gcc_unicode/svn_login_dialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_login_dialog.o -MFDebug_gcc_unicode/svn_login_dialog.o.d  -MM svn_login_dialog.cpp
Debug_gcc_unicode/svn_login_dialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_login_dialog.o.i svn_login_dialog.cpp
Debug_gcc_unicode/svn_preferences_dialog.o: svn_preferences_dialog.cpp Debug_gcc_unicode/svn_preferences_dialog.o.d
	$(CMP) -c svn_preferences_dialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svn_preferences_dialog.o
Debug_gcc_unicode/svn_preferences_dialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svn_preferences_dialog.o -MFDebug_gcc_unicode/svn_preferences_dialog.o.d  -MM svn_preferences_dialog.cpp
Debug_gcc_unicode/svn_preferences_dialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svn_preferences_dialog.o.i svn_preferences_dialog.cpp
Debug_gcc_unicode/svnblamedialog.o: svnblamedialog.cpp Debug_gcc_unicode/svnblamedialog.o.d
	$(CMP) -c svnblamedialog.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svnblamedialog.o
Debug_gcc_unicode/svnblamedialog.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svnblamedialog.o -MFDebug_gcc_unicode/svnblamedialog.o.d  -MM svnblamedialog.cpp
Debug_gcc_unicode/svnblamedialog.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svnblamedialog.o.i svnblamedialog.cpp
Debug_gcc_unicode/svnblameeditor.o: svnblameeditor.cpp Debug_gcc_unicode/svnblameeditor.o.d
	$(CMP) -c svnblameeditor.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svnblameeditor.o
Debug_gcc_unicode/svnblameeditor.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svnblameeditor.o -MFDebug_gcc_unicode/svnblameeditor.o.d  -MM svnblameeditor.cpp
Debug_gcc_unicode/svnblameeditor.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svnblameeditor.o.i svnblameeditor.cpp
Debug_gcc_unicode/svncommand.o: svncommand.cpp Debug_gcc_unicode/svncommand.o.d
	$(CMP) -c svncommand.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svncommand.o
Debug_gcc_unicode/svncommand.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svncommand.o -MFDebug_gcc_unicode/svncommand.o.d  -MM svncommand.cpp
Debug_gcc_unicode/svncommand.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svncommand.o.i svncommand.cpp
Debug_gcc_unicode/svncommandhandler.o: svncommandhandler.cpp Debug_gcc_unicode/svncommandhandler.o.d
	$(CMP) -c svncommandhandler.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svncommandhandler.o
Debug_gcc_unicode/svncommandhandler.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svncommandhandler.o -MFDebug_gcc_unicode/svncommandhandler.o.d  -MM svncommandhandler.cpp
Debug_gcc_unicode/svncommandhandler.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svncommandhandler.o.i svncommandhandler.cpp
Debug_gcc_unicode/svnstatushandler.o: svnstatushandler.cpp Debug_gcc_unicode/svnstatushandler.o.d
	$(CMP) -c svnstatushandler.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svnstatushandler.o
Debug_gcc_unicode/svnstatushandler.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svnstatushandler.o -MFDebug_gcc_unicode/svnstatushandler.o.d  -MM svnstatushandler.cpp
Debug_gcc_unicode/svnstatushandler.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svnstatushandler.o.i svnstatushandler.cpp
Debug_gcc_unicode/svnxml.o: svnxml.cpp Debug_gcc_unicode/svnxml.o.d
	$(CMP) -c svnxml.cpp -fPIC $(CCFLAGS) $(INCLUDES) -o Debug_gcc_unicode/svnxml.o
Debug_gcc_unicode/svnxml.o.d:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -MTDebug_gcc_unicode/svnxml.o -MFDebug_gcc_unicode/svnxml.o.d  -MM svnxml.cpp
Debug_gcc_unicode/svnxml.o.i:
	$(CMP) -fPIC $(CCFLAGS) $(INCLUDES) -E -o Debug_gcc_unicode/svnxml.o.i svnxml.cpp

Subversion2: $(Subversion2Objects)
	$(CMP) -fPIC  -shared  -o ../$(OUTPUT_DIR)/Subversion2.so $(Subversion2Objects) $(PLUGIN_LINK_FLAGS) 
	cp -f ../$(OUTPUT_DIR)/Subversion2.so ../Runtime/plugins/.

-include Debug_gcc_unicode/*.o.d


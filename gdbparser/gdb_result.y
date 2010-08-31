%{
// Copyright Eran Ifrah(c)
%}

%{

#include <string>
#include <vector>
#include <map>
#include <cstdio>
#define YYSTYPE std::string
#define YYDEBUG 0        /* get the pretty debugging code to compile*/

#ifdef yylex
#undef yylex
#define yylex gdb_result_lex
#endif

int  gdb_result_lex();
void gdb_result_error(const char*);
bool setGdbLexerInput(const std::string &in, bool ascii, bool wantWhitespace);
void gdb_result_lex_clean();
int  gdb_result_parse();
void cleanup();

extern std::string gdb_result_lval;
static std::map<std::string, std::string>               sg_attributes;
static std::vector<std::map<std::string, std::string> > sg_children;
static std::vector<std::string>                         sg_locals;
%}

%token GDB_DONE
%token GDB_RUNNING
%token GDB_CONNECTED
%token GDB_ERROR
%token GDB_EXIT
%token GDB_STACK_ARGS
%token GDB_VALUE
%token GDB_ARGS
%token GDB_FRAME
%token GDB_NAME
%token GDB_STRING
%token GDB_LEVEL
%token GDB_FRAME
%token GDB_ESCAPED_STRING
%token GDB_LOCALS
%token GDB_INTEGER
%token GDB_OCTAL
%token GDB_HEX
%token GDB_FLOAT
%token GDB_IDENTIFIER
%token GDB_NUMCHILD
%token GDB_TYPE
%token GDB_DATA
%token GDB_ADDR
%token GDB_ASCII
%token GDB_CHILDREN
%token GDB_CHILD
%token GDB_MORE
%token GDB_VAROBJ
%token GDB_BREAKPOINT_TABLE
%token GDB_NR_ROWS
%token GDB_NR_COLS
%token GDB_HDR
%token GDB_BODY
%token GDB_BKPT
%%

parse: children_list
	 | parse children_list
	 ;

children_list:    { cleanup(); } child_pattern
				|  error {
				//printf("CodeLite: syntax error, unexpected token '%s' found\n", gdb_result_lval.c_str());
				}
			;

child_pattern :   '^' GDB_DONE ',' GDB_NUMCHILD '=' GDB_STRING ',' GDB_CHILDREN '=' list_open children list_close
				| '^' GDB_DONE ',' GDB_NAME '=' GDB_STRING ',' {sg_attributes[$4] = $6;} child_attributes
				{
					sg_children.push_back( sg_attributes );
					sg_attributes.clear();
				}
				/* ^done,locals=[{name="pcls",type="ChildClass *",value="0x0"},{name="s",type="string *",value="0x3e2550"}] */
				| '^' GDB_DONE ',' GDB_LOCALS '=' list_open locals list_close
				/* ^done,locals={varobj={exp="str",value="{...}",name="var6",numchild="1",type="string",typecode="STRUCT",dynamic_type="",in_scope="true",block_start_addr="0x00001e84",block_end_addr="0x00001f38"},varobj={exp="anotherLocal",value="2",name="var7",numchild="0",type="int",typecode="INT",dynamic_type="",in_scope="true",block_start_addr="0x00001e84",block_end_addr="0x00001f38"}} */
				| '^' GDB_DONE ',' GDB_LOCALS '=' list_open mac_locals list_close
				/*^done,stack-args=[frame={level="0",args=[{name="argc",type="int",value="1"},{name="argv",type="char **",value="0x3e2570"}]}]*/
				| '^' GDB_DONE ',' GDB_STACK_ARGS '=' list_open GDB_FRAME '=' list_open GDB_LEVEL '=' GDB_STRING ',' GDB_ARGS '=' list_open locals list_close list_close list_close
				/* ^done,stack-args={frame={level="0",args={varobj={exp="argc",value="1",name="var8",numchild="0",type="int",typecode="INT",dynamic_type="",in_scope="true",block_start_addr="0x00001e76",block_end_addr="0x00001f42"},varobj={exp="argv",value="0xbffffaa8",name="var9",numchild="1",type="char **",typecode="PTR",dynamic_type="",in_scope="true",block_start_addr="0x00001e76",block_end_addr="0x00001f42"}}}} */
				| '^' GDB_DONE ',' GDB_STACK_ARGS '=' list_open GDB_FRAME '=' list_open GDB_LEVEL '=' GDB_STRING ',' GDB_ARGS '=' list_open mac_locals list_close list_close list_close
				/* ^done,BreakpointTable={nr_rows="3",nr_cols="6",hdr=[{width="7",alignment="-1",col_name="number",colhdr="Num"},{width="14",alignment="-1",col_name="type",colhdr="Type"},{width="4",alignment="-1",col_name="disp",colhdr="Disp"},{width="3",alignment="-1",col_name="enabled",colhdr="Enb"},{width="10",alignment="-1",col_name="addr",colhdr="Address"},{width="40",alignment="2",col_name="what",colhdr="What"}],body=[bkpt={number="1",type="breakpoint",disp="keep",enabled="y",addr="0x77c35571",at="<msvcrt!_assert+11>",times="0",original-location="assert"},bkpt={number="2",type="breakpoint",disp="keep",enabled="y",addr="0x004014d4",func="main",file="C:/TestArea/WxConsole/consoleproj.cpp",fullname="C:/TestArea/WxConsole/consoleproj.cpp",line="63",times="0",original-location="main"},bkpt={number="3",type="breakpoint",disp="keep",enabled="y",addr="0x004014bb",func="main",file="C:/TestArea/WxConsole/consoleproj.cpp",fullname="C:/TestArea/WxConsole/consoleproj.cpp",line="61",times="1",original-location="*4199611"}]} */
				| '^' GDB_DONE ',' GDB_BREAKPOINT_TABLE '=' list_open bpt_table_hdr bpt_table_body list_close
				;

/* {nr_rows="3",nr_cols="6",hdr=[{width="7",alignment="-1",col_name="number",colhdr="Num"},{width="14",alignment="-1",col_name="type",colhdr="Type"},{width="4",alignment="-1",col_name="disp",colhdr="Disp"},{width="3",alignment="-1",col_name="enabled",colhdr="Enb"},{width="10",alignment="-1",col_name="addr",colhdr="Address"},{width="40",alignment="2",col_name="what",colhdr="What"}*/
bpt_table_hdr : GDB_NR_ROWS '=' GDB_STRING ',' GDB_NR_COLS '=' GDB_STRING ',' GDB_HDR '=' list_open bpt_hdr_table_description list_close
			  ;
/** breakpoint related
 */
bpt_hdr_table_description : list_open bpt_table_description_attr list_close
					  | list_open bpt_table_description_attr list_close ',' bpt_hdr_table_description
					  ;

bpt_table_description_attr  : GDB_IDENTIFIER '=' GDB_STRING
							| GDB_IDENTIFIER '=' GDB_STRING ',' bpt_table_description_attr
							;

/* body=[bkpt={number="1",type="breakpoint",disp="keep",enabled="y",addr="0x77c35571",at="<msvcrt!_assert+11>",times="0",original-location="assert"},bkpt={number="2",type="breakpoint",disp="keep",enabled="y",addr="0x004014d4",func="main",file="C:/TestArea/WxConsole/consoleproj.cpp",fullname="C:/TestArea/WxConsole/consoleproj.cpp",line="63",times="0",original-location="main"},bkpt={number="3",type="breakpoint",disp="keep",enabled="y",addr="0x004014bb",func="main",file="C:/TestArea/WxConsole/consoleproj.cpp",fullname="C:/TestArea/WxConsole/consoleproj.cpp",line="61",times="1",original-location="*4199611"}]*/
bpt_table_body : ',' GDB_BODY '=' list_open breakpoints list_close
			   ;

breakpoints : GDB_BKPT '=' list_open child_attributes list_close { sg_children.push_back( sg_attributes ); sg_attributes.clear();}
			           | GDB_BKPT '=' list_open child_attributes list_close  { sg_children.push_back( sg_attributes ); sg_attributes.clear();} ',' breakpoints
					   ;
/**
 * Locals parsing
 */
mac_locals  : GDB_VAROBJ '=' '{' child_attributes '}'
			{
				sg_children.push_back( sg_attributes );
				sg_attributes.clear();
			}
			| GDB_VAROBJ '=' '{' child_attributes '}' {sg_children.push_back( sg_attributes ); sg_attributes.clear(); } ',' mac_locals
			;

locals      : '{' child_attributes '}'
			{
				sg_children.push_back( sg_attributes );
				sg_attributes.clear();
			}
			| '{' child_attributes '}' {sg_children.push_back( sg_attributes ); sg_attributes.clear(); } ',' locals
			;

list_open :  '['
			|'{'
			;

list_close: ']'
			|'}'
			;

children     : GDB_CHILD '=' '{' child_attributes '}' {
					sg_children.push_back( sg_attributes );
					sg_attributes.clear();
				}
             | GDB_CHILD '=' '{' child_attributes '}' {sg_children.push_back( sg_attributes ); sg_attributes.clear(); } ',' children
			 ;

child_attributes :  child_key '=' GDB_STRING { sg_attributes[$1] = $3; }
				 |  child_key '=' GDB_STRING { sg_attributes[$1] = $3; } ',' child_attributes
				 ;

child_key: GDB_NAME       {$$ = $1;}
		 | GDB_NUMCHILD   {$$ = $1;}
		 | GDB_TYPE       {$$ = $1;}
		 | GDB_VALUE      {$$ = $1;}
		 | GDB_ADDR       {$$ = $1;}
		 | GDB_IDENTIFIER {$$ = $1;}
		 ;
%%
void cleanup()
{
	sg_attributes.clear();
	sg_children.clear();
	sg_locals.clear();
}

void gdbParseListChildren( const std::string &in, std::vector<std::map<std::string, std::string> > &children)
{
	cleanup();

	setGdbLexerInput(in, true, false);
	gdb_result_parse();

	children = sg_children;
	gdb_result_lex_clean();
}

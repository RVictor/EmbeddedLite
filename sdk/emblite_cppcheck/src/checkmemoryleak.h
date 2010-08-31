/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



//---------------------------------------------------------------------------
#ifndef checkmemoryleakH
#define checkmemoryleakH
//---------------------------------------------------------------------------

/// @addtogroup Checks
/// @{


/**
 * Check for memory leaks
 *
 * The checking is split up into two specialized classes.
 * - CheckMemoryLeakInFunction can detect when a function variable is allocated but not deallocated properly.
 * - CheckMemoryLeakInClass can detect when a class variable is allocated but not deallocated properly.
 */

#include "check.h"

#include <list>
#include <string>
#include <vector>

class Token;

/** @brief Base class for memory leaks checking */
class CheckMemoryLeak
{
private:
    const Tokenizer * const tokenizer;
    ErrorLogger * const errorLogger;

    /** Disable the default constructors */
    CheckMemoryLeak();

    /** Disable the default constructors */
    CheckMemoryLeak(const CheckMemoryLeak &);

    /** disable assignment operator */
    void operator=(const CheckMemoryLeak &);

    /**
     * Report error. Similar with the function Check::reportError
     * @param location the token where the error occurs
     * @param severity the severity of the bug
     * @param id type of message
     * @param msg text
     */
    void reportErr(const Token *location, Severity::e severity, const std::string &id, const std::string &msg) const;

    /**
     * Report error. Similar with the function Check::reportError
     * @param callstack callstack of error
     * @param severity the severity of the bug
     * @param id type of message
     * @param msg text
     */
    void reportErr(const std::list<const Token *> &callstack, Severity::e severity, const std::string &id, const std::string &msg) const;

public:
    CheckMemoryLeak(const Tokenizer *t, ErrorLogger *e)
            : tokenizer(t), errorLogger(e)
    {

    }

    /** What type of allocation are used.. the "Many" means that several types of allocation and deallocation are used */
    enum AllocType { No, Malloc, gMalloc, New, NewArray, File, Fd, Pipe, Dir, Many };

    void memoryLeak(const Token *tok, const std::string &varname, AllocType alloctype, bool all);

    /**
     * Get type of deallocation at given position
     * @param tok position
     * @param varnames variable names
     * @return type of deallocation
     */
    AllocType getDeallocationType(const Token *tok, const char *varnames[]) const;

    /**
     * Get type of deallocation at given position
     * @param tok position
     * @param varid variable id
     * @return type of deallocation
     */
    AllocType getDeallocationType(const Token *tok, unsigned int varid) const;

    /**
     * Get type of allocation at given position
     */
    AllocType getAllocationType(const Token *tok2, unsigned int varid) const;

    /**
     * Get type of reallocation at given position
     */
    AllocType getReallocationType(const Token *tok2, unsigned int varid) const;

    bool isclass(const Tokenizer *_tokenizer, const Token *typestr) const;

    void memleakError(const Token *tok, const std::string &varname, bool all);
    void resourceLeakError(const Token *tok, const std::string &varname, bool all);

    void deallocDeallocError(const Token *tok, const std::string &varname);
    void deallocuseError(const Token *tok, const std::string &varname);
    void mismatchSizeError(const Token *tok, const std::string &sz);
    void mismatchAllocDealloc(const std::list<const Token *> &callstack, const std::string &varname);

    /** What type of allocated memory does the given function return? */
    AllocType functionReturnType(const Token *tok) const;
};



/**
 * @brief CheckMemoryLeakInFunction detects when a function variable is allocated but not deallocated properly.
 *
 * The checking is done by looking at each function variable separately. By repeating these 4 steps over and over:
 * -# locate a function variable
 * -# create a simple token list that describes the usage of the function variable.
 * -# simplify the token list.
 * -# finally, check if the simplified token list contain any leaks.
 */

class CheckMemoryLeakInFunction : private Check, private CheckMemoryLeak
{
public:
    /** This constructor is used when registering this class */
    CheckMemoryLeakInFunction() : Check(), CheckMemoryLeak(0, 0)
    { }

    /** This constructor is used when running checks */
    CheckMemoryLeakInFunction(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
            : Check(tokenizer, settings, errorLogger), CheckMemoryLeak(tokenizer, errorLogger)
    { }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
    {
        CheckMemoryLeakInFunction checkMemoryLeak(tokenizer, settings, errorLogger);
        checkMemoryLeak.check();
    }

    void check();

#ifndef _MSC_VER
private:
#endif


    bool matchFunctionsThatReturnArg(const Token *tok, unsigned int varid) const;

    /**
     * Check if there is a "!var" match inside a condition
     * @param tok      first token to match
     * @param varid    variabla id
     * @param endpar   if this is true the "!var" must be followed by ")"
     * @return true if match
     */
    bool notvar(const Token *tok, unsigned int varid, bool endpar = false) const;

    /**
     * Inspect a function call. the call_func and getcode are recursive
     * @param tok          token where the function call occurs
     * @param callstack    callstack
     * @param varid        variable id to check
     * @param alloctype    if memory is allocated, this indicates the type of allocation
     * @param dealloctype  if memory is deallocated, this indicates the type of deallocation
     * @param all          is the code simplified according to the "--all" rules or not?
     * @param sz           not used by call_func - see getcode
     * @return These are the possible return values:
     * - NULL : no significant code
     * - "recursive" : recursive function
     * - "alloc" : the function returns allocated memory
     * - "dealloc" : the function deallocates the variable
     * - "dealloc_"
     * - "use" : the variable is used (unknown usage of the variable => the checking bails out)
     * - "callfunc"
     * - "&use"
     */
    const char * call_func(const Token *tok, std::list<const Token *> callstack, const unsigned int varid, AllocType &alloctype, AllocType &dealloctype, bool &all, unsigned int sz);

    /**
     * Extract a new tokens list that is easier to parse than the "_tokenizer->tokens()", the
     * extracted tokens list describes how the given variable is used.
     * The getcode and call_func are recursive
     * @param tok start parse token
     * @param callstack callstack
     * @param varid variable id
     * @param alloctype keep track of what type of allocation is used
     * @param dealloctype keeps track of what type of deallocation is used
     * @param classmember should be set if the inspected function is a class member
     * @param all has the getcode been simplified according to the "--all" rules?
     * @param sz size of type, used to check for mismatching size of allocation. for example "int *a;" => the sz is "sizeof(int)"
     * @return Newly allocated token array. Caller needs to release reserved
     * memory by calling Tokenizer::deleteTokens(returnValue);
     * Returned tokens:
     * - alloc : the variable is allocated
     * - dealloc : the variable is deallocated
     * - realloc : the variable is reallocated
     * - use : unknown usage -> bail out checking of this execution path
     * - &use : the address of the variable is taken
     * - ::use : calling member function of class
     * - assign : the variable is assigned a new value
     * - if : there is an "if"
     * - if(var) : corresponds with "if ( var != 0 )"
     * - if(!var) : corresponds with "if ( var == 0 )"
     * - ifv : the variable is used in some way in a "if"
     * - loop : corresponds to either a "for" or a "while"
     * - continue : corresponds to "continue"
     * - break : corresponds to "break"
     * - goto : corresponds to a "goto"
     * - return : corresponds to a "return"
     */
    Token *getcode(const Token *tok, std::list<const Token *> callstack, const unsigned int varid, AllocType &alloctype, AllocType &dealloctype, bool classmember, bool &all, unsigned int sz);

    /**
     * Simplify code e.g. by replacing empty "{ }" with ";"
     * @param tok first token. The tokens list can be modified.
     * @param all is the code simplified according to the "--all" rules or not
     */
    void simplifycode(Token *tok, bool &all);

    static const Token *findleak(const Token *tokens, bool all);

    /**
     * Checking the variable varname
     * @param Tok1 start token
     * @param varname name of variable (for error messages)
     * @param varid variable id
     * @param classmember is the scope inside a class member function
     * @param sz size of type.. if the variable is a "int *" then sz should be "sizeof(int)"
     */
    void checkScope(const Token *Tok1, const std::string &varname, unsigned int varid, bool classmember, unsigned int sz);

    void getErrorMessages()
    {
        memleakError(0, "varname", false);
        resourceLeakError(0, "varname", false);

        deallocDeallocError(0, "varname");
        deallocuseError(0, "varname");
        mismatchSizeError(0, "sz");
        std::list<const Token *> callstack;
        mismatchAllocDealloc(callstack, "varname");
    }

    std::string name() const
    {
        return "Memory leaks (function variables)";
    }

    std::string classInfo() const
    {
        return "Is there any allocated memory when a function goes out of scope";
    }

    void parse_noreturn();
    std::set<std::string> noreturn;
};



/**
 * @brief %Check class variables, variables that are allocated in the constructor should be deallocated in the destructor
 */

class CheckMemoryLeakInClass : private Check, private CheckMemoryLeak
{
public:
    CheckMemoryLeakInClass() : Check(), CheckMemoryLeak(0, 0)
    { }

    CheckMemoryLeakInClass(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
            : Check(tokenizer, settings, errorLogger), CheckMemoryLeak(tokenizer, errorLogger)
    { }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
    {
        CheckMemoryLeakInClass checkMemoryLeak(tokenizer, settings, errorLogger);
        checkMemoryLeak.check();
    }

    void check();

private:
    void parseClass(const Token *tok1, std::vector<const char *> &classname);
    void variable(const char classname[], const Token *tokVarname);

    void getErrorMessages()
    { }

    std::string name() const
    {
        return "Memory leaks (class variables)";
    }

    std::string classInfo() const
    {
        return "If the constructor allocate memory then the destructor must deallocate it.";
    }
};



/** @brief detect simple memory leaks for struct members */

class CheckMemoryLeakStructMember : private Check, private CheckMemoryLeak
{
public:
    CheckMemoryLeakStructMember() : Check(), CheckMemoryLeak(0, 0)
    { }

    CheckMemoryLeakStructMember(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
            : Check(tokenizer, settings, errorLogger), CheckMemoryLeak(tokenizer, errorLogger)
    { }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
    {
        CheckMemoryLeakStructMember checkMemoryLeak(tokenizer, settings, errorLogger);
        checkMemoryLeak.check();
    }

    void check();

private:

    void getErrorMessages()
    { }

    std::string name() const
    {
        return "Memory leaks (struct members)";
    }

    std::string classInfo() const
    {
        return "Don't forget to deallocate struct members";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif

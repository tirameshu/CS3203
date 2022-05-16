#pragma once

#include <unordered_set>
#include <unordered_map>
#include "StatementTypeEnum.h"

/*
    The UsesStore is a PKB component that stores information between statements(UsesS) or
    procedures(UsesP) and variables. This component is responsible for storing the information of
    the Uses design abstraction. It offers the neccessary setter methods for populating its data
    structures, query methods and getter methods for accessing neccessary information.
*/

class UsesStore {
private:
    //UsesS
    // A map from statement number to a set of variables used by the statement
    static std::unordered_map<int, std::unordered_set<std::string>> stmtUsesMap; 

    // A map from variable to set of statement numbers that uses it
    static std::unordered_map<std::string, std::unordered_set<int>> varUsedMap;

    // A list of all used variables in the program
    static std::unordered_set<std::string> allUsedVarList; 

    // A list of all used variables by print statements
    static std::unordered_set<std::string> allPrintUsedVarList; 

    // A list of all assign statements using a variable
    static std::unordered_set<int> allUsesAssignStmts; 

    // A list of all print statements stmtNo (a print statement is guaranteed to use, thus it is stored in this component)
    static std::unordered_set<int> printStmtList;

    // A list of all if statements using a variable
    static std::unordered_set<int> allUsesIfStmts; 

    // A list of all while statements using a variable
    static std::unordered_set<int> allUsesWhileStmts; 

    // Print statement mappings
    // A map of print statement to the variable used by it
    static std::unordered_map<int, std::string> printStmtToVarMap;

    // A map of variables used to a list of print statements using it
    static std::unordered_map < std::string, std::unordered_set<int>> varToPrintStmtMap; 
    
    //UsesP
    // A map of all procedure name to variables used in the procedure
    static std::unordered_map<std::string, std::unordered_set<std::string>> procUsesMap;

    // A map of all variables to the procedures using them
    static std::unordered_map<std::string, std::unordered_set<std::string>> varUsesPMap;

    // A list of all procedure names that uses a variable
    static std::unordered_set<std::string> allUsesProc;

    // A list of all call statements that uses a variable indirectly
    static std::unordered_set<int> allUsesCallStmts; 

    // The total number of used variables across all statements, used for Query Optimization purposes
    static int totalVarsUsedForStmts; 

    // The total number of used variables across all print statements, used for Query Optimization purposes
    static int totalVarsUsedForPrintStmts; 

    // The total number of used variables across all assign statements, used for Query Optimization purposes
    static int totalVarsUsedForAssignStmts;

    // The total number of used variables across all if statements, used for Query Optimization purposes
    static int totalVarsUsedForIfStmts; 

    // The total number of used variables across all while statements, used for Query Optimization purposes
    static int totalVarsUsedForWhileStmts;

    // The total number of used variables across all call statements, used for Query Optimization purposes
    static int totalVarsUsedForCallStmts;

    // The total number of used variables across all procedures, used for Query Optimization purposes
    static int totalVarsUsedForProcs; 

public:
    UsesStore();

    // clears all data structures
    static void clear();

    // ============ SETTER METHODS ==============
    // add uses method for Parser to use on assign/print stmts
    static bool addUses(int stmtNo, int stmtType, std::unordered_set<std::string> listOfUsedVars);

    // add uses method for DesignExtractor to add if/while stmt uses transitive property
    static bool addUsesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar);

    // add assign stmts that uses variable(s)
    static bool addAssignStmt(int stmtNo);

    // add print stmts
    static bool addPrintStmt(int stmtNo);

    // adds a if stmt that uses variable(s)
    static bool addIfUses(int stmtNo);

    //adds a while stmt that uses variable(s)
    static bool addWhileUses(int stmtNo);   

    //adds a call stmt that uses variable(s)
    static bool addCallUses(int stmtNo);

    //adds a UsesP relationship
    //called by both parser and DE
    //parser calls to populate direct proc used variables
    //de calls to populate used variables as a result of call stmts
    static bool addProcUses(std::string procName, std::unordered_set<std::string> listOfVars);

    // ============ QUERY METHODS ==============
    //check if var is used by stmt
    static bool isVarUsedInStmt(int stmtNo, std::string var);

    //check if var is used by proc
    static bool isVarUsedInProc(std::string procName, std::string var);

    //check if var is used by any stmt
    static bool isVarUsed(std::string var);

    //check if stmt uses any var
    static bool isStmtUses(int stmtNo);

    //check if proc uses any var
    static bool procUses(std::string procName);

    //check if there exists any if stmt that has a uses relationship
    static bool hasIfUses();

    //check if there exists any while stmt that has a uses relationship
    static bool hasWhileUses();

    //check if there exists any assign stmt that has a uses relationship
    static bool hasAssignUses();

    //check if there exists any proc that has a usesP relationship
    static bool hasProcUses();

    //check if there exists any uses relation
    static bool hasUses();


    // ============ GETTER METHODS ==============
    // gets a map of statements to corresponding uses relationships
    static std::unordered_map<int, std::unordered_set<std::string>> getStmtUsesMap(); 

    // gets a map of variables used to the corresponding stmtNos
    static std::unordered_map<std::string, std::unordered_set<int>> getVarUsedMap();

    // gets a map of proc names to variables used
    static std::unordered_map<std::string, std::unordered_set<std::string>> getProcUsesMap();

    // gets a list of all variables used
    static std::unordered_set<std::string> getAllUsedVarList(); 

    // gets a list of all variables used by print statements
    static std::unordered_set<std::string> getAllPrintUsedVarList();

    // gets a list of all if statements that uses variable(s)
    static std::unordered_set<int> getAllUsesIfStmts(); 

    // gets a list of all while statements that uses variable(s)
    static std::unordered_set<int> getAllUsesWhileStmts();

    // gets a list of all assign statements that uses variable(s)
    static std::unordered_set<int> getAllUsesAssignStmts(); 

    // gets a list of all print statements
    static std::unordered_set<int> getPrintStmtList(); 

    //gets a list of all procs with uses relationship
    static std::unordered_set<std::string> getAllUsesProcs();

    //gets list of vars used in stmtNo 
    static std::unordered_set<std::string> getUsedVarsInStmt(int stmtNo);

    //gets list of stmts using var 
    static std::unordered_set<int> getStmtsUsingVar(std::string var);

    //gets a list of vars used in proc
    static std::unordered_set<std::string> getUsedVarsInProc(std::string procName);

    //gets a list of procs using var
    static std::unordered_set<std::string> getProcsUsingVar(std::string var);

    //get variables used by print statements
    static std::unordered_set<std::string> getPrintVars();

    //get variables used by while statements
    static std::unordered_set<std::string> getVarsUsedInWhile();

    //get variables used by if statements
    static std::unordered_set<std::string> getVarsUsedInIf();

    //get variables used by assignment statements
    static std::unordered_set<std::string> getVarsUsedInAssign(); 
    
    //get all call stmts that uses var
    static std::unordered_set<int> getAllUsesCallStmts();

    //get variables used by call statements
    static std::unordered_set<std::string> getVarsUsedInCall();

    //get printStmtToVarMap
    static std::unordered_map<int, std::string> getPrintStmtToVarMap();

    //get varToPrintStmtMap
    static std::unordered_map<std::string, std::unordered_set<int>> getVarToPrintStmtMap();

    // gets the total number of used variables for stmts
    static int getTotalVarsUsedForStmts();

    // gets the total number of used variables for print stmts
    static int getTotalVarsUsedForPrintStmts();

    // gets the total number of used variables for assign stmts
    static int getTotalVarsUsedForAssignStmts();

    // gets the total number of used variables for if stmts
    static int getTotalVarsUsedForIfStmts();

    // gets the total number of used variables for while stmts
    static int getTotalVarsUsedForWhileStmts();

    // gets the total number of used variables for call stmts
    static int getTotalVarsUsedForCallStmts();

    // gets the total number of used variables for procedures
    static int getTotalVarsUsedForProcs();
};

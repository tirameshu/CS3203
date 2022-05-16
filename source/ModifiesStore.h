#pragma once

#include <unordered_set>
#include <unordered_map>
#include "StatementTypeEnum.h"

/*
    The ModifiesStore is a PKB component that stores information between statements(ModifiesS) or 
    procedures(ModifiesP) and variables. This component is responsible for storing the information of
    the Modifies design abstraction. It offers the neccessary setter methods for populating its data 
    structures, query methods and getter methods for accessing neccessary information.
*/

class ModifiesStore {
private:
    //ModifiesS
    // A map from statement number to a set of variables modified by the statement
    static std::unordered_map<int, std::unordered_set<std::string>> stmtModifiesMap; 

    // A map from variable to set of statement numbers that modifies it
    static std::unordered_map<std::string, std::unordered_set<int>> varModifiedMap;

    // A list of all modified variables in the program
    static std::unordered_set<std::string> allModifiedVarList; 

    // A list of all modified variables by read statements
    static std::unordered_set<std::string> allReadModifiedVarList;

    // A list of all modifying statements stmtNo (contains all assign stmt, read stmt, and if/while stmt that has modifies)
    static std::unordered_set<int> modifyingStmtList; 

    // A list of all read statements stmtNo (a read statement is guaranteed to modify, thus it is stored in this component)
    static std::unordered_set<int> readStmtList; 

    // A list of all if modifying statements stmtNo
    static std::unordered_set<int> modifyingIfStmtList; 

    // A list of all while modifying statements stmtNo
    static std::unordered_set<int> modifyingWhileStmtList; 
    
    // Read statement mappings
     // A map of read statement to the variable modified by it
    static std::unordered_map<int, std::string> readStmtToVarMap;

    // A map of variables modified to a list of read statements modifying it
    static std::unordered_map < std::string, std::unordered_set<int>> varToReadStmtMap; 

    //ModifiesP
    // A map of all procedure name to variables modified in the procedure
    static std::unordered_map<std::string, std::unordered_set<std::string>> procModifiesMap; 

    // A map of all variables to the procedures modifiying them
    static std::unordered_map<std::string, std::unordered_set<std::string>> varModifiedProcMap; 

    // A list of all procedure names that modifies a variable
    static std::unordered_set<std::string> allModifiyingProcs; 

    // A list of all call statements that modifies a variable indirectly
    static std::unordered_set<int> modifyingCallStmtList; 

    // The total number of modified variables across all statements, used for Query Optimization purposes
    static int totalVarsModifiedForStmts; 

    // The total number of modified variables across all read statements, used for Query Optimization purposes
    static int totalVarsModifiedForReadStmts; 

    // The total number of modified variables across all assign statements, used for Query Optimization purposes
    static int totalVarsModifiedForAssignStmts;

    // The total number of modified variables across all if statements, used for Query Optimization purposes
    static int totalVarsModifiedForIfStmts; 

    // The total number of modified variables across all while statements, used for Query Optimization purposes
    static int totalVarsModifiedForWhileStmts; 

    // The total number of modified variables across all call statements, used for Query Optimization purposes
    static int totalVarsModifiedForCallStmts;

    // The total number of modified variables across all procedures, used for Query Optimization purposes
    static int totalVarsModifiedForProcs;

public: 
    ModifiesStore();

    // clears all data structures
    static void clear();
       
    // ============ SETTER METHODS ==============
    // add modifies method for Parser to use on assign/read stmts
    static bool addModifies(int stmtNo, int stmtType, std::string var);

    // add modifies method for DesignExtractor to add if/while stmt modifies transitive property
    static bool addModifiesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar);

    // adds a if stmt that modifies a var to modifyingIfStmtList
    static bool addModifyingIfStmt(int stmtNo);

    // adds a while stmt that modifies a var to modifyingWhileStmtList
    static bool addModifyingWhileStmt(int stmtNo);

    // add read stmt to readStmtList
    static bool addReadStmt(int stmtNo);

    //add call stmt that modifies a var to modifyingCallStmtList
    static bool addModifyingCallStmt(int stmtNo);

    //for modifiesP
    //add modified var to proc
    //called by parser and de
    static bool addModifiedVarToProc(std::string procName, std::unordered_set<std::string> listOfVars);

    // ============ QUERY METHODS ==============
    // check if var is modified by stmt
    static bool isVarModifiedInStmt(std::string var, int stmtNo);

    //for modifiesP
    //check if var is modified in proc
    static bool isVarModifiedInProc(std::string var, std::string procName);

    // check if var is modified by any stmt
    static bool isVarModified(std::string var);

    // check if stmt modifies any var
    static bool isStmtModifies(int stmtNo);

    //for modifiesP
    //check if proc modifies any var
    static bool procHasModifies(std::string procName);

    // check if there exists any If stmt that modifies
    static bool hasIfModifies();

    // check if there exists any While stmt that modifies
    static bool hasWhileModifies();

    //for modifiesP
    //checks if there are any ModifiesP relationship in the program
    static bool hasModifiesP();

    //check if there exists any modifies relation
    static bool hasModifies();

    // ============ GETTER METHODS ==============
    // gets a map of statement to corresponding modifies relationship
    static std::unordered_map<int, std::unordered_set<std::string>> getStmtModifiesMap();

    // gets a map of variables modified to the corresponding stmtNo
    static std::unordered_map<std::string, std::unordered_set<int>> getVarModifiedMap();

    // gets a list of all variables modified
    static std::unordered_set<std::string> getAllModifiedVars();

    // gets a list of all variables modified by read statements
    static std::unordered_set<std::string> getAllReadModifiedVars();

    // gets a list of all statement that modifies a variable
    static std::unordered_set<int> getModifyingStmtList();

    // gets a list of all read statement
    static std::unordered_set<int> getReadStmtList();

    // gets a list of all if statements that modifies
    static std::unordered_set<int> getModifyingIfStmtList();

    // gets a list of all while statements that modifies
    static std::unordered_set<int> getModifyingWhileStmtsList();

    //for modifiesP
    //gets a list of proc names that modifies
    static std::unordered_set<std::string> getModifiyingProcs();

    // gets list of stmts that modified var 
    static std::unordered_set<int> getStmtsModifyingVar(std::string var);

    // gets list of vars modified in a stmt 
    static std::unordered_set<std::string> getModifiedVarsInStmt(int stmtNo);

    //for modifiesP
    //gets list of vars modified in proc p
    static std::unordered_set<std::string> getModifiedVarsInProc(std::string p);

    //for modifiesP
    //gets list of proc that modified var 
    static std::unordered_set<std::string> getProcModifyingVar(std::string var);

    // gets list of var modified in read statements 
    static std::unordered_set<std::string> getReadVars();

    // gets list of var modified in while
    static std::unordered_set<std::string> getVarsModifiedInWhile();

    // gets list of var modified in if
    static std::unordered_set<std::string> getVarsModifiedInIf();

    // get list of call stmts that modified var
    static std::unordered_set<int> getModifyingCallStmtList();

    //get readStmtToVarMap
    static std::unordered_map<int, std::string> getReadStmtToVarMap();

    //get varToReadStmtMap
    static std::unordered_map<std::string, std::unordered_set<int>> getVarToReadStmtMap();

    // gets the total number of modified variables for stmts
    static int getTotalVarsModifiedForStmts();

    // gets the total number of modified variables for read stmts
    static int getTotalVarsModifiedForReadStmts();

    // gets the total number of modified variables for assign stmts
    static int getTotalVarsModifiedForAssignStmts();

    // gets the total number of modified variables for if stmts
    static int getTotalVarsModifiedForIfStmts();

    // gets the total number of modified variables for while stmts
    static int getTotalVarsModifiedForWhileStmts();

    // gets the total number of modified variables for call stmts
    static int getTotalVarsModifiedForCallStmts();

    // gets the total number of modified variables for procedures
    static int getTotalVarsModifiedForProcs();
};


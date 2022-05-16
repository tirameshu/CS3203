#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Statement.h"
#include "StatementTypeEnum.h"
#include "FollowsStore.h"
#include "ParentStore.h"
#include "ModifiesStore.h"
#include "UsesStore.h"
#include "CallStore.h"
#include "NextStore.h"
#include "NextBipStore.h"
#include "AffectsStore.h"
#include "AffectsBipStore.h"

#include "AssignStore.h"
#include "IfStore.h"
#include "WhileStore.h"

/*
	The PKB is the main storage component of the entire Static Program Analyzer (SPA). It is responsible for
	the storage of key information regarding the SIMPLE source code parsed. It stores information on all 
	design entities within the program, all design abstraction relationships between design entities, and also
	essential information regarding pattern queries for formulating the QueryEvaluator's result. 

	The PKB has its own general set of data structures. Each design abstraction and pattern query related information
	are seperated into their individual Store components, where information related to those relationships are stored 
	respectively. The PKB's abstract API provides other components the neccessary methods to populate its data structure, 
	query and get neccessary information required by the other components. 

	The Store components used in the PKB are:
	- FollowsStore
	- ParentStore
	- UsesStore
	- ModifiesStore
	- CallStore
	- NextStore
	- AffectsStore
	- NextBipStore
	- AffectsBipStore
	- AssignStore
	- IfStore
	- WhileStore
*/

class PKB {
private:
	// A map from statement number to their respective Statement object, which stores information such as statement type and statement number
	static std::unordered_map<int, Statement> programStmtMap; 

	// A map from procedure name to a list of statement numbers (a vector is used to store the statements in the procedure's stmtList in order)
	static std::unordered_map<std::string, std::vector<int>> procedureStmtMap; 

	// A map from statement number to the enclosing procedure name
	static std::unordered_map<int, std::string> stmtProcedureMap; 

	// A map from if statement numbers to a list of statement numbers (a vector is used to store the statements in the if's stmtList in order)
	static std::unordered_map<int, std::vector<int>> ifStmtMap;

	// A map from if statement numbers to a list of statement numbers (a vector is used to store the statements in the while's stmtList in order)
	static std::unordered_map<int, std::vector<int>> whileStmtMap; 

	// A map from constants to a list of statement numbers which uses the constant
	static std::unordered_map<std::string, std::unordered_set<int>> constantStmtMap;

	// A map from statement numbers to a list of constants used in the statement
	static std::unordered_map<int, std::unordered_set<std::string>> stmtConstantMap;

	// A list of all statement numbers in the program
	static std::unordered_set<int> statementList; 

	// A list of all procedure names in the program
	static std::unordered_set<std::string> procedureList; 

	// A list of all variables in the program
	static std::unordered_set<std::string> variableList; 

	// A list of all constants in the program
	static std::unordered_set<std::string> constantList;

	// A list of all if statement numbers in the program
	static std::unordered_set<int> ifStmtList; 

	// A list of all while statement numbers in the program
	static std::unordered_set<int> whileStmtList;

	// A list of all call statement numbers in the program
	static std::unordered_set<int> callStmtList;

	// adds an assign statement to assignStmtList
	static bool addAssignStmt(int stmt);

	// adds a print statement to printStmtList
	static bool addPrintStmt(int stmt);

	// adds a read statement to readStmtList
	static bool addReadStmt(int stmt);

	// adds an if statement and add to ifStmtList
	static bool addIfStmt(int stmt);

	// adds a while statement whileStmtList
	static bool addWhileStmt(int stmt);

	// adds a call statement callStmtList
	static bool addCallStmt(int stmt);

public:
	PKB();

	// clears all data structures
	static void clear();

	/* ===============================================================================================================
		GENERAL PKB METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// adds a statement to the respective stmtList, maps and statementList
	 static bool addStatement(std::string proc, Statement stmt); 

	// adds a variable to variableList
	 static bool addVariable(std::string var); 

	// adds a procedure to procedureList
	 static bool addProcedure(std::string proc); 

	// adds a constant to constantList
	 static bool addConstant(int stmtNo, std::string const);

	// adds a statement to ifStmtList (statements in both 'then' and 'else' stmtList)
	 static bool addIfContainerStmtList(int ifStmt, int stmt);

	//  adds a statement to whileStmtList
	 static bool addWhileContainerStmtList(int whileStmt, int stmt);

	// ============ GETTER METHODS ==============
	// get statement type of statement
	 static int getTypeOfStmt(int stmt);

	// gets the programStmtMap
	 static std::unordered_map<int, Statement> getProgramStmtMap();

	// gets the procedureStmtMap
	 static std::unordered_map<std::string, std::vector<int>> getProcedureStmtMap();

	// gets the stmtProcedureMap
	 static std::unordered_map<int, std::string> getStmtProcedureMap();

	// gets the ifStmtMap
	 static std::unordered_map<int, std::vector<int>> getIfStmtMap();

	// gets the whileStmtMap
	 static std::unordered_map<int, std::vector<int>> getWhileStmtMap();

	// gets the constantStmtMap
	 static std::unordered_map<std::string, std::unordered_set<int>> getConstantStmtMap();
	
	// gets the stmtConstantMap
	 static std::unordered_map<int, std::unordered_set<std::string>> getStmtConstantMap();

	// gets the procedureList
	 static std::unordered_set<std::string> getProcedureList();

	// gets the variableList
	 static std::unordered_set<std::string> getVariableList();

	// gets the constantList
	 static std::unordered_set<std::string> getConstantList();

	// gets the stmtList
	 static std::unordered_set<int> getStmtList();

	// gets the assignStmtList
	 static std::unordered_set<int> getAssignStmtList(); 

	// gets the printStmtList
	 static std::unordered_set<int> getPrintStmtList(); 

	// gets the readStmtList
	 static std::unordered_set<int> getReadStmtList(); 

	// gets the ifStmtList
	 static std::unordered_set<int> getIfStmtList(); 

	// gets the whileStmtList
	 static std::unordered_set<int> getWhileStmtList(); 

	 // gets the callStmtList
	 static std::unordered_set<int> getCallStmtList();

	/* ===============================================================================================================
		FOLLOWSSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// adds and updates 'followed' and 'follower' properties of stmt1 and stmt2 respectively
	static bool addFollows(int stmt1, int stmt2);

	// adds and updates 'listOfFollowed' property of stmt
	static bool addAllFollowed(int stmt, std::unordered_set<int> listOfFollowed);

	// adds and updates 'listOfFollower' property of stmt
	static bool addAllFollower(int stmt, std::unordered_set<int> listOfFollower);

	// ============ QUERY METHODS ==============
	// checks if a statement has a followed statement associated
	static bool stmtHasFollowed(int stmt);

	// checks if a statement has a follower statement associated
	static bool stmtHasFollower(int stmt);

	// checks if the program has any followed statement
	static bool programHasFollowedStmt();

	// checks if the program has any follower statement
	static bool programHasFollowerStmt();

	// ============ GETTER METHODS ==============
	// gets the 'followed statement' of a stmt
	static int getFollowedOfStmt(int stmt);

	// gets the 'follower statement' of a stmt
	static int getFollowerOfStmt(int stmt);

	// gets the 'listOfFollowed' of a stmt
	static std::unordered_set<int> getFollowedListOfStmt(int stmt);

	// gets the 'listOfFollower' of a stmt
	static std::unordered_set<int> getFollowerListOfStmt(int stmt);

	// gets the list of all Follows pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllFollowsPair();

	// gets the list of all FollowsStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllFollowsStarPair();

	// gets the list of all statements that are followed
	static std::unordered_set<int> getAllFollowedStmt();

	// gets the list of all statements that are follower
	static std::unordered_set<int> getAllFollowerStmt();

	/* ===============================================================================================================
		PARENTSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// adds and updates 'stmtParent' and 'listOfChildren' properties of parentStmt and individual childStmt
	static bool addParent(int parent, std::unordered_set<int> childrenList);

	// adds and updates 'listOfAncestors' property of stmt
	static bool addAncestors(int stmt, std::unordered_set<int> ancestorList);

	// adds and updates 'listOfDescendants' property of stmt
	static bool addDescendants(int stmt, std::unordered_set<int> descendantList);

	// ============ QUERY METHODS ==============
	// checks if a statement has a parent statement associated
	static bool stmtHasParent(int stmt);

	// checks if a statement has any children statements associated
	static bool stmtHasChildren(int stmt);

	// checks if the program has any parent statement
	static bool programHasParentStmt();

	// checks if the program has any child statement
	static bool programHasChildStmt();

	// ============ GETTER METHODS ==============
	// gets the 'parent statement' of a stmt
	static int getParentOfStmt(int stmt);

	// gets the 'listOfChildren' of a stmt
	static std::unordered_set<int> getChildrenListOfStmt(int stmt);

	// gets the 'listOfAncestors' of a stmt
	static std::unordered_set<int> getAncestorsListOfStmt(int stmt);

	// gets the 'listOfDescendants' of a stmt
	static std::unordered_set<int> getDescendantsListOfStmt(int stmt);

	// gets the list of all Parent pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllParentPair();

	// gets the list of all ParentStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllParentStarPair();

	// gets the list of all statements that are parent
	static std::unordered_set<int> getAllParentStmt();

	// gets the list of all statements that are child
	static std::unordered_set<int> getAllChildStmt();

	/* ===============================================================================================================
		MODIFIESSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// add modifies method for Parser to use on assign/read stmts
	static bool addModifies(int stmtNo, int stmtType, std::string var);

	// add modifies method for DesignExtractor add if/while stmt modifies transitive property
	static bool addModifiesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar);
	
	//for modifiesP
	//used by both parser and DE
	//add modifiesP 
	static bool addModifiedVarToProc(std::string procName, std::unordered_set<std::string> listOfVars);

	// ============ QUERY METHODS ==============
	// check if var is modified by stmt
	static bool isVarModifiedInStmt(std::string var, int stmtNo);

	// check if var is modified by any stmt
	static bool isVarModified(std::string var);

	// check if stmt modifies any var
	static bool isStmtModifies(int stmtNo);

	//for modifiesP
	//check if a var is modified in proc
	static bool isVarModifiedInProc(std::string var, std::string procName);

	//for modifiesP
	//check if proc modifies any var
	static bool procHasModifies(std::string procName);

	// check if there exists any If stmt that modifies
	static bool hasIfModifies();

	// check if there exists any While stmt that modifies
	static bool hasWhileModifies();

	//for modifiesP
	//check if there exists any procedure that modifies
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

	// gets a list of all if statements that modifies
	static std::unordered_set<int> getModifyingIfStmtList();

	// gets a list of all while statements that modifies
	static std::unordered_set<int> getModifyingWhileStmtsList();

	//for modifiesP
	// gets a list of a list procedures that modifies
	static std::unordered_set<std::string> getModifiyingProcs();

	// gets list of stmts that modified var 
	static std::unordered_set<int> getStmtsModifyingVar(std::string var);

	//for modifiesP
	// gets list of variables modified in proc p
	static std::unordered_set<std::string> getModifiedVarsInProc(std::string p);

	// gets list of vars modified in a stmt 
	static std::unordered_set<std::string> getModifiedVarsInStmt(int stmtNo);

	//for modifiesP
	//get list of procs modifiying a var
	static std::unordered_set<std::string> getProcModifyingVar(std::string var);

	// gets list of var modified in read statements 
	static std::unordered_set<std::string> getReadVars();

	// gets list of var modified in while
	static std::unordered_set<std::string> getVarsModifiedInWhile();

	// gets list of var modified in if
	static std::unordered_set<std::string> getVarsModifiedInIf();

	// get list of call stms that modified var
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

	/* ===============================================================================================================
		USESSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// add uses method for Parser to use on assign/print stmts
	static bool addUses(int stmtNo, int stmtType, std::unordered_set<std::string> listOfUsedVars);

	// add uses method for DesignExtractor to add if/while stmt uses transitive property
	static bool addUsesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar);

	//for usesP
	//used by both DE and parser
	//add usesP 
	static bool addProcUses(std::string procName, std::unordered_set<std::string> listOfVars);

	// ============ QUERY METHODS ==============
	//check if var is used by stmt
	static bool isVarUsedInStmt(int stmtNo, std::string var);

	//for usesP
	//check if var is used in proc
	static bool isVarUsedInProc(std::string procName, std::string var);

	//check if var is used by any stmt
	static bool isVarUsed(std::string var);

	//check if stmt uses any var
	static bool isStmtUses(int stmtNo);

	//for usesP
	//check if proc uses any var
	static bool procUses(std::string procName);

	//check if there exists any if stmt that has a uses relationship
	static bool hasIfUses();

	//check if there exists any while stmt that has a uses relationship
	static bool hasWhileUses();

	//check if there exists any assign stmt that has a uses relationship
	static bool hasAssignUses();

	//for usesP
	//check if there exists any proc that has a usesP relationship
	static bool hasProcUses();

	//check if there exists any uses relation
	static bool hasUses();

	// ============ GETTER METHODS ==============
	// gets a map of statements to corresponding uses relationships
	static std::unordered_map<int, std::unordered_set<std::string>> getStmtUsesMap();

	// gets a map of variables used to the corresponding stmtNos
	static std::unordered_map<std::string, std::unordered_set<int>> getVarUsedMap();

	//for usesP
	//gets a map of proc name to variables used
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

	//for usesP
	//gets a list of all procs that uses variable(s)
	static std::unordered_set<std::string> getAllUsesProcs();

	//gets list of vars used in stmtNo 
	static std::unordered_set<std::string> getUsedVarsInStmt(int stmtNo);

	//gets list of stmts using var 
	static std::unordered_set<int> getStmtsUsingVar(std::string var);

	//for usesP
	//gets list of vars used in proc
	static std::unordered_set<std::string> getUsedVarsInProc(std::string procName);

	//get variables used by print statements
	static std::unordered_set<std::string> getPrintVars();

	//get variables used by while statements
	static std::unordered_set<std::string> getVarsUsedInWhile();
	
	//for usesP
	//get list of procs using var
	static std::unordered_set<std::string> getProcsUsingVar(std::string var);

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

	/* ===============================================================================================================
		CALLSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============

	//used by parser
	//adds a immediate call relationship to callStore
	static bool addCall(std::string callerP, std::string calleeP, int stmtNo);

	//used by DE
	//adds transitive callers for each callee procedure
	static bool addCallerT(std::string calleeP, std::unordered_set<std::string> callerP);

	//used by DE
	//adds transitive callees for each caller procedure
	static bool addCalleeT(std::string callerP, std::unordered_set<std::string> calleeP);

	// ============ QUERY METHODS ==============
	
	//evaluates Calls(_,p)
	//checks if p has an immediate caller procedure
	static bool hasCaller(std::string p);

	//evaluates Calls(p, _)
	//checks if p has an immediate callee procedure
	static bool hasCallee(std::string p);

	//evaluates Calls(caller, callee)
	//checks if there is a call relationship between caller and callee
	static bool Calls(std::string caller, std::string callee);

	//evalutes Calls*(caller, callee)
	//checks if there is a call* relationship between caller and callee
	static bool CallsTransitive(std::string caller, std::string callee);

	//evaluates Calls(_, _)
	//checks if there exists any call relationship in program
	static bool hasCall();

	// ============ GETTER METHODS ==============
	
	//gets a set of all immediate callee procedures of p
	static std::unordered_set<std::string> getCalleesOfProc(std::string p);

	//gets a set of immediate caller procedures of p
	static std::unordered_set<std::string> getCallersOfProc(std::string p);

	//gets a set of transitive caller procedures of p
	static std::unordered_set<std::string> getTCallersOfProc(std::string p);

	//gets a set of transitive callee procedures of p
	static std::unordered_set<std::string> getTCalleesOfProc(std::string p);

	//gets a set of caller procedures without any callers to themselves
	static std::unordered_set<std::string> getRootProcs();

	//gets a set of callee procedures without any callees to themselves
	static std::unordered_set<std::string> getLeafProcs();

	//gets a map of call stmtNo -> procName called in the stmt
	static std::unordered_map<int, std::string> getCallerStmtToProcMap();

	//gets a map of procName -> call stmtNo calling it
	static std::unordered_map<std::string, std::unordered_set<int>> getProcToCallerStmtsMap();

	//gets a map of procName -> call stmtNo in the proc
	static std::unordered_map<std::string, std::unordered_set<int>> getProcedureToCallStmtMap();

	//gets a map of call stmtNo -> procName that it belongs in
	static std::unordered_map<int, std::string> getCallStmtToProcedureMap();

	//gets a set of call stmtNo that calls proc
	static std::unordered_set<int> getCallerStmts(std::string proc);

	//gets a set of all call stmtNo found in proc
	static std::unordered_set<int> getAllCallStmtsInProc(std::string proc);

	//get a set of all caller stmtNo
	static std::unordered_set<std::string> getListOfAllCallers();

	//get a set of all callee stmtNo
	static std::unordered_set<std::string> getListOfAllCallees();

	//get all procs involved in calls
	static std::unordered_set<std::string> getAllCallProcs();

	// get all calls pairs
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> getCallsPairList();

	// get all callsStar pairs
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> getCallsStarPairList();

	/* ===============================================================================================================
		NEXTSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// adds and updates 'prevStmts' and 'nextStmts' properties of stmt1 and stmt2 respectively
	static bool addNext(int stmt1, int stmt2);

	// adds and updates 'listOfPrevStarStmt' property of stmt
	static bool addAllPrevious(int stmt, std::unordered_set<int> listOfPrevStarStmt);

	// adds and updates 'listOfNextStarStmt' property of stmt
	static bool addAllNext(int stmt, std::unordered_set<int> listOfNextStarStmt);

	// ============ QUERY METHODS ==============
	// checks if a statement has a previous statement in CFG
	static bool stmtHasPrev(int stmt);

	// checks if a statement has a next statement in CFG
	static bool stmtHasNext(int stmt);

	// checks if the program has any previous statement
	static bool programHasPrevStmt();

	// checks if the program has any next statement
	static bool programHasNextStmt();

	// ============ GETTER METHODS ==============
	// gets the set of 'immediate previous statements' of a stmt
	static std::unordered_set<int> getPrevOfStmt(int stmt);

	// gets the set of 'immediate next statements' of a stmt
	static std::unordered_set<int> getNextOfStmt(int stmt);

	// gets the 'all previous statements' of a stmt
	static std::unordered_set<int> getPrevStarListOfStmt(int stmt);

	// gets the 'all next statements' of a stmt
	static std::unordered_set<int> getNextStarListOfStmt(int stmt);

	// gets the list of all Next pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextPair();

	// gets the list of all NextStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextStarPair();

	// gets the list of all statements that come before a node in CFG
	static std::unordered_set<int> getAllPreviousStmt();

	// gets the list of all statements that come after a node in CFG
	static std::unordered_set<int> getAllNextStmt();

	/* ===============================================================================================================
		NEXTBIPSTORE METHODS
		===============================================================================================================*/
	
	// ============ SETTER METHODS ==============
	// adds and updates 'prevBipStmts' and 'nextBipStmts' properties of stmt1 and stmt2 respectively, caller > 0 if it is an interprocedural edge in the NextBip CFG
	static bool addNextBip(int stmt1, int stmt2, int caller);

	// adds and updates 'listOfPrevBipStarStmt' property of stmt
	static bool addAllPreviousBip(int stmt, std::unordered_set<int> listOfPrevBipStarStmt);

	// adds and updates 'listOfNextBipStarStmt' property of stmt
	static bool addAllNextBip(int stmt, std::unordered_set<int> listOfNextBipStarStmt);

	// ============ QUERY METHODS ==============
	// checks if a statement has a previous bip statement in CFG
	static bool stmtHasPrevBip(int stmt);

	// checks if a statement has a next bip statement in CFG
	static bool stmtHasNextBip(int stmt);

	// checks if the program has any previous bip statement
	static bool programHasPrevBipStmt();

	// checks if the program has any next bip statement
	static bool programHasNextBipStmt();

	// ============ GETTER METHODS ==============
	// gets the set of 'immediate previous bip statements' of a stmt
	static std::unordered_set<int> getPrevBipOfStmt(int stmt);

	// gets the set of 'immediate next bip statements' of a stmt
	static std::unordered_set<int> getNextBipOfStmt(int stmt);

	// gets the 'all previous bip statements' of a stmt
	static std::unordered_set<int> getPrevBipStarListOfStmt(int stmt);

	// gets the 'all next bip statements' of a stmt
	static std::unordered_set<int> getNextBipStarListOfStmt(int stmt);

	// gets the list of incoming edges of a statement from the NextBip CFG
	static std::unordered_set<int> getNextBipListOfIncomingOfStmt(int stmt);

	// gets the list of outgoing edges of a statement from the NextBip CFG
	static std::unordered_set<int> getNextBipListOfOutgoingOfStmt(int stmt);

	// gets the list of all NextBip pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextBipPair();

	// gets the list of all NextBipStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextBipStarPair();

	// gets the list of all statements that come before a node in NextBip CFG
	static std::unordered_set<int> getAllPreviousBipStmt();

	// gets the list of all statements that come after a node in NextBip CFG
	static std::unordered_set<int> getAllNextBipStmt();

	/* ===============================================================================================================
		AFFECTSSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	//  adds and updates the affects properties of an assign statement
	static bool addAffects(int stmt, int affected);

	//  adds and updates the affectedByT list of an assign statement
	static bool addAffectedByT(int stmt, std::unordered_set<int> listOfAffectors);

	//  adds and updates the affectingT list of an assign statement
	static bool addAffectingT(int stmt, std::unordered_set<int> listOfAffecting);

	// ============ QUERY METHODS ==============
	// checks if a specific assign statement is affected by another assign statement
	static bool stmtHasAffectedBy(int stmt);

	// checks if a specific assign statement is affecting another assign statement
	static bool stmtHasAffecting(int stmt);

	// checks if the program has an assign statement being affected
	static bool programHasAffected();

	// checks if the program has an assign statement affecting another assign statement
	static bool programHasAffecting();

	// ============ GETTER METHODS ==============
	// gets the stmtAffectsMap
	static std::unordered_map<int, affectsStruct> getStmtAffectsMap();

	// gets the list of all Affects pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsPairList();

	// gets the list of all AffectsT pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsStarPairList();

	// gets the list of assign statements affected by another assign statement
	static std::unordered_set<int> getAffectedList();

	// gets the list of assign statements affecting another assign statement
	static std::unordered_set<int> getAffectingList();

	// gets the list of previous <declaration stmt> affecting specified stmt
	static std::unordered_set<int> getAffectedByOfStmt(int stmt);

	// gets the list of statements affected by the specified stmt
	static std::unordered_set<int> getAffectingOfStmt(int stmt);

	// gets the list of transitive previous <declaration stmt> affecting the specified stmt
	static std::unordered_set<int> getAffectedByTOfStmt(int stmt);

	// gets the list of statements affected by transitive by the specified stmt
	static std::unordered_set<int> getAffectingTOfStmt(int stmt);

	/* ===============================================================================================================
	AFFECTSBIPSTORE METHODS
	===============================================================================================================*/

	// ============ SETTER METHODS ==============
	//  adds and updates the affectsBip properties of an assign statement
	static bool addAffectsBip(int stmt, int affected);

	//  adds and updates the affectedByT list of an assign statement
	static bool addAffectedBipByT(int stmt, std::unordered_set<int> listOfAffectors);

	//  adds and updates the affectingT list of an assign statement
	static bool addAffectingBipT(int stmt, std::unordered_set<int> listOfAffecting);

	// ============ QUERY METHODS ==============
	// checks if a specific assign statement is affected by another assign statement
	static bool stmtHasAffectedBipBy(int stmt);

	// checks if a specific assign statement is affecting another assign statement
	static bool stmtHasAffectingBip(int stmt);

	// checks if the program has an assign statement being affected
	static bool programHasAffectedBip();

	// checks if the program has an assign statement affecting another assign statement
	static bool programHasAffectingBip();

	// ============ GETTER METHODS ==============
	// gets the stmtAffectsBipMap
	static std::unordered_map<int, affectsBipStruct> getStmtAffectsBipMap();

	// gets the list of all AffectsBip pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsBipPairList();

	// gets the list of all AffectsBipT pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsBipStarPairList();

	// gets the list of assign statements affected by another assign statement
	static std::unordered_set<int> getAffectedBipList();

	// gets the list of assign statements affecting another assign statement
	static std::unordered_set<int> getAffectingBipList();

	// gets the list of previous <declaration stmt> affecting specified stmt
	static std::unordered_set<int> getAffectedBipByOfStmt(int stmt);

	// gets the list of statements affected by the specified stmt
	static std::unordered_set<int> getAffectingBipOfStmt(int stmt);

	// gets the list of transitive previous <declaration stmt> affecting the specified stmt
	static std::unordered_set<int> getAffectedBipByTOfStmt(int stmt);

	// gets the list of statements affected by transitive by the specified stmt
	static std::unordered_set<int> getAffectingBipTOfStmt(int stmt);

	/* ===============================================================================================================
		ASSIGNSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// adds and updates properties of an assign statement with its variable(LHS) and expression(RHS)
	static bool addAssign(int stmt, std::string var, std::string exp);

	// ============ GETTER METHODS ==============
	// gets the 'variable' of an assign stmt
	static std::string getVariableOfStmt(int stmt);

	// gets the 'expression' of an assign stmt
	static std::string getExpressionOfStmt(int stmt);

	// gets the map of pair<exp, var> to its corresponding list of assign statements matched
	static std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> getVarExpStmtList();

	// gets the map of variable to list of expressions
	static std::unordered_map <std::string, std::unordered_set<std::string>> getVarToExpMap();

	// gets the map of variable to list of assign statements matched
	static std::unordered_map<std::string, std::unordered_set<int>> getVarAssignStmtList();

	// gets the map of expression to list of assign statements matched
	static std::unordered_map<std::string, std::unordered_set<int>> getExpAssignStmtList();

	/* ===============================================================================================================
		IFSTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// stores if statement information in IfStore
	static bool addIfUses(int stmt, std::unordered_set<std::string> varUsed);

	// ============ GETTER METHODS ==============
	// gets the ifStmtUsesMap
	static std::unordered_map<int, std::unordered_set<std::string>> getIfStmtUsesMap();

	// gets the varToIfStmtMap
	static std::unordered_map<std::string, std::unordered_set<int>> getVarToIfStmtMap();

	// gets the ifUsedVarList
	static std::unordered_set<std::string> getIfUsedVarList();

	// gets the ifUsesStmtList
	static std::unordered_set<int> getIfUsesStmtList();

	// gets the list of variables used by a particular if statement
	static std::unordered_set<std::string> getVarsUsedByIf(int stmt);

	// gets the list of if statements using a particular variable 
	static std::unordered_set<int> getIfStmtsUsingVar(std::string var);

	// gets the total number of control variables used in if stmts
	static int getTotalIfUsedVars();

	/* ===============================================================================================================
		WHILESTORE METHODS
		===============================================================================================================*/

	// ============ SETTER METHODS ==============
	// stores while statement information in WhileStore
	static bool addWhileUses(int stmt, std::unordered_set<std::string> varUsed);

	// ============ GETTER METHODS ==============
	// gets the whileStmtUsesMap
	static std::unordered_map<int, std::unordered_set<std::string>> getWhileStmtUsesMap();

	// gets the varToWhileStmtMap
	static std::unordered_map<std::string, std::unordered_set<int>> getVarToWhileStmtMap();

	// gets the whileUsedVarList
	static std::unordered_set<std::string> getWhileUsedVarList();

	// gets the whileUsesStmtList
	static std::unordered_set<int> getWhileUsesStmtList();

	// gets the list of variables used by a particular while statement
	static std::unordered_set<std::string> getVarsUsedByWhile(int stmt);

	// gets the list of while statements using a particular variable 
	static std::unordered_set<int> getWhileStmtsUsingVar(std::string var);

	// gets the total number of control variables used in while stmts
	static int getTotalWhileUsedVars();
};

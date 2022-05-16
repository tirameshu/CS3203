#include "PKB.h"

std::unordered_map<int, Statement> PKB::programStmtMap; // map stmtNo -> Statement object, stores all the statement types in numerical order
std::unordered_map<std::string, std::vector<int>> PKB::procedureStmtMap; // map procedureName -> stmtNos' (used vector to keep statement in stmtList in order)
std::unordered_map<int, std::string> PKB::stmtProcedureMap; // map stmtNo -> procedureName
std::unordered_map<int, std::vector<int>> PKB::ifStmtMap; // map ifStmtNo -> stmtNos' (used vector to keep statement in stmtList in order)
std::unordered_map<int, std::vector<int>> PKB::whileStmtMap; // map whileStmtNo -> stmtNos' (used vector to keep statement in stmtList in order)
std::unordered_map<std::string, std::unordered_set<int>> PKB::constantStmtMap; // map constant -> stmtNos'
std::unordered_map<int, std::unordered_set<std::string>> PKB::stmtConstantMap; // map stmtNos' -> list of constants used

std::unordered_set<int> PKB::statementList; // store unique statement numbers
std::unordered_set<std::string> PKB::procedureList; // store unique procedure names
std::unordered_set<std::string> PKB::variableList; // store unique variable names
std::unordered_set<std::string> PKB::constantList; // store unique constant values as string
std::unordered_set<int> PKB::ifStmtList; // stores the stmtNo for all if statements
std::unordered_set<int> PKB::whileStmtList; // stores the stmtNo for all while statements
std::unordered_set<int> PKB::callStmtList; // stores the stmtNo for all call statements

PKB::PKB() {};

void PKB::clear() {
	programStmtMap.clear();
	procedureStmtMap.clear();
	stmtProcedureMap.clear();
	ifStmtMap.clear();
	whileStmtMap.clear();
	constantStmtMap.clear();
	stmtConstantMap.clear();


	statementList.clear();
	procedureList.clear();
	variableList.clear();
	constantList.clear();
	ifStmtList.clear();
	whileStmtList.clear();
	callStmtList.clear();

	FollowsStore::clear();
	ParentStore::clear();
	ModifiesStore::clear();
	UsesStore::clear();
	CallStore::clear();
	NextStore::clear();
	NextBipStore::clear();
	AffectsStore::clear();
	AffectsBipStore::clear();

	AssignStore::clear();
	IfStore::clear();
	WhileStore::clear();
}

/* ===============================================================================================================
	GENERAL PKB METHODS
===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addStatement(std::string proc, Statement stmt) {
	int stmtNo = stmt.getStmtNo();
	int stmtType = stmt.getStmtType();

	// invalid stmtType for iteration 1
	if (stmtType == ELSE || stmtType <= 0 || stmtType > CALL) {
		return false;
	}

	// check for duplicate entires of stmtNo
	if (!statementList.emplace(stmtNo).second) {
		return false;
	}

	programStmtMap.emplace(stmtNo, stmt);

	if (!procedureStmtMap.emplace(proc, std::vector<int>{stmtNo}).second) {
		procedureStmtMap.at(proc).push_back(stmtNo);
	}
	stmtProcedureMap.emplace(stmtNo, proc);

	// add statement to the corresponding typeStmtList
	switch (stmtType) {
	case ASSIGN:
		addAssignStmt(stmtNo);
		break;
	case PRINT:
		addPrintStmt(stmtNo);
		break;
	case READ:
		addReadStmt(stmtNo);
		break;
	case IF:
		addIfStmt(stmtNo);
		break;
	case WHILE:
		addWhileStmt(stmtNo);
		break;
	case CALL:
		addCallStmt(stmtNo);
	}

	return true;
}

bool PKB::addVariable(std::string var) {
	variableList.emplace(var);
	return 1;
}

bool PKB::addProcedure(std::string proc) {
	return procedureList.emplace(proc).second;
}

bool PKB::addConstant(int stmtNo, std::string constant) {
	constantList.emplace(constant);

	if (!constantStmtMap.emplace(constant, std::unordered_set<int>{stmtNo}).second) {
		constantStmtMap.at(constant).emplace(stmtNo);
	}

	if (!stmtConstantMap.emplace(stmtNo, std::unordered_set<std::string>{constant}).second) {
		stmtConstantMap.at(stmtNo).emplace(constant);
	}

	return 1;
}

bool PKB::addIfContainerStmtList(int ifStmt, int stmt) {
	if (!ifStmtMap.emplace(ifStmt, std::vector<int>{stmt}).second) {
		ifStmtMap.at(ifStmt).push_back(stmt);
	}
	return 1;
}

bool PKB::addWhileContainerStmtList(int whileStmt, int stmt) {
	if (!whileStmtMap.emplace(whileStmt, std::vector<int>{stmt}).second) {
		whileStmtMap.at(whileStmt).push_back(stmt);
	}
	return 1;
}


// assignStore stores assignStmtList, not PKB
bool PKB::addAssignStmt(int stmt) {
	return AssignStore::addAssignStmt(stmt);
}

// usesStore stores readStmtList, not PKB
bool PKB::addPrintStmt(int stmt) {
	return UsesStore::addPrintStmt(stmt);
}

// modifiesStore stores readStmtList, not PKB
bool PKB::addReadStmt(int stmt) {
	return ModifiesStore::addReadStmt(stmt);
}

bool PKB::addIfStmt(int stmt) {
	return ifStmtList.emplace(stmt).second;
}

bool PKB::addWhileStmt(int stmt) {
	return whileStmtList.emplace(stmt).second;
}

bool PKB::addCallStmt(int stmt) {
	return callStmtList.emplace(stmt).second;
}


// ============ GETTER METHODS ==============
int PKB::getTypeOfStmt(int stmt) {
	return programStmtMap.at(stmt).getStmtType();
}

std::unordered_map<int, Statement> PKB::getProgramStmtMap() {
	return programStmtMap;
}

std::unordered_map<std::string, std::vector<int>> PKB::getProcedureStmtMap() {
	return procedureStmtMap;
}

std::unordered_map<int, std::string> PKB::getStmtProcedureMap() {
	return stmtProcedureMap;
}

std::unordered_map<int, std::vector<int>> PKB::getIfStmtMap() {
	return ifStmtMap;
}

std::unordered_map<int, std::vector<int>> PKB::getWhileStmtMap() {
	return whileStmtMap;
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getConstantStmtMap() {
	return constantStmtMap;
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getStmtConstantMap() {
	return stmtConstantMap;
}

std::unordered_set<std::string> PKB::getProcedureList() {
	return procedureList;
}

std::unordered_set<std::string> PKB::getVariableList() {
	return variableList;
}

std::unordered_set<std::string> PKB::getConstantList() {
	return constantList;
}

std::unordered_set<int> PKB::getStmtList() {
	return statementList;
}

std::unordered_set<int> PKB::getAssignStmtList() {
	return AssignStore::getAssignStmtList();
}

std::unordered_set<int> PKB::getPrintStmtList() {
	return UsesStore::getPrintStmtList();
}

std::unordered_set<int> PKB::getReadStmtList() {
	return ModifiesStore::getReadStmtList();
}

std::unordered_set<int> PKB::getIfStmtList() {
	return ifStmtList;
}

std::unordered_set<int> PKB::getWhileStmtList() {
	return whileStmtList;
}

std::unordered_set<int> PKB::getCallStmtList() {
	return callStmtList;
}

/* ===============================================================================================================
	FOLLOWSSTORE METHODS
===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addFollows(int stmt1, int stmt2) {
	return FollowsStore::addFollows(stmt1, stmt2);
}

bool PKB::addAllFollowed(int stmt, std::unordered_set<int> listOfFollowed) {
	return FollowsStore::addAllFollowed(stmt, listOfFollowed);
}

bool PKB::addAllFollower(int stmt, std::unordered_set<int> listOfFollower) {
	return FollowsStore::addAllFollower(stmt, listOfFollower);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasFollowed(int stmt) {
	return FollowsStore::stmtHasFollowed(stmt);
}

bool PKB::stmtHasFollower(int stmt) {
	return FollowsStore::stmtHasFollower(stmt);
}

bool PKB::programHasFollowedStmt() {
	return FollowsStore::programHasFollowedStmt();
}

bool PKB::programHasFollowerStmt() {
	return FollowsStore::programHasFollowedStmt();
}

// ============ GETTER METHODS ==============
int PKB::getFollowedOfStmt(int stmt) {
	return FollowsStore::getFollowedOfStmt(stmt);
}

int PKB::getFollowerOfStmt(int stmt) {
	return FollowsStore::getFollowerOfStmt(stmt);
}

std::unordered_set<int> PKB::getFollowedListOfStmt(int stmt) {
	return FollowsStore::getFollowedListOfStmt(stmt);
}

std::unordered_set<int> PKB::getFollowerListOfStmt(int stmt) {
	return FollowsStore::getFollowerListOfStmt(stmt);
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllFollowsPair() {
	return FollowsStore::getAllFollowsPair();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllFollowsStarPair() {
	return FollowsStore::getAllFollowsStarPair();
}

std::unordered_set<int> PKB::getAllFollowedStmt() {
	return FollowsStore::getAllFollowedStmt();
}

std::unordered_set<int> PKB::getAllFollowerStmt() {
	return FollowsStore::getAllFollowerStmt();
}

/* ===============================================================================================================
	PARENTSTORE METHODS
===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addParent(int parent, std::unordered_set<int> childrenList) {
	return ParentStore::addParent(parent, childrenList);
}

bool PKB::addAncestors(int stmt, std::unordered_set<int> ancestorList) {
	return ParentStore::addAncestors(stmt, ancestorList);
}

bool PKB::addDescendants(int stmt, std::unordered_set<int> descendantList) {
	return ParentStore::addDescendants(stmt, descendantList);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasParent(int stmt) {
	return ParentStore::stmtHasParent(stmt);
}

bool PKB::stmtHasChildren(int stmt) {
	return ParentStore::stmtHasChildren(stmt);
}

bool PKB::programHasParentStmt() {
	return ParentStore::programHasParentStmt();
}

bool PKB::programHasChildStmt() {
	return ParentStore::programHasChildStmt();
}

// ============ GETTER METHODS ==============
int PKB::getParentOfStmt(int stmt) {
	return ParentStore::getParentOfStmt(stmt);
}

std::unordered_set<int> PKB::getChildrenListOfStmt(int stmt) {
	return ParentStore::getChildrenListOfStmt(stmt);
}

std::unordered_set<int> PKB::getAncestorsListOfStmt(int stmt) {
	return ParentStore::getAncestorsListOfStmt(stmt);
}

std::unordered_set<int> PKB::getDescendantsListOfStmt(int stmt) {
	return ParentStore::getDescendantsListOfStmt(stmt);
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllParentPair() {
	return ParentStore::getAllParentPair();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllParentStarPair() {
	return ParentStore::getAllParentStarPair();
}

std::unordered_set<int> PKB::getAllParentStmt() {
	return ParentStore::getAllParentStmt();
}

std::unordered_set<int> PKB::getAllChildStmt() {
	return ParentStore::getAllChildStmt();
}

/* ===============================================================================================================
	MODIFIESSTORE METHODS
	===============================================================================================================*/
// ============ SETTER METHODS ==============
bool PKB::addModifies(int stmtNo, int stmtType, std::string var) {
	return ModifiesStore::addModifies(stmtNo, stmtType, var);
}

bool PKB::addModifiesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar) {
	return ModifiesStore::addModifiesToContainer(stmtNo, stmtType, listOfVar);
}

bool PKB::addModifiedVarToProc(std::string procName, std::unordered_set<std::string> listOfVars)
{
	return ModifiesStore::addModifiedVarToProc(procName, listOfVars);
}

// ============ QUERY METHODS ==============
bool PKB::isVarModifiedInStmt(std::string var, int stmtNo) {
	return ModifiesStore::isVarModifiedInStmt(var, stmtNo);
}

bool PKB::isVarModified(std::string var) {
	return ModifiesStore::isVarModified(var);
}

bool PKB::isStmtModifies(int stmtNo) {
	return ModifiesStore::isStmtModifies(stmtNo);
}

bool PKB::isVarModifiedInProc(std::string var, std::string procName)
{
	return ModifiesStore::isVarModifiedInProc(var, procName);
}

bool PKB::procHasModifies(std::string procName)
{
	return ModifiesStore::procHasModifies(procName);
}

bool PKB::hasIfModifies() {
	return ModifiesStore::hasIfModifies();
}

bool PKB::hasWhileModifies() {
	return ModifiesStore::hasWhileModifies();
}

bool PKB::hasModifiesP()
{
	return ModifiesStore::hasModifiesP();
}

bool PKB::hasModifies() {
	return ModifiesStore::hasModifies();
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> PKB::getStmtModifiesMap() {
	return ModifiesStore::getStmtModifiesMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarModifiedMap() {
	return ModifiesStore::getVarModifiedMap();
}

std::unordered_set<std::string> PKB::getAllModifiedVars() {
	return ModifiesStore::getAllModifiedVars();
}

std::unordered_set<std::string> PKB::getAllReadModifiedVars() {
	return ModifiesStore::getAllReadModifiedVars();
}

std::unordered_set<int> PKB::getModifyingStmtList() {
	return ModifiesStore::getModifyingStmtList();
}

std::unordered_set<int> PKB::getModifyingIfStmtList() {
	return ModifiesStore::getModifyingIfStmtList();
}

std::unordered_set<int> PKB::getModifyingWhileStmtsList() {
	return ModifiesStore::getModifyingWhileStmtsList();
}

std::unordered_set<std::string> PKB::getModifiyingProcs()
{
	return ModifiesStore::getModifiyingProcs();
}

std::unordered_set<int> PKB::getStmtsModifyingVar(std::string var) {
	return ModifiesStore::getStmtsModifyingVar(var);
}

std::unordered_set<std::string> PKB::getModifiedVarsInProc(std::string p)
{
	return ModifiesStore::getModifiedVarsInProc(p);
}

std::unordered_set<std::string> PKB::getModifiedVarsInStmt(int stmtNo) {
	return ModifiesStore::getModifiedVarsInStmt(stmtNo);
}

std::unordered_set<std::string> PKB::getProcModifyingVar(std::string var)
{
	return ModifiesStore::getProcModifyingVar(var);
}

std::unordered_set<std::string> PKB::getReadVars() {
	return ModifiesStore::getReadVars();
}

std::unordered_set<std::string> PKB::getVarsModifiedInWhile() {
	return ModifiesStore::getVarsModifiedInWhile();
}

std::unordered_set<std::string> PKB::getVarsModifiedInIf() {
	return ModifiesStore::getVarsModifiedInIf();
}

std::unordered_set<int> PKB::getModifyingCallStmtList() {
	return ModifiesStore::getModifyingCallStmtList();
}

std::unordered_map<int, std::string> PKB::getReadStmtToVarMap() {
	return ModifiesStore::getReadStmtToVarMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarToReadStmtMap() {
	return ModifiesStore::getVarToReadStmtMap();
}

int PKB::getTotalVarsModifiedForStmts() {
	return ModifiesStore::getTotalVarsModifiedForStmts();
}

int PKB::getTotalVarsModifiedForReadStmts() {
	return ModifiesStore::getTotalVarsModifiedForReadStmts();
}

int PKB::getTotalVarsModifiedForAssignStmts() {
	return ModifiesStore::getTotalVarsModifiedForAssignStmts();
}

int PKB::getTotalVarsModifiedForIfStmts() {
	return ModifiesStore::getTotalVarsModifiedForIfStmts();
}

int PKB::getTotalVarsModifiedForWhileStmts() {
	return ModifiesStore::getTotalVarsModifiedForWhileStmts();
}

int PKB::getTotalVarsModifiedForCallStmts() {
	return ModifiesStore::getTotalVarsModifiedForCallStmts();
}

int PKB::getTotalVarsModifiedForProcs() {
	return ModifiesStore::getTotalVarsModifiedForProcs();
}

/* ===============================================================================================================
	USESSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addUses(int stmtNo, int stmtType, std::unordered_set<std::string> listOfUsedVars) {
	return UsesStore::addUses(stmtNo, stmtType, listOfUsedVars);
}

bool PKB::addUsesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar) {
	return UsesStore::addUsesToContainer(stmtNo, stmtType, listOfVar);
}

bool PKB::addProcUses(std::string procName, std::unordered_set<std::string> listOfVars)
{
	return UsesStore::addProcUses(procName, listOfVars);
}

// ============ QUERY METHODS ==============
bool PKB::isVarUsedInStmt(int stmtNo, std::string var) {
	return UsesStore::isVarUsedInStmt(stmtNo, var);
}

bool PKB::isVarUsedInProc(std::string procName, std::string var)
{
	return UsesStore::isVarUsedInProc(procName, var);
}

bool PKB::isVarUsed(std::string var) {
	return UsesStore::isVarUsed(var);
}

bool PKB::isStmtUses(int stmtNo) {
	return UsesStore::isStmtUses(stmtNo);
}

bool PKB::hasIfUses() {
	return UsesStore::hasIfUses();
}

bool PKB::hasWhileUses() {
	return UsesStore::hasWhileUses();
}

bool PKB::hasAssignUses() {
	return UsesStore::hasAssignUses();
}

bool PKB::hasProcUses()
{
	return UsesStore::hasProcUses();
}

bool PKB::procUses(std::string procName)
{
	return UsesStore::procUses(procName);
}

bool PKB::hasUses() {
	return UsesStore::hasUses();
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> PKB::getStmtUsesMap() {
	return UsesStore::getStmtUsesMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarUsedMap() {
	return UsesStore::getVarUsedMap();
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::getProcUsesMap()
{
	return UsesStore::getProcUsesMap();
}

std::unordered_set<std::string> PKB::getAllUsedVarList() {
	return UsesStore::getAllUsedVarList();
}

std::unordered_set<std::string> PKB::getAllPrintUsedVarList() {
	return UsesStore::getAllPrintUsedVarList();
}


std::unordered_set<int> PKB::getAllUsesIfStmts() {
	return UsesStore::getAllUsesIfStmts();
}

std::unordered_set<int> PKB::getAllUsesWhileStmts() {
	return UsesStore::getAllUsesWhileStmts();
}

std::unordered_set<int> PKB::getAllUsesAssignStmts() {
	return UsesStore::getAllUsesAssignStmts();
}

std::unordered_set<std::string> PKB::getAllUsesProcs()
{
	return UsesStore::getAllUsesProcs();
}

std::unordered_set<std::string> PKB::getUsedVarsInStmt(int stmtNo) {
	return UsesStore::getUsedVarsInStmt(stmtNo);
}

std::unordered_set<int> PKB::getStmtsUsingVar(std::string var) {
	return UsesStore::getStmtsUsingVar(var);
}

std::unordered_set<std::string> PKB::getUsedVarsInProc(std::string procName)
{
	return UsesStore::getUsedVarsInProc(procName);
}

std::unordered_set<std::string> PKB::getPrintVars() {
	return UsesStore::getPrintVars();
}

std::unordered_set<std::string> PKB::getVarsUsedInWhile() {
	return UsesStore::getVarsUsedInWhile();
}

std::unordered_set<std::string> PKB::getProcsUsingVar(std::string var)
{
	return UsesStore::getProcsUsingVar(var);
}

std::unordered_set<std::string> PKB::getVarsUsedInIf() {
	return UsesStore::getVarsUsedInIf();
}

std::unordered_set<std::string> PKB::getVarsUsedInAssign() {
	return UsesStore::getVarsUsedInAssign();
}

std::unordered_set<int> PKB::getAllUsesCallStmts() {
	return UsesStore::getAllUsesCallStmts();
}

std::unordered_set<std::string> PKB::getVarsUsedInCall()
{
	return UsesStore::getVarsUsedInCall();
}

std::unordered_map<int, std::string> PKB::getPrintStmtToVarMap() {
	return UsesStore::getPrintStmtToVarMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarToPrintStmtMap() {
	return UsesStore::getVarToPrintStmtMap();
}

int PKB::getTotalVarsUsedForStmts() {
	return UsesStore::getTotalVarsUsedForStmts();
}

int PKB::getTotalVarsUsedForPrintStmts() {
	return UsesStore::getTotalVarsUsedForPrintStmts();
}

int PKB::getTotalVarsUsedForAssignStmts() {
	return UsesStore::getTotalVarsUsedForAssignStmts();
}

int PKB::getTotalVarsUsedForIfStmts() {
	return UsesStore::getTotalVarsUsedForIfStmts();
}

int PKB::getTotalVarsUsedForWhileStmts() {
	return UsesStore::getTotalVarsUsedForWhileStmts();
}

int PKB::getTotalVarsUsedForCallStmts() {
	return UsesStore::getTotalVarsUsedForCallStmts();
}

int PKB::getTotalVarsUsedForProcs() {
	return UsesStore::getTotalVarsUsedForProcs();
}

/* ===============================================================================================================
	CALLSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addCall(std::string callerP, std::string calleeP, int stmtNo) {
	return CallStore::addCall(callerP, calleeP, stmtNo);
}


bool PKB::addCallerT(std::string calleeP, std::unordered_set<std::string> callerP) {
	return CallStore::addCallerT(calleeP, callerP);
}

bool PKB::addCalleeT(std::string callerP, std::unordered_set<std::string> calleeP) {
	return CallStore::addCalleeT(callerP, calleeP);
}

// ============ QUERY METHODS ==============
bool PKB::hasCaller(std::string p) {
	return CallStore::hasCaller(p);
}

bool PKB::hasCallee(std::string p) {
	return CallStore::hasCallee(p);
}

bool PKB::Calls(std::string caller, std::string callee)
{
	return CallStore::Calls(caller, callee);
}

bool PKB::CallsTransitive(std::string caller, std::string callee)
{
	return CallStore::CallsTransitive(caller, callee);
}

bool PKB::hasCall() {
	return CallStore::hasCall();
}

// ============ GETTER METHODS ==============
std::unordered_set<std::string> PKB::getCalleesOfProc(std::string p) {
	return CallStore::getCalleesOfProc(p);
}

std::unordered_set<std::string> PKB::getCallersOfProc(std::string p) {
	return CallStore::getCallersOfProc(p);
}

std::unordered_set<std::string> PKB::getTCallersOfProc(std::string p)
{
	return CallStore::getTCallersOfProc(p);
}

std::unordered_set<std::string> PKB::getTCalleesOfProc(std::string p)
{
	return CallStore::getTCalleesOfProc(p);
}

std::unordered_set<std::string> PKB::getRootProcs() {
	return CallStore::getRootProcs();
}

std::unordered_set<std::string> PKB::getLeafProcs() {
	return CallStore::getLeafProcs();
}

std::unordered_map<int, std::string> PKB::getCallerStmtToProcMap()
{
	return CallStore::getCallerStmtToProcMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getProcToCallerStmtsMap()
{
	return CallStore::getProcToCallerStmtsMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getProcedureToCallStmtMap()
{
	return CallStore::getProcedureToCallStmtMap();
}

std::unordered_map<int, std::string> PKB::getCallStmtToProcedureMap()
{
	return CallStore::getCallStmtToProcedureMap();
}

std::unordered_set<int> PKB::getCallerStmts(std::string proc)
{
	return CallStore::getCallerStmts(proc);
}

std::unordered_set<int> PKB::getAllCallStmtsInProc(std::string proc)
{
	return CallStore::getAllCallStmtsInProc(proc);
}

std::unordered_set<std::string> PKB::getListOfAllCallers() {
	return CallStore::getListOfAllCallers();
}

std::unordered_set<std::string> PKB::getListOfAllCallees() {
	return CallStore::getListOfAllCallees();
}

std::unordered_set<std::string> PKB::getAllCallProcs()
{
	return CallStore::getAllCallProcs();
}

std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> PKB::getCallsPairList() {
	return CallStore::getCallsPairList();
}

std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> PKB::getCallsStarPairList() {
	return CallStore::getCallsStarPairList();
}

/* ===============================================================================================================
	NEXTSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addNext(int stmt1, int stmt2) {
	return NextStore::addNext(stmt1, stmt2);
}

bool PKB::addAllPrevious(int stmt, std::unordered_set<int> listOfPrevStarStmt) {
	return NextStore::addAllPrevious(stmt, listOfPrevStarStmt);
}

bool PKB::addAllNext(int stmt, std::unordered_set<int> listOfNextStarStmt) {
	return NextStore::addAllNext(stmt, listOfNextStarStmt);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasPrev(int stmt) {
	return NextStore::stmtHasPrev(stmt);
}

bool PKB::stmtHasNext(int stmt) {
	return NextStore::stmtHasNext(stmt);
}

bool PKB::programHasPrevStmt() {
	return NextStore::programHasPrevStmt();
}

bool PKB::programHasNextStmt() {
	return NextStore::programHasNextStmt();
}

// ============ GETTER METHODS ==============
std::unordered_set<int> PKB::getPrevOfStmt(int stmt) {
	return NextStore::getPrevOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextOfStmt(int stmt) {
	return NextStore::getNextOfStmt(stmt);
}

std::unordered_set<int> PKB::getPrevStarListOfStmt(int stmt) {
	return NextStore::getPrevStarListOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextStarListOfStmt(int stmt) {
	return NextStore::getNextStarListOfStmt(stmt);
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllNextPair() {
	return NextStore::getAllNextPair();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllNextStarPair() {
	return NextStore::getAllNextStarPair();
}

std::unordered_set<int> PKB::getAllPreviousStmt() {
	return NextStore::getAllPreviousStmt();
}

std::unordered_set<int> PKB::getAllNextStmt() {
	return NextStore::getAllNextStmt();
}

/* ===============================================================================================================
	NEXTBIPSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addNextBip(int stmt1, int stmt2, int caller) {
	return NextBipStore::addNextBip(stmt1, stmt2, caller);
}

bool PKB::addAllPreviousBip(int stmt, std::unordered_set<int> listOfPrevBipStarStmt) {
	return NextBipStore::addAllPreviousBip(stmt, listOfPrevBipStarStmt);
}

bool PKB::addAllNextBip(int stmt, std::unordered_set<int> listOfNextBipStarStmt) {
	return NextBipStore::addAllNextBip(stmt, listOfNextBipStarStmt);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasPrevBip(int stmt) {
	return NextBipStore::stmtHasPrevBip(stmt);
}

bool PKB::stmtHasNextBip(int stmt) {
	return NextBipStore::stmtHasNextBip(stmt);
}

bool PKB::programHasPrevBipStmt() {
	return NextBipStore::programHasPrevBipStmt();
}

bool PKB::programHasNextBipStmt() {
	return NextBipStore::programHasNextBipStmt();
}


// ============ GETTER METHODS ==============
std::unordered_set<int> PKB::getPrevBipOfStmt(int stmt) {
	return NextBipStore::getPrevBipOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextBipOfStmt(int stmt) {
	return NextBipStore::getNextBipOfStmt(stmt);
}

std::unordered_set<int> PKB::getPrevBipStarListOfStmt(int stmt) {
	return NextBipStore::getPrevBipStarListOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextBipStarListOfStmt(int stmt) {
	return NextBipStore::getNextBipStarListOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextBipListOfIncomingOfStmt(int stmt) {
	return NextBipStore::getNextBipListOfIncomingOfStmt(stmt);
}

std::unordered_set<int> PKB::getNextBipListOfOutgoingOfStmt(int stmt) {
	return NextBipStore::getNextBipListOfOutgoingOfStmt(stmt);
}


std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllNextBipPair() {
	return NextBipStore::getAllNextBipPair();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAllNextBipStarPair() {
	return NextBipStore::getAllNextBipStarPair();
}

std::unordered_set<int> PKB::getAllPreviousBipStmt() {
	return NextBipStore::getAllPreviousBipStmt();
}

std::unordered_set<int> PKB::getAllNextBipStmt() {
	return NextBipStore::getAllNextBipStmt();
}

/* ===============================================================================================================
	AFFECTSSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addAffects(int stmt, int affected) {
	return AffectsStore::addAffects(stmt, affected);
}

bool PKB::addAffectedByT(int stmt, std::unordered_set<int> listOfAffectors) {
	return AffectsStore::addAffectedByT(stmt, listOfAffectors);
}

bool PKB::addAffectingT(int stmt, std::unordered_set<int> listOfAffecting) {
	return AffectsStore::addAffectingT(stmt, listOfAffecting);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasAffectedBy(int stmt) {
	return AffectsStore::stmtHasAffectedBy(stmt);
}

bool PKB::stmtHasAffecting(int stmt) {
	return AffectsStore::stmtHasAffecting(stmt);
}

bool PKB::programHasAffected() {
	return AffectsStore::programHasAffected();
}

bool PKB::programHasAffecting() {
	return AffectsStore::programHasAffecting();
}

// ============ GETTER METHODS ==============
std::unordered_map<int, affectsStruct> PKB::getStmtAffectsMap() {
	return AffectsStore::getStmtAffectsMap();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAffectsPairList() {
	return AffectsStore::getAffectsPairList();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAffectsStarPairList() {
	return AffectsStore::getAffectsStarPairList();
}

std::unordered_set<int> PKB::getAffectedList() {
	return AffectsStore::getAffectedList();
}

std::unordered_set<int> PKB::getAffectingList() {
	return AffectsStore::getAffectingList();
}

std::unordered_set<int> PKB::getAffectedByOfStmt(int stmt) {
	return AffectsStore::getAffectedByOfStmt(stmt);
}

std::unordered_set<int> PKB::getAffectingOfStmt(int stmt) {
	return AffectsStore::getAffectingOfStmt(stmt);
}


std::unordered_set<int> PKB::getAffectedByTOfStmt(int stmt) {
	return AffectsStore::getAffectedByTOfStmt(stmt);
}

std::unordered_set<int> PKB::getAffectingTOfStmt(int stmt) {
	return AffectsStore::getAffectingTOfStmt(stmt);
}

/* ===============================================================================================================
	AFFECTSBIPSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addAffectsBip(int stmt, int affected) {
	return AffectsBipStore::addAffectsBip(stmt, affected);
}

bool PKB::addAffectedBipByT(int stmt, std::unordered_set<int> listOfAffectors) {
	return AffectsBipStore::addAffectedBipByT(stmt, listOfAffectors);
}

bool PKB::addAffectingBipT(int stmt, std::unordered_set<int> listOfAffecting) {
	return AffectsBipStore::addAffectedBipByT(stmt, listOfAffecting);
}

// ============ QUERY METHODS ==============
bool PKB::stmtHasAffectedBipBy(int stmt) {
	return AffectsBipStore::stmtHasAffectedBipBy(stmt);
}

bool PKB::stmtHasAffectingBip(int stmt) {
	return AffectsBipStore::stmtHasAffectingBip(stmt);
}

bool PKB::programHasAffectedBip() {
	return AffectsBipStore::programHasAffectedBip();
}

bool PKB::programHasAffectingBip() {
	return AffectsBipStore::programHasAffectingBip();
}

// ============ GETTER METHODS ==============
std::unordered_map<int, affectsBipStruct> PKB::getStmtAffectsBipMap() {
	return AffectsBipStore::getStmtAffectsBipMap();
}

std::unordered_set<std::pair<int, int>, IntPairHasher> PKB::getAffectsBipPairList() {
	return AffectsBipStore::getAffectsBipPairList();
}

std::unordered_set<std::pair<int, int>, IntPairHasher>PKB::getAffectsBipStarPairList() {
	return AffectsBipStore::getAffectsBipStarPairList();
}

std::unordered_set<int> PKB::getAffectedBipList() {
	return AffectsBipStore::getAffectedBipList();
}

std::unordered_set<int> PKB::getAffectingBipList() {
	return AffectsBipStore::getAffectingBipList();
}

std::unordered_set<int> PKB::getAffectedBipByOfStmt(int stmt) {
	return AffectsBipStore::getAffectedBipByOfStmt(stmt);
}

std::unordered_set<int> PKB::getAffectingBipOfStmt(int stmt) {
	return AffectsBipStore::getAffectingBipOfStmt(stmt);
}

std::unordered_set<int> PKB::getAffectedBipByTOfStmt(int stmt) {
	return AffectsBipStore::getAffectedBipByTOfStmt(stmt);
}

std::unordered_set<int> PKB::getAffectingBipTOfStmt(int stmt) {
	return AffectsBipStore::getAffectingBipTOfStmt(stmt);
}


/* ===============================================================================================================
	ASSIGNSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addAssign(int stmt, std::string var, std::string exp) {
	return AssignStore::addAssign(stmt, var, exp);
}

// ============ GETTER METHODS ==============
std::string PKB::getVariableOfStmt(int stmt) {
	return AssignStore::getVariableOfStmt(stmt);
}

std::string PKB::getExpressionOfStmt(int stmt) {
	return AssignStore::getExpressionOfStmt(stmt);
}

std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> PKB::getVarExpStmtList() {
	return AssignStore::getVarExpStmtList();
}

std::unordered_map <std::string, std::unordered_set<std::string>> PKB::getVarToExpMap() {
	return AssignStore::getVarToExpMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarAssignStmtList() {
	return AssignStore::getVarAssignStmtList();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getExpAssignStmtList() {
	return AssignStore::getExpAssignStmtList();
}

/* ===============================================================================================================
	IFSTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addIfUses(int stmt, std::unordered_set<std::string> varUsed) {
	return IfStore::addIfUses(stmt, varUsed);
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> PKB::getIfStmtUsesMap() {
	return IfStore::getIfStmtUsesMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarToIfStmtMap() {
	return IfStore::getVarToIfStmtMap();
}

std::unordered_set<std::string> PKB::getIfUsedVarList() {
	return IfStore::getIfUsedVarList();
}

std::unordered_set<int> PKB::getIfUsesStmtList() {
	return IfStore::getIfUsesStmtList();
}

std::unordered_set<std::string> PKB::getVarsUsedByIf(int stmt) {
	return IfStore::getVarsUsedByIf(stmt);
}

std::unordered_set<int> PKB::getIfStmtsUsingVar(std::string var) {
	return IfStore::getIfStmtsUsingVar(var);
}

int PKB::getTotalIfUsedVars() {
	return IfStore::getTotalIfUsedVars();
}

/* ===============================================================================================================
	WHILESTORE METHODS
	===============================================================================================================*/

// ============ SETTER METHODS ==============
bool PKB::addWhileUses(int stmt, std::unordered_set<std::string> varUsed) {
	return WhileStore::addWhileUses(stmt, varUsed);
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> PKB::getWhileStmtUsesMap() {
	return WhileStore::getWhileStmtUsesMap();
}

std::unordered_map<std::string, std::unordered_set<int>> PKB::getVarToWhileStmtMap() {
	return WhileStore::getVarToWhileStmtMap();
}

std::unordered_set<std::string> PKB::getWhileUsedVarList() {
	return WhileStore::getWhileUsedVarList();
}

std::unordered_set<int> PKB::getWhileUsesStmtList() {
	return WhileStore::getWhileUsesStmtList();
}

std::unordered_set<std::string> PKB::getVarsUsedByWhile(int stmt) {
	return WhileStore::getVarsUsedByWhile(stmt);
}

std::unordered_set<int> PKB::getWhileStmtsUsingVar(std::string var) {
	return WhileStore::getWhileStmtsUsingVar(var);
}

int PKB::getTotalWhileUsedVars() {
	return WhileStore::getTotalWhileUsedVars();
}

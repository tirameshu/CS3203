#include "AssignEvaluator.h"

bool AssignEvaluator::checkAssignClauseHolds(QueryPatternClause& currPatternClause) {
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	std::string secondArg = currPatternClause.getSecondArgument();
	ArgumentTypes secondArgType = currPatternClause.getSecondArgumentType();
	bool assignClauseHolds = false;
	bool isExactExpr = currPatternClause.checkExact();

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::VARIABLE) {
		if (secondArgType == ArgumentTypes::ANY) {
			assignClauseHolds = !PKB::getAssignStmtList().empty();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (isExactExpr) {
				assignClauseHolds = PKB::getExpAssignStmtList().count(secondArg) == 1;
			}
			else {
				if (checkStringIsConstVal(secondArg)) {
					if (PKB::getConstantList().count(secondArg) == 1) {
						assignClauseHolds = checkStmtTypeIsAssign(PKB::getAssignStmtList(), PKB::getConstantStmtMap().at(secondArg));
					}
				}
				else if (checkStringIsVarName(secondArg)) {
					if (PKB::isVarUsed(secondArg)) {
						assignClauseHolds = checkStmtTypeIsAssign(PKB::getAllUsesAssignStmts(), PKB::getStmtsUsingVar(secondArg));
					}
				}
				else {
					for (const auto& pair : PKB::getExpAssignStmtList()) {
						std::string expr = pair.first;

						if (expr.find(secondArg) != std::string::npos) {
							assignClauseHolds = true;
							break;
						}
					}
				}
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY) {
			assignClauseHolds = PKB::getVarAssignStmtList().count(firstArg) == 1;
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::getVarAssignStmtList().count(firstArg) == 1) {
				if (isExactExpr) {
					assignClauseHolds = PKB::getVarExpStmtList().count(std::make_pair(firstArg, secondArg)) == 1;
				}
				else {
					if (checkStringIsConstVal(secondArg)) {
						if (PKB::getConstantList().count(secondArg) == 1) {
							std::unordered_set<int> assignStmtSetToFilter = PKB::getConstantStmtMap().at(secondArg);
							assignClauseHolds = checkStmtTypeIsAssign(PKB::getVarAssignStmtList().at(firstArg), assignStmtSetToFilter);
						}
					}
					else if (checkStringIsVarName(secondArg)) {
						if (PKB::isVarUsed(secondArg)) {
							std::unordered_set<int> assignStmtSetToFilter = PKB::getStmtsUsingVar(secondArg);
							assignClauseHolds = checkStmtTypeIsAssign(PKB::getVarAssignStmtList().at(firstArg), assignStmtSetToFilter);
						}
					}
					else {
						auto curr = PKB::getVarToExpMap().at(firstArg);

						for (const std::string& expr : curr) {
							if (expr.find(secondArg) != std::string::npos) {
								assignClauseHolds = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	return assignClauseHolds;
}

bool AssignEvaluator::checkStringIsConstVal(std::string expression) {
	bool isConstVal = true;

	for (const auto& ch : expression) {
		if (!std::isdigit(ch)) {
			isConstVal = false;
			break;
		}
	}

	return isConstVal;
}

bool AssignEvaluator::checkStringIsVarName(std::string expression) {
	bool isVarName = true;

	for (const auto& ch : expression) {
		if (!std::isalnum(ch)) {
			isVarName = false;
			break;
		}
	}

	return isVarName;
}

bool AssignEvaluator::checkStmtTypeIsAssign(std::unordered_set<int>& assignStmtSetForFilter, std::unordered_set<int>& assignStmtSetToFilter) {
	bool stmtTypeIsAssign = false;
	std::unordered_set<int> stmtTypeSet = assignStmtSetForFilter;

	if (stmtTypeSet.size() < assignStmtSetToFilter.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (assignStmtSetToFilter.count(stmt) == 1) {
				stmtTypeIsAssign = true;
				break;
			}
		}
	}
	else {
		for (const int& stmt : assignStmtSetToFilter) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsAssign = true;
				break;
			}
		}
	}

	return stmtTypeIsAssign;
}

std::unordered_map<std::string, std::vector<std::string>> AssignEvaluator::evaluateAssignClause(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	std::string secondArg = currPatternClause.getSecondArgument();
	ArgumentTypes secondArgType = currPatternClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> patternClauseTable;
	bool isExactExpr = currPatternClause.checkExact();

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::ANY) {
			patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAssignStmtList()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (isExactExpr) {
				auto curr = PKB::getExpAssignStmtList();

				if (curr.count(secondArg) == 1) {
					patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(curr.at(secondArg)));
				}
			}
			else {
				if (checkStringIsConstVal(secondArg)) {
					if (PKB::getConstantList().count(secondArg) == 1) {
						std::vector<std::string> assignStmtList = getStmtTypeIsAssignList(PKB::getAssignStmtList(), PKB::getConstantStmtMap().at(secondArg));
						patternClauseTable.emplace(patternName, assignStmtList);
					}
				}
				else if (checkStringIsVarName(secondArg)) {
					if (PKB::isVarUsed(secondArg)) {
						std::vector<std::string> assignStmtList = getStmtTypeIsAssignList(PKB::getAllUsesAssignStmts(), PKB::getStmtsUsingVar(secondArg));
						patternClauseTable.emplace(patternName, assignStmtList);
					}
				}
				else {
					std::vector<std::string> assignStmtList;

					for (const auto& pair : PKB::getExpAssignStmtList()) {
						std::string expr = pair.first;
						std::unordered_set<int> assignStmts = pair.second;

						if (expr.find(secondArg) != std::string::npos) {
							for (int stmt : assignStmts) {
								assignStmtList.push_back(std::to_string(stmt));
							}
						}
					}

					patternClauseTable.emplace(patternName, assignStmtList);
				}
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY) {
			auto curr = PKB::getVarAssignStmtList();

			if (curr.count(firstArg) == 1) {
				patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(curr.at(firstArg)));
			}
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::getVarAssignStmtList().count(firstArg) == 1) {
				if (isExactExpr) {
					std::pair<std::string, std::string> assignStmt = std::make_pair(firstArg, secondArg);
					auto curr = PKB::getVarExpStmtList();

					if (curr.count(assignStmt) == 1) {
						patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(curr.at(assignStmt)));
					}
				}
				else {
					if (checkStringIsConstVal(secondArg)) {
						if (PKB::getConstantList().count(secondArg) == 1) {
							std::vector<std::string> assignStmtList = getStmtTypeIsAssignList(PKB::getVarAssignStmtList().at(firstArg), PKB::getConstantStmtMap().at(secondArg));
							patternClauseTable.emplace(patternName, assignStmtList);
						}
					}
					else if (checkStringIsVarName(secondArg)) {
						if (PKB::isVarUsed(secondArg)) {
							std::vector<std::string> assignStmtList = getStmtTypeIsAssignList(PKB::getVarAssignStmtList().at(firstArg), PKB::getStmtsUsingVar(secondArg));
							patternClauseTable.emplace(patternName, assignStmtList);
						}
					}
					else {
						std::vector<std::string> assignStmtList;
						auto curr = PKB::getVarAssignStmtList().at(firstArg);

						for (int stmt : curr) {
							std::string expr = PKB::getExpressionOfStmt(stmt);

							if (expr.find(secondArg) != std::string::npos) {
								assignStmtList.push_back(std::to_string(stmt));
							}
						}

						patternClauseTable.emplace(patternName, assignStmtList);
					}
				}
			}
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		if (secondArgType == ArgumentTypes::ANY) {
			std::vector<std::string> assignStmtList;
			std::vector<std::string> assignVarModifiedList;

			// std::unordered_map<std::string, std::unordered_set<int>> 
			for (const auto& pair : PKB::getVarAssignStmtList()) {
				auto var = pair.first;

				for (const auto& stmt : pair.second) {
					assignStmtList.push_back(std::to_string(stmt));
					assignVarModifiedList.push_back(var);
				}
			}

			patternClauseTable.emplace(patternName, assignStmtList);
			patternClauseTable.emplace(firstArg, assignVarModifiedList);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			std::vector<std::string> assignStmtList;
			std::vector<std::string> assignVarModifiedList;

			if (isExactExpr) {
				auto curr = PKB::getExpAssignStmtList();

				if (curr.count(secondArg) == 1) {
					auto stmtList = curr.at(secondArg);

					for (int stmt : stmtList) {
						std::string var = PKB::getVariableOfStmt(stmt);
						assignStmtList.push_back(std::to_string(stmt));
						assignVarModifiedList.push_back(var);
					}
				}
			}
			else {
				if (checkStringIsConstVal(secondArg)) {
					if (PKB::getConstantList().count(secondArg) == 1) {
						assignStmtList = getStmtTypeIsAssignList(PKB::getAssignStmtList(), PKB::getConstantStmtMap().at(secondArg));

						for (const std::string& stmt : assignStmtList) {
							assignVarModifiedList.push_back(PKB::getVariableOfStmt(std::stoi(stmt)));
						}
					}
				}
				else if (checkStringIsVarName(secondArg)) {
					if (PKB::isVarUsed(secondArg)) {
						assignStmtList = getStmtTypeIsAssignList(PKB::getAllUsesAssignStmts(), PKB::getStmtsUsingVar(secondArg));

						for (const std::string& stmt : assignStmtList) {
							assignVarModifiedList.push_back(PKB::getVariableOfStmt(std::stoi(stmt)));
						}
					}
				}
				else {
					for (const auto& pair : PKB::getExpAssignStmtList()) {
						std::string expr = pair.first;
						std::unordered_set<int> assignStmts = pair.second;

						if (expr.find(secondArg) != std::string::npos) {
							for (int stmt : assignStmts) {
								std::string var = PKB::getVariableOfStmt(stmt);
								assignStmtList.push_back(std::to_string(stmt));
								assignVarModifiedList.push_back(var);
							}
						}
					}
				}
			}	

			patternClauseTable.emplace(patternName, assignStmtList);
			patternClauseTable.emplace(firstArg, assignVarModifiedList);
		}
	}

	return patternClauseTable;
}

std::vector<std::string> AssignEvaluator::getStmtTypeIsAssignList(std::unordered_set<int>& assignStmtSetForFilter, std::unordered_set<int>& assignStmtSetToFilter) {
	std::vector<std::string> stmtTypeIsAssignList;
	std::unordered_set<int> stmtTypeSet = assignStmtSetForFilter;

	if (stmtTypeSet.size() < assignStmtSetToFilter.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (assignStmtSetToFilter.count(stmt) == 1) {
				stmtTypeIsAssignList.push_back(std::to_string(stmt));
			}
		}
	}
	else {
		for (const int& stmt : assignStmtSetToFilter) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsAssignList.push_back(std::to_string(stmt));
			}
		}
	}

	return stmtTypeIsAssignList;
}

int AssignEvaluator::estimateAssignClauseTableSize(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	std::string secondArg = currPatternClause.getSecondArgument();
	ArgumentTypes secondArgType = currPatternClause.getSecondArgumentType();
	int estimatedTableSize = 0;
	bool isExactExpr = currPatternClause.checkExact();

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAssignStmtList().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (isExactExpr) {
				auto& curr = PKB::getExpAssignStmtList();

				if (curr.count(secondArg) == 1) {
					estimatedTableSize = curr.at(secondArg).size();
				}
			}
			else {
				if (checkStringIsConstVal(secondArg)) {
					if (PKB::getConstantList().count(secondArg) == 1) { // check this
						estimatedTableSize = std::min(PKB::getAssignStmtList().size(), PKB::getConstantStmtMap().at(secondArg).size());
					}
				}
				else if (checkStringIsVarName(secondArg)) {
					if (PKB::isVarUsed(secondArg)) { // check this
						estimatedTableSize = std::min(PKB::getAllUsesAssignStmts().size(), PKB::getStmtsUsingVar(secondArg).size());
					}
				}
				else {
					estimatedTableSize = PKB::getAssignStmtList().size();
				}
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY) {
			auto& curr = PKB::getVarAssignStmtList();

			if (curr.count(firstArg) == 1) {
				estimatedTableSize = curr.at(firstArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::getVarAssignStmtList().count(firstArg) == 1) {
				if (isExactExpr) {
					std::pair<std::string, std::string> assignStmt = std::make_pair(firstArg, secondArg);
					auto& curr = PKB::getVarExpStmtList();

					if (curr.count(assignStmt) == 1) {
						estimatedTableSize = curr.at(assignStmt).size();
					}
				}
				else {
					if (checkStringIsConstVal(secondArg)) {
						if (PKB::getConstantList().count(secondArg) == 1) { // check this
							estimatedTableSize = std::min(PKB::getVarAssignStmtList().at(firstArg).size(), PKB::getConstantStmtMap().at(secondArg).size());
						}
					}
					else if (checkStringIsVarName(secondArg)) {
						if (PKB::isVarUsed(secondArg)) { // check this
							estimatedTableSize = std::min(PKB::getVarAssignStmtList().at(firstArg).size(), PKB::getStmtsUsingVar(secondArg).size());
						}
					}
					else {
						estimatedTableSize = PKB::getVarAssignStmtList().at(firstArg).size();
					}
				}
			}
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAssignStmtList().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (isExactExpr) {
				auto& curr = PKB::getExpAssignStmtList();

				if (curr.count(secondArg) == 1) {
					estimatedTableSize = curr.at(secondArg).size();
				}
			}
			else {
				if (checkStringIsConstVal(secondArg)) {
					if (PKB::getConstantList().count(secondArg) == 1) { // check this
						estimatedTableSize = std::min(PKB::getAssignStmtList().size(), PKB::getConstantStmtMap().at(secondArg).size());
					}
				}
				else if (checkStringIsVarName(secondArg)) {
					if (PKB::isVarUsed(secondArg)) { // check this
						estimatedTableSize = std::min(PKB::getAllUsesAssignStmts().size(), PKB::getStmtsUsingVar(secondArg).size());
					}
				}
				else {
					estimatedTableSize = PKB::getAssignStmtList().size();
				}
			}
		}
	}

	return estimatedTableSize;
}
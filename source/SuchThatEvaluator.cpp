#include "SuchThatEvaluator.h"

bool SuchThatEvaluator::checkSuchThatClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	RelRef relref = currSuchThatClause.getRelRef();
	bool suchThatClauseHolds = false;

	switch (relref) {
	case RelRef::MODIFIES_S:
		suchThatClauseHolds = ModifiesEvaluator::checkModifiesClauseHolds(currSuchThatClause);
		break;
	case RelRef::MODIFIES_P:
		suchThatClauseHolds = ModifiesEvaluator::checkModifiesClauseHolds(currSuchThatClause);
		break;
	case RelRef::USES_S:
		suchThatClauseHolds = UsesEvaluator::checkUsesClauseHolds(currSuchThatClause);
		break;
	case RelRef::USES_P:
		suchThatClauseHolds = UsesEvaluator::checkUsesClauseHolds(currSuchThatClause);
		break;
	case RelRef::PARENT:
		suchThatClauseHolds = ParentEvaluator::checkParentClauseHolds(currSuchThatClause);
		break;
	case RelRef::PARENT_T:
		suchThatClauseHolds = ParentStarEvaluator::checkParentStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::FOLLOWS:
		suchThatClauseHolds = FollowsEvaluator::checkFollowsClauseHolds(currSuchThatClause);
		break;
	case RelRef::FOLLOWS_T:
		suchThatClauseHolds = FollowsStarEvaluator::checkFollowsStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::CALLS:
		suchThatClauseHolds = CallsEvaluator::checkCallsClauseHolds(currSuchThatClause);
		break;
	case RelRef::CALLS_T:
		suchThatClauseHolds = CallsStarEvaluator::checkCallsStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::NEXT:
		suchThatClauseHolds = NextEvaluator::checkNextClauseHolds(currSuchThatClause);
		break;
	case RelRef::NEXT_T:
		suchThatClauseHolds = NextStarEvaluator::checkNextStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP:
		suchThatClauseHolds = NextBipEvaluator::checkNextBipClauseHolds(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP_T:
		suchThatClauseHolds = NextBipStarEvaluator::checkNextBipStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::AFFECTS:
		suchThatClauseHolds = AffectsEvaluator::checkAffectsClauseHolds(currSuchThatClause);
		break;
	case RelRef::AFFECTS_T:
		suchThatClauseHolds = AffectsStarEvaluator::checkAffectsStarClauseHolds(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP:
		suchThatClauseHolds = AffectsBipEvaluator::checkAffectsBipClauseHolds(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP_T:
		suchThatClauseHolds = AffectsBipStarEvaluator::checkAffectsBipStarClauseHolds(currSuchThatClause);
		break;
	}

	return suchThatClauseHolds;
}

bool SuchThatEvaluator::checkSuchThatClausePossible(QuerySuchThatClause& currSuchThatClause) {
	RelRef relref = currSuchThatClause.getRelRef();
	bool suchThatClausePossible = true;

	if (relref == RelRef::CALLS || relref == RelRef::CALLS_T || relref == RelRef::FOLLOWS || relref == RelRef::FOLLOWS_T
		|| relref == RelRef::NEXT || relref == RelRef::NEXT_BIP || relref == RelRef::PARENT || relref == RelRef::PARENT_T) {
		std::string firstArg = currSuchThatClause.getFirstArgument();
		ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
		std::string secondArg = currSuchThatClause.getSecondArgument();

		if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
			suchThatClausePossible = false;
		}
	}

	return suchThatClausePossible;
}

std::unordered_map<std::string, std::vector<std::string>> SuchThatEvaluator::evaluateSuchThatClause(QuerySuchThatClause& currSuchThatClause) {
	RelRef relref = currSuchThatClause.getRelRef();
	std::unordered_map<std::string, std::vector<std::string>> suchThatClauseTable;

	switch (relref) {
	case RelRef::MODIFIES_S:
		suchThatClauseTable = ModifiesEvaluator::evaluateModifiesClause(currSuchThatClause);
		break;
	case RelRef::MODIFIES_P:
		suchThatClauseTable = ModifiesEvaluator::evaluateModifiesClause(currSuchThatClause);
		break;
	case RelRef::USES_S:
		suchThatClauseTable = UsesEvaluator::evaluateUsesClause(currSuchThatClause);
		break;
	case RelRef::USES_P:
		suchThatClauseTable = UsesEvaluator::evaluateUsesClause(currSuchThatClause);
		break;
	case RelRef::PARENT:
		suchThatClauseTable = ParentEvaluator::evaluateParentClause(currSuchThatClause);
		break;
	case RelRef::PARENT_T:
		suchThatClauseTable = ParentStarEvaluator::evaluateParentStarClause(currSuchThatClause);
		break;
	case RelRef::FOLLOWS:
		suchThatClauseTable = FollowsEvaluator::evaluateFollowsClause(currSuchThatClause);
		break;
	case RelRef::FOLLOWS_T:
		suchThatClauseTable = FollowsStarEvaluator::evaluateFollowsStarClause(currSuchThatClause);
		break;
	case RelRef::CALLS:
		suchThatClauseTable = CallsEvaluator::evaluateCallsClause(currSuchThatClause);
		break;
	case RelRef::CALLS_T:
		suchThatClauseTable = CallsStarEvaluator::evaluateCallsStarClause(currSuchThatClause);
		break;
	case RelRef::NEXT:
		suchThatClauseTable = NextEvaluator::evaluateNextClause(currSuchThatClause);
		break;
	case RelRef::NEXT_T:
		suchThatClauseTable = NextStarEvaluator::evaluateNextStarClause(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP:
		suchThatClauseTable = NextBipEvaluator::evaluateNextBipClause(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP_T:
		suchThatClauseTable = NextBipStarEvaluator::evaluateNextBipStarClause(currSuchThatClause);
		break;
	case RelRef::AFFECTS:
		suchThatClauseTable = AffectsEvaluator::evaluateAffectsClause(currSuchThatClause);
		break;
	case RelRef::AFFECTS_T:
		suchThatClauseTable = AffectsStarEvaluator::evaluateAffectsStarClause(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP:
		suchThatClauseTable = AffectsBipEvaluator::evaluateAffectsBipClause(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP_T:
		suchThatClauseTable = AffectsBipStarEvaluator::evaluateAffectsBipStarClause(currSuchThatClause);
		break;
	}

	return suchThatClauseTable;
}

int SuchThatEvaluator::estimateSuchThatClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	RelRef relref = currSuchThatClause.getRelRef();
	int estimatedTableSize = 0;

	switch (relref) {
	case RelRef::MODIFIES_S:
		estimatedTableSize = ModifiesEvaluator::estimateModifiesClauseTableSize(currSuchThatClause);
		break;
	case RelRef::MODIFIES_P:
		estimatedTableSize = ModifiesEvaluator::estimateModifiesClauseTableSize(currSuchThatClause);
		break;
	case RelRef::USES_S:
		estimatedTableSize = UsesEvaluator::estimateUsesClauseTableSize(currSuchThatClause);
		break;
	case RelRef::USES_P:
		estimatedTableSize = UsesEvaluator::estimateUsesClauseTableSize(currSuchThatClause);
		break;
	case RelRef::PARENT:
		estimatedTableSize = ParentEvaluator::estimateParentClauseTableSize(currSuchThatClause);
		break;
	case RelRef::PARENT_T:
		estimatedTableSize = ParentStarEvaluator::estimateParentStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::FOLLOWS:
		estimatedTableSize = FollowsEvaluator::estimateFollowsClauseTableSize(currSuchThatClause);
		break;
	case RelRef::FOLLOWS_T:
		estimatedTableSize = FollowsStarEvaluator::estimateFollowsStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::CALLS:
		estimatedTableSize = CallsEvaluator::estimateCallsClauseTableSize(currSuchThatClause);
		break;
	case RelRef::CALLS_T:
		estimatedTableSize = CallsStarEvaluator::estimateCallsStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::NEXT:
		estimatedTableSize = NextEvaluator::estimateNextClauseTableSize(currSuchThatClause);
		break;
	case RelRef::NEXT_T:
		estimatedTableSize = NextStarEvaluator::estimateNextStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP:
		estimatedTableSize = NextBipEvaluator::estimateNextBipClauseTableSize(currSuchThatClause);
		break;
	case RelRef::NEXT_BIP_T:
		estimatedTableSize = NextBipStarEvaluator::estimateNextBipStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::AFFECTS:
		estimatedTableSize = AffectsEvaluator::estimateAffectsClauseTableSize(currSuchThatClause);
		break;
	case RelRef::AFFECTS_T:
		estimatedTableSize = AffectsStarEvaluator::estimateAffectsStarClauseTableSize(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP:
		estimatedTableSize = AffectsBipEvaluator::estimateAffectsBipClauseTableSize(currSuchThatClause);
		break;
	case RelRef::AFFECTS_BIP_T:
		estimatedTableSize = AffectsBipStarEvaluator::estimateAffectsBipStarClauseTableSize(currSuchThatClause);
		break;
	}

	return estimatedTableSize;
}
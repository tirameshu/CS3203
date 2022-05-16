#pragma once

#include "QueryEvaluatorUtil.h"
#include "SuchThatEvaluator.h"
#include "PatternEvaluator.h"
#include "Optimizer.h"

/*
	The QueryEvaluator is the main component responsible for evaluating the Query object
	created by the QueryParser after parsing query statements. 
	
	It does the following:
	- Check for query validity
	- Filter evaluated clause results for synonyms that are found in result-cl
	- Evaluates unique result-cl synonyms and combines with filtered clause results if any
	- Format query result
*/

class QueryEvaluator {
public:
	static std::unordered_set<std::string> evaluateQuery(Query& query);

private:
	static std::unordered_set<std::string> formatQueryResults(std::unordered_map<std::string, std::vector<std::string>>& resultTable, std::vector<std::string>& resultSynonyms, std::vector<ArgumentTypes>& resultTypes, std::vector<AttrName>& resultAttrNames);
};

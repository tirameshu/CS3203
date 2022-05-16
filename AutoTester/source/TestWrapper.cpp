#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...

	// Init parserReader creation
	ParserReader parserReader;

	parserReader.parseProgram(filename);

	// Init design extractor
	DesignExtractor::processProgram();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	QueryParser queryParser;

	Query queryObj = queryParser.parseQuery(query);
	// FOR DEBUGGING ONLY
	std::cout << queryObj.printQuery();
	std::cout << "COMPLETED QUERY PARSING" << std::endl;

	std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(queryObj);

	for (const auto& element : resultSet) {
		results.push_back(element);
	}

	// then pass the parsedQuery (Query object) to query evaluator

	  // store the answers to the query in the results list (it is initially empty)
	  // each result must be a string.

	// Note: results should be returned from the PQL printer
}

/*=== DESTRUCTOR ===*/
TestWrapper::~TestWrapper() {
	// Clears all static list
	PKB::clear();
}

#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/CallStore.h"
#include "CallStoreTestData.h"
#include "cassert"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestCallStore)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore) {
			CallStore::clear();
			//add direct call
			CallStore::addCall("testA", "testB", 7);
			CallStore::addCall("testA", "testC", 8);
			CallStore::addCall("testB", "testC", 11);
			CallStore::addCall("testC", "testD", 13);

			//add transitive call
			CallStore::addCallerT("testB", { "testA" });
			CallStore::addCallerT("testC", { "testA", "testB" });
			CallStore::addCallerT("testD", { "testC", "testA", "testB" });

			CallStore::addCalleeT("testA", { "testB" , "testC", "testD" });
			CallStore::addCalleeT("testB", { "testC", "testD" });
			CallStore::addCalleeT("testC", { "testD" });
		}

		TEST_METHOD(addCall_Duplicate) {
			Assert::IsFalse(CallStore::addCall("testA", "testB", 7));
		}

		TEST_METHOD(hasCaller_True) {
			Assert::IsTrue(CallStore::hasCaller("testB"));
		}

		TEST_METHOD(hasCaller_ProcExists_False) {
			Assert::IsFalse(CallStore::hasCaller("testA"));
		};

		TEST_METHOD(hasCaller_ProcNotExists_False) {
			Assert::IsFalse(CallStore::hasCaller("testF"));
		}

		TEST_METHOD(hasCallee_True) {
			Assert::IsTrue(CallStore::hasCallee("testA"));
		}

		TEST_METHOD(hasCallee_ProcExists_False) {
			Assert::IsFalse(CallStore::hasCallee("testD"));
		}

		TEST_METHOD(hasCallee_ProcNotExists_False) {
			Assert::IsFalse(CallStore::hasCallee("testF"));
		}

		TEST_METHOD(Calls_True) {
			bool result = CallStore::Calls("testA", "testC");
			Assert::IsTrue(result);
		}

		TEST_METHOD(Calls_False) {
			bool result = CallStore::Calls("testA", "testD");
			Assert::IsFalse(result);
		}

		TEST_METHOD(CallsT_True) {
			bool result = CallStore::CallsTransitive("testA", "testD");
			Assert::IsTrue(result);
		}

		TEST_METHOD(CallsT_False) {
			bool result = CallStore::CallsTransitive("testC", "testB");
			Assert::IsFalse(result);
		}
		TEST_METHOD(hasCall_True) {
			Assert::IsTrue(CallStore::hasCall());
		}

		TEST_METHOD(getCalleesOfProc_NonEmpty) {
			std::unordered_set<std::string> expected = { "testB", "testC" };
			std::unordered_set<std::string> result = CallStore::getCalleesOfProc("testA");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getCalleesOfProc_Empty_ProcExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getCalleesOfProc("testD");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getCalleesOfProc_Empty_ProcNotExists) {
			std::unordered_set<std::string> result = CallStore::getCalleesOfProc("testF");

			Assert::IsTrue(result == std::unordered_set<std::string> {});
		}

		TEST_METHOD(getCallersOfProc_NonEmpty) {
			std::unordered_set<std::string> expected = { "testA", "testB" };
			std::unordered_set<std::string> result = CallStore::getCallersOfProc("testC");

			Assert::IsTrue(expected == result);
		}
		
		TEST_METHOD(getCallersOfProc_Empty_ProcExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getCallersOfProc("testA");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getCallersOfProc_Empty_ProcNotExists) {
			std::unordered_set<std::string> result = CallStore::getCallersOfProc("testF");

			Assert::IsTrue(result == std::unordered_set<std::string> {});
		}

		TEST_METHOD(getTCallersOfProc_NonEmpty) {
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			std::unordered_set<std::string> result = CallStore::getTCallersOfProc("testD");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getTCallersOfProc_Empty_ProcExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getTCallersOfProc("testA");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getTCallersOfProc_Empty_ProcNotExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getTCallersOfProc("testF");

			Assert::IsTrue(expected == result);
		}


		TEST_METHOD(getTCalleesOfProc_NonEmpty) {
			std::unordered_set<std::string> expected = { "testB", "testC", "testD" }; 
			std::unordered_set<std::string> result = CallStore::getTCalleesOfProc("testA");

			Assert::IsTrue(expected == result);
		}


		TEST_METHOD(getTCalleesOfProc_Empty_ProcExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getTCalleesOfProc("testD");

			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(getTCalleesOfProc_Empty_ProcNotExists) {
			std::unordered_set<std::string> expected = {};
			std::unordered_set<std::string> result = CallStore::getTCalleesOfProc("testF");

			Assert::IsTrue(expected == result);
		}
		
		TEST_METHOD(getRootProcs_True) {
			std::unordered_set<std::string> result = CallStore::getRootProcs();
			Assert::IsTrue(result == expectedListOfRootProcs);
		}

		TEST_METHOD(getLeafProcs_True) {
			std::unordered_set<std::string> result = CallStore::getLeafProcs();
			Assert::IsTrue(result == expectedListOfLeafProcs);
		}

		TEST_METHOD(getCallerStmtToProcedureMap_True) {
			std::unordered_map<int, std::string> result = CallStore::getCallerStmtToProcMap();
			Assert::IsTrue(result == expectedCallerStmtToProcMap);
		}

		TEST_METHOD(getProcToCallerStmtsMap_True) {
			std::unordered_map<std::string, std::unordered_set<int>> result = CallStore::getProcToCallerStmtsMap();
			Assert::IsTrue(result == expectedProcToCallerStmtsMap);
		}

		TEST_METHOD(getProcedureToCallStmtMap_True) {
			std::unordered_map<std::string, std::unordered_set<int>> result = CallStore::getProcedureToCallStmtMap();
			Assert::IsTrue(result == expectedProcedureToCallStmtMap);
		}

		TEST_METHOD(getCallStmtToProcedureMap_True) {
			std::unordered_map<int, std::string> result = CallStore::getCallStmtToProcedureMap();
			Assert::IsTrue(result == expectedCallStmtToProcedureMap);
		}

		TEST_METHOD(getCallerStmts_NonEmpty) {
			std::unordered_set<int> expected = { 8, 11 };
			std::unordered_set<int> result = CallStore::getCallerStmts("testC");
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getCallerStmts_Empty_ProcExists) {
			std::unordered_set<int> expected = {};
			std::unordered_set<int> result = CallStore::getCallerStmts("testA");
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getCallerStmts_Empty_ProcNotExists) {
			std::unordered_set<int> expected = {};
			std::unordered_set<int> result = CallStore::getCallerStmts("testF");
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getAllCallStmtsInProc_NonEmpty) {
			std::unordered_set<int> expected = {7, 8};
			std::unordered_set<int> result = CallStore::getAllCallStmtsInProc("testA");

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getAllCallStmtsInProc_Empty_ProcExists) {
			std::unordered_set<int> expected = {};
			std::unordered_set<int> result = CallStore::getAllCallStmtsInProc("testD");

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getAllCallStmtsInProc_Empty_ProcNotExists){
			std::unordered_set<int> expected = {};
			std::unordered_set<int> result = CallStore::getAllCallStmtsInProc("testF");

			Assert::IsTrue(result == expected);
		}
	};
}
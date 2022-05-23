# Motivation
Some companies spend ~80% of software budgets on software maintenance. During software maintenance, programmers spend ~50% of the time understanding a program. Therefore, methods and tools that can ease program understanding can cut development costs substantially.

During program maintenance, programmers often try to locate code relevant to the maintenance task at hand. Here are examples of questions programmers might ask to locate code of interest:

- I need to find code that implements salary computation rules!
- Where is variable x modified? Where is it used?
- I need to find all statements with the sub-expression x * y + z!
- Which statements affect the value of x at statement #120?
- Which statements can be affected if I modify statement #20?
- A programmer may need to examine a large amount of code to answer those questions. Doing this by hand may be time-consuming and error-prone.

# Static Program Analyzer (SPA)
A Static Program Analyzer (SPA) is an interactive tool that automatically answers queries about programs. We will design and implement a SPA for the SIMPLE programming language.

The SPA can be used by users (programmers) in the following way:

1. John, a programmer, is given a task to fix a crashing error in a program.
2. He feeds the program into SPA for automated analysis. The SPA parses a program into the internal representation stored in a Program Knowledge Base (PKB).
3. He starts using the SPA to help him find program statements that cause the crash by entering queries. The SPA evaluates the queries and displays the results.
4. He analyzes query results and examines related sections of the program to locate the source of the error.
5. He finds program statement(s) responsible for an error and modifies the program to fix the error. He can ask the SPA with more queries to examine the possible unwanted ripple effects of the changes.

From the users' point of view, static analysis requires three actions:

1. Enter the source program
2. Enter queries
3. View query results

# How It Works
SPA works by:

- Analyzing a source program and extract relevant program design entities, program design abstractions, Abstract Syntax Tree (AST), and program Control Flow Graph (CFG)
- Storing the information in a PKB
- Providing the user with the means to ask queries written in a formal Program Query Language (PQL)
- Processing the PQL queries based on the information found in the PKB
- Returning the results to the user

# Usage

Visual Studio version used: Visual Studio 2019

Configuration to use: Release

## Instructions
Open the StartupSPASolution.sln using Visual Studio 2019 and build the solution under the Release configuration. Do set the AutoTester as the Startup Project.
The AutoTester can run in two ways:
1.	AutoTester can run through the Local Windows Debugger. Do note that this would run based on the ..\Team20\Code20\Tests\Sample_source.txt and ..\Team20\Code20\Tests\Sample_queries.txt. Currently, the Sample_source.txt and Sample_queries.txt are based on the files provided in the original repository which contain call stmts and more than 1 procedure which are not part of the Iteration 1 Concrete Syntax Grammar for SIMPLE. 
a.	To run other SIMPLE source programs and queries using Local Windows Debugger, replace the text in Sample_source.txt and Sample_queries.txt with the desired SIMPLE source program text and query text respectively.
2.	AutoTester can also run through the command line interface. After building the solution under the Release configuration, the AutoTester.exe would be found under ..\Team20\Code20\Release. Navigate to the location of the AutoTester.exe.
a.	To run the AutoTester against a SIMPLE source file and query file, the following command can be used (assuming user has navigated to ..\Team20\Code20\Release):

.\AutoTester.exe <SIMPLE-source> <SIMPLE-queries> <output-xml>

Where <SIMPLE-source> <SIMPLE-queries> <output-xml> are the relative locations of the respective files from ..\Team20\Code20\Release. For example, if the respective files are under ..\Team20\Code20\Tests, the command would be:

.\AutoTester.exe ..\Tests\Sample_source.txt ..\Tests\Sample_queries.txt ..\Tests\out.xml

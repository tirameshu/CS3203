Visual Studio version used: Visual Studio 2019
Configuration to use: Release

Instructions
Open the StartupSPASolution.sln using Visual Studio 2019 and build the solution under the Release configuration. Do set the AutoTester as the Startup Project.
The AutoTester can run in two ways:
1.	AutoTester can run through the Local Windows Debugger. Do note that this would run based on the ..\Team20\Code20\Tests\Sample_source.txt and ..\Team20\Code20\Tests\Sample_queries.txt. Currently, the Sample_source.txt and Sample_queries.txt are based on the files provided in the original repository which contain call stmts and more than 1 procedure which are not part of the Iteration 1 Concrete Syntax Grammar for SIMPLE. 
a.	To run other SIMPLE source programs and queries using Local Windows Debugger, replace the text in Sample_source.txt and Sample_queries.txt with the desired SIMPLE source program text and query text respectively.
2.	AutoTester can also run through the command line interface. After building the solution under the Release configuration, the AutoTester.exe would be found under ..\Team20\Code20\Release. Navigate to the location of the AutoTester.exe.
a.	To run the AutoTester against a SIMPLE source file and query file, the following command can be used (assuming user has navigated to ..\Team20\Code20\Release):

.\AutoTester.exe <SIMPLE-source> <SIMPLE-queries> <output-xml>

Where <SIMPLE-source> <SIMPLE-queries> <output-xml> are the relative locations of the respective files from ..\Team20\Code20\Release. For example, if the respective files are under ..\Team20\Code20\Tests, the command would be:

.\AutoTester.exe ..\Tests\Sample_source.txt ..\Tests\Sample_queries.txt ..\Tests\out.xml
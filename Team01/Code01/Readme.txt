1. Configuration:
Visual Studio Version: Visual Studio 2019 Enterprise
Configuration: Release
Platform: Win32
QT Version: QT 5.14.2



2. Building:
Compile on Windows.
Open StartupSPASolution.sln in the Code01 folder.
On the top bar, ensure the configuration is set to "Release" and platform is set to "Win32"
Right click the solution ("Solution 'StartupSPASolution' (5 of 5 projects)") then click "Build Solution"

If an error occurs related to QT VS Tools, "...no QT version assigned to project Gui\Gui.vcxproj...", ensure the following is done:
Right-click on Gui project > Qt Project Settings > Properties tab, ensure that the version is "msvc2017" or "5.14.2".
In Qt Project Settings > QT Modules tab ensure that the checkbox for Core, GUI and Widgets are checked.

If an error occurs related to SPA.lib, or unresolved externals, please ensure that the SPA.lib is linked to each of the projects. It can be done as follows:
Right-click on AutoTester Project > Properties > Configuration Properties (In the sidebar) > Linker > Input select Additional Dependencies and click the down arrow.
Then click Edit. In the first textbox, please paste in the following without quotes:
"$(SolutionDir)$(Configuration)\SPA.lib"

Once built, the binaries can be found in Code01\Release or Code01\Debug (if you chose to build with Debug configuration)



3. Running Unit & Integration Tests:
After building the entire solution, open the Test Explorer (From the top bar of Visual Studio, Test > Test Explorer)
The Test Explorer window should pop up.
Click "Run All Tests In View" (The leftmost green triangle button in the Test Explorer)



4. Running Systems Test:
We run the Systems Test via AutoTester.
We have 3 global test cases.
To run each of test cases via the AutoTester from a command prompt, navigate to the executable file (Code01\Release or Code01\Debug (if you chose to build with Debug configuration)) and input: 

AutoTester ..\..\Tests01\Iteration1_Global_1_source.txt ..\..\Tests01\Iteration1_Global_1_queries.txt ..\..\Tests01\out.xml

AutoTester ..\..\Tests01\Iteration1_Global_2_source.txt ..\..\Tests01\Iteration1_Global_2_queries.txt ..\..\Tests01\out.xml

AutoTester ..\..\Tests01\Iteration1_Global_3_source.txt ..\..\Tests01\Iteration1_Global_3_queries.txt ..\..\Tests01\out.xml



5. Bonus Tester
RUNTESTS.PY
A simple script to automate testing :)

Requirements
Make sure that you have python 3 or higher installed!
Make sure that the AutoTester was compiled in Debug Configuration as the python script searches for the AutoTester executable in the Debug folder of Code01.

Checking All Tests (Grammar)
Open your file explorer and navigate to "Team01/Tests01/" directory
Open your favourite terminal and run: py RunTests.py --check System --depth 1
Done! RunTests will tell you which source files are grammatically valid or not, and why!

Labelling All Tests
Open your file explorer and navigate to "Team01/Tests01/" directory
Open your favourite terminal and run: py RunTests.py --label System --depth 1
Done! All grammatically correct source files will have a .lab file created!

Running All Tests
Ensure that you have built AutoTester
Open your file explorer and navigate to "Team01/Tests01/" directory
Open your favourite terminal and run: py RunTests.py --run System --depth 1
Wait for it to finish...
Done! Open RunTestsOutput directory to see all AutoTester outputs
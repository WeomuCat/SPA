1. Configuration:
Visual Studio Version: Visual Studio 2019 Enterprise
Configuration: Release
Platform: Win32
QT Version: QT 5.14.2



2. Building:
Steps:
1. Compile on Windows.
2. Open StartupSPASolution.sln in the Code01 folder.
3. On the top bar, ensure the configuration is set to "Release" and platform is set to "Win32"
4. Right click the solution ("Solution 'StartupSPASolution' (5 of 5 projects)") then click "Build Solution"

5. If an error occurs related to QT VS Tools, "...no QT version assigned to project Gui\Gui.vcxproj...", ensure the following is done:
6. Right-click on Gui project > Qt Project Settings > Properties tab, ensure that the version is "msvc2017" or "5.14.2".
7. In Qt Project Settings > QT Modules tab ensure that the checkbox for Core, GUI and Widgets are checked.

8. If an error occurs related to SPA.lib, or unresolved externals, please ensure that the SPA.lib is linked to each of the projects. It can be done as follows:
    > Right-click on AutoTester Project > Properties > Configuration Properties (In the sidebar) > Linker > Input select Additional Dependencies and click the down arrow.
9. Then click Edit. In the first textbox, please paste in the following without quotes:
   "$(SolutionDir)$(Configuration)\SPA.lib"

Once built, the binaries can be found in Code01\Release or Code01\Debug (if you chose to build with Debug configuration)



3. Running Unit & Integration Tests:
Steps:
1. After building the entire solution, open the Test Explorer (From the top bar of Visual Studio, Test > Test Explorer)
    > The Test Explorer window should pop up.
2. Click "Run All Tests In View" (The leftmost green triangle button in the Test Explorer)



4. Running Systems Test:
There are 2 different ways to run the current System Tests. But before we begin, note that:

> We have a total of 18 SIMPLE Source Files and about 3424 SIMPLE queries.
> Because we have 2 different ways to run the test, there are 2 separate files containing the different queries.
    > The first way has the set of source and queries found in the directory Team01\Test01\System
    > The second way has the set of source and queries found in the directory Team01\Test01\Published
    > Both the sources and queries are exactly the same, just that the queries are compield into a single file for ease of use for the second way.

**First Way**:
The first way would be to use the Bonus Tester, or the RunTests.py.

Steps:
1. Ensure that you have built AutoTester
2. Open your file explorer and navigate to "Team01\Tests01\" directory. This should be where the "RunTests.py" python script should be found.
3. Open your favourite terminal and run without the quotes: "py RunTests.py --run System" 
    > Or generally, "py RunTests.py --run System\Directory\To\.dep"
    > In the event of any you want to know the other functionalities of the script, check Section 5 of this README.
4. Wait for it to finish... It should indicate.
5. Done! Open RunTestsOutput directory to see all AutoTester outputs of the .xml files.

**Second way**:
1. Ensure that you have built AutoTester
2. To run each of test cases via the AutoTester from a command prompt, navigate to the executable file (Code01\Release or Code01\Debug (if you chose to build with Debug configuration)) and input: 

AutoTester ..\..\Tests01\Published\D1-Letters_source.txt ..\..\Tests01\Published\D1-Letters_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-Arknights_source.txt ..\..\Tests01\Published\D2-Arknights_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-Bird_source.txt ..\..\Tests01\Published\D2-Bird_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-CatDog_source.txt ..\..\Tests01\Published\D2-CatDog_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-Life_source.txt ..\..\Tests01\Published\D2-Life_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-Pokemon_source.txt ..\..\Tests01\Published\D2-Pokemon_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-Triple_source.txt ..\..\Tests01\Published\D2-Triple_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D2-VTuber_source.txt ..\..\Tests01\Published\D2-VTuber_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D3-Japanese_source.txt ..\..\Tests01\Published\D3-Japanese_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\D3-Minecraft_source.txt ..\..\Tests01\Published\D3-Minecraft_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\Invalid-Colors_source.txt ..\..\Tests01\Published\Invalid-Colors_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\OptimizationStress-Affector_source.txt ..\..\Tests01\Published\OptimizationStress-Affector_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\OptimizationStress-PokemonAgain_source.txt ..\..\Tests01\Published\OptimizationStress-PokemonAgain_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\Parser-AmbiguousVariables_source.txt ..\..\Tests01\Published\Parser-AmbiguousVariables_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\Parser-ManyBrackets_source.txt ..\..\Tests01\Published\Parser-ManyBrackets_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\Parser-Minified1_source.txt ..\..\Tests01\Published\Parser-Minified1_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\Parser-Minified2_source.txt ..\..\Tests01\Published\Parser-Minified2_queries.txt ..\..\Tests01\out.xml
AutoTester ..\..\Tests01\Published\QPP-Source_source.txt ..\..\Tests01\Published\QPP-Source_queries.txt ..\..\Tests01\out.xml

3. Done! For each file, the out.xml is located as specified in the command.

**Others**
NOTE: For the queries in the following file,
"Team01\Tests01\System\OptimizationStress\PokemonAgain_Stress_Mixed"

"Tests01\Published\OptimizationStress-PokemonAgain_queries.txt" 

Please note that failing the test case is indicative of passing while timeout is indicative of failing.

This is because this test case is merely a stress test to ensure that our program can run under the time limit and we did not provide all possible answers for the queries thus it would not be able to pass according to the AutoTester. 

Failing would mean that the program ran under the time limit. While timeout would mean that our program was not optimized enough to finish within the time limit thus failing the stress test.



5. Bonus Tester
RUNTESTS.PY
A simple script to automate testing :)

Requirements
> Make sure that you have python 3 or higher installed!
> Make sure that the AutoTester was compiled in Debug Configuration as the python script searches for the AutoTester executable in the Debug folder of Code01.

Getting Help
1. Open your file explorer and navigate to "Team01/Tests01/" directory
2. Open your favourite terminal and run: py RunTests.py --help
3. Done! RunTests will tell you what features it has.

Checking All Tests (Grammar)
1. Open your file explorer and navigate to "Team01/Tests01/" directory
2. Open your favourite terminal and run: py RunTests.py --check System
3. Done! RunTests will tell you which source files are grammatically valid or not, and why!

Labelling All Tests
1. Open your file explorer and navigate to "Team01/Tests01/" directory
2. Open your favourite terminal and run: py RunTests.py --label System
3. Done! All grammatically correct source files will have a .lab file created!

Running All Tests
1. Look above in Section 4.

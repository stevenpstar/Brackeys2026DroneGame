cd game/build
ninja.exe
cd .. ..
xcopy /s/Y/q "F:\Projects\BrackeysDroneGame\game\build\dronebrackeys.dll" "F:\Projects\BrackeysDroneGame\exec\build"
xcopy /s/Y/q "F:\Projects\BrackeysDroneGame\game\build\dronebrackeys.lib" "F:\Projects\BrackeysDroneGame\exec\build"
xcopy /s/Y/q "F:\Projects\BrackeysDroneGame\game\assets" "F:\Projects\BrackeysDroneGame\exec\game\assets"
xcopy /s/Y/q "F:\Projects\BrackeysDroneGame\game\include\game" "F:\Projects\BrackeysDroneGame\exec\include\game"

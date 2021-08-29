mkdir skribbl
xcopy /E /I Assets skribbl\Assets
xcopy /E /I fonts skribbl\fonts
xcopy /E /I x64\Debug\*.dll skribbl\x64\Debug
xcopy /E /I x64\Debug\*.exe skribbl\x64\Debug
xcopy /E /I x64\Debug\*.ilk skribbl\x64\Debug
xcopy /E /I x64\Debug\*.idb skribbl\x64\Debug
xcopy /E /I x64\Debug\*.pdb skribbl\x64\Debug
copy skribbl.bat skribbl\skribbl.bat
echo INFO: Zip the skribbl folder

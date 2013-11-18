del /s /Q data
mkdir data
xcopy.exe data_org data /E /D /Y /I
cd data
del /s *.pvr
del /s *.dds
del /s *.bat
del /s *.sh
cd ..

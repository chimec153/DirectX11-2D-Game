cd..
cd..

xcopy	.\GameEngine\Include\*.h .\Engine\Include\	/s /y /d
xcopy	.\GameEngine\Include\*.iln .\Engine\Include\	/s /y /d
xcopy	.\GameEngine\Include\*.hpp .\Engine\Include\	/s /y /d

xcopy	.\GameEngine\Bin\*.lib	.\Engine\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\GameClient\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\GameEditor\Bin\	/s /y /d
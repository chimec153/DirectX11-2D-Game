cd..
cd..

xcopy	.\GameEngine\Include\*.h .\Engine\Include\	/s /y /d
xcopy	.\GameEngine\Include\*.inl .\Engine\Include\	/s /y /d
xcopy	.\GameEngine\Include\*.hpp .\Engine\Include\	/s /y /d

xcopy	.\GameEngine\Bin\*.lib	.\Engine\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\GameClient\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\GameEditor\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\Editor\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\Server\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.lib	.\Client\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\Engine\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\GameClient\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\GameEditor\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\Editor\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\Server\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\*.dll	.\Client\Bin\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\Engine\Bin\Resource\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\GameClient\Bin\Resource\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\GameEditor\Bin\Resource\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\Editor\Bin\Resource\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\Server\Bin\Resource\	/s /y /d
xcopy	.\GameEngine\Bin\Resource\*.*	.\Client\Bin\Resource\	/s /y /d
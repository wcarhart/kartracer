@echo off
@lcc %1.c
@lcclnk %1.obj opengl32.lib glu32.lib glut32.lib
set name=%1
shift
start /d "%~dp0" %name%.exe %*
pause >nul
del *.obj %name%.exe
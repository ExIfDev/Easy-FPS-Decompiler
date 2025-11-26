# Easy FPS Decompiler

## About
EasyFPSE Decompiler is a simple CLI tool that allows you to protect or decompile games made with the Easy FPS Editor.

## Features
- Unpacking an EFPSE compatible project from a compiled game
- Protecting a game against decompilation

## Usage Examples

 Protecting a game:
 ```EasyFPSE_decompiler.exe -p <path to Data.pak> <output directory>```
 This will create a new ```Data.pak``` file in the output directory, paste it in your game root directory.

 Decompiling a game: ```EasyFPSE_decompiler.exe -u <path to Data.pak> <output directory>``` This will create a folder named ```Data``` in the chosen output directory, you can rename the directory as you please and simply open it using EasyFPSEditor.

 ## Use cases
 - recovering a lost project from a built game
 - learning
 - modding
 - maintaining abandoned games
   
This software was *not* meant to "steal" others work, and it's developer (me) does not condone such behavior, please be mindful and don't abuse this software for non ethical purposes.

 ## Why no source code?

 For the time being only a compiled binary is avaliable because having the tool be open sourced would defy the protection capabilities of the software.


![GitHub all releases](https://img.shields.io/github/downloads/ExIfDev/Easy-FPS-Decompiler/total?style=for-the-badge)


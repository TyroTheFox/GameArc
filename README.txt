//////////////////////////////////////////
//GAMEARC ENGINE-------------------------------
//////////////////////////////////////////

Kieran Clare - P16163779
------------------------
24/01/2019 - v2

Use the Test Bed Example for set up and use. Unit Testing added to demonstate how key aspects of the code work.

Controls:

WASD - Movement
Mouse - Rotate Camera View
Q - Switch Between 1st and 3rd person cameras

Debug Console:
` - Toggle Console

Shift - Upper Case
Backspace - Delete Previous Character
Enter - Execute Command

Type commands followed by arguments separated by spaces into the console for Debugging Controls

Debugging Codes:

Static Camera Controls (Alters Event Camera variables):
Change Rotation: setSRot <camera name> <x> <y> <z>
Change Translation: setSTran <camera name> <x> <y> <z>

Scene Controls: 
Change Scene: changeScene <sceneName>
List Scenes: listScenes

Player Controls: 
Set First Person Perspective Flag: setFPC <0/1>
Set 3rd Person Camera Offset: setCamOffset <offset>
Set Mouse Sensitivity: setMouseSen <value>
Set Field of View: setFOV <fov>
Set Player Speed: setPlayerSpeed <speed>

RGB Colour Controls:
Set Colour: setColour <Game Object name> <red> <green> <blue> <alpha>

Model Controls:
Change Model: changeModel <fileLocation>

Text UI Controls:
Change Text of UI Element: changeText <Game Object name> <newText>

Scene Controls:
Delete Object: delete <Game Object Name>

# CS-207-Project: Taking Attendance with the Help of a RFID Sensor


## Novel Contribution
In this project, My contribution is adding the ability of adding/removing IDs in the run-time using a special card called master card, swiping this card allows for another menu on the LCD to show, the system automatically detects if the swept card is stored or not by searching for it in the internal EEPROM. There is another contribution that is a checker's card, which does not allow any attendance to be taken after that card is swiped, meaning no late attendance is allowed, I have also added the ability of removing the master's card and the ability of removing everything on the system.

## General idea
* The general idea is to store the IDs on the internal EEPROM with a sequence, while at the same time store the *master* card, in a different location, this master card will allow for adding/removing IDs.
* The EEPROM consists of 1024 locations addressed from 0 to 1023, each location is one byte.
* The ID is 4 bytes long, and we have an extra byte to know whether this student has attended or not, so a total of 5 bytes for each student.
* We also have 5 bytes for a master card, 4 for master card ID and 1 extra byte to know whether is there a master card stored in the system or not.
* We also have 5 bytes for a checker card, 4 for checker card ID and 1 extra byte to know whether is there a checker card stored in the system or not.
* We also have one last byte used to keep track of how many students are registered in the system.
* This leaves 1013 locations, only 1010 are usable when divided by 5 this gives us locations for 202 students, which is not bad. 
* The general idea is all about location manipulation with simple equations, like for example the first student is stored in the location (5 + 5 + 1 – 1) which is (10), we subtracted (1) for the offset of counting from zero.
* Now to get the 3rd student, the equation is simple, 10 + (3 - 1) \* 5, which is 20.
* By knowing the total number of students it is easy to move across them to add or remove one of them.
* There exists two buttons for resetting the whole system’s data, or to only reset the master.
* The program will never continue without a valid master and checker in the system.

## Materials Required
1. 1 x Breadboard
2. 1 x Arduino Uno
3. 1 x Soldering station
4. 1 x RFID RC 522 Card reader Module
5. 6 x Smart Card chip
6. 1 x serial LCD module display
7. 3 x 220 Ohm, 0.25W Carbon Film Resistor
8. 2 x buttons
9. 3 x LEDS (any three color you like)
10. 1 x buzzer
11. Jumpers Wires

## Schematic/project layouts
Here is the logical connections schematic for the project:

![Connections schematic](https://github.com/asr586/CS-207-Project/blob/main/Images/Connections.png?raw=true)

And here is the Fritzing sketch with the components:

![Arduino sketch](https://github.com/asr586/CS-207-Project/blob/main/Images/arduino_sketch.png?raw=true)

Due to a breaking of my LCD that i used with a potentiometer. I bought a new lcd and have to make some changes in my design and code. i attached both the design and code as if it helps you. The Arduino code that I used for this design is available on this repo as "Images/Attendance_old".

![Arduino sketch, temporary broken version](https://github.com/asr586/CS-207-Project/blob/main/Images/arduino_sketch2.png?raw=true)


## Final product & demo
Final project, showing all components and their connections:

![Built project (A)](https://github.com/asr586/CS-207-Project/blob/main/Images/build_1.jpeg?raw=true)
![Built project (B)](https://github.com/asr586/CS-207-Project/blob/main/Images/build_2.jpeg?raw=true)

Video showing full project in motion:

[![Explanation video](https://img.youtube.com/vi/PqkwWktK4Xg/0.jpg)](https://www.youtube.com/watch?v=PqkwWktK4Xg)

## Credits
* Project coding and building: Arshdeep S.
* Original inspiration for this project: *team_chkr* on Arduino's Project Hub - https://create.arduino.cc/projecthub/team_chkr/rfid-based-smart-attendance-system-46b045

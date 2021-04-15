# CS-207-Project: Taking Attendance with the Help of a RFID Sensor

## Introduction
In this paper we describe a modification of a simple Arduino based RFID attendance system. The original project idea, which is everywhere on the internet, makes use of the RTC and SD Cards to log when did the students mark their attendance, but from my point view, they all lacked something very important, which is the ability of adding/removing students out of the system *easily*. I would like to keep the system very simple, instead of connecting to databases and so, I think that storing the IDs on the internal EEPROM is sufficient for the demonstration, maybe as an improvement we can instead grab the IDs from a database and still have the ability to add/remove IDs using a master card.

## Novel Contribution
My contribution is adding the ability of adding/removing IDs in the run-time using a special card called master card, swiping this card allows for another menu on the LCD to show, the system automatically detects if the swept card is stored or not by searching for it in the internal EEPROM. There is another contribution that is a doctor's card, which does not allow any attendance to be taken after his card is swiped, meaning no late attendance is allowed, I have also added the ability of removing the master's card and the ability of removing everything on the system.

## Motivation
Usually the normal attendance system is time consuming and may contain many errors due to bad handwriting and so on, this is because it is taken on paper, but using an RFID tag to which every student has his own tag, will make things much easier and faster, and the ability to add/remove IDs in the run-time without going back to the code, is much more efficient as well. And with the hope of integrating the system with Wi-Fi and storing the IDs on a database instead of internal EEPROM, this is going to be amazing.

## General idea
The general idea is to store the IDs on the internal EEPROM with a sequence, while at the same time store the *master* card, in a different location, this master card will allow for adding/removing IDs.
The EEPROM consists of 1024 locations addressed from 0 to 1023, each location is one byte.
The ID is 4 bytes long, and we have an extra byte to know whether this student has attended or not, so a total of 5 bytes for each student.
We also have 5 bytes for a master card, 4 for master card ID and 1 extra byte to know whether is there a master card stored in the system or not.
We also have 5 bytes for a doctor card, 4 for doctor card ID and 1 extra byte to know whether is there a doctor card stored in the system or not.
We also have one last byte used to keep track of how many students are registered in the system.
This leaves 1013 location, only 1010 are usable when divided by 5 this gives us locations for 202 students, which is not bad. 
The general idea is all about location manipulation with simple equations, like for example the first student is stored in the location (5 + 5 + 1 – 1) which is (10), we subtracted (1) for the offset of counting from zero.
Now to get the 3rd student, the equation is simple, 10 + (3 - 1) \* 5, which is 20.
By knowing the total number of students it is easy to move across them to add or remove one of them.
There exists two buttons for resetting the whole system’s data, or to only reset the master.
The program will never continue without a valid master and doctor in the system.

## Materials Required
1. Arduino UNO
2. RFID MFRC522
3. LCD 16x2
4. Red, blue and white LEDs.
5. Buzzer
6. Two pushbuttons

## Team Roles
If I were a part of a team, I would discuss what sort of roles each teammate would play. For example, perhaps one of us is not particularly experienced at programming but really good at documentation -- it'd probably be a good idea to have that person work on the github documentation / writing the paper. Etc. etc.
If I were a part of a team, maybe we could have researched into PCBs design and manufacturing so that we can order one for our project.
If I were a part of a team, maybe we could have done the Wi-Fi integration part with an actual database hosted on the internet.

## Summary
If this project turns out as proposed, it would be a huge contribution to the already made systems with SD-cards logging and RTCs, the only thing left would to be integrating both to a database, and voila we have an amazing system. 

## Schematic/project layouts
Here is the logical connections schematic for the project:

![Connections schematic](https://github.com/asr586/CS-207-Project/blob/main/Images/Connections.png?raw=true)

And here is the Fritzing sketch with the components:

![Arduino sketch](https://github.com/asr586/CS-207-Project/blob/main/Images/arduino_sketch.png?raw=true)

Due to a failing LCD module, I had to temporarily add a potentiometer and make some connections so that the program could work properly. After I got a new LCD, I could go back to the original Fritzing design. The Arduino code that I used for this temporary (non-final) design is available on this repo as "Images/Attendance_old"

![Arduino sketch2](https://github.com/asr586/CS-207-Project/blob/main/Images/arduino_sketch.png?raw=true)


## Final product & demo
Final project, showing all components and their connections:

![Built project (A)](https://github.com/asr586/CS-207-Project/blob/main/Images/build_1.jpeg?raw=true)
![Built project (B)](https://github.com/asr586/CS-207-Project/blob/main/Images/build_2.jpeg?raw=true)

Video showing full project in motion:

[![Explanation video](https://img.youtube.com/vi/PqkwWktK4Xg/0.jpg)](https://www.youtube.com/watch?v=PqkwWktK4Xg)

## Credits
Project coding and building: Arshdeep


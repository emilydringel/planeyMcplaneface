# Kinetic Sculptures: Planey McPlaneFace

For our Module 3 Kinetic Sculpture, my partner (Brian Ma) and I built a plane with propellers on the front wings and turning pieces on the back wings. The plane is controlled through a WiFi connection to http://134.122.113.13/bm3027 or http://134.122.113.13/server, which will run all 4 motors for 30 seconds and then stop.

You can learn more about our artistic vision and installation here: https://medium.com/@emilydringel/kinetic-sculptures-planey-mcplaneface-def8508d2264.

The piece includes a base plane shape made out of cardboard with a triangular body and two wings. In the front, DC brush motors are connected to propellers 3D printed with blue PLA filament using an edited model of a fidget spinner from TinkerCad. In the back, two MicroServos are connected to the wings using a combination of soldered paperclips, hot glue, and tape. All motors are held in place with zip ties. Each set of motors is connected to an ESP32 using a breadboard and a combination of soldered and solderless wires. The plane is held up by a set of 8 Q-tip “legs”. 

Code for each ESP32 is included in this repository. The WiFi connection is built on this API: https://github.com/mbennett12/kinetic-sculpture-webapi. The wiring set-up and motor code is based off of tutorials 18.1 and 17.2 from this textbook: https://drive.google.com/file/d/1GLq7iRY_I1D5Fgma1Rd3yVhNt3dg7K0T/view.

The 3D printing model .stl file is also included in this repository. 

# BlindZer
Wireless enabled Window Blinds Roller<br>
<img src="http://thingiverse-production-new.s3.amazonaws.com/renders/1b/4c/5b/7c/6b/9c8a4cd324916dcc4e85334bb4fedf4f_preview_featured.JPG"></img>
<br>

### Video DEMO
[to be inserted HERE]
### Assembly Preview
<a href="https://github.com/CCinCapital/BlindZer/blob/master/3D%20STL%20Files/Assemly_PREVIEW.STL">Click Here</a>
### Compatibility
    + Window Blinds with hexagonal Rod. [Rod thickness: 8mm edge to edge]
### Software Requirement
    + Arduino IDE with ESP8266 Board Support package installed
    + Python
### Hardware Requirement
    + Access to 3D printer
    + Node MCU v0.9
    + TowerPro MG996R Servo Pack [Modified for continous rotation]
    + IRL2703 or any NMOS that able to drive Id > 500ma when Vgs = 3.3v
    + TCST2103 Optical Interrupter
    + Few Resistors [2K x1 ; 2.2K x2; 3.3K x1; ]
    + Breadboard or Protoboard
    + Jumper wires
    + Scerw [M3 x1 ; Wall Mounting x2 ]
    + Soldering Station & Solder & Flux
    + Phillip screwdriver 
    + Wire cutter & Pilers
### Installation
#### Hardware
       ----- Circuit -----
    1. Build the circuit by refering ../Schematic/BlindZer_Sche.pdf
    
       ----- Part Assembly -----
    1. Navigate to "../3D STL Files/" folder of this project
    2. 3D print all the .STL part at 1:1 ratio [mm]
    3. Assemble the 3D printed part refer to the instruction in "../Assembling Instruction/"
    
       ----- Modify the Servo -----
    1. Modify the servo for continous rotation [Follow the instruction below]
<a href="http://www.instructables.com/id/How-to-hack-a-servo-for-continuous-rotation-Towe/?ALLSTEPS">Click here to learn "How to modify a servo for continous rotation?"</a>
#### Software
    1. Clone or Download this repository
    2. Install the runtime environment
    
       ----- Modify Arduino Code -----
    3. Navigate to the Code at ../Code/Main/Main.ino
    4. Modify the code (Change the SSID and PASSWORD parameter) 
    5. Upload code to your board and observe the Serial Terminal to find line "Use this URL to connect: http://xxx.xxx.xxx.xxx"
    6. Record the URL
  
       ----- Modify Python Code -----
    7. Navigate to the Script at ../Code/GUIhttpRequest.py   
    8. Edit the Code and change the [self.IP = "xxx.xxx.xxx.xxx"] parameter to the IP recorded (URL without 'http://')

## Usage
       ----- Powering the BlindZer -----
    1. Use USB cable (directly plug into the onboard usb jack of NodeMCU)
    2. Powering using 4 x AA battery through the 2Pin header (see schematic)

       ----- Control using Webpage -----    
    1. Open the URL in a WebBrwoser
    
       ----- Control using Python GUI -----
    1. Run the GUIhttpRequest.py
## TODO
       ----- Firmware -----
    - Remember current position
    - implement Sleep(); and Weakup();
    
       ----- Python -----
    - Automatically control the BlindZer based on weather/time/Sun Height information
## Contributing
    + Fork it
## Credits
    http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/  by Magesh Jayakumar
## License
<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.

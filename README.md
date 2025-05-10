# Arduino Cyclometer

 A screen that displays the speed and distance of a bike using an Arduino Uno.

## Link to simulate it (Tinkercad account needed)

https://www.tinkercad.com/things/kTiTbG7bsDE-cyclocomputer

 ### Tools Used

    Tinkercad for circuit simulation and prototyping
    
### Design

 ![image](https://github.com/user-attachments/assets/a1889269-d5a7-423b-8e23-b7e8c2941306)

### Wiring Schematic

![image](https://github.com/user-attachments/assets/e3855296-3ef9-4d89-927a-a9247edae880)


### Circuit Components

    1x Arduino Uno

    1x Ultrasonic distance sensor

    1x Red, 112 (0x70) 7-segment clock display

    1x push-buttons

    Breadboard + jumper wires

## How It Works
The code uses a constant representing the circumference of the bike wheel.
Typically, a cyclometer detects each wheel rotation using a magnet. In this simulation, the ultrasonic sensor replaces that mechanism. When the detected distance crosses a specific threshold (simulating a spoke passing in front of the sensor), the distance and speed variables are updated.

    Distance is cumulative, increasing by the wheel’s circumference on each detection.

    Speed is calculated using the time between two rotations (in m/s).

    The ultrasonic sensor works by emitting a sound wave and calculating the distance based on the time it takes for the echo to return, using the speed of sound.

    A button is used to switch between displaying speed and distance

### State Management

Several variables are used 
      
    totalDistance: keeps track of the total distance traveled by the bike.
    
    speed: current speed of the bike, calculated from the time between two rotations.
    
    lastRotationTime: stores the time when the last wheel rotation was detected, used for speed calculation.
    
    spokePreviouslyDetected: helps detect state changes by remembering if the spoke was already detected.
    
    displaySpeed: toggles the display between distance and speed modes when the button is pressed.
    
    lastButtonPress: records the last time the button was pressed, used for debouncing.
    
    debounceDelay: sets the minimum delay between valid button presses to filter out bouncing.
    
    lastButtonState: remembers the previous state of the button to detect rising or falling edges for clean toggles.    
                                                        
## What I learned

    I learned how to use an ultrasonic sensor to measure distance and internal pull-up resistor to detect input from button.

### Challenge I encountered:

    Button input was prone to bugs due to bouncing and false triggers. 
    Solution:
    In most cases, buttons are read with HIGH as the default state, but in this project, I used the internal pull-up resistor, so a button press pulls the pin LOW instead.
    This avoids floating inputs but makes the logic slightly more complex to implement correctly.

### Possible Improvements

    The display is limited to 4 digits, so the total distance cannot go beyond 999 meters. 
    One possible solution would be to switch units dynamically or use a larger display module to support more digits.

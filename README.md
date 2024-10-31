# Configuration File: config.txt

The configuration file will have one line each specifying the window size, font format, player, bullet specification ,, and enemy specifications.
Lines will be given in that order, with the following syntax:

## Window W H FL FS

- This line declares that th eSFML Window must be constructed with width W and height H, each of which will be integers. FL is the frame limit that the window should be set to, and FS will be an integer which specifies whether to display the application in full-screen mode (1) or not (0)

## Font F S R G B 
- This lines defines th efont which is to be used to draw text for this program. The format of the line is as follows:
    - Font file F           std::string (it will have no spaces)
    - Font Size S           int
    - RGB Color (R, G, B)   int, int, int

# Player Specification 
- Player SR CR S FR FG FB OR OG OB OT V
    - Shape Radius      SR          int
    - Collision Radius  CR          int
    - Speed             S           float
    - Fill Color        FR, FG, FB  int, int, int
    - Outline Color     OR, OG, OB  int, int, int
    - Outline Thickness OT          int
    - Shape Vertices    V           int

# Enemy Specification

- Enemy: SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
    - Shape Radius          SR          int
    - Collision Radius      CR          int
    - Min/Max Speed         SMIN, SMAX  float, float
    - Outline Color         OR, OG, OB  int, int, int
    - Outline Thickness     OT          int
    - Min/Max Vertices      VMIN, VMAX  int, int
    - Small Lifespan        L           int
    - Spawn Interval        SP          int

# Bullet Specification 

- Bullet: SR CR S FR FG FB OR OG OB OT V L
    - Shape Radius          SR          int
    - Collision Radius      CR          int
    - Speed                 S           float
    - Fill Color            FR, FG, FB  int, int, int
    - Outline Color         OR, OG, OB  int, int, int
    - Outline Thickness     OT          int
    - Shape Vertices        V           int
    - Lifespan              L           int

Hints: 59:19

      ===============================================================
      Simulation of MD (detector of LHAASO)  -  developed in Geant4 -
      ===============================================================
    
                             MD simulation
                             -------------

    This code simulate the MD detector of LHAASO (Cherenkov tanks). It
    is developed in Geant4 (an Object-Oriented Toolkit for simulation
    in High Energy Physic).
    It reproduces the geometry of the detector, includes parameters such
    as: reflectivity, absorption length, ...
    Finally, we obtain a ROOT file with the signal = f(time).



    1- GEOMETRY DEFINITION

    The MD detector is a  Cherenkov tank burried at 2.5 m. The tank has a 
    diameter of 6.8 and 1.2m high, filled with pure water. A PMT (photo-
    multiplier Tube : converting light in an electronic signal) is situates
    at the top-center of the tank (in contact with the water). The wall of
    the tank are in Tyvek (high reflective material ~ 98%).
    Severals parameters define the MD detector: 
    - the 2.5m above the tank (soil),
    - the thickness of the tank (concrete),
    - the Tyvek,
    - the water,
    - the PMT,


    2- XML FILES
    
    For more readability and to avoid a systematic compilation when changing
    a parameter, I use XML files to store the various paremeter of MD.
    (cf. xml repository).


    3- AN EVENT : The primary generator

    The primary kinematic consists of a single particle which hits the
    surface or the 2.5m layer above the tank. The type of the particle
    and its energy are set in the PrimaryGeneratorAction class, and can
    be changed via the G4 build-in commands of ParticleGun class (see
    the macros vis.mac).

    In addition one can choose randomly the impact point of the incident
    particle. The corresponding interactive command is built in
    PrimaryGeneratorMessenger class (see run2.mac).

    A RUN is a set of events.


    4- VISUALIZATION

    The Visualization Manager is set in the main().
    The initialisation of the drawing is done via a set of /vis/ commands
    in the macro vis.mac. This macro is automatically read from the main 
    in case of interactive running mode.

    By default, vis.mac opens a DAWNFILE, suitable for viewing in DAWN,
    and an OGLX for OpenGl.
    You can switch to other graphics systems by commenting out this line
    and instead uncommenting one of the other /vis/open statements, such as
    HepRepFile or HepRepXML.

    The DAWNFILE, HepRepFile and HepRepXML drivers are always available 
    (since they require no external libraries), but the OGLIX driver 
    requires:
    1- the visualisation & interfaces categories have been compiled
    with the environment variable G4VIS_BUILD_OPENGLX_DRIVER.
    2- MDCalib.cc has been compiled with G4VIS_USE_OPENGLX.

    The HepRepXML driver outputs a zip file that can be unzipped into 
    several individual HepRep files, each viewable in WIRED.



    5- HOW TO START ?

    - compile and link to generate an executable
    % cd MD
    % gmake

    - execute MD in 'batch' mode from macro files
    % MDCalib   vis.mac

    - execute MD in 'interactive mode' with visualization
    % MDCalib
    ....
    Idle>      ---> type your commands. For instance:
    Idle> /run/beamOn 
    ....
    Idle> /run/beamOn 10
    ....				
    Idle> /control/execute newgeom.mac
    ....
    Idle> exit



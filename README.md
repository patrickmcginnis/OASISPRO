# OASISPRO

![Screenshot of Device in Therapy](/Documentation/Screen_1.png "Screenshot of Device in Therapy")

Project - OasisPro medical device simulation

Name: Patrick McGinnis
ID: 101172946
	-ui (helped with design, designed the recording display)
	-program logic (implementing/running/recording a session, display of recordings, battery implementation)
	--documentation (use cases)
  
Name: Avetik Kocharyan
ID: 100793437
	-ui (design of scaffold, buttons and icons)
	-program logic (battery and various other displays, turn on/off, connecton test, button functionalities, saving prefered intensities)
	-documentation (uml class diagram, traceability matrix, readme)

Name: Tristan Dardani
ID: 101172946
	-session class integration
	-documentation (use cases, use case diagram, sequence diagrams)
	-helped with overall project design/structure

Name: Daniel Yang
ID: 101194970
	-ui (debugging of certain features)
	-program logic (battery, bar graph)
	-documentation (traceability matrix)


Instructions: (code compiles and runs)
	-> Build and run on QT creator in the course vm.

General Description:
	Purpose:
	The purpose of this project is to simulate a medical therapy device called OasisPro. The device functions to aleviate pain
	through providing the user with Cranio-Electro Stimulation (CES). The CES (session type determines the frequency/nature of the pulse)
	is administered to the user for a certain time period (called a group) using electrodes connected to earlobes. 
	The project simulates the physical device through an interface built using the Qt C++ framework.
	The device design in this project does not reconstitute all of the functionality of the original device (also has functionality
	that the original device does not have) but comes close to it.
	
	Differences from the actual device:	
	Some of the differences betweeen the actual device and this application simulating it include:
	1. Although the actual device has many sessions (including custom) and groups, this simulation has only 3 groups (20s, 45s and user defined) and
	   4 sessions ("ALPHA", "DELTA", "THETA", "MET"). The user defined group lets session go on until the user stops the session by turning off the device.
	2. The simulation has extra buttons: A button marked "T" that the user can use to change between groups (in the original device the power button
	   acheives this functionality) and a record button which enables the user to record details of ran sessions (in the original device there is no
	   recording feature).
	3. The simulation has a "Session on" icon which turns on when a session is started/is running. The original device does not have this icon and displays
	   a "softOn" annimation when a session is selected for running. We found the icon approach to be better for programming purposes as many annimations 
	   increase the chance of interference between various displays (often time same icons are used in the original device to display different things such
	   as battery display, intensity, connection status and etc. Please see the "Issues" section of this document for further explanation). 
	4. The original device automatically turns off when a session is not selected withing 2 minutes. Though it would have been easy to implement this feature
	   in the simulation (by using a Qt singleshot timer), we decided not to. The decision was based on the fact that the natural draining of the battery 
	   would essentially acheve this. Also, since in the simulation we consider a 20 second session to be a 20 min session and 45 second session to be a 
	   45 min session, turning the device off after 2 seconds would not give enough time for the user to go through sessions and groups and make a selection.
	5. In the original device all sessions end with a "softOff" animation. We made a slight change to this in the our application. In our application,
	   if the session ends naturally (i.e. to completion 20s or 45s) "softOff" does not take place. Rather, just the "Session On" icon turns off. On the other
	   hand if the session is interrupted (user presses the power button while the session is still running), "SoftOff" animation takes place (notice that all
	   sessions running for a "user defined" time group must end in "SoftOff" since the only way to end them is to press the power button or wait till the 
           battery runs out). This decision was made to distinguish between interrupted sessions and sessions that go to completion.
	6. In the original device some button functionalities invlove pressing and holding the button for a few seconds to achieve a task. In this program this feature was
 	   not implemented. All buttons function by just pressing and releasing them (the released() signal for buttons in signal and slots mechanism).
	
	All these changes were discussed with the professor and approved by him.

	General use instructions:
	The ui created by Qt resembles the physical device with the changes discussed above. It has buttons that are clickable. In addition, the ui has an 
	area (admin area of sorts) designed for simulating the charging of the battery and attachment of electrodes to the earlobes. The battery can be "charged" 
        by essentially setting its value in the QDoubleSpinBox when the device is off. Connection of the electrodes to the earlobes is simulated by the user selecting the
	connection status from the QComboBox (three options: "No  connection", "Okay connection", "Excellent connection"). The physical device is simulated by
	a png scaffold that contains buttons (pngs), icons (pngs) and a display (QList) where recorded sessions can be viewed.
	A user can start using the device by checking the battery level then turning it on by pressing the power button. Subsequently, the user can change betweeen
	groups (times for therapy) by pressing the button marked by "T" (default group is 20s). The "user designed" group lets the session run for indefinate amount  
	of time. In this case the session is stopped either when battery runs out or when the user turns off the device. One of the four available sessions can be chosen by pressing 
	the up/down arrow buttons (default session is "ALPHA"). The select button starts the chosen session if connection is not "No  connection". While the session is running
	the up/down arrow buttons can be used to change the intensity of session, and the select button can be used to save a prefered intensity for that particular
	session. The battery depletion of the device is dependent on time (machine is on OR session is running), intensity and contact of the electrodes. The battery
	level can be monitored in two ways (it is displayed when the device is turned on and subsequently displayed periodically at pre-set battery levels during the 
	running of a session and it can be monitored outside of the physical device (in the admin area) in a QDoubleSpinBox). QDoubleSpinBox way of tracking the battery is more percise since
	the value of the battery (value between 0 and 100) is displayed/updated in real-time. The connection test is displayed as described in the manual of Oasis Pro. In this application,
	the test is done every time a session is started. The sessions go on for their predetermined amount of time (groups: 20s, 45s, user defined). User defined sessions 
	last indefinatly until the user turns the machine off or the battery runs out. When session is on, "session on" icon turns on and when it stops, the icon turns off.
	When the session is stopped by the user turning the device off, a "Soft off" animation takes place (bar graph scrolling from 8 to 1). The device will shut down if battery runs out
	(independent if session is running or not).
	   


Content of Submission:
	-> This readme.
	-> Documentation
	-> Code files

	Documentation:
		Use Cases, Use Case Diagram, Sequence Diagrams, Traceability Matrix, UML class diagram.
		
	Code files:
		Classes Involved (.h and .cpp files): Mainwindow.cpp, session.cpp, Mainwindow.h, session.h, main.cpp
			
		Other: res folder (all the images used for the ui: pngs), mainwindow.ui, resources.qrc TeamProject.pro, TeamProject.pro.user
			

Explantion of the design:	
	Patterns implemented: 
		Observer: (Qt signals and slots):This pattern was extensively used to connect and update the various components of the system.
		State: the Mainwindow class changes its behaviour when internal state changes (attributes such as sessonStatus, powerStatus) are altered.
		Singleton: only one instance of MainWindow class is created.
		Antipattern: code is not modular and all of it is concentrated in 2 classes.
	
	The program consists of two classes that are responsible for simulating the function of the OasisPro device (please see UML diagram). The MainWindow class essentially represents
	all the wiring/programming inside the actual OasisPro device. The session class is responsible for the 4 different therapies that are available on the 
	current simulation of the device. The MainWindow class has a QList that holds four instances of the sessions (pointers to them). This design (composition) allows to save
	a prefered intensity for each session by having an appropriate attribute (preferedIntensity) inside the session class.
	The program logic is facilitiated by largely boolean atributes that represent the state of the device at any given instance. Based on the state of these values, the system 
	decides how to behave (which buttons will be enabled/disabled, which icons will be turned on/off, which timers will be started/stopped and etc.) (please see sequence diagrams).
	Battery depletion is simulated by a function connected to a QTimer instance that calls this function every second when the device is turned on. The various attributes of the
	program (such as power status, session status, connection status) determine the rate at which the battery (an attribute of type double) is depleted inside this function
	every second. The simulation of a the running of a session is also achieved with the help of a QTimer instance. Every time a session is started, this timer is started 
	which calls a function every second. The function keeps track of elapsed time since a session is started. This function also does the recording of session details (again 
	based on a boolean value informing the system if recording is required). The sequence diagrams demonstrate the flow of logic (and function calls) during the described activites 
	above (please see sequence diagrams).

	 

Issues:
	Interference of the displays of various objects/functions:		
		The nature of the device (and timers in qt) makes it possible interfence between displays of various parameters of
		the machine to occur.
		Sometimes the connection test result interferes with the battery display. There may also be interferences 
		between intensity display and battery display. This was ran by the professor who acknowledged the issue and approved the design.

	Timing of pressing the buttons:
		At times when buttons are pressed very quickly (> 1/sec), the display does not catch up fast enough. This issue was also brought up to the 
		professor, who aknowledged it and said it was fine.
	
	Pressing lots of buttons quickly:
		This may result in laggyness. Again, this issue was brought up to the professor, who aknowledged it and said it was fine.


Improvements:
	Make code more modular:
		Currently the program is composed of two classes (excluding QT objects/classes). It would be a better design to break the code up
		into appropriate classes. This way if extra features are added to the device in the future it would be easier to modify the code 
		to accomodate these additions.
	Follow the Model-View-Controller principle: 
		Design the application in such a way where the ui (view) is separate from the device. This will allow for easier modification of the 
		device logic in case a need arises to.
	Implement Qt layouts for the ui:
		This would let the system to respond better when the mainwindow is resized by the user. Currently, the window is not responsive.


Test Cases (Tested scenarios and steps how to test - all working):
	These steps were followed to test the various features/requirments of our program simulating the OasisPro medical device.

1. Running a session for a preditermined amount of time (20 or 45 sec):
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on.
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running.
		-The session will end after the selected time.
		-"Session On" icon will turn off.
		
		
2. Running a session for a preditermined amount of time (20 or 45 sec) + record:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on.
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".
	-Press the record button (note this button can be pressed anytime before the session ends and it will function).		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running.
		-The session will end after the selected time.
		-"Session On" icon will turn off.
		-The details of the ran session will be displayed on the historyList QListWidget.

3. Running a session for a preditermined amount of time (20 or 45 sec) + end early by pressing the power button:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on.
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Press the power button while the session is running.
		-The session will end.
		-"Session On" icon will turn off.
		-"Soft of Animation" will happen.
		-The device will turn off by resetting all of its icons/LEDs
	

4. Running a session for a preditermined amount of time (20 or 45 sec) + record + end early by pressing the power button:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on.
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".	
	-Press the record button (note this button can be pressed anytime before the session ends and it will function).	
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Press the power button while the session is running.
		-The session will end.
		-"Session On" icon will turn off.
		-The details of the ran session will be displayed on the historyList QListWidget.
		-"Soft of Animation" will happen.
		-The device will turn off by resetting all of its icons/LEDs

5. Running a session for a user-defined amount of time:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last ivon with a "+" logo).
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running.
	-Turn off the session (otherwise it will go indefinatly until battery runs out) by pressing the power button.
		-The session will end.
		-"Session On" icon will turn off.
		-"Soft of Animation" will happen.
		-The device will turn off by resetting all of its icons/LEDs.

6. Running a session for a user-defined amount of time + record:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last one with a "+" logo).
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".
	-Press the record button (note this button can be pressed anytime before the session ends and it will function).		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running.
	-Turn off the session (otherwise it will go indefinatly until battery runs out) by pressing the power button. //note if battery runs out the session WILL NOT be recorded
		-The session will end.
		-"Session On" icon will turn off.
		-The details of the ran session will be displayed on the historyList QListWidget.
		-"Soft of Animation" will happen.
		-The device will turn off by resetting all of its icons/LEDs

7. Running a session and battery running low/out:
	-Prior to turning on the device manually change the battery to low (15-30) this way won't have to wait long for testing.
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last one with a "+" logo). //select user defined to make sure session does not end before batery is 0
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running.
		-The session continue to run.
		-Once battery is bellow 20, two bars will flash and turn off.
		-Once battery is bellow 10, one bars will flash and turn off.
		-Once battery is bellow 5, one bars will flash and turn off.
		-The device run for a little longer until the battery is 0.
		-"Session On" icon will turn off.
		-The device will turn off by resetting all of its icons/LEDs.


8. Interrupting a session by disconnecting the electrodes in the middle of a session then restarting/resuming the session:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on. //recommend choosing 20 seconds since easier to test
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".
	-Press the record button (note this button can be pressed anytime before the session ends and it will function). //easier to test if recording is on	
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Simulate the disconnection of the electrodes by choosing the "No connection" from the QComboBox and wait.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn off.
		- The device will wait until the electrodes are disconnected or battery runs out.
	-Simulate the reconnection of the electrodes by choosing the "Okay connection" or "Excellent Connection" from the QComboBox and wait.
		-Connection test is perforemed and displayed and session will resume.
		-"Session On" icon will turn on.
	-Wait for the session to finish (if the group selected was not user defined).
		-The session will end after the selected time.
		-"Session On" icon will turn off.
		-The details of the ran session will be displayed on the historyList QListWidget.
		
		

9. Saving a prefered intensity for a particular session:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times).
		-Appropriate group icons turn on/off to indicate on which group the user is currently on.
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".
	-Press the record button (note this button can be pressed anytime before the session ends and it will function). //record because easier to test	
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Change the intensity by using the up/down buttons.
		-The intensity is displayed on the bar graph for a short period of time (current intensity will blink).
	-Press the select button to save the intensity for that particular session and wait for session to end.
		-The session will end after the selected time.
		-"Session On" icon will turn off.
		-The details of the ran session will be displayed on the historyList QListWidget. 
	-Turn off the device, turn it back on and select the previously ran session as described above.
		-The saved intensity will be used as default. (This can be observed by changing the intensity or waiting for session to end and recording to be displayed)

10. Obsereving the periodic display of battery during runnig of a session:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last one with a "+" logo). //easier to observe battery display this way since session runs indef
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running and observe the bar graph.
		-At pre-determined battery levels (periodically), the bar graph will display the current battery level.
	-Turn off the session (otherwise it will go indefinatly until battery runs out) by pressing the power button.
		-The session will end.
		-"Session On" icon will turn off.
		-"Soft of Animation" will happen.
		-The device will turn off by resetting all of its icons/LEDs.


11. Obsereving the battery running out while session is running:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last one with a "+" logo). //easier to observe battery display this way since session runs indef
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".		
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running and observe the bar graph.
		-At pre-determined battery levels (periodically), the bar graph will display the current battery level.
		-At 20% battery life the bar graph will blink (2 bottom bars) for a short period of time (session will go on).
		-At 10% battery life the bar graph will blink (1 bottom bars) for a short period of time (session will go on).
		-At 5% battery life the bar graph will blink (1 bottom bars) for a short period of time.
		-The device will turn off once 0% battery is reached (session will be interrupted and stopped).

12. Obsereving the battery running out while session is running + recording is on:
	-Turn on the decive (make sure the battery is not 0).
		-Battery is displayed by the device.
		-By defaul the 20s group and "ALPHA" session are set/selected (their icons turn on).
	-Use "T" button to change through groups (times) until user defined icon is lit (last one with a "+" logo). //easier to observe battery display this way since session runs indef
		-User defined group icon will turn on (all other ones will be off).
	-Use up/down arrow buttons to change through sessions.
		-Appropriate session icons turn on/off to indicate on which session the user is currently on.
		-Appropriate bar graph icons turn on/off to indicate on which session (number 1 to 4) the user is currently on.
	-Set the QComboBox to "Okay Connection" or "Excellent Connection".
	-Press the record button (note this button can be pressed anytime before the session ends and it will function).	
	-Press the select button to start the session.
		-Connection test is perforemed and displayed.
		-"Session On" icon will turn on.
	-Wait while the session is running and observe the bar graph.
		-At pre-determined battery levels (periodically), the bar graph will display the current battery level.
		-At 20% battery life the bar graph will blink (2 bottom bars) for a short period of time (session will go on).
		-At 10% battery life the bar graph will blink (1 bottom bars) for a short period of time (session will go on).
		-At 5% battery life the bar graph will blink (1 bottom bars) for a short period of time.
		-The device will turn off once 0% battery is reached (session will be interrupted and stopped).
		-The device will NOT record the session details of the session. //the resoning behind this implementation is that the battery has ran out and thus the device does not have energy for recording.

	


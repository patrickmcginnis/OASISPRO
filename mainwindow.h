#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QList>
#include <QLabel>
#include <QTimer>
#include <QString>

#include <QElapsedTimer>
#include <QDeadlineTimer>
#include <QDebug>
#include <QDateTime>

#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool powerStatus; //1 = device is on, 0 = device is off
    bool sessionStatus; //1 = session is selected, 0 = session is not selected
    bool connectionTestStatus; //1 = test done, 0 = test not done

    short int currentGroup; //0 = 20 min, 1 = 45 min, 2 = user designed
    short int currentSession; //0 = alpha, 1 = delta, 2 = theta, 3 = MET
    int currentConnection; //0 = no, 1 = okay, 2 = excellent
    int isRecording; // 0 is no 1 is yes

    int intensity; //can take values between 1 and 8 only
    double battery; //can take values between 0.0 and 100.0 only
    QTimer *sTimer;
    QTimer *bTimer;
    int elapsedTime;

    QList<session*> sessionlist; //holds all the sessions

    //battery display switches to achieve one time periodic display of battery level during session
    bool batSwith1; // when 0 does not display battery when 1 displayes battery between 60 and 80
    bool batSwith2; // when 0 does not display battery when 1 displayes battery between 40 and 60
    bool batSwith3; // when 0 does not display battery when 1 displayes battery between 20 and 40
    bool batSwith4; // when 0 does not display battery when 1 displayes battery between 10 and 20
    bool batSwith5; // when 0 does not display battery when 1 displayes battery between 5 and 10
    bool batSwith6; // when 0 does not display battery when 1 displayes battery between 0 and 5

    void resetBatSwitches(); //sets the value of all the swithes to 1 when the device is first turned on or off

    void loadSession(); //creates the sessions and populates the sessionlist with them

    //void turnOnLED(); //turns on the LED above the power button
    //void turnOffLED(); //turns off the LED above the power button
    void resetGroupIcons();
    void resetSessionIcons();
    //void resetBarGraphIcons();

    void displayBatteryLevel(); //show current battery state
    void displayConnectionResult(); //show current connection status

    void displaySession(); //show which session the user is currently on
    //void displaySessionOnStatus(); //display the label that indicates that a session is running
    void displayGroup(); //show which group/time option the user is currently on

    void displayIntensity(); //show which intensity level the user is currently on

    void displayUpDownScrollBar(); //no connection status display

    void softOffAnimation(); //offing the device when the session is running (not used yet)
    void recordTherapy(); //button function to update the list with a history


private slots:
    void changePowerStatus(); //turn on/off the device
    void selectGroup(); //select the time/group for the therapy
    void upArrowButtonOperations(); //select session and intensity through the arrow buttons
    void downArrowButtonOperations(); //select session and intensity through the arrow buttons
    void selectButtonOperations(); //select the session/save intensity

    void resetBarGraphIcons();

    void resetLR(); //turn off the L and R labels

    //setting the bars
    void setBarOne();
    void setBarTwo();
    void setBarThree();
    void setBarFour();
    void setBarFive();
    void setBarSix();
    void setBarSeven();
    void setBarEight();

    //blinking of the bar graph labels
    void blinkBarOne();
    void blinkBarTwo();
    void blinkBarThree();
    void blinkBarFour();
    void blinkBarFive();
    void blinkBarSix();
    void blinkBarSeven();
    void blinkBarEight();
    void blinkTwoBottomBars();

    void blinkTwoTopBars();//for connection status display

    void blinkTwoMES();//for connection display


    void changeBatteryLevel(double newval); //change battery from the display (admin)

    void updateCurrentConnection(int val); //change connection from the display (admin)
    void turnOnLED(); //turns on the LED above the power button
    void turnOffLED(); //turns off the LED above the power button

    void useTimer(); //timer to track a current running session

    void drainBattery(); //selfexplanitory
    void startRecording();

    void displaySessionOnStatus(); //display the label that indicates that a session is running

};
#endif // MAINWINDOW_H

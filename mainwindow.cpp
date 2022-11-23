#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSession();

    //timer to track session
    sTimer = new QTimer(this);
    connect(sTimer, SIGNAL(timeout()), this, SLOT(useTimer()));
    elapsedTime = 0;

    bTimer = new QTimer(this);
    connect(bTimer, SIGNAL(timeout()), this, SLOT(drainBattery()));


    //initial setup of attribtes at instantiation#
    powerStatus = 0;
    sessionStatus = 0;
    connectionTestStatus = 0;
    isRecording = 0;


    currentGroup = 0;
    currentSession = 0;
    currentConnection = 0;
    intensity = 1;

    battery = 100.0;

    //button connections
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(changePowerStatus()));
    connect(ui->changeGroupButton, SIGNAL(released()), this, SLOT(selectGroup()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(upArrowButtonOperations()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(downArrowButtonOperations()));
    connect(ui->selectButton, SIGNAL(released()), this, SLOT(selectButtonOperations()));
    connect(ui->recordButton, SIGNAL(released()), this, SLOT(startRecording()));

    //connect the admin spinbox holding the battery level to the battery attribute
    //connect(ui->batdoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double d){battery = d;});
    //doing it the function way (bottom) gives more freedom to do extra stuff as opposed to the top way
    connect(ui->batdoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);
    //connect the admin spinbox holding the connection status to the currentConnection attribute
    connect(ui->concomboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateCurrentConnection);
}

MainWindow::~MainWindow()
{
    delete sessionlist[0];
    delete sessionlist[1];
    delete sessionlist[2];
    delete sessionlist[3];

    delete ui;
}


void MainWindow::loadSession()
{
    session* s1;
    session* s2;
    session* s3;
    session* s4;

    s1 = new session(0, 1);
    s2 = new session(1, 1);
    s3 = new session(2, 1);
    s4 = new session(3, 1);

    sessionlist.append(s1);
    sessionlist.append(s2);
    sessionlist.append(s3);
    sessionlist.append(s4);
}

void MainWindow::changePowerStatus()
{
    if(powerStatus == 0 && battery > 0.0){ //turn on

        powerStatus = 1;
        bTimer->start(1000);
        turnOnLED();

        resetBatSwitches();

        displayBatteryLevel();
        ui->G1->setStyleSheet("QLabel {image: url(:/res/thirty.png); background-color : green; }");
        ui->S1->setStyleSheet("QLabel {image: url(:/res/alpha.png); background-color : green; }");
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->historyList->setEnabled(true);
    }
    else if (powerStatus == 1 && battery > 0.0){ //turn off
        if(sessionStatus == 1){
            recordTherapy();
            displaySessionOnStatus();
            softOffAnimation();
            powerStatus = 0;
            bTimer->stop();
            sessionStatus = 0;
            connectionTestStatus = 0;


            currentGroup = 0;
            currentSession = 0;
            resetGroupIcons();
            resetSessionIcons();
            isRecording = 0;
            ui->recordingLabel->setDisabled(true);
            ui->historyList->setDisabled(true);

            resetBatSwitches();
            return;
        }

        powerStatus = 0;
        bTimer->stop();
        sessionStatus = 0;
        connectionTestStatus = 0;


        currentGroup = 0;
        currentSession = 0;


        resetGroupIcons();
        resetSessionIcons();
        resetBarGraphIcons();
        isRecording = 0;
        ui->recordingLabel->setDisabled(true);
        ui->historyList->setDisabled(true);
        displaySessionOnStatus();
        turnOffLED();

        resetBatSwitches();
    }
}

void MainWindow::selectGroup()
{
    if(sessionStatus == 0 && powerStatus == 1){ //after choosing session can't change group
        currentGroup = currentGroup + 1;
        if(currentGroup > 2){
            currentGroup = 0;
        }
        displayGroup();
    }
}

void MainWindow::upArrowButtonOperations()
{
    if(sessionStatus == 0 && powerStatus == 1){ //change session
        currentSession = currentSession + 1;
        if(currentSession > 3){
            currentSession = 0;
        }
        displaySession();
    }
    else if(sessionStatus == 1 && powerStatus == 1 && connectionTestStatus == 1 && currentConnection != 0){ //change intensity
        intensity = intensity + 1;
        if(intensity > 8){
            intensity = 8;
        }
        displayIntensity();
    }
}

void MainWindow::downArrowButtonOperations()
{
    if(sessionStatus == 0 && powerStatus == 1){ //change session
        currentSession = currentSession - 1;
        if(currentSession < 0){
            currentSession = 3;
        }
        displaySession();
    }
    else if(sessionStatus == 1 && powerStatus == 1 && connectionTestStatus == 1 && currentConnection != 0){ //change intensity
        intensity = intensity - 1;
        if(intensity < 1){
            intensity = 1;
        }
        displayIntensity();
    }
}

void MainWindow::selectButtonOperations()
{
    if(powerStatus == 1 && sessionStatus == 0 && battery > 0.0){ //start session

        displayConnectionResult();
        connectionTestStatus = 1;
        if(currentConnection == 0) {
            return;
        }
        sessionStatus = 1;
        sTimer->start(1000);

        intensity = sessionlist[currentSession]->getPreferedIntensity();
        //displayIntensity(); //interferes with the conection test so lets not have it here
    }
    else if(powerStatus == 1 && sessionStatus == 1 && battery > 0.0){ //set prefered intensity of a session
        sessionlist[currentSession]->setPreferedIntensity(intensity);
    }

}

void MainWindow::displayBatteryLevel()
{

    if(battery <= 100.0 && battery > 90.0){
        //8bars
        //QElapsedTimer timer;
        //timer.start();
        //while(!timer.hasExpired(3000)){
        //qDebug() << ui->IL1->styleSheet();
        //}

        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : red; }");

    }
    else if(battery <= 90.0 && battery > 80.0){
        //7bars
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");

    }
    else if(battery <= 80.0 && battery > 70.0){
        //6bars
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");

    }
    else if(battery <= 70.0 && battery > 60.0){
        //5bars
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");

    }
    else if(battery <= 60.0 && battery > 40.0){
        //4bars
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");

    }
    else if(battery <= 40.0 && battery > 20.0){
        //3bars
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : white; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");

    }
    else if(battery <= 20.0 && battery > 10.0){
        //2bars need to be blinking
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : white; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : white; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");
        QTimer::singleShot(1000, this, SLOT(blinkTwoBottomBars()));
        QTimer::singleShot(1200, this, SLOT(blinkTwoBottomBars()));
        QTimer::singleShot(1400, this, SLOT(blinkTwoBottomBars()));
        QTimer::singleShot(1600, this, SLOT(blinkTwoBottomBars()));
        QTimer::singleShot(1800, this, SLOT(blinkTwoBottomBars()));
        QTimer::singleShot(2000, this, SLOT(blinkTwoBottomBars()));

    }
    else if(battery <= 10.0 && battery > 0.0){
        //1bar need to be blinking
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : white; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : white; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : white; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");
        QTimer::singleShot(1000, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1200, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1400, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1600, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1800, this, SLOT(blinkBarOne()));
        QTimer::singleShot(2000, this, SLOT(blinkBarOne()));
    }
    QTimer::singleShot(3000, this, SLOT(resetBarGraphIcons()));
}

void MainWindow::displayConnectionResult()
{
    resetBarGraphIcons();
    QTimer::singleShot(1000, this, SLOT(blinkTwoMES()));
    QTimer::singleShot(1200, this, SLOT(blinkTwoMES()));
    QTimer::singleShot(1400, this, SLOT(blinkTwoMES()));
    QTimer::singleShot(1600, this, SLOT(blinkTwoMES()));
    QTimer::singleShot(1800, this, SLOT(blinkTwoMES()));
    QTimer::singleShot(2000, this, SLOT(blinkTwoMES()));

    if(currentConnection == 0){
        ui->Right->setStyleSheet("QLabel {\ncolor : red;\n}"); //turn on R label
        ui->Left->setStyleSheet("QLabel {\ncolor : red;\n}"); //turn on R label
        //blink the top two bars
        QTimer::singleShot(1000, this, SLOT(blinkTwoTopBars()));
        QTimer::singleShot(1200, this, SLOT(blinkTwoTopBars()));
        QTimer::singleShot(1400, this, SLOT(blinkTwoTopBars()));
        QTimer::singleShot(1600, this, SLOT(blinkTwoTopBars()));
        QTimer::singleShot(1800, this, SLOT(blinkTwoTopBars()));
        QTimer::singleShot(2000, this, SLOT(blinkTwoTopBars()));
        //graph bar scroll up and down
        displayUpDownScrollBar();
        //...//
        QTimer::singleShot(5550, this, SLOT(resetLR())); //turn off L/R labels

    }
    else if (currentConnection == 1){
        ui->Right->setStyleSheet("QLabel {\ncolor : yellow;\n}"); //turn on R label
        ui->Left->setStyleSheet("QLabel {\ncolor : yellow;\n}"); //turn on R label
        //turn on the middle bar labels - yellow
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        QTimer::singleShot(3000, this, SLOT(resetBarGraphIcons())); //reset the bar graph after 3 seconds
        QTimer::singleShot(4550, this, SLOT(resetLR())); //turn off L/R labels
    }
    else if (currentConnection == 2){
        ui->Right->setStyleSheet("QLabel {\ncolor : green;\n}"); //turn on R label
        ui->Left->setStyleSheet("QLabel {\ncolor : green;\n}"); //turn on R label
        //turn on the bottom bar labels - green
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        QTimer::singleShot(3000, this, SLOT(resetBarGraphIcons())); //reset the bar graph after 3 seconds
        QTimer::singleShot(4550, this, SLOT(resetLR())); //turn off L/R labels
    }

}

void MainWindow::displaySession()
{
    resetSessionIcons();
    resetBarGraphIcons();
    if(currentSession == 0){
        ui->S1->setStyleSheet("QLabel {image: url(:/res/alpha.png); background-color : green; }");
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
    }
    if(currentSession == 1){
        ui->S2->setStyleSheet("QLabel {image: url(:/res/delta.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        //ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
    }
    if(currentSession == 2){
        ui->S3->setStyleSheet("QLabel {image: url(:/res/theta.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        //ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        //ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
    }
    if(currentSession == 3){
        ui->S4->setStyleSheet("QLabel {image: url(:/res/met.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        //ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        //ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        //ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
    }
}

void MainWindow::displayGroup()
{
    resetGroupIcons();
    if(currentGroup == 0){
        ui->G1->setStyleSheet("QLabel {image: url(:/res/thirty.png); background-color : green; }");
    }
    if(currentGroup == 1){
        ui->G2->setStyleSheet("QLabel {image: url(:/res/fortyfive.png); background-color : green; }");
    }
    if(currentGroup == 2){
        ui->G3->setStyleSheet("QLabel {image: url(:/res/userdef.png); background-color : green; }");
    }
}

void MainWindow::displayIntensity()
{
    resetBarGraphIcons();
    if(intensity == 1){
        //blinking of current intensity - 1
        QTimer::singleShot(1000, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1250, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1500, this, SLOT(blinkBarOne()));
        QTimer::singleShot(1750, this, SLOT(blinkBarOne()));
        QTimer::singleShot(2000, this, SLOT(setBarOne()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display

    }
    else if(intensity == 2){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        //blinking of current intensity - 2
        QTimer::singleShot(1000, this, SLOT(blinkBarTwo()));
        QTimer::singleShot(1250, this, SLOT(blinkBarTwo()));
        QTimer::singleShot(1500, this, SLOT(blinkBarTwo()));
        QTimer::singleShot(1750, this, SLOT(blinkBarTwo()));
        QTimer::singleShot(2000, this, SLOT(setBarTwo()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
    else if(intensity == 3){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        //blinking of current intensity - 3
        QTimer::singleShot(1000, this, SLOT(blinkBarThree()));
        QTimer::singleShot(1250, this, SLOT(blinkBarThree()));
        QTimer::singleShot(1500, this, SLOT(blinkBarThree()));
        QTimer::singleShot(1750, this, SLOT(blinkBarThree()));
        QTimer::singleShot(2000, this, SLOT(setBarThree()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
    else if(intensity == 4){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        //blinking of current intensity - 4
        QTimer::singleShot(1000, this, SLOT(blinkBarFour()));
        QTimer::singleShot(1250, this, SLOT(blinkBarFour()));
        QTimer::singleShot(1500, this, SLOT(blinkBarFour()));
        QTimer::singleShot(1750, this, SLOT(blinkBarFour()));
        QTimer::singleShot(2000, this, SLOT(setBarFour()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
    else if(intensity == 5){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        //blinking of current intensity - 5
        QTimer::singleShot(1000, this, SLOT(blinkBarFive()));
        QTimer::singleShot(1250, this, SLOT(blinkBarFive()));
        QTimer::singleShot(1500, this, SLOT(blinkBarFive()));
        QTimer::singleShot(1750, this, SLOT(blinkBarFive()));
        QTimer::singleShot(2000, this, SLOT(setBarFive()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display

    }
    else if(intensity == 6){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        //blinking of current intensity - 6
        QTimer::singleShot(1000, this, SLOT(blinkBarSix()));
        QTimer::singleShot(1250, this, SLOT(blinkBarSix()));
        QTimer::singleShot(1500, this, SLOT(blinkBarSix()));
        QTimer::singleShot(1750, this, SLOT(blinkBarSix()));
        QTimer::singleShot(2000, this, SLOT(setBarSix()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
    else if(intensity == 7){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        //blinking of current intensity - 7
        QTimer::singleShot(1000, this, SLOT(blinkBarSeven()));
        QTimer::singleShot(1250, this, SLOT(blinkBarSeven()));
        QTimer::singleShot(1500, this, SLOT(blinkBarSeven()));
        QTimer::singleShot(1750, this, SLOT(blinkBarSeven()));
        QTimer::singleShot(2000, this, SLOT(setBarSeven()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
    else if(intensity == 8){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
        //blinking of current intensity - 8
        QTimer::singleShot(1000, this, SLOT(blinkBarEight()));
        QTimer::singleShot(1250, this, SLOT(blinkBarEight()));
        QTimer::singleShot(1500, this, SLOT(blinkBarEight()));
        QTimer::singleShot(1750, this, SLOT(blinkBarEight()));
        QTimer::singleShot(2000, this, SLOT(setBarEight()));
        QTimer::singleShot(2100, this, SLOT(resetBarGraphIcons())); //turn off the intensity display
    }
}

void MainWindow::displayUpDownScrollBar() //this is for the "no connection" connection status
{
    //resetBarGraphIcons();
    QTimer::singleShot(2050, this, SLOT(blinkBarOne()));
    QTimer::singleShot(2250, this, SLOT(blinkBarTwo()));
    QTimer::singleShot(2450, this, SLOT(blinkBarThree()));
    QTimer::singleShot(2650, this, SLOT(blinkBarFour()));
    QTimer::singleShot(2850, this, SLOT(blinkBarFive()));
    QTimer::singleShot(3050, this, SLOT(blinkBarSix()));
    QTimer::singleShot(3250, this, SLOT(blinkBarSeven()));
    QTimer::singleShot(3450, this, SLOT(blinkBarEight()));


    QTimer::singleShot(3850, this, SLOT(blinkBarEight()));
    QTimer::singleShot(4050, this, SLOT(blinkBarSeven()));
    QTimer::singleShot(4250, this, SLOT(blinkBarSix()));
    QTimer::singleShot(4450, this, SLOT(blinkBarFive()));
    QTimer::singleShot(4650, this, SLOT(blinkBarFour()));
    QTimer::singleShot(4850, this, SLOT(blinkBarThree()));
    QTimer::singleShot(5050, this, SLOT(blinkBarTwo()));
    QTimer::singleShot(5250, this, SLOT(blinkBarOne()));

}

void MainWindow::displaySessionOnStatus()
{
    if(sessionStatus == 1 && currentConnection != 0){
        ui->sessionOnLebel->setStyleSheet("QLabel {\ncolor : green;\n}"); //turn on Session On icon
    }
    else{
        ui->sessionOnLebel->setStyleSheet("QLabel {\ncolor : white;\n}");
    }
}

void MainWindow::changeBatteryLevel(double newval)
{
    battery = newval;
}

void MainWindow::updateCurrentConnection(int val)
{
    currentConnection = val;
    //qDebug() << currentConnection;
    if(powerStatus == 1 && connectionTestStatus == 1){
        displayConnectionResult();
    }
}

//setting individual bars so they can be used in SLOTS
void MainWindow::setBarOne(){
    ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
}
void MainWindow::setBarTwo(){
    ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
}
void MainWindow::setBarThree(){
    ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
}
void MainWindow::setBarFour(){
    ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
}
void MainWindow::setBarFive(){
    ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
}
void MainWindow::setBarSix(){
    ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
}
void MainWindow::setBarSeven(){
    ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
}
void MainWindow::setBarEight(){
    ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : red; }");
}

void MainWindow::blinkTwoMES()
{
    if(ui->MESL->styleSheet() == "QLabel {\nimage: url(:/res/MES-L.png);\nbackground-color : grey;\n}" &&
       ui->MESR->styleSheet() == "QLabel {\nimage: url(:/res/MES-R.png);\nbackground-color : grey;\n}"){

        ui->MESL->setStyleSheet("QLabel {\nimage: url(:/res/MES-L.png);\nbackground-color : red;\n}");
        ui->MESR->setStyleSheet("QLabel {\nimage: url(:/res/MES-R.png);\nbackground-color : red;\n}");
    }
    else{
        ui->MESL->setStyleSheet("QLabel {\nimage: url(:/res/MES-L.png);\nbackground-color : grey;\n}");
        ui->MESR->setStyleSheet("QLabel {\nimage: url(:/res/MES-R.png);\nbackground-color : grey;\n}");
    }
}

void MainWindow::blinkBarOne()
{
    if(ui->IL1->styleSheet() == "QLabel {\nimage: url(:/res/one.png);\nbackground-color : white;\n}" ||
       ui->IL1->styleSheet() == "QLabel {image: url(:/res/one.png); background-color : white; }"){
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
    }
    else{
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : white; }");
    }
}
void MainWindow::blinkBarTwo()
{
    if(ui->IL2->styleSheet() == "QLabel {\nimage: url(:/res/two.png);\nbackground-color : white;\n}" ||
       ui->IL2->styleSheet() == "QLabel {image: url(:/res/two.png); background-color : white; }"){
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
    }
    else{
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : white; }");
    }
}
void MainWindow::blinkBarThree()
{
    if(ui->IL3->styleSheet() == "QLabel {\nimage: url(:/res/three.png);\nbackground-color : white;\n}" ||
       ui->IL3->styleSheet() == "QLabel {image: url(:/res/three.png); background-color : white; }"){
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : green; }");
    }
    else{
        ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : white; }");
    }
}
void MainWindow::blinkBarFour()
{
    if(ui->IL4->styleSheet() == "QLabel {\nimage: url(:/res/four.png);\nbackground-color : white;\n}" ||
       ui->IL4->styleSheet() == "QLabel {image: url(:/res/four.png); background-color : white; }"){
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : yellow; }");
    }
    else{
        ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : white; }");
    }
}
void MainWindow::blinkBarFive()
{
    if(ui->IL5->styleSheet() == "QLabel {\nimage: url(:/res/five.png);\nbackground-color : white;\n}" ||
       ui->IL5->styleSheet() == "QLabel {image: url(:/res/five.png); background-color : white; }"){
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : yellow; }");
    }
    else{
        ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
    }
}
void MainWindow::blinkBarSix()
{
    if(ui->IL6->styleSheet() == "QLabel {\nimage: url(:/res/six.png);\nbackground-color : white;\n}" ||
       ui->IL6->styleSheet() == "QLabel {image: url(:/res/six.png); background-color : white; }"){
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : yellow; }");
    }
    else{
        ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
    }
}
void MainWindow::blinkBarSeven()
{
    if(ui->IL7->styleSheet() == "QLabel {\nimage: url(:/res/seven.png);\nbackground-color : white;\n}" ||
       ui->IL7->styleSheet() == "QLabel {image: url(:/res/seven.png); background-color : white; }"){
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
    }
    else{
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
    }
}
void MainWindow::blinkBarEight()
{
    if(ui->IL8->styleSheet() == "QLabel {\nimage: url(:/res/eight.png);\nbackground-color : white;\n}" ||
       ui->IL8->styleSheet() == "QLabel {image: url(:/res/eight.png); background-color : white; }"){
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : red; }");
    }
    else{
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");
    }
}

void MainWindow::blinkTwoBottomBars()
{
    if((ui->IL1->styleSheet() == "QLabel {\nimage: url(:/res/one.png);\nbackground-color : white;\n}" ||
       ui->IL1->styleSheet() == "QLabel {image: url(:/res/one.png); background-color : white; }") &&
       (ui->IL2->styleSheet() == "QLabel {\nimage: url(:/res/two.png);\nbackground-color : white;\n}" ||
       ui->IL2->styleSheet() == "QLabel {image: url(:/res/two.png); background-color : white; }")){

        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : green; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : green; }");
    }
    else{
        ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : white; }");
        ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : white; }");
    }
}

void MainWindow::blinkTwoTopBars()
{
    if((ui->IL7->styleSheet() == "QLabel {\nimage: url(:/res/seven.png);\nbackground-color : white;\n}" ||
       ui->IL7->styleSheet() == "QLabel {image: url(:/res/seven.png); background-color : white; }") &&
       (ui->IL8->styleSheet() == "QLabel {\nimage: url(:/res/eight.png);\nbackground-color : white;\n}" ||
       ui->IL8->styleSheet() == "QLabel {image: url(:/res/eight.png); background-color : white; }")){

        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : red; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : red; }");
    }
    else{
        ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
        ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");
    }
}

void MainWindow::turnOnLED()
{
    ui->LED->setStyleSheet("QLabel {background-color : green; }");
}

void MainWindow::turnOffLED()
{
    ui->LED->setStyleSheet("QLabel {background-color : grey; }");
}

void MainWindow::resetGroupIcons()
{
    ui->G1->setStyleSheet("QLabel {image: url(:/res/thirty.png); background-color : grey; }");
    ui->G2->setStyleSheet("QLabel {image: url(:/res/fortyfive.png); background-color : grey; }");
    ui->G3->setStyleSheet("QLabel {image: url(:/res/userdef.png); background-color : grey; }");
}

void MainWindow::resetSessionIcons()
{
    ui->S1->setStyleSheet("QLabel {image: url(:/res/alpha.png); background-color : grey; }");
    ui->S2->setStyleSheet("QLabel {image: url(:/res/delta.png); background-color : grey; }");
    ui->S3->setStyleSheet("QLabel {image: url(:/res/theta.png); background-color : grey; }");
    ui->S4->setStyleSheet("QLabel {image: url(:/res/met.png); background-color : grey; }");
}

void MainWindow::resetBarGraphIcons()
{
    ui->IL1->setStyleSheet("QLabel {image: url(:/res/one.png); background-color : white; }");
    ui->IL2->setStyleSheet("QLabel {image: url(:/res/two.png); background-color : white; }");
    ui->IL3->setStyleSheet("QLabel {image: url(:/res/three.png); background-color : white; }");
    ui->IL4->setStyleSheet("QLabel {image: url(:/res/four.png); background-color : white; }");
    ui->IL5->setStyleSheet("QLabel {image: url(:/res/five.png); background-color : white; }");
    ui->IL6->setStyleSheet("QLabel {image: url(:/res/six.png); background-color : white; }");
    ui->IL7->setStyleSheet("QLabel {image: url(:/res/seven.png); background-color : white; }");
    ui->IL8->setStyleSheet("QLabel {image: url(:/res/eight.png); background-color : white; }");
}

void MainWindow::resetLR()
{
    ui->Right->setStyleSheet("QLabel {\ncolor : white;\n}");
    ui->Left->setStyleSheet("QLabel {\ncolor : white;\n}");
}

void MainWindow::softOffAnimation()
{
    QTimer::singleShot(1850, this, SLOT(blinkBarEight()));
    QTimer::singleShot(2050, this, SLOT(blinkBarSeven()));
    QTimer::singleShot(2250, this, SLOT(blinkBarSix()));
    QTimer::singleShot(2450, this, SLOT(blinkBarFive()));
    QTimer::singleShot(2650, this, SLOT(blinkBarFour()));
    QTimer::singleShot(2850, this, SLOT(blinkBarThree()));
    QTimer::singleShot(3050, this, SLOT(blinkBarTwo()));
    QTimer::singleShot(3250, this, SLOT(blinkBarOne()));
    QTimer::singleShot(3350, this, SLOT(turnOffLED()));
    QTimer::singleShot(3550, this, SLOT(resetBarGraphIcons()));
}

void MainWindow::useTimer(){
    //Maybe use this function to do some battery drain
    //this is the second timer for therapies
    displaySessionOnStatus();
    if(currentConnection == 0){
        sessionStatus = 0;
        return;
    }
    if(battery > 0 && powerStatus == 1){
        sessionStatus = 1;
    }
    elapsedTime += 1;
    //here it checks if a group with a max timer occurs and stops the therapy
    switch(currentGroup){
        case 0:
            if(elapsedTime == 20){
                recordTherapy();
                displaySessionOnStatus();
            }
        break;
        case 1:
            if(elapsedTime == 45){
                recordTherapy();
                displaySessionOnStatus();
            }
        break;
    }
}

void MainWindow::recordTherapy() {
    //this function is used to record a therapy and update the history view
    //if there isn't a running session don't record
    if(isRecording == 0){
        sTimer->stop();
        sessionStatus = 0;
        elapsedTime = 0;
        intensity = 1;
        return;
    }

    if(elapsedTime == 0 || sessionStatus == 0){
        return;
    }
    sTimer->stop();
    sessionStatus = 0;
    QString ses = "";
    int mins;
    int secs;
    mins = elapsedTime / 60;
    secs = elapsedTime - (mins * 60);
    QString t = QString::number(mins) + "m" + QString::number(secs) + "s";
    switch(currentSession){
    case 0:
        ses = "Alpha";
        break;
    case 1:
        ses = "Delta";
        break;
    case 2:
        ses = "Theta";
        break;
    case 3:
        ses = "MET  ";
    }
    QString s = ses.leftJustified(11, ' ')
            + "|     " + t.leftJustified(10, ' ')
            + "|" + QString::number(intensity).rightJustified(5, ' ');
    elapsedTime = 0;
    ui->historyList->addItem(s);
    intensity = 1;
}

void MainWindow::startRecording() {
    if(isRecording == 0){
        isRecording = 1;
        ui->recordingLabel->setEnabled(true);
    } else {
        isRecording = 0;
        ui->recordingLabel->setDisabled(true);
    }
}

void MainWindow::resetBatSwitches()
{
    batSwith1 = 1;
    batSwith2 = 1;
    batSwith3 = 1;
    batSwith4 = 1;
    batSwith5 = 1;
    batSwith6 = 1;
}

void MainWindow::drainBattery() {
    //All needs to be done is some logic for levels of battery drain
    //variable depending on therapy, intensity, and connection to the skin
    double drainRate = 0.1;
    if(sessionStatus){
        drainRate *= intensity;
        //modify battery based on therapy
        drainRate += currentGroup*0.2;
        //modify battery based on connection to skin
        if (currentConnection == 1){
            drainRate *= 0.5;
        }
    }

    changeBatteryLevel(battery - drainRate);


    ui->batdoubleSpinBox->setValue(battery);
    if(battery == 0){

        bTimer->stop();

        sTimer->stop(); //added for battery running out, otherwise bad things happen
        elapsedTime = 0; //added for battery running out, otherwise bad things happen


        powerStatus = 0;
        sessionStatus = 0;
        connectionTestStatus = 0;

        currentGroup = 0;
        currentSession = 0;

        //softOffAnimation();
        resetGroupIcons();
        resetSessionIcons();
        resetBarGraphIcons();
        displaySessionOnStatus();

        isRecording = 0;
        ui->recordingLabel->setDisabled(true);

        resetBatSwitches();

        turnOffLED();
    }
    else if(battery < 80 && battery > 60 && sessionStatus == 1 && batSwith1 == 1){
        displayBatteryLevel();
        batSwith1 = 0; //to display once
    }
    else if(battery < 60 && battery > 40 && sessionStatus == 1 && batSwith2 == 1){
        displayBatteryLevel();
        batSwith2 = 0; //to display once
    }
    else if(battery < 40 && battery > 20 && sessionStatus == 1 && batSwith3 == 1){
        displayBatteryLevel();
        batSwith3 = 0; //to display once
    }
    else if(battery < 20 && battery > 10 && sessionStatus == 1 && batSwith4 == 1){
        displayBatteryLevel();
        batSwith4 = 0; //to display once
    }
    else if(battery < 10 && battery > 5 && sessionStatus == 1 && batSwith5 == 1){
        displayBatteryLevel();
        batSwith5 = 0; //to display once
    }
    else if(battery < 5 && battery > 0 && sessionStatus == 1 && batSwith6 == 1){
        displayBatteryLevel();
        batSwith6 = 0; //to display once
    }
}

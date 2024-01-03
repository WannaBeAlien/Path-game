/* Creates the mainwindow for the game and all the buttons
 * and items in it. Serves as the main hub for the gamecode.
 *
 * Nimi: Teemu Juhola
 * Opiskelijanumero 50280414
 * Käyttäjätunnus: dvteju
 * E-mail: teemu.s.juhola@tuni.fi
 * */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.hh"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Path");

    initbuttons();
}

MainWindow::~MainWindow()
{
    delete Board;
    delete ui;

}


void MainWindow::initbuttons()
{

    // Creating layout where items will be placed
    QWidget* central = new QWidget(this);
    QGridLayout* gLayout = new QGridLayout(central);

    // Initializing the game logic
    Board = new GameBoard;



    //Initializes top row
    for(unsigned int i = 1; i <= COLUMS; ++i)
    {
        // Pointing to the new button and making coordinates for it
        Point point;
        QPushButton* pushButton = new QPushButton(this);
        pushButton->setStyleSheet("background-color: green"); // Setting color
        pushButton->setFixedHeight(100);
        pushButton->setFixedWidth(100);

        gLayout->addWidget(pushButton, 0, i);
        point.y = 0;
        point.x = i-1;
        // Combining coordinates for the button
        points_.insert({pushButton, point});

        // And giving functionality to it
        connect(pushButton, &QPushButton::clicked, this, &MainWindow::clickToMove);
    }

    // central rows
    for(unsigned int i = 2; i <= COLUMS; ++i)
    {
        Point point;
        QPushButton* pushButton = new QPushButton(this);
        pushButton->setFixedHeight(100);
        pushButton->setFixedWidth(100);

        gLayout->addWidget(pushButton, i, 2);
        point.y = i-1;
        point.x = 1;
        points_.insert({pushButton, point});

        connect(pushButton, &QPushButton::clicked, this, &MainWindow::clickToMove);

        if (i == 3)
        {
            QPushButton* pushButton = new QPushButton(this);
            pushButton->setFixedHeight(100);
            pushButton->setFixedWidth(100);

            gLayout->addWidget(pushButton, i, COLUMS-1);
            point.x = 2;
            point.y = i-1;
            points_.insert({pushButton, point});

            connect(pushButton, &QPushButton::clicked, this, &MainWindow::clickToMove);
        }
    }

    // bottom row
    for(unsigned int i = 1; i <= COLUMS; ++i)
    {
        Point point;
        QPushButton* pushButton = new QPushButton(this);
        pushButton->setFixedHeight(100);
        pushButton->setFixedWidth(100);

        pushButton->setStyleSheet("background-color: red"); // Setting color

        gLayout->addWidget(pushButton, ROWS, i);
        point.y = 4;
        point.x = i-1;
        points_.insert({pushButton, point});

        connect(pushButton, &QPushButton::clicked, this, &MainWindow::clickToMove);
    }

    // Initialize the Reset button.
    QPushButton* resetButton = new QPushButton(this);

    resetButton->setText("RESET");

    gLayout->addWidget(resetButton, ROWS+1, 4);

    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);

    //Placing the movecounter
    MoveCounter = new QLabel(this);

    textToShow = "Moves so far: " + QString::number(total_moves);

    gLayout->addWidget(MoveCounter, ROWS+1, 1);
    MoveCounter->setText(textToShow);
    MoveCounter->setFont(QFont("Arial", 10));

    // Timer and display related to it
    Timer = new QTimer(this);
    LCD = new QLCDNumber(this);

    gLayout->addWidget(LCD, ROWS+1, 2);
    connect(Timer, &QTimer::timeout, this, &MainWindow::updateLCD);


    quitButton = new QPushButton(this);
    quitButton->setText("Quit");
    gLayout->addWidget(quitButton, ROWS+1, 3);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);

    pauseButton = new QPushButton(this);
    pauseButton->setText("Pause");
    gLayout->addWidget(pauseButton, ROWS+2, 4);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseGame);


    setCentralWidget(central);

}

void MainWindow::startTimer()
{
    Timer->start(1000);

}

void MainWindow::stopTimer(){
    Timer->stop();
}

void MainWindow::updateLCD(){
//Updating timer display
    time++;
    LCD->display(time);
}

void MainWindow::UpdateTurnCounter(){

    total_moves += 1;
    textToShow = "Moves so far: " + QString::number(total_moves);

    MoveCounter->setText(textToShow);



}

void MainWindow::gameOver() {


    Paused = true;
    hasWon = true;
    stopTimer();
    for (auto button : points_){
        if (button.second.y <= 4){
            button.first->setDisabled(true);
        }
    }

    pauseButton->setDisabled(true);

    // Notification about winning

    QMessageBox winMessage;
    winMessage.setWindowTitle("Game over");
    winMessage.setText("You've completed the game! \n"
                    "It took you " + QString::number(total_moves) + " moves"
                                            " and " + QString::number(time) + " seconds. \n"
                                                "You can play again by pressing 'reset'.");

    winMessage.exec();
}

void MainWindow::resetButtonColors(){
/* Iterates through the buttons and
 * changes their color based on the row
 * */

    for (auto Button : points_){
        if (Button.second.y == 0) {
            Button.first->setStyleSheet("background-color: green");
        }
        else if(Button.second.y == 4){
            Button.first->setStyleSheet("background-color: red");
        } else {
            Button.first->setStyleSheet("background-color: light grey");
        }
    }

}

void MainWindow::resetGame() {
/* Resets all attributes related to the game
 * and creates a new gameboard and deletes the previous
 * */


    if (Paused) {
        for (auto button : points_){
            if (button.second.y <= 4){
                button.first->setDisabled(false);
            }
        }

        // To avoid scenarios where the button and bool states
        // get mixed up.
        if (hasWon) {
            pauseButton->setDisabled(false);

        }
    }
    // Resetting attributes back to default
    Paused = false;
    hasWon = false;
    total_moves = 0;
    time = 0;
    stopTimer();
    LCD->display(0);
    textToShow = "Moves so far: " + QString::number(total_moves);
    MoveCounter->setText(textToShow);
    pauseButton->setText("Pause");

    // Making a new board
    delete Board;
    Board = new GameBoard;
    resetButtonColors();




}

void MainWindow::changeButtonColors(QPushButton* movedPiece){

        //Prevents counting empty button moves
        if (Board->which_slot(Destination) == EMPTY && Board->which_slot(start) == EMPTY){
            return;
        }



        if(total_moves == 0) {
            startTimer();
        }



        if(Board->which_slot(Destination) == GREEN) {
            movedPiece->setStyleSheet("background-color: green");

            //
        }
        else if (Board->which_slot(Destination) == RED) {
            movedPiece->setStyleSheet("background-color: red");

            //
        }

        UpdateTurnCounter();


        if (Board->is_game_over()){
            //end the game with a function
            gameOver();
        }

        // Change the button that is now empty to grey
        for (auto button : points_) {
            if (button.second == start){
                button.first->setStyleSheet("background-color: light grey");
                break;
            }
        }

}



void MainWindow::pauseGame(){


    if (!Paused){
        stopTimer();
        Paused = true;

        for (auto button : points_){
            if (button.second.y <= 4){
                button.first->setDisabled(true);
            }
        }

        pauseButton->setText("Resume");
    } else {
        if (total_moves != 0){
            startTimer();
        }
        Paused = false;

        for (auto button : points_){
            if (button.second.y <= 4){
                button.first->setDisabled(false);
            }
        }
        pauseButton->setText("Pause");
    }
}


void MainWindow::clickToMove(){


    for (auto Button : points_) {

        if (Button.first == sender()){

            // If a valid button was previously clicked
            if(ButtonWasToggled){
                ButtonWasToggled = false;
                Destination = Button.second;

                if(Destination == start){
                    ButtonWasToggled = true;
                }

                // If the next click is made on a colored button it becomes the new start point
                else if (Board->which_slot(Destination) == GREEN
                         || Board->which_slot(Destination) == RED){
                    start = Destination;
                    ButtonWasToggled = true;
                }
                else {
                    bool moveSuccess = Board->move(start, Destination);

                    if (moveSuccess) {

                        // Calling function that changes the button colors on the board
                        changeButtonColors(Button.first);

                    }
                    else {
                        ButtonWasToggled = true;
                    }
                }
            }
            else {
                // click was valid and preparing for the next
                ButtonWasToggled = true;
                start = Button.second;
            }
        }
    }
}





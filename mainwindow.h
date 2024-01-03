#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gameboard.hh"
#include <memory>
#include <QGridLayout>
#include <QPushButton>
#include <QMainWindow>
#include <iostream>
#include <QLabel>
#include <QTimer>
#include <QLCDNumber>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    //Initializes the items in the UI
    void initbuttons();

    /* Iterates through buttons and sets start and destination points
     * Start point is the previous one clicked and destination is
     * the next one unless if the move cannot be done
     * in that case that button becomes the new start point
     **/
    void clickToMove();

    /* Adding to the turn counter and show the new text*/
    void UpdateTurnCounter();

    // Resets the logic of the game
    void resetGame();

    //Resets The UI
    void resetButtonColors();
    void startTimer();
    void updateLCD();

    //Updating timer display
    void stopTimer();

    /*@brief ends the game and gives a popup
     * Ends the game and stops timer. Pops up a notification letting
     * you know that you've won */
    void gameOver();

    /* Changes button colors by getting the color from the gameboard logic
     * */
    void changeButtonColors(QPushButton *movedPiece);

    /* Pauses the game by stopping the timer and disabling
     * game buttons. Same function will also reverse and
     * continue the game if it was previously paused */
    void pauseGame();


    ~MainWindow();

private slots:



private:
    Ui::MainWindow *ui;
    GameBoard* Board;
    Point start = {0, 0};
    Point Destination = {0, 0};
    unsigned int total_moves = 0;
    bool ButtonWasToggled = false;

    // Text shown in the movecounter
    QString textToShow;
    int time = 0;
    bool Paused = false;
    bool hasWon = true;

    std::map<QPushButton*, Point> points_;
    QPushButton* quitButton;

    QLabel* MoveCounter;

    QTimer* Timer;

    QPushButton* resetButton;

    QLCDNumber* LCD;


    QPushButton* pauseButton;

};
#endif // MAINWINDOW_H

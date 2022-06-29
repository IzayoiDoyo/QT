#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>

#include<QPushButton>
#include<QKeyEvent>
#include<QPainter>
#include<QPaintEvent>
#include<QDebug>
#include<QTimer>

#include<list>
using std::list;

#include"libs/constants.h"
#include"libs/gamemap.h"
#include"libs/gametick.h"
#include"libs/kbinput.h"

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev -> key() == Qt::Key_Q)
    {
        if(gameStatus != __gameTick::inGame)
            exit(0);
    }
    if(ev -> key() == Qt::Key_R)
    {
        if(gameStatus == __gameTick::paused)
            backToMenu();
    }
    if(ev -> key() == Qt::Key_Space)
    {
        if(gameStatus == __gameTick::endGame)
            backToMenu();
        if(gameStatus == __gameTick::inGame)
            pauseGame();
    }
    if(gameStatus == __gameTick::menu)
    {
        if(ev -> key() == Qt::Key_1)
        {
            startGame(1);
        }
        if(ev -> key() == Qt::Key_2)
        {
            startGame(2);
        }
        if(ev -> key() == Qt::Key_3)
        {
            startGame(3);
        }
        if(ev -> key() == Qt::Key_4)
        {
            startGame(4);
        }
        if(ev -> key() == Qt::Key_5)
        {
            startGame(5);
        }
        if(ev -> key() == Qt::Key_6)
        {
            startGame(6);
        }
    }
    keyboardStatus.update(ev, true);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    keyboardStatus.update(ev, false);
}

QImage MapSolid(){return gameMap.getWholeMapSolid();}
QImage MapLiquid(){return gameMap.getWholeMapLiquid();}
QImage PlayerState(int r1, int r2, int w1, int w2)//HP & MP 's percent
{
    QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
    QPainter temp(&ret);
    QPen pen;
    pen.setColor(QColor(0, 0, 0, barAlpha));
    QBrush brush;
    brush.setColor(QColor(125, 45, 45, barAlpha));// Back Color
    brush.setStyle(Qt::SolidPattern);
    temp.setPen(pen), temp.setBrush(brush);

    temp.drawRoundRect(barSideWidth, barSideHeight, barWidth, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth, barSideHeight, barWidth, barHeight, barRoundX, barRoundY);
    //paint HP(empty ver)
    temp.drawRoundRect(barSideWidth, barSideHeight + barDistance, barWidth, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth, barSideHeight + barDistance, barWidth, barHeight, barRoundX, barRoundY);
    //paint MP(empty ver)
    brush.setColor(QColor(255, 0, 0, barAlpha));// Red
    temp.setBrush(brush);
    temp.drawRoundRect(barSideWidth, barSideHeight, barWidth * r1 / 100, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth * r2 / 100, barSideHeight, barWidth * r2 / 100, barHeight, barRoundX, barRoundY);
    //paint HP(full ver)
    brush.setColor(QColor(0, 0, 255, barAlpha));// Blue
    temp.setBrush(brush);
    temp.drawRoundRect(barSideWidth, barSideHeight + barDistance, barWidth * w1 / 100, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth * w2 / 100, barSideHeight + barDistance, barWidth * w2 / 100, barHeight, barRoundX, barRoundY);
    //paint MP(full ver)

    QImage h1("..\\QT\\resources\\images\\player\\stay.png");
    temp.drawPixmap(barSideWidth - headSize - 5, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h1));
    QImage h2("..\\QT\\resources\\images\\player\\stay.png");
    h2 = h2.mirrored(true, false);
    temp.drawPixmap(screenWidth * 16 - (barSideWidth - headSize - 5) - headSize, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h2));
    //paint head picture
    return ret;
}
QImage AllBullet()
{
    QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
    QPainter temp(&ret);
    QPixmap img("..\\QT\\resources\\images\\effects\\bullet.png");
    list<__player::bullet>::iterator it = __player::L.begin();
    while (it != __player::L.end())
    {
        temp.drawPixmap((int)round((it -> posX) * 16 - 3), (int)round(screenHeight * 16 - 1 - (it -> posY) * 16 + 3), 6, 6, img);
        it++;
    }
    return ret;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (gameStatus == __gameTick::menu)
    {
        QPixmap bg(MainWindow::width(), MainWindow::height());
        bg.fill(Qt::white);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), bg);
    }
    if (gameStatus == __gameTick::inGame)//
    {
        QPixmap ms = QPixmap::fromImage(MapSolid()); //solid map
        QPixmap ml = QPixmap::fromImage(MapLiquid()); //liquid map
        QPixmap p1 = QPixmap::fromImage(__player::P1.GetPlayerState()); //player 1
        QPixmap p2 = QPixmap::fromImage(__player::P2.GetPlayerState()); //player 2
        QPixmap bl = QPixmap::fromImage(AllBullet());
        int r1 = __player::P1.GetPlayerHPRate(), r2 = __player::P2.GetPlayerHPRate();
        int w1 = __player::P1.GetPlayerMPRate(), w2 = __player::P2.GetPlayerMPRate();
        QPixmap st = QPixmap::fromImage(PlayerState(r1, r2, w1, w2));//state
        QImage result(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
        QPainter temp(&result);
        temp.drawPixmap(0, 0, ms);// paint the map solid
        temp.drawPixmap(0, 0, p1);// paint player1
        temp.drawPixmap(0, 0, p2);// paint player2
        temp.drawPixmap(0, 0, ml);// paint the map liquid
        temp.drawPixmap(0, 0, bl);// paint the map liquid
        temp.drawPixmap(0, 0, st);// paint the map liquid
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), QPixmap::fromImage(result));
        //print
    }
}


void MainWindow::startGame(int gameThemeId)
{
    keyboardStatus.clear();
    __player::P1.initialize(1, screenWidth / 3 * 1 - 0.5 * playerheight, screenHeight - 2);
    __player::P2.initialize(0, screenWidth / 3 * 2 - 0.5 * playerheight, screenHeight - 2);
    __gameTick::gameStatus = inGame;
    if(gameThemeId == 1)setTheme(plain);
    if(gameThemeId == 2)setTheme(desert);
    if(gameThemeId == 3)setTheme(ocean);
    if(gameThemeId == 4)setTheme(mountains);
    if(gameThemeId == 5)setTheme(nether);
    if(gameThemeId == 6)setTheme(the_end);
    gameMap.mapInit();
    repaint();
}
void MainWindow::GlobalTick()
{
    if (__gameTick::gameStatus == inGame)
    {
        __gameTick::tick();
        repaint();
    }
    gameClock->start(timePerTick);
}
void MainWindow::backToMenu()
{
    __gameTick::gameStatus = menu;
    // printMenu();
}
void MainWindow::pauseGame()
{
    __gameTick::gameStatus = paused;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , gameClock(new QTimer(this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,960);
    gameClock->start(timePerTick);
    connect(gameClock, SIGNAL(timeout()), this, SLOT(GlobalTick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


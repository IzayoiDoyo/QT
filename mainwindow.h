#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    void paintEvent(QPaintEvent *);
    void startGame(int gameThemeId);
    void backToMenu();
    void pauseGame();
    QTimer * gameClock;
public slots:
    void GlobalTick();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

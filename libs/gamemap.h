#ifndef _QTHWK_GAMEMAP_H_
#define _QTHWK_GAMEMAP_H_

#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

namespace __gameMap
{
    const int Width = 48, Height = 36;
    const string dir1 = "..\\QT\\resources\\images\\base\\";
    const string dir2 = "..\\QT\\resources\\images\\environment\\";
    const string themeName[6] = { "desert", "end", "mountains", "nether", "ocean", "plain" };
    enum theme { desert, the_end, mountains, nether, ocean, plain } nowTheme;
    enum block { empty, solid, liquid, platform };
    // 48 * 36 screen
    void setTheme(theme x){nowTheme = x;}
    class gameMapStructure
    {
    private:
        string source;
        int map[Height][Width];
        theme themeType;
        block sta[Height][Width];
        const enum block typ[25] = {
        empty, solid, empty, solid, solid, empty, liquid, liquid, solid, solid,
        liquid, solid, solid, solid, solid, solid, solid, solid, empty, liquid,
        solid };
    public:
        void mapInit()//load the map
        {
            themeType = nowTheme;
            source = dir1 + themeName[(unsigned int)nowTheme] + ".txt";
            FILE* Input = fopen(source.c_str(), "r");
            for (int i = Height - 1; i >= 0; i--)
            {
                int tmp;
                for (int j = 0; j < Width; j++)
                {
                    fscanf(Input, "%d", &tmp);
                    map[i][j] = tmp;
                    sta[i][j] = typ[map[i][j]];
                }
            }
        }
        enum block getBlockType(int x, int y)//(x, y)
        {
            return sta[y][x];//0:empty, 1:solid, 2:liquid, 3:platform
        }
        QImage getWholeMap()//print the whole map
        {
            QImage ret(Width * 16, Height * 16, QImage::Format_RGBA8888);
            QPixmap tmp;
            QPainter painter(&ret);
            for (int i = 0; i < Height; i++)
            {
                for (int j = 0; j < Width; j++)
                {
                    tmp.load((dir2 + to_string(map[i][j]) + ".png").c_str());
                    painter.drawPixmap(j * 16, (Height - 1 - i) * 16, 16, 16, tmp);
                }
            }
            return ret;
        }
    }gameMap;
}
using __gameMap::setTheme;
using __gameMap::gameMap;
using __gameMap::desert;
using __gameMap::the_end;
using __gameMap::mountains;
using __gameMap::nether;
using __gameMap::ocean;
using __gameMap::plain;
using __gameMap::empty;
using __gameMap::solid;
using __gameMap::liquid;
using __gameMap::platform;


#endif // _QTHWK_GAMEMAP_H_

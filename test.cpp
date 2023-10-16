/*
 * Siklodi Zalan
 * Nyari gyakorlat 2023
 */

/*
 * How to run this program:
 * powershell:
 * C:/TDM-GCC-32/bin/g++.exe -fdiagnostics-color=always -g D:\Info\University\Nyari_gyakorlat\Graphics\test.cpp -o D:\Info\University\Nyari_gyakorlat\Graphics\test.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 * bash:
 * /mnt/c/TDM-GCC-32/bin/g++.exe -fdiagnostics-color=always -g /mnt/d/Info/University/Nyari_gyakorlat/Graphics/test.cpp -o /mnt/d/Info/University/Nyari_gyakorlat/Graphics/test.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 */

/*
 * read_polygon(vector<Point> &polygon, vector<Point> &wall, vector<vector<Point>> &between)
 *      return value:
 *          void
 *      input parameters:
 *          vector<Point> &polygon - ebben vannak eltarolva a poligon pontjai
 *          vector<Point> &wall - ebben vannak eltarolva a sktraight skeleton pontjai
 *          vector<vector<Point>> &between - ebben vannak eltaroilva a szogfelezok pontjai
 *      - az skeleton.cpp program altal generalt file-bol beolvassa a poligont,
 *        annak szogfelezoit es a belso straight skeletonjat
 *
 * calculate_wall_size(vector<Point> &polygon, vector<Point> &wall, vector<double> &wall_size, vector<vector<Point>> &between)
 *      return value:
 *          void
 *      input parameters:
 *          vector<Point> &polygon - a poligon pontjait tartalmazza
 *          vector<Point> &wall - a straight skeleton pontjait tartalmazza, vagyis a "fal" pontjait
 *          vector<double> &wall_size - a falak vastagsagat tartalmazza
 *          vector<vector<Point>> &between - a szogfelezok pontjait tartalmazza
 *      - kiszamolja egy adott falresz tavolsagat a poligon megfelelo oldalaitol
 *      - ha ket egymas melletti fal vastagsaga 10^(-5) pontossaggal megegyezik, akkor ezeket a
 *        vastagsagokat egyenlove tesszuk
 *
 * expand_walls(vector<Point> &wall, vector<vector<Point>> &between)
 *      return value:
 *          void
 *      input parameters:
 *          vector<Point> &wall - a belso straight skeleton pontjait tartalmazza
 *          vector<vector<Point>> &between - a szogfelezok pontjait tartalmazza
 *      - azokon a reszeket, ahol a straight skeletonhoz ket szogfelezo tartozik
 *        kiterjeszti a belso straight skeleton pontjat, ugy, hogy az erintse
 *        a poligon oldalat
 *
 * put_together_final_walls(vector<Point> &wall, vector<Wall> &final_walls, vector<double> &wall_size)
 *      return value:
 *          void
 *      input parameters:
 *          vector<Point> &wall - a belso straight skeleton pontjiat tartalmazza
 *          vector<Wall> &final_walls - a vegso falakat tartalmazza, azok vastagsagaval egyutt
 *          vector<double> &wall_size - a falreszek vastagsagit tartalmazza
 *      - a kulonbozo vastagsagu falreszeket "kidobjuk a kukaba", igy a straigth skeletonunkban lesznek
 *        megszakitasok, olyan reszek, ahol nincs fal => azekre a reszkere falat teszunk, ugy hogy a
 *        reszek melletti falakat osszekotjuk
 *      - a falakoz hozzarandeljuk a vastagsaigjaikat
 *      - ezeket a vastagsagokat megszorozzuk kettovel, hogy a fal tenyleges vastagsagat adja, ne csak
 *        a straight skeletonnak a tavolsagat a poligon megfelelo oldalatol
 *
 * draw(int zoom, vector<Point> &polygon, vector<Wall> &final_walls, vector<vector<Point>> &between)
 *      return value:
 *          void
 *      input parameters:
 *          int zoom - egy ertek, amellyel beszorozzuk a pontokat, hogy azok nagyobbnak latszanak a kepernyon
 *          vector<Point> &polygon - a poligon pontjait tartalmazza
 *          vector<Wall> &final_walls - a vegso falak pontjiat es vastagsagjait tartalmazza
 *          vector<vector<Point>> &between - a szogfelezok pontjiat tartalmazza
 *      - kirajzolja a poligont feherrel, a szogfelezoket pirossal es a falakat, vagyis a kiterjesztett
 *        belso straigth skeletont zold szinnel
 *
 * write_walls(vector<Wall> &final_walls)
 *      return value:
 *          void
 *      input parameters:
 *          vector<Wall> &final_walls - a vegso falak pontjiat es vastagsagjait tartalmazza
 *      - kiirja az OUTPUT_FILE_PATH altal megadott file-ba a falakat ez azok vastagsagat a kovetkezo formatumban:
 *        minden sor ket pontot altal meghatarozott falreszt es annak vastagsagat tartalmazza
 *        => az elso pont x es y koordinataja szokozzel van elvalasztva egymastol, mig ezek egy tabbal vannak elvalasztva
 *           a masodik ponttol, melynek x es y koordinatai ugyancsak szokozzel vannak elvalasztva
 *      - a ket ponttol ket tabbal van elvalasztva az adott fal vastagsaga
 *      - pl: x1 y1   x2 y2       wall_size
 */

/*
 * Barmilyen kerdes merul fel, elerheto vagyok a kovetkezo email cimen: zalansiklodi@yahoo.com
 */

#include <graphics.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

const string INPUT_FILE_PATH = "D:\\Info\\University\\Nyari_gyakorlat\\CGAL\\skeleton\\polygon.txt";
const string OUTPUT_FILE_PATH = "D:\\Info\\University\\Nyari_gyakorlat\\CGAL\\skeleton\\walls.txt";
const double PRECISION = 0.00001;

struct Point
{
    double x, y;
};

struct Line
{
    Point p1, p2;
};

struct Wall
{
    Line line;
    double size;
};

void read_polygon(vector<Point> &polygon, vector<Point> &wall, vector<vector<Point>> &between)
{
    vector<Line> initial_walls;

    fstream fin;
    fin.open(INPUT_FILE_PATH, ios::in);

    if (fin.is_open())
    {
        string data, arrow;
        double first, second, third, fourth;
        while (fin >> data >> first >> second >> arrow >> third >> fourth)
        {
            if (data == "CH")
            {
                polygon.push_back({first, second});
            }
            else if (data == "BH")
            {
                int i = 0;
                for (i = 0; i < between.size(); i++)
                {
                    if (between[i][0].x == third && between[i][0].y == fourth)
                    {
                        between[i].push_back({first, second});
                        break;
                    }
                }

                if (i == between.size())
                {
                    between.resize(between.size() + 1);
                    between[between.size() - 1].push_back({third, fourth});
                    between[between.size() - 1].push_back({first, second});
                }
            }
            else
            {
                initial_walls.push_back({{first, second}, {third, fourth}});
            }
        }
        fin.close();
    }

    wall.push_back(initial_walls[0].p1);
    wall.push_back(initial_walls[0].p2);
    int i = 0;
    while (wall.size() != between.size())
    {
        for (int j = 0; j < initial_walls.size(); j++)
        {
            if (j != i && initial_walls[j].p1.x == wall[wall.size() - 1].x && initial_walls[j].p1.y == wall[wall.size() - 1].y)
            {
                wall.push_back(initial_walls[j].p2);
                i = j;
                break;
            }
            else if (j != i && initial_walls[j].p2.x == wall[wall.size() - 1].x && initial_walls[j].p2.y == wall[wall.size() - 1].y)
            {
                wall.push_back(initial_walls[j].p1);
                i = j;
                break;
            }
        }
    }
}

void calculate_wall_size(vector<Point> &polygon, vector<Point> &wall, vector<double> &wall_size, vector<vector<Point>> &between)
{
    wall_size.resize(wall.size());
    Point u, u0, v;
    double u_size, h, d;
    Point a, b, c;
    for (int i = 0; i < wall.size(); i++)
    {

        c = wall[i];

        for (int j = 0; j < between.size(); j++)
        {
            if (c.x == between[j][0].x && c.y == between[j][0].y)
            {
                a = between[j][1];
                break;
            }
        }

        for (int j = 0; j < polygon.size(); j++)
        {
            if (a.x == polygon[j].x && a.y == polygon[j].y)
            {
                b = polygon[(j + 1) % polygon.size()];
                break;
            }
        }

        u.x = b.x - a.x;
        u.y = b.y - a.y;
        u_size = sqrt(u.x * u.x + u.y * u.y);
        u0.x = u.x / u_size;
        u0.y = u.y / u_size;

        v.x = c.x - a.x;
        v.y = c.y - a.y;
        h = sqrt(v.x * v.x + v.y * v.y);

        d = u0.x * v.x + u0.y * v.y;

        wall_size[i] = sqrt(h * h - d * d);
    }

    for (int i = 0; i < wall_size.size() - 1; i++)
    {
        if (abs(wall_size[i] - wall_size[i + 1]) < PRECISION)
        {
            wall_size[i + 1] = wall_size[i];
        }
    }

    for (int i = 0; i < wall_size.size(); i++)
    {
        wall_size[i] *= 2;
    }
}

void expand_walls(vector<Point> &wall, vector<vector<Point>> &between)
{
    for (int i = 0; i < between.size(); i++)
    {
        double first = 0, second = 0;
        int wall_expand_index;
        if (between[i].size() > 2)
        {
            for (int j = 0; j < wall.size(); j++)
            {
                if (wall[j].x == between[i][0].x && wall[j].y == between[i][0].y)
                {
                    wall_expand_index = j;
                    break;
                }
            }
            for (int j = 1; j < between[i].size(); j++)
            {
                first += between[i][j].x;
                second += between[i][j].y;
            }
            first /= (between[i].size() - 1);
            second /= (between[i].size() - 1);
            wall[wall_expand_index] = {first, second};
        }
    }
}

void put_together_final_walls(vector<Point> &wall, vector<Wall> &final_walls, vector<double> &wall_size)
{
    for (int i = 0; i < wall_size.size() - 1; i++)
    {
        if (wall_size[i] == wall_size[i + 1])
        {
            final_walls.push_back({wall[i], wall[i + 1]});
        }
    }

    double a1, a2, b1, b2, c1, c2, x0, y0;
    for (int i = 0; i < final_walls.size() - 1; i++)
    {
        if (final_walls[i].line.p2.x != final_walls[i + 1].line.p1.x || final_walls[i].line.p2.y != final_walls[i + 1].line.p1.y)
        {
            a1 = final_walls[i].line.p2.y - final_walls[i].line.p1.y;
            b1 = final_walls[i].line.p1.x - final_walls[i].line.p2.x;
            c1 = final_walls[i].line.p2.x * final_walls[i].line.p1.y - final_walls[i].line.p1.x * final_walls[i].line.p2.y;
            a2 = final_walls[i + 1].line.p2.y - final_walls[i + 1].line.p1.y;
            b2 = final_walls[i + 1].line.p1.x - final_walls[i + 1].line.p2.x;
            c2 = final_walls[i + 1].line.p2.x * final_walls[i + 1].line.p1.y - final_walls[i + 1].line.p1.x * final_walls[i + 1].line.p2.y;

            x0 = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
            y0 = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1);

            final_walls[i].line.p2 = {x0, y0};
            final_walls[i + 1].line.p1 = final_walls[i].line.p2;
        }
    }

    for (int i = 0; i < final_walls.size(); i++)
    {
        final_walls[i].size = wall_size[i * 2];
    }
}

void draw(int zoom, vector<Point> &polygon, vector<Wall> &final_walls, vector<vector<Point>> &between)
{
    int screen_width = 1500;
    int screen_height = 750;
    int origo_x = screen_width / 2;
    int origo_y = screen_height / 2;
    initwindow(screen_width, screen_height);
    setcolor(LIGHTGRAY);
    line(0, screen_height / 2, screen_width, screen_height / 2);
    line(screen_width / 2, 0, screen_width / 2, screen_height);

    setcolor(WHITE);
    for (int i = 0; i < polygon.size(); i++)
    {
        int j = (i + 1) % polygon.size();
        line(polygon[i].x * zoom + origo_x, screen_height - polygon[i].y * zoom - origo_y, polygon[j].x * zoom + origo_x, screen_height - polygon[j].y * zoom - origo_y);
    }

    setcolor(GREEN);
    for (int i = 0; i < final_walls.size(); i++)
    {
        line(final_walls[i].line.p1.x * zoom + origo_x, screen_height - final_walls[i].line.p1.y * zoom - origo_y, final_walls[i].line.p2.x * zoom + origo_x, screen_height - final_walls[i].line.p2.y * zoom - origo_y);
    }

    setcolor(RED);
    for (int i = 0; i < between.size(); i++)
    {
        for (int j = 1; j < between[i].size(); j++)
        {
            line(between[i][0].x * zoom + origo_x, screen_height - between[i][0].y * zoom - origo_y, between[i][j].x * zoom + origo_x, screen_height - between[i][j].y * zoom - origo_y);
        }
    }

    getch();
    closegraph();
}

void write_walls(vector<Wall> &final_walls)
{
    fstream fout;
    fout.open(OUTPUT_FILE_PATH, ios::out);

    if (fout.is_open())
    {
        for (int i = 0; i < final_walls.size(); i++)
        {
            fout << final_walls[i].line.p1.x << " " << final_walls[i].line.p1.y << "\t" << final_walls[i].line.p2.x << " " << final_walls[i].line.p2.y << "\t\t" << final_walls[i].size << endl;
        }
        fout.close();
    }
}

int main(int argc, char const *argv[])
{
    vector<Point> polygon;
    vector<Point> wall;
    vector<Wall> final_walls;
    vector<double> wall_size;
    vector<vector<Point>> between;

    read_polygon(polygon, wall, between);
    calculate_wall_size(polygon, wall, wall_size, between);
    expand_walls(wall, between);
    put_together_final_walls(wall, final_walls, wall_size);
    write_walls(final_walls);
    draw(50, polygon, final_walls, between);

    return 0;
}
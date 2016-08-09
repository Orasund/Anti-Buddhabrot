//============================================================================
// Name        : Buddhabort.cpp
// Author      : Lucas Payr
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <stdio.h>
//#include "Drawing.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdlib.h> //Malloc

//using namespace compsys;
using namespace std;

unsigned int W = 1080;//1920;//1280;//1280;//640
unsigned int H = 720;//1080;//720;//720;//480
unsigned int iteration = 1;
//const int F = 200;
const int F = W/4;
const int MAX = 25;
int view = 0;
//const int F = 500;
//const int F = 320;

class complex
{
public:
  double x;
  double y;
  double i;
  double j;
  double j_temp;
  complex():x(0),y(0),i(0),j(0),j_temp(0){};
};

complex *** grid;

bool save()
{
  ofstream file;
  file.open("savefile.txt", std::ofstream::out | std::ofstream::trunc);

  if(!file.is_open())
    return false;
  file << iteration << " " << W << " " << H;
  for(unsigned int i = 0; i<H; i++)
  {
    for(unsigned int j=0;j<W;j++)
    {
      file << " " << grid[j][i]->x;
      file << " " << grid[j][i]->y;
      file << " " << grid[j][i]->i;
      file << " " << grid[j][i]->j_temp;
    }
  }
  file.close();
  return true;
}

bool load()
{
  ifstream file;
  file.open("savefile.txt");
  if(!file.is_open())
    return false;

  file >> iteration;
  unsigned int w;
  unsigned int h;
  file >> w;
  file >> h;
  if(w!=W || h!=H)
  {
    iteration = 1;
    file.close();
    return false;
  }
  for(unsigned int i = 0; i<H; i++)
  {
    for(unsigned int j=0;j<W;j++)
    {
      file >> grid[j][i]->x;
      file >> grid[j][i]->y;
      file >> grid[j][i]->i;
      file >> grid[j][i]->j_temp;
    }
  }
  file.close();
  return true;
}

/*********************************************
*240<=h<300,  0<=s<=1,  0<=l<=1
*********************************************/
sf::Color hslToColor(float h, float s, float l)
{
  if(s>1)
	s = 1;
  if(l>1)
	l = 1;
  float c = (1-abs(2*l-1)*s;
  float x = c*(1-abs(((h/60)%2)-1));
  float m = l -c/2;
  sf::Color color((x+m)*255,m*255,((c+m)255);
  return color;
}

void drawPoint(int col, int row,sf::RenderWindow& window)
{
  sf::RectangleShape point(sf::Vector2f(1, 1));
  sf::Color color;
  int c;
  if(view == 1)
    c=grid[col][row]->i;
  else
    c=grid[col][row]->j;
  color = hslToColor(240,(100-c*2)/100,(c*5)/100);
  //color = sf::Color(c*(0xFFFFFF/50)*0x100+0xFF);
  //color = sf::Color(c*5,c*5,c*255);
  point.setFillColor(color);
  point.setPosition(col, row);
  window.draw(point);
}

void render(int col,int row,sf::RenderWindow& window)
{
  //Setup
  double c_re = (col - W/2.0)*4.0/W;
  double c_im = (row - H/2.0)*4.0/W;
  double x = 0, y = 0;

  //calculation
  x = grid[col][row]->x;
  y = grid[col][row]->y;
  grid[col][row]->x = x*x - y*y + c_re;
  grid[col][row]->y = 2*x*y + c_im;
  if
  (
    grid[col][row]->x*grid[col][row]->x + grid[col][row]->y*grid[col][row]->y
    <= 4
  )
  {
    grid[col][row]->i++;
  }

  x=((grid[col][row]->x*W)/4.0)+ (W/2.0);
  y=((grid[col][row]->y*W)/4.0)+ (H/2.0);
  if(x>0&&y>0&&x<W&&y<H)
    grid[int(x)][int(y)]->j_temp++;

  //point.setFillColor(sf::Color(30,30,30));
  drawPoint(col,row,window);
  //fillRectangle(col,row,1,1,grid[col][row]->j*(0xFFFFFF/50));
}

int main()
{
  /* SETUP START */
  grid = new complex**[W];
  for(unsigned int j=0;j<W;j++)
  {
    grid[j] =  new complex*[H];
  }

  for(unsigned int i = 0; i<H; i++)
  {
    for(unsigned int j=0;j<W;j++)
    {
      grid[j][i] = new complex;
    }
  }

  sf::RenderWindow window(sf::VideoMode(W, H), "Buddhabrot");
  sf::RectangleShape point(sf::Vector2f(1, 1));

  if(load())
    cout << "loaded saved file"<<endl;
  else if(save())
    cout << "file created"<<endl;

  /* SETUP END */
  while (window.isOpen())
  {

    //Reset
    for (unsigned int row = 0; row < H; row++)
    {
      for (unsigned int col = 0; col < W; col++)
      {
        grid[col][row]->j=grid[col][row]->j_temp;
        grid[col][row]->j_temp=0;
      }
    }

    for (unsigned int row = 0; row < H; row++)
    {
      for (unsigned int col = 0; col < W; col++)
      {
        //Controll handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          window.close();
        if(window.isOpen()==false)
          return 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          view = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          view = 0;

        render(col, row,window);
      }
      window.display();
    }
    cout << iteration <<endl;
    if(save())
      cout << "saved file"<<endl;
    iteration++;
  }

  cout << "close the program to exit" << endl;

//  endDrawing();
  return 0;
}

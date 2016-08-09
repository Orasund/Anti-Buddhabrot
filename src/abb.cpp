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
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdlib.h> //Malloc

using namespace std;

unsigned int W = 1920;//1920;//1280;//1280;//640
unsigned int H = 1080;//1080;//720;//720;//480
unsigned int iteration = 1;
const int MAX = 25;
int view = 0;

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

complex grid[1920][1080];

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
      file << " " << grid[j][i].x;
      file << " " << grid[j][i].y;
      file << " " << grid[j][i].i;
      file << " " << grid[j][i].j_temp;
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
      file >> grid[j][i].x;
      file >> grid[j][i].y;
      file >> grid[j][i].i;
      file >> grid[j][i].j_temp;
    }
  }
  file.close();
  return true;
}

void drawPoint(int col, int row,sf::RenderWindow& window)
{
  sf::RectangleShape point(sf::Vector2f(1, 1));
  sf::Color color;
  int c;
  if(view == 1)
    c=grid[col][row].i;
  else
    c=grid[col][row].j;
  color = sf::Color(c*(0xFFFFFF/50)*0x100+0xFF);
  //color = sf::Color(c*5,c*5,c*255);
  point.setFillColor(color);
  point.setPosition(col, row);
  window.draw(point);
}

void calcPoint(int col, int row)
{
  //Setup
  double c_re = (col - W/2.0)*4.0/W;
  double c_im = (row - H/2.0)*4.0/W;
  double x = 0, y = 0;

  //calculation
  x = grid[col][row].x;
  y = grid[col][row].y;
  grid[col][row].x = x*x - y*y + c_re;
  grid[col][row].y = 2*x*y + c_im;
  if
  (
    grid[col][row].x*grid[col][row].x + grid[col][row].y*grid[col][row].y
    <= 4
  )
  {
    grid[col][row].i++;
  }

  x=((grid[col][row].x*W)/4.0)+ (W/2.0);
  y=((grid[col][row].y*W)/4.0)+ (H/2.0);
  if(x>0&&y>0&&x<W&&y<H)
    grid[int(x)][int(y)].j_temp++;

}

int main()
{
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
        grid[col][row].j=grid[col][row].j_temp;
        grid[col][row].j_temp=0;
      }
    }

    for (unsigned int row = 0; row < H; row++)
    	for (unsigned int col = 0; col < W; col++)
    		calcPoint(col, row);

	for (unsigned int row = 0; row < H; row++)
		for (unsigned int col = 0; col < W; col++)
			drawPoint(col, row, window);

    window.display();

    cout << iteration <<endl;
    if(save())
      cout << "saved file"<<endl;
    iteration++;
  }

  cout << "close the program to exit" << endl;
  return 0;
}

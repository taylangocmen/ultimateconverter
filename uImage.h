/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uImage.h
 * Author: gocmenta
 *
 * Created on March 13, 2017, 10:24 PM
 */

#ifndef UIMAGE_H
#define UIMAGE_H

#include "utils.h"

using namespace std;

class uImage {
public:
  uImage();
  uImage(const uImage& orig);
  virtual ~uImage();
  
private:
  unsigned short*** pxArr;
  
};

#endif /* UIMAGE_H */


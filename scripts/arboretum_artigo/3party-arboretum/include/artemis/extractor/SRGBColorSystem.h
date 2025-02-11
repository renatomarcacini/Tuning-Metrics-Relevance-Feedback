/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @file
*
* This file contains the definition of the YUVColorSystem class.
*
* @version 1.0
* @date 01-10-2014
*/
#ifndef SRGBCOLORSYSTEM_H
#define SRGBCOLORSYSTEM_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <artemis/image/Pixel.h>

/**
* @version 1.0.
*/
class SRGBColorSystem {

    public:
        SRGBColorSystem();
        ~SRGBColorSystem();

        double getROfSRGB(Pixel p);
        double getGOfSRGB(Pixel p);
        double getBOfSRGB(Pixel p);
};

#endif

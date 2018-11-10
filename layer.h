//
// Created by noki on 2018/11/10.
//

#ifndef UROP_LAYER_H
#define UROP_LAYER_H
#include "allocation.h"
struct edges{
    int player;
    vector<int> B;
};

class layer {
private:
    allocation allo;
    vector<int> Xplayers;
    vector<int> Yplayers;
    vector<edges> Xedges;
    vector<edges> Yedges;
};


#endif //UROP_LAYER_H

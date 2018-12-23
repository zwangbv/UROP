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
public:

    layer(vector<edges> xedges, vector<edges> yedges,int newd,int newz);
    ~layer();
    void addX(vector<edges> Xedges);
    void addY( vector<edges> Yedges);
    vector<int> getXplayers(){return Xplayers;};
    vector<int> getYplayers(){return Yplayers;};
    vector<edges> getXedges(){return Xedges;};
    vector<edges> getYedges(){return Yedges;};
private:
    vector<int> Xplayers;
    vector<int> Yplayers;
    vector<edges> Xedges;
    vector<edges> Yedges;
    int d;
    int z;
};


#endif //UROP_LAYER_H

//
// Created by noki on 2018/11/10.
//

#include "layer.h"
#include "allocation.h"

layer::layer(vector<edges> xedges, vector<edges> yedges,int newd,int newz){
    addX(xedges);
    addY(yedges);
    d=newd;
    z=newz;
}
layer::~layer(){}
void layer::addX(vector<edges> newXedge){
    Xplayers.clear();
    for (int i=0;i<newXedge.size();i++){
        Xplayers.push_back(newXedge[i].player);
    }
    Xedges=newXedge;
}
void layer::addY( vector<edges> newYedges){
    Yplayers.clear();
    for (int i=0;i<newYedges.size();i++){
        Yplayers.push_back(newYedges[i].player);
    }
    Yedges=newYedges;
}
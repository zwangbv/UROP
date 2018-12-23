//
// Created by noki on 2018/11/10.
//
#include <queue>
#include <iostream>
#include<stdio.h>
#include <limits.h>
#include <string.h>
#include "allocation.h"
#include <vector>
#include<algorithm>
using namespace std;


allocation::allocation(double fat){
    num_players = 0;
    num_resources = 0;
    fatvalue = fat;
}

allocation::~allocation(){}

void allocation::addPlayer(int player){
    players.push_back(player);
    num_players = players.size();
}

void allocation::addResources(int resource, double value){
    resources.push_back(resource);
    values.push_back(value);
    num_resources = resources.size();
}

void allocation::addRequirement(int player, int resource){
    int index_player, index_resource;
    vector<int>::iterator it = find(players.begin(), players.end(), player);
    if (it != players.end())
        index_player=it - players.begin();
    else
        return;

    vector<int>::iterator it2 = find(resources.begin(), resources.end(), resource);
    if (it2 != resources.end())
        index_resource=it2 - resources.begin();
    else
        return;
    requirement[index_player][index_resource] = 1;
}



int** allocation::buildRequirement(){
    //requirement[player][resource]
    requirement  =  new int* [num_players];
    for(int i = 0; i < num_players; i++) {
        requirement[i] = new int[num_resources];
    }
    for(int i = 0; i < num_players; i++){
        for(int j = 0; j < num_resources; j++){
            requirement[i][j] = 0;
        }
    }
    return requirement;
}

int** allocation::buildMaxMatching(){//player & resources Adjacency matrix, size: __maxNodes*__maxNodes, if in M ij=ji=1
    int __maxNodes= num_players+num_resources;
    maxMatching  =  new int* [__maxNodes];
    for(int i = 0; i < __maxNodes; i++) {
        maxMatching[i] = new int[__maxNodes];
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            maxMatching[i][j] = 0;
        }
    }
    int **temp =  new int* [num_players+num_resources];
    for(int i = 0; i < num_players+num_resources; i++) {
        temp[i] = new int[num_resources+num_resources];
    }
    for(int i = 0; i < num_players+num_resources; i++){
        for(int j = 0; j <  num_players+num_resources; j++){
            temp[i][j] = 0;
        }
    }
    for(int i = 0; i < num_players; i++){
        for(int j = num_players; j <  num_players+num_resources; j++){
            if(requirement[i][j-num_players]==1&&values[j-num_players]>=fatvalue){
            temp[i][j] =1;
            temp[j][i]=1;
            }
        }
    }
    queue<int> Q;
    int matching[__maxNodes]; /* result*/
    int check[__maxNodes];
    int prev[__maxNodes];
    memset(matching, -1, sizeof(matching));
    memset(check, -1, sizeof(check));
    for (int i=0; i<num_players; ++i) {
        if (matching[i] == -1) {
            while (!Q.empty()) Q.pop();
            Q.push(i);
            prev[i] = -1; // 设 i 为路径起点
            bool flag = false; // 尚未找到增广路
            while (!Q.empty() && !flag) {
                int u = Q.front();
                for (int v = 0;v < __maxNodes && !flag; ++v) {
                    if(temp[u][v]==0)
                        continue;
                    if (check[v] != i) {
                        check[v] = i;
                        Q.push(matching[v]);
                        if (matching[v] >= 0) {
                            prev[matching[v]] = u;
                        } else {
                            flag = true;
                            int d=u, e=v;
                            while (d != -1) {
                                int t = matching[d];
                                matching[d] = e;
                                matching[e] = d;
                                d = prev[d];
                                e = t;
                            }
                        }
                    }
                }
                Q.pop();
            }
        }
    }
    for(int i=0;i<__maxNodes;i++){
        if(matching[i]>0){
            maxMatching[i][matching[i]]=1;
            maxMatching[matching[i]][i]=1;
        }
    }
    return maxMatching;
}

void allocation:: printMax(){
    int __maxNodes= num_players+num_resources;
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            cout<<Gm[i][j]<<",";
        }
        cout<<endl;
    }
}
int** allocation::buildGm(){//in M i,j, Gm=1, not in M, Gm ji=1
    int __maxNodes= num_players+num_resources;
    Gm  =  new int* [__maxNodes];
    for(int i = 0; i < __maxNodes; i++) {
        Gm[i] = new int[__maxNodes];
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j =0; j < __maxNodes; j++) {
            Gm[j][i] = 0;
        }
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j =0; j < __maxNodes; j++) {
            if(i<num_players && j>=num_players){
                if (requirement[i][j - num_players] == 1 && values[j - num_players]  >= fatvalue) {
                    if (maxMatching[i][j] == 1)
                        Gm[j][i] = 1;//j to i
                    else
                        Gm[i][j] = 1;//i to j
                }
            }
        }
    }
    return Gm;
}
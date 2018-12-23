#include <iostream>
#include "allocation.h"
#include <limits.h>
#include <string.h>
#include <queue>
#include "vector"
#include "algorithm"
#include "iterator"
#include "layer.h"

using namespace std;

const double EXPECTED_T=30;
const double EXPECTED_LAMDA=0.9;
const double Mu=1;//used in collapse phase
void printM(int ** m){
    int __maxNodes= 25;
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            cout<<m[i][j]<<",";
        }
        cout<<endl;
    }
}
void printVector(vector<int> v){
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<", ";
    }
}
void collapsePhase(allocation& , vector<edges>& , vector<edges>& , vector<layer>& );
allocation generateAllocation(){
    allocation b = allocation(EXPECTED_T/EXPECTED_LAMDA);
//    b.addPlayer(0);
//    b.addPlayer(1);
//    b.addPlayer(2);
//    b.addPlayer(3);
//    b.addResources(0,2);
//    b.addResources(1,2);
//    b.addResources(2,2);
//    b.addResources(3,2);
//    b.addResources(4,1);
//    b.addResources(5,1);
//    b.addResources(6,1);
//    b.addResources(7,1);
//    b.buildRequirement();
//
////    b.addRequirement(0,0);
////    b.addRequirement(0,2);
//    b.addRequirement(1,0);
//    b.addRequirement(2,2);
//    b.addRequirement(2,0);
//    b.addRequirement(2,1);
//    b.addRequirement(3,3);
//    b.addRequirement(0,4);
//    b.addRequirement(0,5);
//    b.addRequirement(0,6);
//    b.addRequirement(0,7);
//
//    b.addRequirement(1,5);
//    b.addRequirement(2,6);
//    b.addRequirement(3,7);

for(int i =0;i<10;i++){
    b.addPlayer(i);
    b.addResources(i,11+i);

}
for(int i =10;i<15;i++){
        b.addResources(i,50);
}
    b.buildRequirement();
    for(int i =0;i<10;i++){
        for(int j =10;j<15;j++){
            b.addRequirement(i,j);
        }
    }

    for(int i =0;i<10;i++){
        for(int j =0;j<7;j++){
             b.addRequirement(i,(i+j)%10);
        }
    }

    b.buildMaxMatching();
    b.buildGm();

    return b;
}

/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(int** rGraph, int s, int t, int parent[], int V)
{
    bool visited[V];
    for(int i=0;i<V;i++){
        visited[i]=false;
    }
    queue<int> q;
    q.push(s);
    visited[s]=true;
    parent[s] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();//delete the first element
        for (int i=0; i<V; i++)
        {
            if (!visited[i] && rGraph[u][i] > 0)
            {
                q.push(i);
                parent[i] = u;//the previous nodes
                visited[i] = true;
            }
        }
    }
    return (visited[t]);
}
int fordFulkerson(int** graph, int s, int t, int V)
{
    int u, v;

    int** rGraph=new int*[V];
    for(int i = 0; i < V; i++) {
        rGraph[i] = new int[V];
    }

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];

    int parent[V];  // This array is filled by BFS and to store path

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent, V))
    {
        // the maximum flow through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow
    return max_flow;
}

int** fordFulkerson_graph(int** graph, int s, int t, int V)
{
    int u, v;

    int** rGraph=new int*[V];
    for(int i = 0; i < V; i++) {
        rGraph[i] = new int[V];
    }

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];

    int parent[V];  // This array is filled by BFS and to store path

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent, V))
    {
        // the maximum flow through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }
    for(int i = 0; i < V; i++) {
        for(int j= 0; j< V; j++) {
            if(graph[i][j]!=(rGraph[i][j]))
                rGraph[i][j]=graph[i][j];
            else
                rGraph[i][j]=0;
        }
    }
    // Return the overall flow
    return rGraph;
}

int fM(allocation allo,vector<int> svector, vector<int> tvector){
    int **Gm=allo.buildGm();
    int num_players=allo.playerSize();
    int num_resources=allo.resourceSize();
    int __maxNodes=num_players+num_resources;
    int **maxFlow  =  new int* [__maxNodes+2];
    for(int i = 0; i < __maxNodes+2; i++) {
        maxFlow[i] = new int[__maxNodes+2];
    }
    for(int i = 0; i < __maxNodes+2; i++){
        for(int j = 0; j < __maxNodes+2; j++){
            maxFlow[i][j]=0;
        }
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            maxFlow[i][j]=Gm[i][j];
        }
    }
    for(int i = 0; i <svector .size() ; i++){
        maxFlow[__maxNodes][svector[i]]=1;
    }
    for(int i = 0; i <tvector .size() ; i++){
        maxFlow[tvector[i]][__maxNodes+1]=1;
    }
    return fordFulkerson(maxFlow, __maxNodes, __maxNodes+1, __maxNodes+2);
}
int** paths(allocation allo,vector<int> svector, vector<int> tvector){
    int **Gm=allo.buildGm();
    int num_players=allo.playerSize();
    int num_resources=allo.resourceSize();
    int __maxNodes=num_players+num_resources;
    int **maxFlow  =  new int* [__maxNodes+2];
    for(int i = 0; i < __maxNodes+2; i++) {
        maxFlow[i] = new int[__maxNodes+2];
    }
    for(int i = 0; i < __maxNodes+2; i++){
        for(int j = 0; j < __maxNodes+2; j++){
            maxFlow[i][j]=0;
        }
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            maxFlow[i][j]=Gm[i][j];
        }
    }
    for(int i = 0; i <svector .size() ; i++){
        maxFlow[svector[i]][__maxNodes]=1;
    }
    for(int i = 0; i <tvector .size() ; i++){
        maxFlow[__maxNodes][tvector[i]]=1;
    }
    return fordFulkerson_graph(maxFlow, __maxNodes, __maxNodes+1, __maxNodes+2);
}
vector<int> vectors_set_union(vector<int> v1,vector<int> v2){
    vector<int> v;
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));//Union
    return v;
}

vector<int> vectors_set_difference(vector<int> v1,vector<int> v2){
    vector<int> v;
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    set_difference(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));
    return v;
}

bool checkAddable(allocation allo,int p,vector<int> x,vector<int> y,vector<int> I){
    vector<int> tvector = vectors_set_union(x,I);
    int a = fM(allo,y, tvector);

    vector<int> tvector2;
    tvector2.push_back(p);
    tvector = vectors_set_union(tvector,tvector2);
    int b = fM(allo,y, tvector);
   if((a+1)==b)
       return true;
   else
       return false;
}
vector<int> checkAddableEdge(allocation allo,int p,vector<int> x,vector<int> y,vector<int> I,vector<int> B){
    vector<int> newB;
    double expectValue1 = EXPECTED_T / EXPECTED_LAMDA;
    double expectValue2 = (EXPECTED_T / EXPECTED_LAMDA) + EXPECTED_T;
    auto it = B.begin();
    double totalValue = 0;
    while (it != B.end()){
        auto it2 = find(x.begin(), x.end(), *it);
        if(it2 != x.end())
            continue;
        it2 = find(y.begin(),y.end(), *it);
        if(it2 != y.end())
            continue;
        it2 = find(I.begin(), I.end(), *it);
        if(it2 != I.end())
            continue;
        if(allo.getValue(p,*it)>0){
            totalValue+=allo.getValue(p,*it);
            if(totalValue>=expectValue2)
                break;
            newB.push_back(*it);
        }
        ++it;
    }
    if(totalValue <= expectValue1){
        vector<int> temp;
        return temp;
    }else
        return newB;
//  auto it = B.begin();
//  double totalValue = 0;
//    while (it != B.end()){
//        auto it2 = find(x.begin(), x.end(), *it);
//        if(it2 != x.end())
//            return false;
//        it2 = find(y.begin(),y.end(), *it);
//        if(it2 != y.end())
//            return false;
//        it2 = find(I.begin(), I.end(), *it);
//        if(it2 != I.end())
//            return false;
//        totalValue+=allo.getValue(p,*it);
//        ++it;
//    }
//    double expectValue1 = EXPECTED_T / EXPECTED_LAMDA;
//    double expectValue2 = (EXPECTED_T / EXPECTED_LAMDA) + EXPECTED_T;
//    if(totalValue <= expectValue2 && totalValue >= expectValue1)
//        return checkAddable(allo, p, x,y, I);
//    else
//        return false;
}

vector<int> checkUnblockable(allocation allo, int p, vector<int> B, vector<int> eps) { //eps represent the connect resource with p
    vector<int> Bprime(B);
    double expectValue=EXPECTED_T/EXPECTED_LAMDA;
    auto it = Bprime.begin();
    while (it != Bprime.end()){
        auto it2 = find(eps.begin(), eps.end(), *it);
        if(it2 != eps.end())
            it=Bprime.erase(it);
        ++it;
    }
    it = Bprime.begin();
    double totalValue = 0;
    int minValue = INT_MAX;
    auto itMin = it;
    while (it != Bprime.end()){
        double temp=allo.getValue(p,*it);
        totalValue+=allo.getValue(p,*it);
        if(temp<minValue){
            itMin=it;
            minValue=temp;
        }
        ++it;
    }
    it = Bprime.begin();
    if(totalValue<expectValue){
        vector<int> temp;
        return temp;
    }
    else{
        while(totalValue-allo.getValue(p,*itMin)>=expectValue){
            Bprime.erase(itMin);
            totalValue = 0;
            minValue = INT_MAX;
            itMin = it;
            while (it != Bprime.end()){
                double temp=allo.getValue(p,*it);
                totalValue+=allo.getValue(p,*it);
                if(temp<minValue){
                    itMin=it;
                    minValue=temp;
                }
                ++it;
            }

        }
        return Bprime;
    }
}


void buildPhase(allocation& allo, vector<edges>& eps, vector<edges>& I, vector<layer>& L){

    vector<int> newXplayers;
    vector<int> newYplayers;
    vector<edges> newXedges;
    vector<edges> newYedges;

    vector<int> Y_less_than_l;//players under l layer
    vector<int> X_less_than_l;
    vector<int> I_player;
    for(int i = 0; i < L.size();i++){
        Y_less_than_l = vectors_set_union(L[i].getYplayers(),Y_less_than_l);
        X_less_than_l = vectors_set_union(L[i].getXplayers(),X_less_than_l);
    }
    X_less_than_l = vectors_set_union(newXplayers,X_less_than_l);

    for(int i = 0 ; i < I.size();i++){
        vector<int> temp (1,I[i].player);
        I_player = vectors_set_union(temp,I_player);
    }
    //step2
    int count = 0;
    while(count < allo.playerSize()){
        count++;

        if(checkAddable(allo,count,X_less_than_l,Y_less_than_l,I_player)){
            vector<int> Yr_less_than_l;//players under l layer
            vector<int> Xr_less_than_l;
            vector<int> I_resource;
            vector<int> eps_resource;
            for(int i = 0; i < L.size();i++){
                vector<edges> Xvector=L[i].getXedges();
                vector<edges> Yvector=L[i].getYedges();
                for(int j = 0;j < Xvector.size();j++){
                    if(Xvector[j].player == count)
                        Xr_less_than_l=vectors_set_union(Xvector[j].B,Xr_less_than_l);
                }
                for(int j = 0;j < Yvector.size();j++){
                    if(Yvector[j].player == count)
                        Yr_less_than_l=vectors_set_union(Yvector[j].B,Yr_less_than_l);
                }
            }
            for(int i=0; i<I.size();i++){
                if(I[i].player==count)
                    I_resource=vectors_set_union(I[i].B,I_resource);
            }
            for(int i=0; i<eps.size();i++){
                if(eps[i].player==count)
                    eps_resource=vectors_set_union(eps[i].B,eps_resource);
            }
            vector<int> wholeResource;
            for(int i=0; i<allo.resourceSize();i++){
                wholeResource.push_back(i);
            }
            vector<int> B = checkAddableEdge(allo, count,Xr_less_than_l,Yr_less_than_l,I_resource,wholeResource);
            if(!B.empty()){
                vector<int> Bprime = checkUnblockable(allo, count, B,eps_resource);
                if(!Bprime.empty()){
                    edges tempEdge;
                    tempEdge.player = count;
                    tempEdge.B = Bprime;
                    I.push_back(tempEdge);
                    vector<int> temp (1,count);
                    I_player=vectors_set_union(temp,I_player);
                }
            }
        }

             //step3
        int count = 0;
        while(count < allo.playerSize()){
            count++;
            if(checkAddable(allo,count,X_less_than_l,Y_less_than_l,I_player)){
                vector<int> Yr_less_than_l;//players under l layer
                vector<int> Xr_less_than_l;
                vector<int> I_resource;
                vector<int> eps_resource;
                for(int i = 0; i < L.size();i++){
                    vector<edges> Xvector=L[i].getXedges();
                    vector<edges> Yvector=L[i].getYedges();
                    for(int j = 0;j < Xvector.size();j++){
                        if(Xvector[j].player == count)
                            Xr_less_than_l=vectors_set_union(Xvector[j].B,Xr_less_than_l);
                    }
                    for(int j = 0;j < Yvector.size();j++){
                        if(Yvector[j].player == count)
                            Yr_less_than_l=vectors_set_union(Yvector[j].B,Yr_less_than_l);
                    }
                }
                for(int i=0; i<I.size();i++){
                    if(I[i].player==count)
                        I_resource=vectors_set_union(I[i].B,I_resource);
                }
                for(int i=0; i<eps.size();i++){
                    if(eps[i].player==count)
                        eps_resource=vectors_set_union(eps[i].B,eps_resource);
                }
                vector<int> wholeResource;
                for(int i=0; i<allo.resourceSize();i++){
                    wholeResource.push_back(i);
                }
                vector<int> B = checkAddableEdge(allo, count,Xr_less_than_l,Yr_less_than_l,I_resource,wholeResource);
                if(!B.empty()){
                    newXplayers.push_back(count);
                    edges tempEdge;
                    tempEdge.player=count;
                    tempEdge.B=B;
                    newXedges.push_back(tempEdge);
                }
            }
        }

        //step 4
        vector<bool> boolVector(allo.resourceSize(),false);
        for(int j=0; j<newXedges.size();j++){
            for(int i = 0; i<newXedges[j].B.size(); i++){
                boolVector[newXedges[j].B[i]]=true;
            }
        }
        for(int i = 0; i<eps.size(); i++){
            for(int j=0; j<eps[i].B.size();j++){
                if(boolVector[eps[i].B[j]]){
                    newYplayers.push_back(eps[i].player);
                    edges tempEdge;
                    tempEdge.player=eps[i].player;
                    vector<int> tempB;
                    tempB.push_back(eps[i].B[j]);
                    tempEdge.B=tempB;
                    newYedges.push_back(tempEdge);
                }
            }
        }

    }
    int d= fM(allo,Y_less_than_l,vectors_set_union(X_less_than_l,I_player));
    //需要检查xpalyer重复？
    int z=newXplayers .size();
    layer newL(newXedges,newYedges,d,z);
    cout<<"in build "<<I.size()<<endl;
    L.push_back(newL);
    collapsePhase(allo,eps,I,L);
    return;
}

void collapsePhase(allocation& allo, vector<edges>& eps, vector<edges>& I, vector<layer>& L){
//step1

    int l=L.size();
    bool Collapsible=false;

    vector<int**> gamma;
    vector<vector<edges>>Ii;
    vector<int> Itarget;
    vector<int> Yi;
    vector<vector<int>> Ii_player;
    int t=l+1;
    int num_players=allo.playerSize();
    int num_resources=allo.resourceSize();
    int __maxNodes=num_players+num_resources;

    for(int i=0;i<I.size();i++){
        vector<int> temp;
        temp.push_back(I[i].player);
        Itarget=vectors_set_union(temp,Itarget);
    }

    for(int i=0;i<l;i++){
        vector<int> Yi_player;
        vector<int> Ii_temp;
        for(int j=0;j<L[i].getYedges().size();j++){
            Yi_player.push_back(L[i].getYedges()[j].player);
        }
        Yi=vectors_set_union(Yi,Yi_player);
        gamma.push_back(paths(allo,Yi_player,Itarget));
        for(int j=0;j<__maxNodes+2;j++){
            if((gamma[i])[j][__maxNodes+1]==1){
                Ii_temp.push_back(j);
            }
        }
        for(int j=0;j<I.size();j++){
            for(int k=0;k<Ii_temp.size();k++){
                if(I[j].player==Ii_temp[k]){
                    Ii[i].push_back(I[j]);
                }
            }
        }
        Ii_player.push_back(Ii_temp);
       // cout<<Ii_temp.size()<<Yi_player.size()<<endl;
        if(Ii_temp.size()>=(Mu*Yi_player.size())){
            Collapsible=true;
            t=i;
            break;
        }
    }
    if(Collapsible==false){
        buildPhase(allo, eps, I, L);
        return;
    }
    cout<<endl;
    //step 2
    while(L.size()>(t+1)){
        L.pop_back();
    }
    vector<int> tempI1;
    for(int i=0;i<t;i++){
        tempI1=vectors_set_union(tempI1,Ii_player[i]);
    }
    vector<edges> tempI2;
    for(int i=0;i<tempI1.size();i++){
        for(int j=0;j<I.size();j++){
            if(tempI1[i]==I[j].player){
                tempI2.push_back(I[j]);
            }
        }
    }
    I=tempI2;

    //step3a
    vector<edges> V;
    vector<edges> Yt=L[t].getYedges();
    for(int j=0;j<__maxNodes+2;j++){
        if((gamma[t])[__maxNodes][j]==1){
            for(int i = 0; i< Yt.size();i++){
                if(Yt[i].player == j){
                    V.push_back(Yt[i]);
                }
            }
        }
    }


//    int** matching   =  new int* [__maxNodes];
//    for(int i = 0; i < __maxNodes; i++) {
//        matching[i] = new int[__maxNodes];
//    }
//    for(int i = 0; i < __maxNodes; i++){
//        for(int j = 0; j < __maxNodes; j++){
//            matching[i][j] = allo.getMaxMatching()[i][j];
//        }
//    }

    int** gammaTplus   =  new int* [__maxNodes+2];
    for(int i = 0; i < __maxNodes+2; i++) {
        gammaTplus[i] = new int[__maxNodes+2];
    }
    for(int i = 0; i < __maxNodes+2; i++){
        for(int j = 0; j < __maxNodes+2; j++){
            gammaTplus[i][j] = ( gamma[t])[i][j];
        }
    }

    for(int j=0;j<__maxNodes+2;j++){
        if((gammaTplus)[__maxNodes][j] == 1 && (gammaTplus)[j][__maxNodes+1] ==1 ){
            (gammaTplus)[__maxNodes][j] = 0;
            (gammaTplus)[j][__maxNodes+1] = 0;
        }
    }
    for(int i=0;i<__maxNodes+2;i++) {
        for (int j = 0; j < __maxNodes + 2; j++) {
            if(gammaTplus[i][j]==1 || gammaTplus[j][i]==1){
                if(allo.getMaxMatching()[i][j]==1){
                    gammaTplus[i][j]=0;
                    gammaTplus[j][i]=0;
                }else{
                    gammaTplus[i][j]=1;
                    gammaTplus[j][i]=1;
                }

            }
        }
    }
    allo.setMaxMatching(gammaTplus);
    allo.buildGm();

//step 3b
    vector<edges> It=Ii[t];

    for(int i=0;i<It.size();i++){
        bool exist=false;
        for(int j=0;j<eps.size();j++){
            if(It[i].player==eps[j].player){
                vector<int> newB= vectors_set_union(It[i].B,eps[j].B);
                eps[j].B =newB;
                exist=true;
            }
        }
        if(exist==false)
            eps.push_back(It[i]);
    }
    cout<<"eeee";
// step 3c
    for(int i=0;i<V.size();i++){
        for(int j=0;j<eps.size();j++){
            if(It[i].player==eps[j].player){
                vector<int> newB= vectors_set_difference(eps[j].B,V[i].B);
                eps[j].B = newB;
                if(eps[j].B.empty()){
                    auto it=eps.begin();
                    it=it+j;
                    eps.erase(it);
                    j--;
                }
            }
        }

    }

    if(t==0) {

        return;
    }

    for(int i=0;i<V.size();i++){
        for(int j=0;j<Yt.size();j++){
            if(It[i].player==Yt[j].player){
                vector<int> newB= vectors_set_difference(Yt[j].B,V[i].B);
                Yt[j].B = newB;
                if(Yt[j].B.empty()){
                    auto it=Yt.begin();
                    it=it+j;
                    Yt.erase(it);
                    j--;
                }
            }
        }
    }
    L[t].addY(Yt);


    //step 4
    vector<edges> Xt=L[t].getXedges();


    for(int i =0;i<Xt.size();i++){
        int p=Xt[i].player;
        vector<int> B=Xt[i].B;
        vector <int> eps_resource;
        for(int i=0; i<eps.size();i++){
            if(eps[i].player==p)
                eps_resource=vectors_set_union(eps[i].B,eps_resource);
        }
        if(!checkUnblockable(allo,p,B,eps_resource).empty()){
            auto it=Xt.begin();
            it=it+i;
            Xt.erase(it);
        }
    }
    buildPhase(allo,eps,I,L);
}
void partialAllocation(allocation allo,  vector<edges>& eps, vector<edges>& I, vector<layer>& L){
    int** M=allo.getMaxMatching();
    int num_players=allo.playerSize();
    int num_resources=allo.resourceSize();
    int __maxNodes= num_players+num_resources;
    vector<int> resourceLeft;
    vector<int> playerLeft;
    vector<int> realPlayerLeft;

    for(int i =0;i< __maxNodes;i++){
        bool tempCheck=false;
        for(int j=0;j<__maxNodes;j++){
            if(M[i][j]==1)
                tempCheck=true;

        }
        if(!tempCheck){
            if(i>=num_players){
                resourceLeft.push_back(i-num_players);
            }else{
                playerLeft.push_back(i);
            }
        }
    }
    for(int i=0;i<playerLeft.size();i++){
        vector<int> tempB;
        vector<int> copyResource(resourceLeft);
        double valueB=0;
       int tempPlayer=playerLeft[i];
       auto it=copyResource.begin();
        while(valueB<EXPECTED_T/EXPECTED_LAMDA){
            if(it==copyResource.end())
                break;
            double tempValue=allo.getValue(tempPlayer,*it);
            if(tempValue!=0){
                valueB=valueB+tempValue;
                tempB.push_back(*it);
                copyResource.erase(it);
                it--;
            }
            if(valueB>EXPECTED_T/EXPECTED_LAMDA)
                break;
            it++;
        }
        if(valueB>=EXPECTED_T/EXPECTED_LAMDA){
            edges tempEdge;
            tempEdge.player=tempPlayer;
            tempEdge.B=tempB;
            eps.push_back(tempEdge);
            resourceLeft=copyResource;
        }else{
            realPlayerLeft.push_back(tempPlayer);
        }
    }
    for(int i=0;i<realPlayerLeft.size();i++){
        L.clear();
        vector<edges> yedges;
        vector<edges> xedges;
        edges tmepEdges;
        vector<int> emptyB;
        tmepEdges.player=realPlayerLeft[i];
        tmepEdges.B=emptyB;
        yedges.push_back(tmepEdges);
        layer L0(xedges,yedges,0,0);
        L.push_back(L0);
        buildPhase(allo,eps,I,L);
    }
    cout<<"eps::"<<endl;
    for(int i=0;i<eps.size();i++){
        for(int j=0;j<eps[i].B.size();j++)
        cout<<"("<<eps[i].player<<", "<<eps[i].B[j]<<")"<<" ";
        cout<<endl;
    }

}

void test_max_flow(){//test of Max flow
    int V=10;
    int** graph=new int*[V];
    for(int i = 0; i < V; i++) {
        graph[i] = new int[V];
    }
    for(int i = 0; i < V; i++) {
        for(int j= 0; j< V; j++) {
            graph[i] [j]= 0;
        }
    }
//    for(int i = 0; i < 5; i++)
//        graph[0][2+i]=1;
//    for(int i = 0; i <3; i++)
//        graph[7+i][1]=1;
    graph[0][2]=1;
    graph[0][6]=1;
    graph[3][1]=1;
    graph[4][1]=1;
    graph[6][1]=1;

    graph[2][7]=1;
    graph[7][3]=1;
    graph[4][7]=1;
    graph[8][4]=1;
    graph[4][9]=1;
    graph[5][8]=1;
    graph[9][5]=1;
    graph[6][9]=1;
    int** rGraph=fordFulkerson_graph(graph, 0, 1, 10);
    for(int i = 0; i < V; i++) {
        for(int j= 0; j< V; j++) {
            cout<<" "<<rGraph[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    for(int i = 0; i < V; i++) {
        for(int j= 0; j< V; j++) {
            if(graph[i][j]==(1-rGraph[i][j]))
                cout<<" "<<graph[i][j];
            else
                cout<<" "<<0;
        }
        cout<<endl;
    }
}
int main() {
//    double expectvalue=(EXPECTED_T/EXPECTED_LAMDA);
//    allocation b = generateAllocation(expectvalue);
//    cout<<2;
//    b.printMax();
//    b.buildGm();

    //test of Max flow
    allocation allo = generateAllocation();
    vector<edges> eps;
    vector<edges> I;
    vector<layer> L;
    partialAllocation(allo, eps, I, L);

    return 0;
}
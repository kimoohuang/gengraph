/*
 * Author: Jianmeng Huang 
 * <jianmenghuang<AT>gmail.com>
 *
 * File: graph.h
 * Create Date: 2014-01-11 16:25:02
 *
 */
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<map>

using namespace std;

enum LogType_enum {CLICKEVENT, INMETHOD, TAINTSINK, STRINMET, OTHER};
typedef enum LogType_enum LogType;

class Node{
    public:
        int number;
        LogType log_type;    // onclickevent, inmethod or taintsink
        string log_detail;   // classname#method
        string log_extra;    //extra info: for Method: string values; for CLICKEVENT: text info
        int log_tag = 0;  // the taint tag

        Node(){}
        Node(string line, LogType type);

        void addNumber(int num);
        void addClickInfo(string line);
        void addLogExtra(string line);
        void printNode();
};

//typedef list<Edge> edge_collection;

typedef map<string, Node*> MapAdjItem;        // map frome the name of the edge.end_point to the Node


// cotains the information of each node.
class MapAdjHead{
    public:
        int filter_flag = 0;
        int begin = 0;
        Node * node = NULL;
        MapAdjItem map_adj_item;

        MapAdjHead(Node * n);
        MapAdjHead(Node * n, bool copy);
        void printNodeDot(std::fstream &file_dot, std::fstream &file_recom);
        void printNodeDot(std::fstream &file_dot);
        void printEdgeDot(std::fstream &file_dot);
};



typedef map<string, MapAdjHead *> MapAdj;

typedef map<string, MapAdj *>  MapHeaders;  // map from the name of CLICKEVENT to the adj table of the relevent submap;

typedef map<string, bool> IsSink; 
typedef map<string, bool> HasFilter; 

class Graph{
    public:
        MapHeaders map_headers;
        IsSink is_sink;  // record whether this mapAdj have path to taintsink.
        HasFilter has_filter;
        

    public:
        Graph(){};
        //Graph(edge_collection * ec,list<node> * init, list<node> * final_point);
        ~Graph(){};
        
};

#endif // graph.h

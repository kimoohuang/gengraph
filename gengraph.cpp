#include<iostream>
#include"graph.h"

using namespace std;

LogType typeOfLine(string line){
    size_t found ;
    found = line.find("INMETHOD");
    if(found != line.npos)
        return INMETHOD;
    found = line.find("CLICKEVENT");
    if(found != line.npos)
        return CLICKEVENT;
    found = line.find("TaintSink");
    if(found != line.npos)
        return TAINTSINK;
    found = line.find("STRINMET");
    if(found != line.npos)
        return STRINMET;

    return OTHER;
}
//void printNode(Node *){
//}


int main(int argc , char * argv[]){
    Graph * graph = new Graph();

    //if(argc != 6 || (argv[2] != "1" && argv[2] != "2")){
    if(argc != 6 ){
        cout << "Usage:" <<endl;
        cout << "    gengraph --mode 1 LOG_FILE DOT_FILE RECOM_FILE" << endl;
        cout << "       mode 1 : generate DOT_FILE and RECOM_FILE from LOG_FILE." << endl;
        cout << "       LOG_FILE: the log file generated from mobilephone." << endl;
        cout << "       DOT_FILE: the dot file of the output." << endl;
        cout << "       RECOM_FILE: the file of the recommendation." << endl;
        cout << "    gengraph --mode 2 LOG_FILE RECOM_FILE FILTER_FILE" << endl;
        cout << "       mode 2 : generate FILTER_FILE from LOG_FILE from RECOM_FILE." << endl;
        cout << "       LOG_FILE: the log file generated from mobilephone." << endl;
        cout << "       RECOM_FILE: the file of the recommendation." << endl;
        cout << "       FILTER_FILE: the dot file of the filtering rules." << endl;
        exit(0);
    }

    fstream file_dot;
    fstream file_recom;
    fstream file_filter;

    if( atoi(argv[2]) == 1 ){
        file_dot.open(argv[4], ios_base::out);
        file_recom.open(argv[5], ios_base::out);
    }else if( atoi(argv[2]) == 2 ){

        file_recom.open(argv[4], ios_base::in);
        file_filter.open(argv[5], ios_base::out);

    }

    fstream file_log(argv[3], ios_base::in);
    if(file_log == NULL){
        cout << "open LOG_FILE error!" << endl;
        exit(0);
    }


    string line;
    bool log_flag = false;  // false:discard the log; 
    bool is_clickevent = false;      //false: non-clickevent log; true: clickevent
    Node  * temp_click_node = NULL;
    Node * temp_method_node_pre = NULL;
    Node * temp_method_node_cur = NULL;
    MapAdj * temp_map_adj = NULL;
    int count = 1;

    while(getline(file_log, line)){
        LogType type = typeOfLine(line);
        switch(type){
            case CLICKEVENT:
                {
                    log_flag = true;
                    if(is_clickevent == false){
                        is_clickevent = true;
                        temp_click_node = new Node(line, CLICKEVENT);
                    }else {
                        temp_click_node->addClickInfo(line);
                    }
                }
                break;
            case INMETHOD:
                {
                    if(log_flag == true){
                        if(is_clickevent == true){
                            is_clickevent = false; //change the  state  from clickevent to non-clickevent
                            // add the clickevent node to the graph 
                            MapHeaders::iterator it;
                            it = graph->map_headers.find(temp_click_node->log_detail);
                            if(it == graph->map_headers.end()){
                                //no record
                                //MapAdjHead * temp_map_adj_head =  new MapAdjHead(temp_click_node);
                                temp_click_node->addNumber(count++);  // only if there is no record in the mapheader, the node is used and stored.
                                temp_map_adj = new MapAdj();
                                temp_map_adj->insert(MapAdj::value_type(temp_click_node->log_detail, new MapAdjHead(temp_click_node)));
                                graph->map_headers.insert(MapHeaders::value_type(temp_click_node->log_detail,temp_map_adj));
                            }else{
                                //already have a record
                                temp_map_adj = it->second; // get the eixted map_adj
                                string  temp_logtail = temp_click_node->log_detail;
                                delete(temp_click_node);  // free the temp_click_node
                                temp_click_node = temp_map_adj->find(temp_logtail)->second->node;
                            }
                            temp_method_node_pre = temp_click_node;
                            //temp_click_node->printNode();   //----debug
                        }

                        temp_method_node_cur = new Node(line, INMETHOD);
                        //temp_method_node_cur->printNode();   //----debug
                        MapAdj::iterator it_m;
                        it_m = temp_map_adj->find(temp_method_node_cur->log_detail);
                        if(it_m == temp_map_adj->end()){
                            // if no record in the MapAdj , add one;
                            temp_method_node_cur->addNumber(count++);
                            temp_map_adj->insert(MapAdj::value_type(temp_method_node_cur->log_detail, new MapAdjHead(temp_method_node_cur)));
                        }else{
                            delete(temp_method_node_cur);
                            temp_method_node_cur = it_m->second->node;
                        }
                        temp_map_adj->find(temp_method_node_pre->log_detail)->second->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, temp_method_node_cur));
                        temp_method_node_pre = temp_method_node_cur;

                    }else
                        continue;
                }
                break;
            case TAINTSINK:
                {
                    if(log_flag == true){
                        is_clickevent = false;
                        //log_flag = false;
                        graph->is_sink.insert(IsSink::value_type(temp_click_node->log_detail , true));

                        temp_method_node_cur = new Node(line, TAINTSINK);
                        //temp_method_node_cur->printNode();   //----debug
                        MapAdj::iterator it_m;
                        it_m = temp_map_adj->find(temp_method_node_cur->log_detail);
                        if(it_m == temp_map_adj->end()){
                            // if no record in the MapAdj , add one;
                            temp_method_node_cur->addNumber(count++);
                            temp_map_adj->insert(MapAdj::value_type(temp_method_node_cur->log_detail, new MapAdjHead(temp_method_node_cur)));
                        }else{
                            delete(temp_method_node_cur);
                            temp_method_node_cur = it_m->second->node;
                        }
                        temp_map_adj->find(temp_method_node_pre->log_detail)->second->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, temp_method_node_cur)); // add edge 
                        temp_method_node_pre = temp_method_node_cur;
                    }else
                        continue;
                }
                break;
            case STRINMET:
                {
                    if(log_flag == true){
                        is_clickevent = false; //change the  state  from clickevent to non-clickevent
                        temp_method_node_pre->addLogExtra(line);
                    }
                }
                break;
            default:
                {
                    continue;
                }
                break;
        }//switch
    }//while


    if( atoi(argv[2]) == 1 ){
        //print the  graph

        /* recommendation:
         * the code is mixed with the PRINTEDGE code for convenience.
         */
        cout << "Done!" << endl;
        cout << "The dot file is in " << argv[4] << ", and the recommendation is in " << argv[5] << "." << endl;
        //file_recom << "Recommendation:" << endl;

        MapHeaders::iterator it_mh;
        MapAdj::iterator it_ma;
        file_dot << "digraph G {" << endl;
        //cout << "N0 [shape=ellipse,color=yellow,style=bold,label=\"Begin\"];" << endl;
        for(it_mh = graph->map_headers.begin();it_mh != graph->map_headers.end();++it_mh){
            if(graph->is_sink.find(it_mh->first)->second == true){
                file_recom << "#CLICKEVENT:" << it_mh->first << endl;            
                for(it_ma = it_mh->second->begin();it_ma != it_mh->second->end(); ++it_ma){
                    it_ma->second->printNodeDot(file_dot, file_recom); 
                }
                file_recom << endl << endl;
            }
        }


        for(it_mh = graph->map_headers.begin();it_mh != graph->map_headers.end();++it_mh){
            //cout << "N0 -> N" <<  
            if(graph->is_sink.find(it_mh->first)->second == true){
                for(it_ma = it_mh->second->begin();it_ma != it_mh->second->end(); ++it_ma){
                    it_ma->second->printEdgeDot(file_dot);    
                }
            }
        }
        file_dot << "}";

    }else if( atoi(argv[2]) == 2){
        size_t pos;
        temp_map_adj = NULL;
        // read the recom file
        while(getline(file_recom, line)){
            if(line.find("#") == line.npos){
                pos = line.find("CLICKEVENT"); 
                if(pos != line.npos){
                    string click_name = line.substr(pos+11);
                    MapHeaders::iterator it;
                    it = graph->map_headers.find(click_name);
                    //mark the CLICKEVENT in the graph map_headers.
                    graph->has_filter.insert(HasFilter::value_type(click_name,true));
                    temp_map_adj = it->second;
                }else{
                    pos = line.find("libcore");
                    if(pos != line.npos){
                        string sink_name = line.substr(pos);
                        MapAdj::iterator it_ma;
                        for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end(); ++it_ma){
                            if(it_ma->second->map_adj_item.find(sink_name) != it_ma->second->map_adj_item.end())
                                // mark the father node of the sink node
                                it_ma->second->filter_flag = 1;
                        }
                    }
                }
            }// if line.find #
        }//while getline

        temp_map_adj = NULL;
        MapAdj * temp_map_adj2 = NULL;
        HasFilter::iterator it_hf;
        for(it_hf = graph->has_filter.begin(); it_hf != graph->has_filter.end(); ++it_hf){
            MapHeaders::iterator it; // the CLICKEVENT node
            it = graph->map_headers.find(it_hf->first);
            if(temp_map_adj == NULL){
                MapAdjItem::iterator it_mai;  // mark the begin methods and erase the CLICKEVENT nodes
                // insert an start node
                it->second->insert(MapAdj::value_type("START",new MapAdjHead(new Node("START",OTHER))));
                for(it_mai = it->second->find(it->first)->second->map_adj_item.begin();it_mai != it->second->find(it->first)->second->map_adj_item.end();++it_mai){
                    it->second->find(it_mai->first)->second->begin = 1;
                    it->second->find("START")->second->map_adj_item.insert(MapAdjItem::value_type(it_mai->first,it_mai->second));
                }
                it->second->erase(it->first);
                temp_map_adj = it->second;
            }else{  // combine all the filter graphes into one


                temp_map_adj2 = it->second;
                MapAdj::iterator it_ma_t; 
                MapAdj::iterator it_ma; 
                for(it_ma = temp_map_adj2->begin();it_ma!=temp_map_adj2->end();++it_ma){
                    it_ma_t = temp_map_adj->find(it_ma->first);
                    if(it_ma_t != temp_map_adj->end()){
                        it_ma_t->second->filter_flag += it_ma->second->filter_flag;
                    }else{
                        //temp_map_adj->insert(MapAdj::value_type(it_ma->first,it_ma->second));
                        temp_map_adj->insert(MapAdj::value_type(it_ma->first,new MapAdjHead(it_ma->second->node,true)));
                    }
                }// for   insert nodes.

                MapAdjItem::iterator it_mai_1;  // mark the begin methods and erase the CLICKEVENT nodes
                for(it_mai_1 = it->second->find(it->first)->second->map_adj_item.begin();it_mai_1 != it->second->find(it->first)->second->map_adj_item.end();++it_mai_1){
                    //it->second->find(it_mai_1->first)->second->begin = 1;
                    temp_map_adj->find("START")->second->map_adj_item.insert(MapAdjItem::value_type(it_mai_1->first,temp_map_adj->find(it_mai_1->first)->second->node));
                }
                temp_map_adj->erase(it->first);
                temp_map_adj2->erase(it->first);


                // update teh edges
                for(it_ma = temp_map_adj2->begin();it_ma!=temp_map_adj2->end();++it_ma){
                    it_ma_t = temp_map_adj->find(it_ma->first); //items in temp_map_adj
                    MapAdjItem::iterator it_mai;
                    MapAdjItem::iterator it_mai_t;
                    for(it_mai = it_ma->second->map_adj_item.begin();it_mai != it_ma->second->map_adj_item.end(); ++it_mai){
                        it_mai_t = it_ma_t->second->map_adj_item.find(it_mai->first);
                        if(it_mai_t == it_ma_t->second->map_adj_item.end()){
                            it_ma_t->second->map_adj_item.insert(MapAdjItem::value_type(it_mai->first, temp_map_adj->find(it_mai->first)->second->node));
                        }
                    }
                    //    temp_map_adj->insert(MapAdj::value_type(it_ma->first,it_ma->second));
                }//for   insert edge



            }//if  temp_map_header
        }// for has_filter

        MapAdj::iterator it_ma_t;
        MapAdj::iterator it_ma;
        MapAdjItem::iterator it_mai_2;
        MapAdjItem::iterator it_mai;
        MapAdjItem::iterator it_mai_t;
        for(it_ma_t = temp_map_adj->begin();it_ma_t != temp_map_adj->end();++it_ma_t){
            if(it_ma_t->second->node->log_type == TAINTSINK){
                for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end();++it_ma){
                    if(it_ma->second->filter_flag != 0){
                            cout <<"fuck"<<endl;
                        it_mai_2 = it_ma->second->map_adj_item.find(it_ma_t->first);
                        if(it_mai_2 != it_ma->second->map_adj_item.end()){
                            it_ma->second->map_adj_item.erase(it_ma_t->first);//erase the sink edge
                            for(it_mai = it_ma_t->second->map_adj_item.begin();it_mai != it_ma_t->second->map_adj_item.end(); ++it_mai){
                                it_mai_t = it_ma->second->map_adj_item.find(it_mai->first);
                                if(it_mai_t == it_ma->second->map_adj_item.end()){
                                    it_ma->second->map_adj_item.insert(MapAdjItem::value_type(it_mai->first, temp_map_adj->find(it_mai->first)->second->node));
                                }
                            }
                        }
                    }

                }
            }//if TAINTSINK
        }

        for(it_ma_t = temp_map_adj->begin();it_ma_t != temp_map_adj->end();++it_ma_t){
            if(it_ma_t->second->node->log_type == TAINTSINK){
                temp_map_adj->erase(it_ma_t->first);
            }
        }

        //print the filter graph
        //MapAdj::iterator it_ma;
        file_filter << "digraph G {" << endl;
        for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end(); ++ it_ma){
            it_ma->second->printNodeDot(file_filter);
        }//for it_ma
        for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end(); ++ it_ma){
            it_ma->second->printEdgeDot(file_filter);
        }//for it_ma
        file_filter << "}";
    }//if argv



    file_dot.close();
    file_log.close();
    file_filter.close();
    file_recom.close();
}// main

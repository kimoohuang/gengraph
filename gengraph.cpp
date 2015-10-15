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
//

//compare the log_detail of two nodes, if equal, return TRUE, else retrun false.
bool compareNode(Node * a, Node * b){
    if(a == NULL || b == NULL)
        return false;
    if(a->log_detail.compare(b->log_detail) == 0)
        return true;
    else 
        return false;
}

void freeMapAdj(MapAdj * ma){
    MapAdj::iterator ma_it;
    if(ma != NULL){
        for(ma_it = ma->begin(); ma_it != ma->end(); ++ma_it)
        {
            //delete((*ma_it)->node);
            (*ma_it)->node = NULL;
        }
    }
}


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
    bool already = false;
    bool first_method = true;
    Node  * temp_click_node = NULL;
    Node * temp_method_node_tri = NULL;
    Node * temp_method_node_pre = NULL;
    Node * temp_method_node_cur = NULL;
    MapAdjHead * mapadjhead_cli = NULL;
    MapAdjHead * mapadjhead_tri = NULL;
    MapAdjHead * mapadjhead_pre= NULL;
    MapAdjHead * mapadjhead_cur= NULL;
    MapAdj * temp_map_adj = NULL;
    MapAdj * map_adj = NULL;
    int count = 1;

    while(getline(file_log, line)){
        LogType type = typeOfLine(line);
        switch(type){
            case CLICKEVENT:
                {
                    log_flag = true;
                    first_method = true;
                    if(temp_map_adj != NULL){
                        freeMapAdj(temp_map_adj);
                        //delete(temp_map_adj);
                        temp_map_adj = NULL;
                    }
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
                                map_adj = new MapAdj();
                                //temp_map_adj->insert(MapAdj::value_type(temp_click_node->log_detail, new MapAdjHead(temp_click_node)));
                                mapadjhead_cur = new MapAdjHead(temp_click_node);  // create a new NODE
                                mapadjhead_cli = mapadjhead_cur; 
                                map_adj->push_back(mapadjhead_cur);

                                graph->map_headers.insert(MapHeaders::value_type(temp_click_node->log_detail,map_adj));
                            }else{
                                already = true;
                                //already have a record
                                map_adj = it->second; // get the eixted map_adj
                                //string  temp_logtail = temp_click_node->log_detail;
                                delete(temp_click_node);  // free the temp_click_node
                                mapadjhead_cur = map_adj->front();  
                                temp_click_node = mapadjhead_cur->node;
                            }
                            temp_method_node_tri= temp_method_node_pre;
                            temp_method_node_pre = temp_click_node;
                            mapadjhead_tri = mapadjhead_pre;
                            mapadjhead_pre = mapadjhead_cur;
                            //temp_click_node->printNode();   //----debug
                        }

                        if(first_method){
                            first_method = false;
                            temp_map_adj = new MapAdj();
                        }

                        temp_method_node_cur = new Node(line, INMETHOD);  // the cur method node is created now.


                        if(already){
                            MapAdjItem::iterator mai_it;
                            mai_it =mapadjhead_pre->map_adj_item.find(temp_method_node_cur->log_detail);
                            if(mai_it != mapadjhead_pre->map_adj_item.end()){
                                delete(temp_method_node_cur);
                                mapadjhead_cur = mai_it->second;
                                temp_method_node_cur = mapadjhead_cur->node;
                            }else{
                                already = false;
                                temp_method_node_cur->addNumber(count++);
                                mapadjhead_cur = new MapAdjHead(temp_method_node_cur);
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail,mapadjhead_cur));
                                temp_map_adj->push_back(mapadjhead_cur);
                            }
                        }else{
                            //temp_method_node_cur->printNode();   //----debug
                            if(compareNode(temp_method_node_pre,temp_method_node_cur)){ // one method loop
                                delete(temp_method_node_cur);
                                temp_method_node_cur = temp_method_node_pre;
                                mapadjhead_cur = mapadjhead_pre;
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, mapadjhead_cur));

                            }else if(compareNode(temp_method_node_tri,temp_method_node_cur)){ // two method loop
                                delete(temp_method_node_cur);
                                temp_method_node_cur = temp_method_node_tri;
                                mapadjhead_cur = mapadjhead_tri;
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, mapadjhead_cur));
                            }else {  // we have to keep  new node
                                temp_method_node_cur->addNumber(count++);
                                mapadjhead_cur = new MapAdjHead(temp_method_node_cur);
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail,mapadjhead_cur));
                                temp_map_adj->push_back(mapadjhead_cur);
                            }
                        }
                        temp_method_node_tri= temp_method_node_pre;
                        temp_method_node_pre = temp_method_node_cur;
                        mapadjhead_tri = mapadjhead_pre;
                        mapadjhead_pre = mapadjhead_cur;

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

                        if(already){
                            MapAdjItem::iterator mai_it;
                            mai_it =mapadjhead_pre->map_adj_item.find(temp_method_node_cur->log_detail);
                            if(mai_it != mapadjhead_pre->map_adj_item.end()){
                                delete(temp_method_node_cur);
                                mapadjhead_cur = mai_it->second;
                                temp_method_node_cur = mapadjhead_cur->node;
                            }else{
                                already = false;
                                temp_method_node_cur->addNumber(count++);
                                mapadjhead_cur = new MapAdjHead(temp_method_node_cur);
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail,mapadjhead_cur));
                                temp_map_adj->push_back(mapadjhead_cur);
                            }
                        }else{
                            if(compareNode(temp_method_node_pre,temp_method_node_cur)){ // one method loop
                                delete(temp_method_node_cur);
                                temp_method_node_cur = temp_method_node_pre;
                                mapadjhead_cur = mapadjhead_pre;
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, mapadjhead_cur));
                            }else if(compareNode(temp_method_node_tri,temp_method_node_cur)){ // two method loop
                                delete(temp_method_node_cur);
                                temp_method_node_cur = temp_method_node_tri;
                                mapadjhead_cur = mapadjhead_tri;
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail, mapadjhead_cur));
                            }else {  // we have to keep  new node
                                temp_method_node_cur->addNumber(count++);
                                mapadjhead_cur = new MapAdjHead(temp_method_node_cur);
                                mapadjhead_pre->map_adj_item.insert(MapAdjItem::value_type(temp_method_node_cur->log_detail,mapadjhead_cur));
                                temp_map_adj->push_back(mapadjhead_cur);
                            }
                        }
                        //                        temp_method_node_tri= NULL;
                        //                        temp_method_node_pre = NULL;
                        //                        mapadjhead_tri = NULL;
                        //                        mapadjhead_pre = NULL;
                        first_method = true;
                        if(temp_map_adj->size()>1)
                            map_adj->insert(map_adj->end(), temp_map_adj->begin(), temp_map_adj->end());
                        //freeMapAdj(temp_map_adj);
                        delete(temp_map_adj);
                        temp_map_adj = NULL;
                        temp_method_node_tri= NULL; 
                        temp_method_node_pre = temp_click_node;
                        mapadjhead_tri = NULL;
                        mapadjhead_pre = mapadjhead_cli;

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

                    if(temp_map_adj != NULL){
                        freeMapAdj(temp_map_adj);
                        //delete(temp_map_adj);
                        temp_map_adj = NULL;
                    }

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
                    (*it_ma)->printNodeDot(file_dot, file_recom); 
                }
                file_recom << endl << endl;
            }
        }

        for(it_mh = graph->map_headers.begin();it_mh != graph->map_headers.end();++it_mh){
            //cout << "N0 -> N" <<  
            if(graph->is_sink.find(it_mh->first)->second == true){
                for(it_ma = it_mh->second->begin();it_ma != it_mh->second->end(); ++it_ma){
                    (*it_ma)->printEdgeDot(file_dot);    
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
                            if((*it_ma)->map_adj_item.find(sink_name) != (*it_ma)->map_adj_item.end())
                                // mark the father node of the sink node
                                (*it_ma)->filter_flag = 1;
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
                mapadjhead_cur =  new MapAdjHead(new Node("START",OTHER));  //store the start node
                //it->second->push_back(mapadjhead_cur);
                for(it_mai = it->second->front()->map_adj_item.begin();it_mai != it->second->front()->map_adj_item.end();++it_mai){
                    it->second->front()->begin = 1;
                    mapadjhead_cur->map_adj_item.insert((*it_mai));
                }
                it->second->erase(it->second->begin());
                it->second->insert(it->second->begin(),mapadjhead_cur);
                temp_map_adj = it->second;
            }else{  // combine all the filter graphes into one

                temp_map_adj2 = it->second;
                MapAdj::iterator it_ma_t; 
                MapAdj::iterator it_ma; 
                /*
                   for(it_ma = temp_map_adj2->begin();it_ma!=temp_map_adj2->end();++it_ma){
                   it_ma_t = temp_map_adj->find(it_ma->first);
                   if(it_ma_t != temp_map_adj->end()){
                   it_ma_t->second->filter_flag += it_ma->second->filter_flag;
                   }else{
                //temp_map_adj->insert(MapAdj::value_type(it_ma->first,it_ma->second));
                temp_map_adj->insert(MapAdj::value_type(it_ma->first,new MapAdjHead(it_ma->second->node,true)));
                }
                }// for   insert nodes.
                */
                temp_map_adj->insert(temp_map_adj->end(),temp_map_adj2->begin()+1,temp_map_adj2->end());  //insert nodes except the CLICKNODE

                MapAdjItem::iterator it_mai_1;  // mark the begin methods and erase the CLICKEVENT nodes
                for(it_mai_1 = it->second->front()->map_adj_item.begin();it_mai_1 != it->second->front()->map_adj_item.end();++it_mai_1){
                    //it->second->find(it_mai_1->first)->second->begin = 1;
                    mapadjhead_cur->map_adj_item.insert((*it_mai_1));
                }

                /*
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
                */


            }//if  temp_map_header
        }// for has_filter

        MapAdj::iterator it_ma_t;
        MapAdj::iterator it_ma;
        MapAdjItem::iterator it_mai_2;
        MapAdjItem::iterator it_mai;
        MapAdjItem::iterator it_mai_t;
        for(it_ma_t = temp_map_adj->begin();it_ma_t != temp_map_adj->end();++it_ma_t){
            if((*it_ma_t)->node->log_type == TAINTSINK){
                for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end();++it_ma){
                    if((*it_ma)->filter_flag != 0){
                        it_mai_2 = (*it_ma)->map_adj_item.find((*it_ma_t)->node->log_detail);
                        if(it_mai_2 != (*it_ma)->map_adj_item.end()){
                            (*it_ma)->map_adj_item.erase((*it_ma_t)->node->log_detail);//erase the sink edge
                            for(it_mai = (*it_ma_t)->map_adj_item.begin();it_mai != (*it_ma_t)->map_adj_item.end(); ++it_mai){
                                it_mai_t = (*it_ma)->map_adj_item.find(it_mai->first);
                                if(it_mai_t == (*it_ma)->map_adj_item.end()){
                                    (*it_ma)->map_adj_item.insert((*it_mai));
                                }
                            }
                        }
                    }

                }
            }//if TAINTSINK
        }

        bool del_all = false;
        while(!del_all){ 
            for(it_ma_t = temp_map_adj->begin();it_ma_t != temp_map_adj->end();++it_ma_t){
                del_all = true;
                if((*it_ma_t)->node->log_type == TAINTSINK){
                    temp_map_adj->erase(it_ma_t);
                    del_all = false;
                    break;
                }
            }
        }

        //print the filter graph
        //MapAdj::iterator it_ma;
        count = 0;
        file_filter << "#The hash value of epsilon is 1160755946; And the hash of ∑ is 2146952987."<< endl;
        file_filter << "digraph G {" << endl;
            file_filter << "node [shape = circle];" << endl; 
        int numoffil = 0;
        //MapAdjItem::iterator it_mai; 
        for(it_mai = temp_map_adj->front()->map_adj_item.begin(); it_mai != temp_map_adj->front()->map_adj_item.end(); ++it_mai){
            if(it_mai->second->node!=NULL)
                numoffil ++;
        }
        cout << numoffil <<endl;

        for(it_ma = temp_map_adj->begin();it_ma != temp_map_adj->end(); ++ it_ma){
            //(*it_ma)->printNodeDot(file_filter);
            (*it_ma)->printFilNodeDot(file_filter,count++, numoffil);
        }//for it_ma
            file_filter << "N0 -> N0 [ label = \""<< BKDRHash("∑")<<"\" ];" << endl; 

        for(int i_count=0; i_count< numoffil; ++i_count)
            file_filter << "N0 -> N"<<i_count+1<< " [ label = \""<<BKDRHash("epsilon")<<"\" ];" << endl;
        temp_map_adj->front()->printFront(file_filter, numoffil);
        for(it_ma = temp_map_adj->begin()+1;it_ma != temp_map_adj->end(); ++ it_ma){
            //(*it_ma)->printEdgeDot(file_filter);
            (*it_ma)->printFilEdgeDot(file_filter,numoffil);
        }//for it_ma
        file_filter << "}";
        file_filter << "#"<<count + numoffil<<endl;
    }//if argv



    file_dot.close();
    file_log.close();
    file_filter.close();
    file_recom.close();
}// main

#include"graph.h"

Node::Node(string line, LogType type){
    //   number = num;
    log_type = type;
    switch(type){
        case INMETHOD:
            {
                size_t p1 = line.find("tag:");
                size_t p2 = line.find(" Method:");
                if(p1 != line.npos && p2 != line.npos){
                    log_tag = atoi(line.substr(p1+4, p2-p1-4).c_str());
                    log_detail = line.substr(p2+8);
                    log_extra = "String args:";
                }
            }
            break;
        case CLICKEVENT:
            {
                size_t p1 = line.find("ONCLICK");
                if(p1 != line.npos){
                    log_detail = line.substr(p1+8);
                    log_extra = "no";
                    log_tag = 0;
                }
                size_t p3 = line.find("class");
                if(p3 != line.npos){
                    log_extra = line.substr(p3+6);
                    log_tag = 0;
                    log_detail = "onclick";
                    //log_tag = 9;  // why 9?
                }
            }
            break;
        case TAINTSINK:
            {
                size_t p4 = line.find("TaintSink");
                //size_t p5 = line.find("tag");
                //size_t p6 = line.find(" data");
                if(p4 != line.npos){
                    log_detail = line.substr(p4 +10);
                }

            }
            break;
        default:
            break;
    }
}
void Node::addNumber(int num){
    number = num;
}

void Node::addClickInfo(string line){
    size_t p1 = line.find("ONCLICK");
    if(p1 != line.npos){
        log_detail = line.substr(p1+8);
    }
}

void Node::addLogExtra(string line){
    size_t p1 = line.find("#");
    if(p1 != line.npos){
        log_extra.append(line.substr(p1));
    }
}

void Node::printNode(){
    switch(log_type){
        case INMETHOD: {
                           cout << "INMETHOD:";
                       }
                       break;
        case CLICKEVENT:
                       {
                           cout << "CLICKEVENT";
                       }
                       break;
        case TAINTSINK:
                       {
                           cout << "TAINTSINK";
                       }
                       break;
        default:
                       break;
    }
    cout << log_tag << log_detail << endl;
}

MapAdjHead::MapAdjHead(Node * n){
    node = n;
    //number = num;
}

MapAdjHead::MapAdjHead(Node * n, bool copy){
    node = new Node();

    node->number = n->number;
    node->log_type = n->log_type;
    node->log_detail = n->log_detail;
    node->log_tag = n->log_tag;
}

/*
 * replace s2 to s3 in string s1.
 */

string& string_replace(string& s1,const string& s2,const string& s3){
    size_t pos=0;
    size_t a=s2.size();
    size_t b=s3.size();
    while((pos=s1.find(s2,pos))!=string::npos)
    {
        s1.replace(pos,a,s3);
        pos+=b;
    }
    return s1;
}

/*
 * add \n to the string every 40 postions.
 */
string& string_insert_newline(string & str){
    size_t pos=40;
    if(str.length() > 50){
        for(pos = 40; pos < str.length();pos+=40){
            str.insert(pos,"-\\n");
            pos +=3;
        }
    }
    return str;
}

void MapAdjHead::printNodeDot(fstream &dot_file, fstream &file_recom){
    if(node != NULL){
    dot_file << "N" << node->number << " [";
        switch(node->log_type){
            case INMETHOD:
                {   
                    if(begin ==0 ){
                        if(node->log_extra.length() > 13)
                            dot_file << "shape=box,color=black,style=bold,label=\"Node:" <<node->number<<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail << "\\n" <<node->log_extra << "\"];" << endl;
                        else
                            dot_file << "shape=box,color=black,style=bold,label=\"Node:" <<node->number <<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail << "\"];" << endl;
                    }
                    else{
                        if(node->log_extra.length() > 13)
                            dot_file << "shape=box,color=blue,style=bold,label=\"Node:" <<node->number<<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail << "\\n" <<node->log_extra << "\"];" << endl;
                        else
                            dot_file << "shape=box,color=blue,style=bold,label=\"Node:" <<node->number <<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail << "\"];" << endl;
                    }
                }
                break;
            case CLICKEVENT:
                {
                    if(node->log_extra.length() > 3)
                        dot_file << "shape=ellipse,color=blue,style=bold,label=\"Node:" << node->number << "\\n" << node->log_detail<<"\\n"<<node->log_extra << "\"];" << endl;
                    else
                        dot_file << "shape=ellipse,color=blue,style=bold,label=\"Node:" << node->number << "\\n" << node->log_detail << "\"];" << endl;
                }
                break;
            case TAINTSINK:
                {
                    file_recom << "    #----> Node:" << node->number << " " << node->log_detail  << endl;
                    dot_file << "shape=ellipse,color=red,style=bold,label=\"Node:" << node->number << "\\n" << string_insert_newline(string_replace(string_replace(node->log_detail,"\"",""),"]","")) << "\"];" << endl;
                }
                break;
            default:
                dot_file << "shape=ellipse,color=blue,style=bold,label=\"START\"];" << endl;
                break;

        }
    }else
        dot_file << "N0 [shape=ellipse,color=blue,style=bold,label=\"START\"];" << endl;

}

void MapAdjHead::printNodeDot(fstream &dot_file){
    if(node != NULL){
    dot_file << "N" << node->number << " [";
        switch(node->log_type){
            case INMETHOD:
                {
                    if(filter_flag == 0){
                        if(node->log_extra.length() > 13)
                            dot_file << "shape=box,color=black,style=bold,label=\"" <<"Tag:"<< node->log_tag << "\\n" << node->log_detail << "\\n" <<node->log_extra << "\"];" << endl;
                        else
                            dot_file << "shape=box,color=black,style=bold,label=\"" <<"Tag:"<< node->log_tag << "\\n" << node->log_detail << "\"];" << endl;
                    }
                    else{
                        if(node->log_extra.length() > 13)
                            dot_file << "shape=box,color=red,style=bold,label=\"Block the Message!" <<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail<<"\\n"<<node->log_extra << "\"];" << endl;
                        else
                            dot_file << "shape=box,color=red,style=bold,label=\"Block the Message!" <<"\\nTag:"<< node->log_tag << "\\n" << node->log_detail << "\"];" << endl;
                    }

                }
                break;
            case CLICKEVENT:
                {
                    if(node->log_extra.length() > 3)
                        dot_file << "shape=ellipse,color=blue,style=bold,label=\"Node:" << node->number << "\\n" << node->log_detail<<"\\n"<<node->log_extra << "\"];" << endl;
                    else
                        dot_file << "shape=ellipse,color=blue,style=bold,label=\"Node:" << node->number << "\\n" << node->log_detail << "\"];" << endl;
                }
                break;
            case TAINTSINK:
                {
                    dot_file << "shape=ellipse,color=red,style=bold,label=\"Node:" << node->number << "\\n" << string_insert_newline(string_replace(string_replace(node->log_detail,"\"",""),"]","")) << "\"];" << endl;
                }
                break;
            default:
                dot_file << "shape=ellipse,color=blue,style=bold,label=\"START\"];" << endl;
                break;
        }
    }else
        dot_file << "N0 [shape=ellipse,color=blue,style=bold,label=\"START\"];" << endl;
}

void MapAdjHead::printEdgeDot(fstream &dot_file){
    MapAdjItem::iterator it_mai;
    for(it_mai = map_adj_item.begin();it_mai != map_adj_item.end(); ++it_mai){
        if(it_mai->second->node != NULL)
            dot_file << "N" << node->number << " -> " << "N" << it_mai->second->node->number << ";" << endl;
    }
}

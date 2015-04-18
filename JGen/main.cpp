#include <cstdlib>
#include <fstream>
#include <string>
#include <time.h>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <algorithm>
#include <iostream>



using namespace std;
int memory [] = {4, 8 ,12, 16 };
int cores [] = {2, 4, 6, 8};


double fRand(double fMin, double fMax)
{
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

int getDep(int tasksnum){
    return (int)fRand(0,tasksnum);
}

void GenerateTasks(string input, string output){
    
    std::ifstream tasks(input);
    ofstream trace;
    trace.open(output);
    std::string str;
    while(std::getline(tasks,str)) {    
        std::mt19937 rng, rng2;
        rng.seed(std::random_device()());
        rng2.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0,3);
        int index1 = dist6(rng);
         int index2 = dist6(rng2);
         trace <<  str << "\t"<<memory[index1]<<"\t" << cores[index2] << std::endl;
    }
    trace.close();
}

void isCycleHelper(vector<list<int> > &graph, int node, vector<int> &status, bool &found){
    if(found || status[node] == 1)
        return;
    if(status[node] == 0){
        found = true;
        return;
    }
    status[node] = 0;
    for(list<int>::iterator iter = graph[node].begin(); iter != graph[node].end(); ++iter){
        isCycleHelper(graph, *iter, status, found);
    }
    status[node] = 1;
}

bool isCycle(vector<list<int> > &graph) {
    int graph_size = graph.size();
    //-1 not visited, 0 visiting, 1 visited
    vector<int> status(graph_size, -1);
    status[0] = 0;
    bool found = false;
    for(int i=0; i<graph.size();i++) {
        for(list<int>::iterator iter = graph[i].begin(); iter != graph[i].end(); ++iter){
            isCycleHelper(graph, *iter, status, found);
        }
    }
    status[0] = 1;
    return found;
}


void GenerateJobs(string inputfile){
    std::vector<string> tasks;
    std::list<int> task;
    std::ifstream input(inputfile);
    string str;
    while(std::getline(input, str)){
        tasks.push_back(str);
    }
    input.close();
    std::cout<<"Tasks size = " <<tasks.size()<<"\t";
    
    std::mt19937 tasksrand;
    tasksrand.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(5,20);
    int tasksnum = dist6(tasksrand);
   
    std::vector<std::list<int>> job,tmp;
    std::list<int>::const_iterator tmp_iter;
    ofstream output;
    output.open("jobs_trace.txt");
    int depnum, count, t = 0;
    int jobid = 1;
    while(!tasks.empty()) {
        tasksnum = dist6(tasksrand);
        std::cout<<tasksnum<<std::endl;
        if(tasksnum > tasks.size())
            tasksnum = tasks.size();
        //tasksnum = 5;
        for(int i=0; i<tasksnum; i++){
            tmp.push_back(task);
        }
        
        for(int i =0 ; i < tasksnum; i++){
            depnum = count = (int)fRand(1,3);
            std::cout<<"depnum\t"<<depnum<<std::endl;
            int x =5;
            while(count != 0) {
                t = getDep(tasksnum);
                if (!(std::find(tmp[i].begin(), tmp[i].end(), t) != tmp[i].end())) {
                    tmp[i].push_back(t);
                    x--;
                    if (t != i && !isCycle(tmp))
                        count --;
                    else
                        tmp[i].pop_back();
                
                } else {
                    x--;
                }
                
                if(x==0)
                    count =0;
            }
        }
        
        
        //print the job, tmp.size, tmp
        
        //std::cout << jobid<<"\t"<<tasksnum<<std::endl;
        output << jobid<<"\t"<<tmp.size()<<std::endl;

     
        for(int i=0; i < tmp.size(); i++) {
            //std::cout << tasks[tasks.size()-1]<<"\t"<<tmp[i].size()<<"\t";
            output << i<<"\t"<< tasks[tasks.size()-1]<<"\t"<<tmp[i].size()<<"\t";
            for(tmp_iter = tmp[i].begin(); tmp_iter !=tmp[i].end(); ++tmp_iter) {
                std::cout<<"tmp size = "<<tmp[i].size()<<std::endl;
              //  std::cout<<*tmp_iter<<"\t";
                output <<*tmp_iter<<"\t";
            }
            //std::cout<<std::endl;
            output<<std::endl;
            tasks.pop_back();
        
        }
        tmp.clear();
        
        std::cout<<"Current size of tasks = " <<tasks.size()<<std::endl;
        jobid++;
        
    }
    output.close();
    std::cout<<"done!"<<std::endl;


}






int main(int argc, char** argv) {
    
    GenerateJobs("tasks");

    return 0;
}


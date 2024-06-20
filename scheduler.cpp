#include<bits/stdc++.h>
using namespace std;
#define ll long long

struct process{
    ll arrivalTime;
    ll burstTime;
    ll completionTime;
    ll waitingTime;
    ll turnAroundTime;
    ll responseTime;
    ll num;
}p[1000];


bool comp(struct process a, struct process b){
    int x = a.arrivalTime;
    int y = b.arrivalTime;
    if(x<y) return true;
    return false;
}
int shortestjob(int n,int time, vector<int>& vis){
    int minD = INT_MAX, ind=-1;
    for(int i=0;i<n;i++){
        if(vis[i]==1)
        continue;
        if(p[i].arrivalTime<=time){
            if(minD>p[i].burstTime){
                ind = i;
                minD = p[i].burstTime;
            }
        }
        else
        break;
    }
    return ind;
}
int shortest_rem_job(int n,int time, vector<int>& vis, vector<ll>& remTime){
    int minD = INT_MAX, ind=-1;
    for(int i=0;i<n;i++){
        if(vis[i]==1)
        continue;
        if(p[i].arrivalTime<=time){
            if(minD>remTime[i]){
                ind = i;
                minD = remTime[i];
            }
        }
        else
        break;
    }
    return ind;
}
void round_robin_job(int n,int time, vector<int>& vis,vector<int>& present,queue<int>&q, int curr){
    int minD = INT_MAX, ind=-1;
    for(int i=0;i<n;i++){
        if(vis[i]==1)
        continue;
        if(p[i].arrivalTime<=time && i!=curr && present[i]!=1){

            q.push(i);
            present[i]=1;
        }
        if(p[i].arrivalTime>time){
            break;
        }
    }

}
int main(){
    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    int type;
    infile >> type;
    // 1 for FCFS,  2 for SJf 3 for SRTF 4 for Round Robin
    int n; infile >> n;
    //First Come First Serve
    int timeQuantum;
    
    infile>>timeQuantum;
    for(int i=0; i<n; i++){
        infile >> p[i].arrivalTime >> p[i].burstTime;
        p[i].num = i+1;
    }
    vector<string>gantt_chart;
    double avgTurnAroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;
    sort(p, p+n, comp);
    if(type == 1){
        p[0].waitingTime = p[0].responseTime = 0;
        p[0].completionTime = p[0].burstTime;
        p[0].turnAroundTime = p[0].burstTime;
        avgTurnAroundTime = p[0].burstTime;

            gantt_chart.push_back("P1");
        for(int i=1; i<n; i++){
            string st="P" + ((char)(i+49));
            gantt_chart.push_back(st);
            p[i].completionTime = max(p[i-1].completionTime, p[i].arrivalTime) + p[i].burstTime;
            p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
            p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
            p[i].responseTime = p[i].waitingTime;
            avgResponseTime += p[i].responseTime;
            avgWaitingTime += p[i].waitingTime;
            avgTurnAroundTime += p[i].turnAroundTime;
        }
        avgResponseTime/=n;
        avgWaitingTime/=n;
        avgTurnAroundTime/=n;
        for(int i=0; i<n; i++){
            auto it = p[i];
            outfile<<it.num<<"\t"<< it.arrivalTime << "\t" << it.burstTime << "\t" << it.completionTime << "\t" << it.turnAroundTime << "\t" << it.waitingTime << "\t" << it.responseTime << endl;
        }
    }
    else if(type==2){
         // Shortest Job Serve
        vector<int>vis(n,0);
        int done = 0; ll time=p[0].arrivalTime;
        int prev=-1;
        while(done!=n){

            
            int i= shortestjob(n,time,vis);
            // outfile<<" i "<<i<<"\t";
            string st="P" + ((char)(i+49));
            gantt_chart.push_back(st);
                if(done==0){
                p[i].completionTime = p[i].arrivalTime+ p[i].burstTime;
                }
                else{
                p[i].completionTime = max(p[prev].completionTime, p[i].arrivalTime) + p[i].burstTime;
                }
                p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
                p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
                p[i].responseTime = p[i].waitingTime;

                avgResponseTime +=p[i].responseTime;
                avgWaitingTime += p[i].waitingTime;
                avgTurnAroundTime += p[i].turnAroundTime; 

                vis[i]=1;
                prev=i;
                done++; time = max(p[i].completionTime,p[i+1].arrivalTime);
            
        }
        avgResponseTime/=n;
        avgWaitingTime/=n;
        avgTurnAroundTime/=n;
        for(int i=0;i<n;i++){
            auto it = p[i];
            outfile<<it.num<<"\t"<<it.arrivalTime<<"\t"<<it.burstTime<<"\t"<<it.completionTime<<"\t"<<it.turnAroundTime<<"\t"<<it.waitingTime<<"\t"<<it.responseTime<<endl;
        }

    }
    else if(type == 3){
         // Shortest Remaining Job First Serve
        vector<int>vis(n,0);
        vector<ll> remTime(n);
        vector<ll> startTime(n);
        
        for(int i=0;i<n;i++){
            remTime[i]=p[i].burstTime;
        }
        int done = 0; ll time=p[0].arrivalTime;
        int prev=-1;
        while(done!=n){

            
            int i= shortest_rem_job(n,time,vis,remTime);
            // outfile<<"\tP"<<(i+1)<<"\t";
            string st="P" + ((char)(i+49));
            gantt_chart.push_back(st);
            
                if(remTime[i]==p[i].burstTime){
                    startTime[i]=time;
                }
                time++;
                remTime[i]--;
                if(remTime[i]==0){
                p[i].completionTime = time;
                p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
                p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
                p[i].responseTime = startTime[i]-p[i].arrivalTime;

                avgResponseTime +=p[i].responseTime;
                avgWaitingTime += p[i].waitingTime;
                avgTurnAroundTime += p[i].turnAroundTime; 

                vis[i]=1; 
                done++;
                }
                prev=i;
                
            
        }
        avgResponseTime/=n;
        avgWaitingTime/=n;
        avgTurnAroundTime/=n;
        for(int i=0;i<n;i++){
            auto it = p[i];
            outfile<<it.num<<"\t"<<it.arrivalTime<<"\t"<<it.burstTime<<"\t"<<it.completionTime<<"\t"<<it.turnAroundTime<<"\t"<<it.waitingTime<<"\t"<<it.responseTime<<endl;
        }

    }
    else if(type==4){
        // Round Robin (RR)
        vector<int>vis(n,0);
        vector<ll> remTime(n);
        vector<ll> startTime(n);
        vector<int> present(n);

        for(int i=0;i<n;i++){
            remTime[i]=p[i].burstTime;
        }
        queue<int>q;
        int done = 0; ll time=p[0].arrivalTime;
        int prev=-1;
        
        while(done!=n){

            if(q.empty()){
            round_robin_job(n,time,vis,present,q,-1);
            }
            int i=q.front();
            q.pop();
            // outfile<<" P"<<(i+1)<<" ";
            string st="P" + ((char)(i+49));
            gantt_chart.push_back(st);
            
                if(remTime[i]==p[i].burstTime){
                    startTime[i]=time;
                }
                int inc;
                if(remTime[i]<timeQuantum){
                    inc=remTime[i];
                }
                else{
                    inc=timeQuantum;
                }
                time +=inc;
                remTime[i]-=inc;

                round_robin_job(n,time,vis,present,q,i);
    
                if(remTime[i]==0){

                p[i].completionTime = time;
                p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
                p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
                p[i].responseTime = startTime[i]-p[i].arrivalTime;

                avgResponseTime +=p[i].responseTime;
                avgWaitingTime += p[i].waitingTime;
                avgTurnAroundTime += p[i].turnAroundTime; 

                vis[i]=1; 
                done++;
                }
                else{
                    q.push(i);
                }
                
            
        }
        avgResponseTime/=n;
        avgWaitingTime/=n;
        avgTurnAroundTime/=n;
        outfile<<"\n";
        for(int i=0;i<n;i++){
            auto it = p[i];
            outfile<<it.num<<"\t"<<it.arrivalTime<<"\t"<<it.burstTime<<"\t"<<it.completionTime<<"\t"<<it.turnAroundTime<<"\t"<<it.waitingTime<<"\t"<<it.responseTime<<endl;
        }
        
    } 
        outfile<<avgResponseTime<<"\n";
        outfile<<avgWaitingTime<<"\n";
        outfile<<avgTurnAroundTime<<"\n"; 

    outfile.close();
    infile.close();
    return 0;
}

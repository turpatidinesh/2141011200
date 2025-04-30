#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <csignal>
using namespace std;
namespace fs=std::filesystem;
struct ProcessInfo{
	int pid;
	string name;
	double cpuUsage;
	long memoryUsage;
};
string readFileValue(const string &path){
	ifstream file(path);
	string value;
	if (file.is_open()){
		getline(file,value);
	}
	return value;
}
double getSystemUptime(){
	ifstream file("/proc/uptime");
	double uptime;
	if (file.is_open()){
		file >> uptime;
	}
	return uptime;
}
ProcessInfo getProcessInfo(int pid,double systemUptime){
	ProcessInfo pinfo;
    pinfo.pid = pid;
	ifstream file("/proc/" + to_string(pid) + "/stat");
	string line;
	long utime=0, stime=0, starttime=0;
	if(file.is_open()){
		getline(file,line);
		istringstream ss(line);
		string token;
		int count=0;
		
		while(ss >> token){
			count++;
			if(count==2) pinfo.name=token;
			else if(count==14) utime=stol(token);
			else if(count==15) stime=stol(token);
			else if(count==22) starttime=stol(token);
		}
	}
	
	ifstream memFile("/proc/" + to_string(pid) + "/status");
	if (memFile.is_open()){
		string key, value,unit;
		while(memFile >> key >> value >> unit){
			if (key=="VmRSS"){
				pinfo.memoryUsage=stol(value);
				break;
			}
		}
	}
	long total_time = utime + stime;
	double seconds = systemUptime - (starttime / sysconf(_SC_CLK_TCK));
	pinfo.cpuUsage = (total_time / sysconf(_SC_CLK_TCK)) / seconds * 100;
	return pinfo;
	
	
}

vector <ProcessInfo> getAllProcesses(){
	vector <ProcessInfo> processes;
	double systemUptime= getSystemUptime();
	for (const auto &entry : fs::directory_iterator("/proc")){
		if (entry.is_directory()){
			string filename=entry.path().filename().string();
			if (all_of(filename.begin(),filename.end(), ::isdigit)){
				int pid=stoi(filename);
				processes.push_back(getProcessInfo(pid,systemUptime));
			}
		}
	}
	return processes;
}

void sortProcesses(vector<ProcessInfo> &processes, bool sortByCpu){
	if(sortByCpu){
		sort(processes.begin(),processes.end(),[](const ProcessInfo &a, const ProcessInfo &b)
		{
			return a.cpuUsage > b.cpuUsage;
		});
	}
	else{
		sort(processes.begin(),processes.end(),[](const ProcessInfo &a, const ProcessInfo &b)
		{
			return a.memoryUsage > b.memoryUsage;
		});
	}
}
	

int main(){
	vector <ProcessInfo> processes = getAllProcesses();
	sortProcesses(processes,true);
	cout<<"PID\tCPU%\tMemory (kB)\tName\n";
	for (size_t i=0;i<min(processes.size(),size_t(10));i++){
		cout<<processes[i].pid<<"\t"<<processes[i].cpuUsage<<"%\t"<<processes[i].memoryUsage<<"%\t"<<processes[i].name<<"%\n";
	}
	int targetPid;
	cout<<"Enter PID to kill: ";
	cin>>targetPid;
	if(targetPid > 0){
		if (kill(targetPid, SIGKILL) == 0){
			cout<<"Process "<< targetPid << " terminated successfully\n";
		}
		else{
			perror("Failed to kill the process");
		}
	}
	return 0;
}




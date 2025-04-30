#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;
bool isNumber(const string &s){
	return  !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

string getProcessName(int pid){
	ifstream file("/proc/" + to_string(pid) + "/stat");
	string line, processName;
	if(file.is_open()){
		getline(file,line);
		istringstream ss(line);
		string token;
		int count=0;
		while(ss >> token){
			count++;
			if(count==2){
				processName=token;
				break;
			}
		}
	}
	return processName;
}

int main(){
	cout<<"Active processes: "<<endl;
	for (const auto &entry : fs::directory_iterator("/proc")){
		if(entry.is_directory()){
			string filename=entry.path().filename().string();
			if (isNumber(filename)){
				int pid=stoi(filename);
				string processName = getProcessName(pid);
				cout<<"PID: "<<pid<<"|Name: " << processName<<endl;
			}
		}
	}	
	return 0;
}


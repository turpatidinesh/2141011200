#include <iostream>
using namespace std;
#include <sys/sysinfo.h>
void displayMemoryInfo(){
	struct sysinfo info;
	if (sysinfo(&info)==0){
		cout<< "Total used RAM (info.totalram in MB): "<<info.totalram/(1024*1024)<<"MB\n";
		cout<< "Total used RAM (info.totalram in GB): "<<(info.totalram/(1024*1024))/1024<<"GB\n";
		cout<< "Total unused RAM (info.freeram in MB): "<<info.freeram/(1024*1024)<<"MB\n";
		cout<< "Total shared RAM (info.sharedram in MB): "<<info.sharedram/(1024*1024)<<"MB\n";
	}
}
int main(){
	displayMemoryInfo();
	return 0;
}


#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<vector>
#include<sys/sem.h>
#include<signal.h> 

using namespace std;

int killAllProcesses(vector<pid_t>);
int createProcess(pid_t);
void handler(int);

vector<pid_t> pids;
pid_t pid;
int count = 0;
bool fl = false;

int main(){
	signal(SIGUSR1, handler);
	char command;		
	cout << "'+' - создать процесс;\n'-' - уничтожить процесс;\n'q' - выход." <<endl;
	while(true){
		cin>>command;	
		switch(command){
			case '+':
				createProcess(pid = fork()); 
				pids.push_back(pid); break;
			case '-':
				if(pids.size() == 0){
						std::cout << "0 child processes" << endl;
						break;
				}			
				kill(pids.back(), SIGKILL);  
				pids.pop_back();     break;
			case 'q':
			killAllProcesses(pids); 
			exit(0); 				 break;
			default: std::cout << "Error command" << endl;
		}
	}
	
	return 0;
}
void handler(int sig){				// обработчик сигнала для родителя
	signal(SIGUSR1, handler);			
	count++;
	if(count==pids.size())
		count = 0;
		kill(pids[count], SIGUSR2);
	return;
}


int killAllProcesses(vector<pid_t> pids){
	
	for(auto &a: pids){
		kill(a, SIGKILL);
	}
	
	return 0;
}

void handler2(int sig){				//обработчик сигнала для дитя
	signal(SIGUSR2, handler2);	
	fl = true;
	return;
}

int createProcess(pid_t pid){			
	switch(pid){
		default:
			usleep(100000);
			if(pids.size() == 0){
				kill(pid, SIGUSR2);
			}
			break;
		case -1: perror("fork"); exit(0); break;
		case  0:
			signal(SIGUSR2, handler2);
			string str = "PID: ";
			str+=to_string(getpid());
			std::cout << "Создан новый процесс" << endl;
			while(true){
				if(fl == true){
					for(auto &a: str){
						std::cout << a << flush;
						usleep(100000);
					}
					std::cout<<endl;
					fl = false;
					kill(getppid(), SIGUSR1);
				}
				usleep(100000);
			}
			break;
	}
return 0;
}
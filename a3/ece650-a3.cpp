#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include<iostream>
using namespace std;

int pipeRgentoA1_status,pipeA1toA2_status;
int pipeRgentoA1[2];
int pipeA1toA2[2];

pid_t child_rgen,child_rgen_status;
int pid_rgen_status,pid_a1_status;

pid_t child_a1,child_a1_status;

pid_t child_a2,child_a2_status;

std::vector<pid_t> kids;




void spawn_a2()
{
	//cout<<"child : Executing a2!\n"; //Debug_Point

	//input from pipeA1toA2
	dup2(pipeA1toA2[0],STDIN_FILENO);

	//output to stdout

	//close fds
	close(pipeA1toA2[0]);
	close(pipeA1toA2[1]);

	//exec
	execvp("./ece650-a2",nullptr);

	cerr<<"Error: Exec of a2!\n";
	exit(1);
}

void spawn_a1()
{
	
	dup2(pipeRgentoA1[0],STDIN_FILENO);

	
	dup2(pipeA1toA2[1],STDOUT_FILENO);

	
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
	close(pipeA1toA2[0]);
	close(pipeA1toA2[1]);

	
	char* pyArg[3];
	pyArg[0]=(char*)"/usr/bin/python3";
	pyArg[1]=(char*)"ece650-a1.py";
	pyArg[2]= nullptr;
	execv("/usr/bin/python3",pyArg);
	cerr<<"Error:Exec of a1!\n";
	exit(1);
}


void spawn_rgen(char** argv1)
{
	//cout<<"child: Executing rgen!\n"; //Debug_Point

	// output to pipeRgentoA1
	dup2(pipeRgentoA1[1],STDOUT_FILENO);
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
	execvp("./rgen",argv1);
	cerr<<"Error:Exec of rgen!\n";
	exit(1);
}

int main(int argc, char** argv){


 std::string svalue;
    int sint_value = 10; // num of streets randint [2,k] k >=2
    std::string nvalue; 
    int nint_value = 5; //num of line segs per street rand int [1,k] k >= 1
    std::string lvalue;
    int lint_value = 5; //wait rand time rand int [5,k] k>=5
    std::string cvalue;
    int cint_value = 20; //xy coord range rand int [-k,k] k>=1
    int c;

    opterr = 0;

    // expected options are '-s value', '-n value', '-l value', and '-c value'
    while ((c = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (c)
        {
        case 's':
            svalue = optarg;
            sint_value = atoi(svalue.c_str());
            if(sint_value < 2) {
                std::cerr << "Error: Option -s must have value >= 2" << std::endl;
                return 1;
            }
            break;

        case 'n':
            nvalue = optarg;
            nint_value = atoi(nvalue.c_str());
            if(nint_value < 1) {
                std::cerr << "Error: Option -n must have value >= 1" << std::endl;
                return 1;
            }
            break;

        case 'l':
            lvalue = optarg;
            lint_value = atoi(lvalue.c_str());
            if(lint_value < 5) {
                std::cerr << "Error: Option -l must have value >= 5" << std::endl;
                return 1;
            }
            break;

        case 'c':
            cvalue = optarg;
            cint_value = atoi(cvalue.c_str());
            if(cint_value < 1) {
                std::cerr << "Error: Option -c must have value >= 1" << std::endl;
                return 1;
            }
            break;
        
        case '?':
            std::cerr << "Error: unknown option: " << optopt << std::endl;
            return 1;
        default:
            return 0;
        }



// Create pipeRgentoA1
	pipeRgentoA1_status=pipe(pipeRgentoA1);

	if (pipeRgentoA1_status == -1)
	{
	  cerr<<"Error:creating the pipeRgentoA1"<<endl;
	  exit(1);
	}

//=================PIPE Rgen to A1 CREATED ========================================

// Forking (child_rgen)
	//cout<<"Parent: giving birth to child_rgen!\n"; //Debug_Point	
  	if ((child_rgen= fork()) == -1)
  	{
    cerr<<"Error: bad child_rgen forking!"<<endl;
    exit(1); 		
  	}
  	else if(child_rgen ==0)
  	{
  		//Inside Child Rgen
  		// rgen --> pipeRgentoA1
  		spawn_rgen(argv);
    }

// =================== pipeRgentoA1 CONTAINS rgen OUTPUT ===========================

// parent
	kids.push_back(child_rgen);
  	//cout<<"Parent: Back to driver with successful spawn_rgen!\n"; //DEBUG_POINT

// create pipeA1toA2
  	pipeA1toA2_status=pipe(pipeA1toA2);

  	if(pipeA1toA2_status == -1)
  	{
	  cerr<<"Error:creating the pipeA1toA2"<<endl;
	  exit(1);  		
  	}

//=================PIPE A1 to A2 CREATED ========================================


// forking (child_a1)
  	if((child_a1=fork())== -1)
  	{
	  cerr<<"Error: bad child_a1 forking!"<<endl;
	  exit(1);		
  	}
  	else if(child_a1 ==0)
  	{
  		// pipeRgentoA1 --> a1 --> pipeA1toA2
  		spawn_a1();
  	}

// =================== pipeA1toA2 CONTAINS A1 OUTPUT ===========================

// parent
	kids.push_back(child_a1);

// close unused fds
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
  	//cout<<"Back to driver with successful spawn_a1!\n";

// forking (child_a2)
  	if((child_a2=fork())== -1)
  	{
	  cerr<<"Error: bad child_a2 forking!"<<endl;
	  exit(1);		
  	}
  	else if(child_a2 ==0)
  	{
  		// pipeA1toA2 --> a2 --> stdout
  		spawn_a2();
  	}

//

//  console to pipeA1toA2 to A2
  	dup2(pipeA1toA2[1],STDOUT_FILENO);
  	close(pipeA1toA2[0]);
  	close(pipeA1toA2[1]);

    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;
    }

    kids.push_back(child_a2);
  	//parent 

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}


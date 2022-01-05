#include <unistd.h>
  #include <iostream>
  #include <fstream>
  #include <vector>
  #include <string>
  #include <algorithm>
  #include <random>
  using namespace std;
  
  
  ifstream urandom("/dev/urandom");
  
  std::vector< std::vector< std::pair<int,int> > > street_points;
  void check_urandom();
  void gatherInputFromUser(int argc, char** argv);
  int randomGen(int min , int max);
  void generateGraph();
  void generateStreetName();
  std::vector<std::pair<int,int>>  genrateStreetCoordinates();
  bool checkIfInputValid(std::vector<std::pair<int,int>> street);
  bool intersects(std::pair<int, int> point1, std::pair<int, int> point2, std::pair<int, int> point3, std::pair<int, int> point4);
  void print_add_graph();  
  int randomwaittime = 5;
  int coordinate_range = 20;

  const std::string CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std:: string values;
  int nstreets = 10;
  int linesegments = 5;
  int trial_count = 0;
  vector<std::string> streetNames;


  int main(int argc, char** argv){
  
      check_urandom();
      gatherInputFromUser(argc, argv);
  
      while(true){
  
  			


              //check if the street names vector is not empty
              if(streetNames.size()!=0){
  
                   for(int i=0; i<streetNames.size()   ;++i)
                      {
                          std::cout<<"rm"<<" \""<<streetNames[i]<<"\""<<std::endl;
                      }
  
  
              }
  
             streetNames.clear();
			 street_points.clear();

              generateGraph();

			  print_add_graph();
		
			 sleep(randomGen(5,randomwaittime));
			  
  
  
      }
  
      return 0;
  
  }
  
  
  void gatherInputFromUser(int argc, char** argv){
      int ar;
      while((ar = getopt(argc, argv, "s:n:l:c:")) != -1){
  
          switch(ar){
  
  
                  case 's':
                          values = optarg;
                          nstreets = atoi(values.c_str());
                          if(nstreets<2){
                          cerr<<"Error: number of streets should be greater than 2"<<endl;
                          }
                    break;
  
                  case 'n':
                          values = optarg;
                          linesegments = atoi(values.c_str());
                          if(linesegments<1){
                          cerr<<"Error: number of line segments should be greater than one"<<endl;
                          }
                      break;
  
                  case 'l':
                          values = optarg;
                          randomwaittime = atoi(values.c_str());
                          if(randomwaittime < 5){
                              cerr<<"Error: wait time cannot be less than 5!!"<<endl;
                          }
                      break;
  
                  case 'c':
                          values = optarg;
                          coordinate_range = atoi(values.c_str());
                          if(coordinate_range < 1){
                              cerr<<"Error: number of co-ordinates cannot be less than one!!";
  
                          }
  
  
          }
  
      }
  
  }

  int randomGen(int min, int max)
{
    std::ifstream urandom("/dev/urandom");

    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    int N = (max - min)+1 ;
    int num=0;     
    while(1)
    {
        
        urandom.read((char*)&num,1);
        //std::cout<<num<<endl;
        num= num % N + min;
        //std::cout<<"RandGen: "<<num<<endl;
        if(num <= max && num >= min)
        {
            urandom.close();
            break;
        }
    }

    return num;
}




void generateStreetName(){

              std::string street_name;
  
              std::random_device random_device;
              std::mt19937 generator(random_device());
              std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
  
              
              std:: random_device rd1;
              std::uniform_int_distribution<int> len(2,10);
              int l = len(rd1);
  
              for(int i = 0; i<l; ++i){
  
                   street_name = street_name + CHARACTERS[distribution(generator)];
              }
  
              if(std::find(streetNames.begin(), streetNames.end(), street_name) == streetNames.end()){
  
             
			  streetNames.push_back(street_name);
              }
              else{
                generateStreetName();
              }
		  
					  

}




std::vector<std::pair<int,int>> genrateStreetCoordinates(){

          int limit = 0;
          std::vector<std::pair<int,int>> street;
        
          int nlgs = randomGen(1,linesegments);
          int numberOfCoord = nlgs+1;
          std::string validInput = "false";

          while(validInput == "false"){
            
			street.clear();
            for(int j = 0; j< numberOfCoord; ++j){
			  
				
              int x_cord = randomGen(-coordinate_range, coordinate_range);
              int y_cord = randomGen(-coordinate_range, coordinate_range);
              std::pair<int,int> point;
              point.first = x_cord;
              point.second = y_cord;
              street.push_back(point);

          }

          limit++;
          if(limit>25){

	  			cerr<<"Error: Maximum attempts to generate valid input has exceeded, please retry (25)"<<endl;
				exit(0); 
              break;
          }

          bool valid = checkIfInputValid(street); 
          if(valid){
            validInput="true";
          }
        }


  return street;
}



bool intersects(std::pair<int, int> point1, std::pair<int, int> point2, std::pair<int, int> point3, std::pair<int, int> point4) {
    int x1 = point1.first;
    int y1 = point1.second;
    int x2 = point2.first;
    int y2 = point2.second;
    int x3 = point3.first;
    int y3 = point3.second;
    int x4 = point4.first;
    int y4 = point4.second;
    float m1;
    float b1;
    float m2;
    float b2;

    int seg1_xmin = std::min(x1, x2);
    int seg1_xmax = std::max(x1, x2);
    int seg2_xmin = std::min(x3, x4);
    int seg2_xmax = std::max(x3, x4);
    int seg1_ymin = std::min(y1, y2);
    int seg1_ymax = std::max(y1, y2);
    int seg2_ymin = std::min(y3, y4);
    int seg2_ymax = std::max(y3, y4);
    int x_interval_max = std::max(seg1_xmin, seg2_xmin);
    int x_interval_min = std::min(seg1_xmax, seg2_xmax);
    int y_interval_max = std::max(seg1_ymin, seg2_ymin);
    int y_interval_min = std::min(seg1_ymax, seg2_ymax);

    // check for vertical overlapping lines
    if ( x1 == x2 && x2 == x3 && x3 == x4 ) {
        if ( (y_interval_min <= y1 && y1 <= y_interval_max) ||
             (y_interval_min <= y2 && y2 <= y_interval_max) ||
             (y_interval_min <= y3 && y3 <= y_interval_max) ||
             (y_interval_min <= y4 && y4 <= y_interval_max) )  {
            return true;
        }
    } else if ( x1 != x2 && x3 != x4) {
        m1 = (y2-y1)/(x2-x1);
        b1 = y1-m1*x1;
        m2 = (y4-y3)/(x4-x3);
        b2 = y3-m2*x3;
        // check if line equations are equal
        if (m1 == m2 && b1 == b2) {
            if (
             ( (x_interval_min <= x1 && x1 <= x_interval_max) ||
             (x_interval_min <= x2 && x2 <= x_interval_max) ||
             (x_interval_min <= x3 && x3 <= x_interval_max) ||
             (x_interval_min <= x4 && x4 <= x_interval_max) )
             &&
             ( (y_interval_min <= y1 && y1 <= y_interval_max) ||
             (y_interval_min <= y2 && y2 <= y_interval_max) ||
             (y_interval_min <= y3 && y3 <= y_interval_max) ||
             (y_interval_min <= y4 && y4 <= y_interval_max) )
                )
            {
            return true;
            }
        }
    }

    float xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    float xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    float ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    float yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if (xden == 0 || yden == 0) {
        return false;
    }

    float xcoor = xnum / xden;
    float ycoor = ynum / yden;
    if ( x_interval_min <= xcoor && xcoor <= x_interval_max &&
        y_interval_min <= ycoor && ycoor <= y_interval_max ) {
            return true;
        }
    return false;
}


  void check_urandom(){
  
      if(urandom.fail()){
  
      cerr<<"Error: failed to open urandom!";
      exit(1);
      }
  
  
  }
  


void print_add_graph() {
    std::string tmp;
    for (int i=0; i < streetNames.size(); i++) {
        
        tmp = tmp + "add \"" + streetNames[i] + "\" ";
        for (auto& p : street_points[i]) {
            tmp = tmp + "(" + std::to_string(p.first) + "," + std::to_string(p.second) + ") ";
        }
        tmp += "\n";
		
    }
   tmp += "gg";
    std::cout << tmp << std::endl;
    // std::cerr << tmp << std::endl;
}

bool checkIfInputValid(std::vector<std::pair<int,int>> street){

		for (int i=0; i < street.size(); i++) {
        for (int j=i+1; j < street.size(); j++) {
            // Check for duplicate points
            if( (street[i].first == street[j].first) && 
                (street[i].second == street[j].second) )
            {
                return false;
            }
            // check if any two segments overlap or intersect
            if (i+1 < street.size() && j+1 < street.size()) {
                if(intersects(street[i], street[i+1], street[j], street[j+1])) {
                    return false;
                }   
            }
        }
    }

    return true;

} 


void generateGraph(){

	int numberOfStreets = randomGen(2,nstreets);
    for(int m = 0; m<numberOfStreets; ++m){
        
         std::vector<std::pair<int,int>> street; 
         generateStreetName();
         street = genrateStreetCoordinates();
         street_points.push_back(street);

  }
    


}

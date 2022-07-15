#include <iostream>
#include <string>
#include <fstream>
#include <cmath>  
#include <vector> 
#include <sys/time.h>
using namespace std;

#define fieldNO 6 // no. of columns in the data file
#define gridx_base 0.015 // for 500 metre
#define gridy_base 0.010 // for 500 metre
#define range_sy 35.0 // coordinate of New Zealand
#define range_ey 38.0
#define range_sx 173.5
#define range_ex 176.5


class property {        // The class property
    int ID;             // Attribute (int property ID)
    string name;        // Attribute (string property name)
    float latitude;     // Attribute (float latitude)
    float longitude;    // Attribute (float longitude)
    int type;           // Attribute (Normal residential: 0; public housing: 1; AirBnb: 2)
    int x;              // Attribute (x-coordinate of region)
    int y;              // Attribute (x-coordinate of region)
    int bltyear;        // Attribute (buit year)

    public:
        void setvalue(int pID, string pName, float platitude, float plongitude, int ptype, int pyear) { // set the values
            ID = pID;
            name = pName;
            latitude = platitude;
            longitude = plongitude;
            type = ptype;
            bltyear = pyear;
        }
        void setregion(int region_x, int region_y) {
            x = region_x;
            y = region_y;
        }
        string outName() {
            return name;
        }
        float outLat() {
            return latitude;
        }
        float outLong() {
            return longitude ;
        }
        int outx() {
            return x;
        }        
        int outy() {
            return y;
        }
        int outID() {
            return ID;
        }
        int outpub() {
            return type;
        }
        int outYear() {
            return bltyear;
        }
};

class cluster {
    vector<property*> listofProperty;
    int id_x;
    int id_y;
    public:
        void setvalue(int pid_x, int pid_y) { // set the values
            id_x = pid_x;
            id_y = pid_y;
        }
        void add(property* pproperty) { // add a property to the vector
            listofProperty.push_back(pproperty);
        }
        void create() {
            listofProperty = {};
        }
        vector<property*> list() {
            return listofProperty;
        }
        property* view(int index) {
            return listofProperty.at(index);
        }
        int cap() {
            return listofProperty.size();
        }
};

float calDistance(float lat1, float long1, float lat2, float long2, float coeffecient) {
       
    float dlat1=lat1*coeffecient;
    float dlong1=long1*coeffecient;
    float dlat2=lat2*coeffecient;
    float dlong2=long2*coeffecient;

    float dLong=dlong1-dlong2;
    float dLat=dlat1-dlat2;

    float aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
    float cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
        //earth's radius from wikipedia varies between 6,356.750 km — 6,378.135 km (˜3,949.901 — 3,963.189 miles)
        //The IUGG value for the equatorial radius of the Earth is 6378.137 km (3963.19 mile)
    const float earth=6378.13;
    float distance=earth*cHarv;
    return distance;
}

void split (string str, char seperator, string* indata) {  
    int currIndex = 0;
    int i = 0;  
    int startIndex = 0; 
    int endIndex = 0;  
    while (i <= str.size() && currIndex < fieldNO) {
        if (str[i] == seperator || i == str.size()) {  
            endIndex = i;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            indata[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        i++;  
    }     
}  

int main(int argc, char *argv[]) {

    string rawfile1 = "./rawfiles/residential.csv";
    ifstream residentialfile(rawfile1);
    string rawfile2 = "./rawfiles/AirBnB.csv";
    ifstream airbnbfile(rawfile2);
    string commandline = "Command line: Test <option: 1 for 500m; 2 for 1000m; 3 for 1500m> <Year: 2003-2022> <1: state house; 2: AirBnB>\0";
    string line;
    string datain[fieldNO];
    int i, j, k;
    property *resident, *airbnb;
    cluster **region;
    int residentNo, airbnbNo;
    int option, year, propertytype;
    int boundx, boundy;
    float coeffecient;
    struct timespec start, end;
    float grid_x, grid_y, grid_factor;

/* getting input arguments */
    if (argc < 3) {
        cout << commandline << endl;
        return 1;
    }
    else
    {
        option = stoi(argv[1]);
        if ((option < 1) || (option > 3)) {
            cout << commandline << endl;
            return 1;            
        }
        else {
            option = option * 500;
        }
        year = stoi(argv[2]);
        if ((year < 2003) || (year > 2022)) {
            cout << commandline << endl;
            return 1;         
        }
        propertytype = stoi(argv[3]);
        if ((propertytype < 1) || (propertytype > 3)) {
            cout << commandline << endl;
            return 1;         
        }
        if (argc > 4) {
            grid_factor = stoi(argv[4]);
            if ((grid_factor < 1) || (grid_factor > 20)) {
                cout << commandline << endl;
                return 1;  
            }
        }
        else {
            grid_factor = 1;
        }
    }
    cout << "Option:" << option << " Year:" << year << endl;
    grid_x = gridx_base * grid_factor; 
    grid_y = gridy_base * grid_factor;

    coeffecient = 4.0*atan(1.0)/180;

    cout << "Creating object for properties" << endl;
    boundx = ceil((range_ex - range_sx)/grid_x);
    boundy = ceil((range_ey - range_sy)/grid_y);

    resident = (property*)malloc(sizeof(property)*1000000);
    airbnb = (property*)malloc(sizeof(property)*100000);
 
    cout << "Creating cluster (region size:" << option << "m)" << endl;
    region = (cluster**)malloc(sizeof(cluster*)*boundx);
    for (i = 0; i < boundx; i++) {
        region[i] = (cluster*)malloc(sizeof(cluster)*boundy);
        for (j = 0; j < boundy; j++) {
            region[i][j].create();
            region[i][j].setvalue(i, j);
        }
    }


     clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    cout << "Reading file: " << rawfile1 << endl;
// Use a while loop together with the getline() function to read the file line by line
    getline (residentialfile, line);

    i = 0;
    while (getline (residentialfile, line)) {
        int region_x, region_y;
        
        split (line, ',', datain);
        resident[i].setvalue(i, datain[0], abs(stof(datain[1])), abs(stof(datain[2])), stoi(datain[3]), stoi(datain[4]));
        if ((resident[i].outLong() > range_ex) || (resident[i].outLat() > range_ey)) {
            cout << resident[i].outLong() << endl;
            cout << resident[i].outLat() << endl;         
        }
        region_x = floor((resident[i].outLong()-range_sx)/grid_x);
        region_y = floor((resident[i].outLat()-range_sy)/grid_y);
        resident[i].setregion(region_x, region_y);
        region[region_x][region_y].add(&(resident[i]));
        i++;
    }   
    residentNo = i;
    cout << "Total number of residential properties read:" << residentNo << endl;

    cout << "Reading file: " << rawfile2 << endl;
// Use a while loop together with the getline() function to read the file line by line
    getline (airbnbfile, line);
    
    i = 0;
    while (getline (airbnbfile, line)) {
        int region_x, region_y, airbnbID;
        
        split (line, ',', datain);
        if (airbnbID != stoi(datain[2])) {
            airbnbID = stoi(datain[2]);
        
            airbnb[i].setvalue(i, datain[2], abs(stof(datain[3])), abs(stof(datain[4])), 2, 2020);
            if ((airbnb[i].outLong() > range_ex) || (airbnb[i].outLat() > range_ey)) {
                cout << airbnb[i].outLong() << endl;
                cout << airbnb[i].outLat() << endl;         
            }
            region_x = floor((airbnb[i].outLong()-range_sx)/grid_x);
            region_y = floor((airbnb[i].outLat()-range_sy)/grid_y);
            airbnb[i].setregion(region_x, region_y);
            region[region_x][region_y].add(&(airbnb[i]));
            i++;
        }
    }   
    airbnbNo = i;
    cout << "Total number of Airbnb read:" << airbnbNo << endl;
/*
    for (i = 0; i < boundx; i++) {
        for (j = 0; j < boundy; j++) {
            if (region[i][j].cap() > 0) {
                for (k = 0; k < region[i][j].cap(); k++) {
                    property *p1;

                    p1 = region[i][j].view(k);
                }
            }
        }
    }
*/
    cout << "Counting" << endl;
    for (i = 0; i < residentNo; i++) {
        int x, y;
        int pubCnt = 0;
        int neigCnt = 0;
        int xs, xe, ys, ye;
        int nb_cluster;
        
        nb_cluster = option / 500;
        if (resident[i].outYear() <= year) {

            x = resident[i].outx();
            y = resident[i].outy();
            xs = x - nb_cluster;
            if (xs < 0) {
                xs = 0;
            }
            ys = y - nb_cluster;
            if (ys < 0) {
                ys = 0;
            }
            xe = x + nb_cluster;
            if (xe >= boundx) {
                xe = boundx - 1;
            }
            ye = y + nb_cluster;
            if (ye >= boundy) {
                ye =  boundy - 1;
            }
            for (j = xs; j <= xe; j++) {
                for (k = ys; k <= ye; k++) {
                    for (property* p2 : region[j][k].list()) {
                        float dist, tdist;

                        if ((resident[i].outID() != (*p2).outID()) && ((*p2).outYear() <= year)) {
                            tdist = option/1000.0;
                            dist = calDistance(resident[i].outLat(), resident[i].outLong(), (*p2).outLat(), (*p2).outLong(), coeffecient);
                            if (dist < tdist) {
//                            cout << resident[i].outLat() << " " << resident[i].outLong() << " " << (*p2).outLat() << " " << (*p2).outLong() << " " << dist << endl;
                                if ((*p2).outpub() == propertytype) {
                                    pubCnt = pubCnt + 1;
                                }
                                neigCnt = neigCnt + 1;
                            }
                        }
                    }
                }
            }
            cout << resident[i].outID()  << ':';
            cout << resident[i].outName() << ':';
            cout << resident[i].outYear()  << ':';
            cout << neigCnt << ':';
            cout << pubCnt << endl;
        }
    }

    free(resident);
    free(airbnb);
    for (i = 0; i < boundx; i++) {
        free(region[i]);
    }
    free(region);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    cout << "Time taken by function: " << ceil(time_taken) << " seconds" << endl;

    return 0;
}
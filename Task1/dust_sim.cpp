#include "Lib.cpp"
#include "constant.cpp"

using namespace std;

// Function to parse command-line arguments
void parseArguments(int argc, char* argv[], int &num_sensors, int &sampling_time, int &sim_interval) {
    num_sensors = DEFAULT_NUM_SENSORS;
    sampling_time = DEFAULT_SAMPLING_TIME;
    sim_interval = DEFAULT_SIM_INTERVAL;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            num_sensors = stoi(argv[++i]); //assign number of sensor through position -n +1  
        } else if (arg == "-st" && i + 1 < argc) {
            sampling_time = stoi(argv[++i]);//assign number of sensor through position -st +1  
        } else if (arg == "-si" && i + 1 < argc) {
            sim_interval = stoi(argv[++i]);//assign number of sensor through position -si + 1
        }
    }

    // Validate the arguments
    if (num_sensors < 1 || num_sensors == NULL) {
        cout << "Invalid number of sensors. Using default value of " << DEFAULT_NUM_SENSORS << ".\n";
        num_sensors = DEFAULT_NUM_SENSORS;
    }

    if (sampling_time < MIN_SAMPLING_TIME || sampling_time == NULL) {
        cout << "Sampling time too short. Using minimum value of " << MIN_SAMPLING_TIME << " seconds.\n";
        sampling_time = MIN_SAMPLING_TIME;
    }

    if (sim_interval < MIN_SIM_INTERVAL || sim_interval == NULL) {
        cout << "Simulation interval too short. Using minimum value of " << MIN_SIM_INTERVAL << " hours.\n";
        sim_interval = MIN_SIM_INTERVAL;
    }
}

// Function to generate a random PM2.5 value INPUT in range
float generatePM25Value() {
    int num_steps = static_cast<int>((MAX_PM25 - MIN_PM25) / RESOLUTION); // how many possible values fit //static_cast like constant
    int random_step = rand() % (num_steps + 1); 
    return MIN_PM25 + random_step * RESOLUTION;
}

// Function to format and print the timestamp
string formatTimestamp(time_t time) { //time_t type used to represent calendar time
    tm* time_info = localtime(&time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y:%m:%d %H:%M:%S", time_info); //function formats date and time information according to a specified format.
    return buffer;
}


void simulateSensorReadingsandWriting(int num_sensors, int sampling_time, int sim_interval) {
    ofstream file("CSV_file/dust_sensor.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file." <<endl;
        return;
    }

    file << "ID,Time,Value\n";

    time_t current_time = time(nullptr);// just return current time not assign to another var
    time_t start_time = current_time - (sim_interval * 3600); // Going back by sim_interval in hours

    for (time_t time = start_time; time <= current_time; time += sampling_time) {
        for (int sensor_id = 1; sensor_id <= num_sensors; ++sensor_id) {
            float pm25_value = generatePM25Value();
            string timestamp = formatTimestamp(time);

            
            file << sensor_id << "," << timestamp << "," << fixed << setprecision(1)
                 << pm25_value << "\n";
        }
    }

    file.close();
    cout << "Data successfully written to dust_sensor.csv" << endl;
}


int main(int argc, char* argv[]) { //number of command line//argument
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation assure for different value signal

    int num_sensors, sampling_time, sim_interval;
    parseArguments(argc, argv, num_sensors, sampling_time, sim_interval);

    simulateSensorReadingsandWriting(num_sensors, sampling_time, sim_interval);

    return 0;
}

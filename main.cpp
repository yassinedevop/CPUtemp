#include <fstream>
#include <iostream>
#include <numeric>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <string>

std::vector<size_t> get_cpu_times() {
    std::ifstream proc_stat("/proc/stat");
    proc_stat.ignore(5, ' '); // Skip the 'cpu' prefix.
    std::vector<size_t> times;
    for (size_t time; proc_stat >> time; times.push_back(time));
    return times;
}
float temperature(float usage){
	float x = 76/100;
	float temp =  0.0;
	temp = 39 + usage * x + 76*2*(exp(usage/(63*76/100)) - 1)/100 ;

	return temp;
}
void sendtemp(float temp){
	std::ofstream monflux("/home/yassine/workspace/server/temp.txt");
	monflux << temp;
	monflux.close();
}
bool get_cpu_times(size_t &idle_time, size_t &total_time) {
    const std::vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4)
        return false;
    idle_time = cpu_times[3];
    total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0);
    return true;
}

int main(int, char *[]) {
	float tempera ;
    size_t previous_idle_time=0, previous_total_time=0;
    for (size_t idle_time, total_time; get_cpu_times(idle_time, total_time); sleep(1)) {
        const float idle_time_delta = idle_time - previous_idle_time;
        const float total_time_delta = total_time - previous_total_time;
        const float utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
//        std::cout << utilization << '%' << std::endl;
        tempera = temperature(utilization);
//        std::cout<< tempera<< "°C"<<std::endl;
        sendtemp(tempera);
        previous_idle_time = idle_time;
        previous_total_time = total_time;
    }
}


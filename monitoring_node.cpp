
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

float getCPUUsage() {
    std::string line;
    float cpu_usage = 0.0;

    std::ifstream file("/proc/stat");
    if (file.is_open()) {
        getline(file, line);
        file.close();
        
        int user, nice, system, idle, iowait, irq, softirq;
        sscanf(line.c_str(), "cpu  %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);
        
        int total = user + nice + system + idle + iowait + irq + softirq;
        int total_idle = idle + iowait;
        cpu_usage = (float)(total - total_idle) / total * 100.0;
    }
    return cpu_usage;
}

float getMemoryUsage() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long totalPhysMem = memInfo.totalram;
    long physMemUsed = memInfo.totalram - memInfo.freeram;

    return (float)physMemUsed / totalPhysMem * 100;
}

void sendDataToServer(float cpu, float memory) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == 0) {
        std::string message = "CPU: " + std::to_string(cpu) + "%, Memory: " + std::to_string(memory) + "%";
        send(sock, message.c_str(), message.length(), 0);
        std::cout << "Sent data: " << message << std::endl;
    } else {
        std::cerr << "Failed to connect to the server." << std::endl;
    }
    close(sock);
}

int main() {
    while (true) {
        float cpu = getCPUUsage();
        float memory = getMemoryUsage();
        sendDataToServer(cpu, memory);
        std::this_thread::sleep_for(std::chrono::seconds(10));  // Send data every 10 seconds
    }
    return 0;
}

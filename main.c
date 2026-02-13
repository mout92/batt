#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Parse command line options
    int print_charge_cycles = 0;
    int opt;

    while((opt = getopt(argc, argv, ":c")) != -1) {
        switch(opt){
            case 'c':
                print_charge_cycles = 1;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    
    // Define paths from sysfs
    char file_cap[] = "/sys/class/power_supply/BAT0/capacity";
    char file_status[] = "/sys/class/power_supply/BAT0/status";

	// Reading current battery capacity
	FILE *fptr_cap;
	fptr_cap = fopen(file_cap, "r");
    if (!fptr_cap) {
        fprintf(stderr, "Battery not found\n");
        exit(1);
    }
	char buffer_cap[128];
	fgets(buffer_cap, 128, fptr_cap);
	fclose(fptr_cap);
	int capacity = atoi(buffer_cap);

	// Reading current battery status
	FILE *fptr_status;
	fptr_status = fopen(file_status, "r");
    if (!fptr_status) {
        fprintf(stderr, "Status not found\n");
        exit(1);
    }
	char status[128];	
	fgets(status, 128, fptr_status);
	int len = strlen(status);
	status[len-1] = '\0';
	fclose(fptr_status);

	// print status
	char status_formatted[128];
	if(!strcmp(status, "Charging")) {
		sprintf(status_formatted, "\e[1;34m%s\e[0m", status);
	}

	if(!strcmp(status, "Discharging")) {
		sprintf(status_formatted, "\e[1;31m%s\e[0m", status);
	}

	if(!strcmp(status, "Full")) {
		sprintf(status_formatted, "\e[1;32m%s\e[0m", status);
	}
	
	// Print percentage in green
	if(capacity >= 50) {
		printf("\e[1;32m%i%%\e[0m - ", capacity);
		printf("%s", status_formatted);
	 }

	// Print percentage in yellow
	if(capacity > 20 && capacity < 50) {
		printf("\e[1;93m%i%%\e[0m - ", capacity);
		printf("%s", status_formatted);
	}

	// Print percentage in red
	if(capacity <= 20) {
		printf("\e[1;31m%i%%\e[0m - ", capacity);
		printf("%s", status_formatted);
	}

    if(print_charge_cycles) {
        FILE* fp = fopen("/sys/class/power_supply/BAT0/cycle_count", "r");
        if(!fp) {
            fprintf(stderr, "Error: cannot open file");
            exit(EXIT_FAILURE);
        }

        char cycle_count[32];
        fgets(cycle_count, 32, fp);
        cycle_count[strlen(cycle_count) - 1] = '\0';
        printf(" - \033[1;33m%s cycles\033[0m", cycle_count);
    }

    printf("\n");

	return 0;
}

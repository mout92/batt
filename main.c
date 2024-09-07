#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char file_cap[] = "/sys/class/power_supply/BAT0/capacity";
char file_status[] = "/sys/class/power_supply/BAT0/status";

int main()
{
	// Reading capacity
	FILE *fptr_cap;
	fptr_cap = fopen(file_cap, "r");
	char buffer_cap[128];
	fgets(buffer_cap, 128, fptr_cap);
	fclose(fptr_cap);
	int capacity = atoi(buffer_cap);

	// Reading status
	FILE *fptr_status;
	fptr_status = fopen(file_status, "r");
	char status[128];	
	fgets(status, 128, fptr_status);
	int len = strlen(status);
	status[len-1] = '\0';
	fclose(fptr_status);

	// prepare colors
	char status_formatted[128];
	if(!strcmp(status, "Charging"))
	{
		sprintf(status_formatted, "\e[1;34m%s\e[0m", status);
	}

	if(!strcmp(status, "Discharging"))
	{
		sprintf(status_formatted, "\e[1;31m%s\e[0m", status);
	}

	if(!strcmp(status, "Full"))
	{
		sprintf(status_formatted, "\e[1;32m%s\e[0m", status);
	}
	
	// Print green
	if(capacity >= 50)
	{
		printf("\e[1;32m%i%%\e[0m -> ", capacity);
		printf("%s\n", status_formatted);
	 }

	// Print yellow
	if(capacity > 20 && capacity < 50)
	{
		printf("\e[1;93m%i%%\e[0m -> ", capacity);
		printf("%s\n", status_formatted);
	}

	// Print red
	if(capacity <= 20)
	{
		printf("\e[1;31m%i%%\e[0m -> ", capacity);
		printf("%s\n", status_formatted);
	}

	return 0;
}

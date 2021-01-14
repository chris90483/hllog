#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

const char* getDateString() {
    time_t timer;
    struct tm* tm_info;
    char day[2];
    char month[2];
    char year[4];
    time(&timer);
    tm_info = localtime(&timer);
    strftime(day, 3, "%d", tm_info);
    strftime(month, 3, "%m", tm_info);
    strftime(year, 5, "%Y", tm_info);

    char *date_str = (char*)malloc(10 * sizeof(char));
    sprintf(date_str, "%s-%s-%s", day, month, year);
    return date_str;
}

void showLog() {
    #ifdef _WIN32
    system("more Desktop\\hardlooplog.txt");
    #else
    system("cat hardlooplog.txt")
    #endif
}

FILE* getLogFile() {
    FILE *logFile;

    #ifdef _WIN32
    logFile = fopen("./Desktop/hardlooplog.txt", "a");
    #else
    logFile = fopen("hardlooplog.txt", "a");
    #endif

    return logFile;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        showLog();
        return 0;
    }
    bool distance_given = 0;
    bool time_given = 0;
    bool description_given = 0;

    float distance_meters = 0;
    float time_hours = 0;
    float time_minutes = 0;
    float time_seconds = 0;
    char description[400]; 
    
    // Afstand
    printf("Afstand (in meters) :> ");
    distance_given = scanf("%f", &distance_meters) > 0;
    if (!distance_given) fflush(stdin); // if scan unsuccessful the input remains in stdin
    
    // Uren
    printf("Uren :> ");
    time_given = scanf("%f", &time_hours) > 0;
    if (!time_given) fflush(stdin); // if scan unsuccessful the input remains in stdin
    
    // Minuten
    printf("Minuten :> ");
    time_given = scanf("%f", &time_minutes) > 0;
    if (!time_given) fflush(stdin); // if scan unsuccessful the input remains in stdin
    
    // Seconden
    printf("Seconden :> ");
    time_given = scanf("%f", &time_seconds) > 0;
    if (!time_given) fflush(stdin); // if scan unsuccessful the input remains in stdin
    
    // Beschrijving
    printf("Beschrijving :> ");
    description_given = scanf(" %[^\n]", &description) > 0;
    if (!description_given) fflush(stdin); // if scan unsuccessful the input remains in stdin
    
    FILE* logFile = getLogFile();
    if (logFile == NULL) {
        printf("kan logbestand niet uitlezen.");
        return -1;
    }

    // add date
    fprintf(logFile, "%-16s", getDateString());

    // add distance and time if given
    if (distance_given) {
        if (time_given) {
            // both distance and time are given, log avg speed
            float speed_kmh = distance_meters / ((time_hours * 60.0 * 60.0) + (time_minutes * 60.0) + time_seconds) * 3.6;
            fprintf(logFile, "%-012.2f%02.0f:%02.0f:%02.0f    Gemiddelde snelheid: %.2f km/h. ", (distance_meters / 1000.0), time_hours, time_minutes, time_seconds, speed_kmh);
            printf("gemiddelde snelheid: %f\n", speed_kmh);
        } else {
            // only distance given
            fprintf(logFile, "%-012.2fn.v.t       ", (distance_meters / 1000.0));
        }
    } else if (time_given) {
        // only time given
        fprintf(logFile, "n.v.t.      %02.0f:%02.0f:%02.0f    ", time_hours, time_minutes, time_seconds);
    }
    
    if (description_given) {
        fprintf(logFile, description);
    }

    printf("Opgeslagen!");
    fprintf(logFile, "\n");
    fclose(logFile);
    
    showLog();

    return 0;
}
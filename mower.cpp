#include "mower.h"
#include <mutex>
using namespace std;
//use an array of waypoints for now.  A more robust mapping mechanism will be required
float LATwaypoint[] = {37.971417,   37.971495,  37.971497,  37.971432,  37.971417,  37.971495,  37.971497,  37.971432};
float LONwaypoint[] = {-87.529813, -87.529810, -87.529698, -87.529695, -87.529813, -87.529810, -87.529698, -87.529695};

int leftspeed;
int rightspeed;

int uSonic[NUM_ULTRAS];

mutex gps_lock;
condition_variable cv_gps;
GPS gps;

mutex drive_lock;
condition_variable cv_drive;

bool first = true;
char adjustment;
char targetspeed;

//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include "MSGHandler.h"
#include "DataSource.h"
#include <vector>
#include <string>

#define   MESH_PREFIX     "hack_mesh"
#define   MESH_PASSWORD   "hack_mesh"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
std::vector<String>data_list;
DataSource gps(false);
MSGHandler msg();

const char* data = "31.91231051,-14.412234543";
unsigned long prev_millis;
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

//Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());

  data_list.push_back(msg);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    String msg;
    for(auto it = data_list.begin(); it != data_list.end(); ++it) msg += (*it) + ',';
    mesh.sendSingle(nodeId, msg);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  gps.addData("lat");
  gps.addData("long");
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  data_list.push_back(data);
  prev_millis = millis();
}

void loop() {
  // it will run the user scheduler as well
  if(millis() - prev_millis > 500){
    for(auto it = data_list.begin(); it != data_list.end(); ++it){
      Serial.printf("Len:%02d,Data:%s\n",data_list.size(),(*it).c_str());
    }
    value lat;
    value long;
    lat.f = 23.435345;
    long.f = 67.13673;
    gps.setData("lat", lat);
    gps.setData("long",long);
    prev_millis = millis();
  }
  mesh.update();
}

#include <Arduino.h>
#include <ros.h>


void setup() 
{
  // put your setup code here, to run once:

const int motor_front_left_pwm = 
const int motor_front_left_encoderA = 
const int motor_front_left_encoderB = 

const int motor_middle_left_pwm = 
const int motor_middle_left_encoderA = 
const int motor_middle_left_encoderB = 

const int motor_rear_left_pwm = 
const int motor_rear_left_encoderA = 
const int motor_rear_left_encoderB = 

const int motor_front_right_pwm = 
const int motor_front_right_encoderA = 
const int motor_front_left_encoderB = 

const int motor_middle_right_pwm = 
const int motor_middle_right_encoderA = 
const int motor_middle_right_encoderB = 

const int motor_rear_right_pwm = 
const int motor_rear_right_encoderA = 
const int motor_rear_right_encoderB = 

analogWrite()


}

void loop() {
  // put your main code here, to run repeatedly:
}


// #include<RMCS2303drive.h>
//Importing the motor driver header files for controlling the motor
#include <ros.h>
#include <std_msgs/Int32.h>
#include<geometry_msgs/Twist.h>
// Handles startup and shutdown of ROS

// RMCS2303 rmcs;      


//parameter Settings "Refer datasheet for details"
byte right_motor_id=7;
byte left_motor_id=6;
int INP_CONTROL_MODE=257;           
int PP_gain=32;
int PI_gain=16;
int VF_gain=32;
int LPR=3526;
int acceleration=5000;
int speed_r;
bool direction_r = 0;
int speed_l;
bool direction_l = 0;
long int Current_position_r;
long int Current_position_l;

//long int Current_Speed;


bool _connected = false;

void onTwist(const geometry_msgs::Twist &msg)
{
  if (!_connected)
  {
    rmcs.Speed(right_motor_id,0); 
    rmcs.Speed(left_motor_id,0);
    digitalWrite(motor_front_left_pwm) 
    return;
  }

  // Cap values at [-1 .. 1]
  float x = max(min(msg.linear.x, 1.0f), -1.0f);
  float z = max(min(msg.angular.z, 1.0f), -1.0f);

  // Calculate the intensity of left and right wheels. Simple version.
  // Taken from https://hackernoon.com/unicycle-to-differential-drive-courseras-control-of-mobile-robots-with-ros-and-rosbots-part-2-6d27d15f2010#1e59
  //float l = (msg.linear.x - msg.angular.z) / 2;
  //float r = (msg.linear.x + msg.angular.z) / 2;

  float L =  0.28;
  float R = 0.065/2;
  float v = msg.linear.x;
  float w = msg.angular.z;
  float speed_r = ((2 * v) + (w * L)) / (2 * R);
  float speed_l = ((2 * v) - (w * L)) / (2 * R);
// Here conversion of wheel velocities to motor RPM has to be done.
  
  //ROS Code Here
  if(speed_r<0){direction_r = 1;}
  else {direction_r = 0;}

  if(speed_l<0){direction_l = 1;}
  else {direction_l = 0;}

  rmcs.Enable_Digital_Mode(right_motor_id,direction_r);
  rmcs.Speed(right_motor_id,abs(speed_r)); 

  rmcs.Enable_Digital_Mode(left_motor_id,direction_l);
  rmcs.Speed(left_motor_id,abs(speed_l)); 
  
}

ros::NodeHandle node;

// Subscribe for wheel velocity.
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &onTwist);

// Keep track of the number of wheel ticks
std_msgs::Int32 right_wheel_tick_count;
ros::Publisher rightPub("right_ticks", &right_wheel_tick_count);

std_msgs::Int32 left_wheel_tick_count;
ros::Publisher leftPub("left_ticks", &left_wheel_tick_count);
//std_msgs::Int32 right_wheel_speed;
//ros::Publisher rightPub_speed("right_speed", &right_wheel_speed); 
// 100ms interval for measurements
const int interval = 100;
long previousMillis = 0;
long currentMillis = 0; 
//This is the data type of cmd_vel topic that we can to receive from the move_base stack for the robot wheel desired speed
///ROS Code Here


bool rosConnected()
{
  // If value changes, notify via LED and console.
  bool connected = node.connected();
  if (_connected != connected)
  {
    _connected = connected;
    digitalWrite(13, connected); // false -> on, true -> off
    //Serial.println(connected ? "ROS connected" : "ROS disconnected");
  }
  else
  {digitalWrite(LED_BUILTIN, connected);
    }
  return connected;
}




void setup()
{

   rmcs.Serial_selection(0);       //Serial port selection:0-Hardware serial,1-Software serial
   rmcs.Serial0(9600);             //Set baudrate for usb serial to monitor data on serial monitor
   //Serial.println("RMCS-2303 Speed control mode demo\r\n\r\n");

   rmcs.begin(&Serial1,9600);    //Uncomment if using hardware serial port for mega2560:Serial1,Serial2,Serial3 and set baudrate. Comment this line if Software serial port is in use
   //rmcs.begin(&myserial,9600);     //Uncomment if using software serial port. Comment this line if using hardware serial.
   rmcs.WRITE_PARAMETER(right_motor_id,INP_CONTROL_MODE,PP_gain,PI_gain,VF_gain,LPR,acceleration,0);    //Uncomment to write parameters to drive. Comment to ignore.

   rmcs.WRITE_PARAMETER(left_motor_id,INP_CONTROL_MODE,PP_gain,PI_gain,VF_gain,LPR,acceleration,0);  
   //rmcs.READ_PARAMETER(slave_id);
   //rmcs.Speed(slave_id,50);
   // Connect to rosserial socket server and init node. (Using default port of 11411)
   //Serial.print("Connecting to ROS serial server at %s\n", server.toString().c_str());
   //node.getHardware()->setConnection(server);
   node.initNode();
   node.advertise(rightPub);
   node.advertise(leftPub);
   //node.advertise(rightPub_speed);
   node.subscribe(sub);
   
}
 
void loop(void) {


if (!rosConnected())
{
  rmcs.Speed(right_motor_id,0);
  rmcs.Speed(left_motor_id,0);
}
   //Here i have changed the speed from 0 to 50 due to some technical reason.
  // Record the time
currentMillis = millis();
 
  // If 100ms have passed, print the number of ticks
if (currentMillis - previousMillis > interval) {    
    previousMillis = currentMillis;
    right_wheel_tick_count.data = rmcs.Position_Feedback(right_motor_id);
    left_wheel_tick_count.data = rmcs.Position_Feedback(left_motor_id);
    //right_wheel_speed.data = rmcs.Speed_Feedback(slave_id);
    rightPub.publish( &right_wheel_tick_count );
    leftPub.publish( &left_wheel_tick_count );
    //rightPub_speed.publish( &right_wheel_speed );
    node.spinOnce();
  }
}
# Google Maps Alarm Clock

## Project Motivation
Commuters must wake up extra early in order to arrive at their obligations on time.  The more traffic on the route to their destination, the earlier they must wake up.  However, traffic typically varies day to day making it difficult to have a healthy and consistent wake schedule. These variances may cause someone to wake too late or too early causing them to lose unnecessary sleep. An alarm clock that utilizes Google Maps data could fix this problem by setting an appropriate alarm ensuring a person has enough time to get to their destination on-time based on the current commuter traffic. 

To do this, the clock will be initialized with several key pieces of data.  Some data may include the average length of the morning routine, a home address, a destination address, and the amount of time needed to “clock-in” once arrived at the destination.  With all necessary data initialized, the clock will continue to query available routes and use that data to set the necessary alarm time needed.  The Clock will handle the morning arithmetic with concrete data so a person may take advantage of every extra minute under the covers to get a good night’s rest

## Description and Approach
The Sparkfun ESP8266 Thing served as the master device. It also served for the Wi-Fi communication protocol. Three slave devices were used with the ESP8266 including an Arduino Uno, a Grove-LCD at 5v, and a Sparkfun LSM6DS3 at 3.3v. All devices connected to the Arduino Uno were connected using the Seeed Base Shield except the ESP8266 which connected directly via digital pin to the I2C communication pins. The devices all used I2C serial communication at 115200 baud. The Base Shield also connected two other digital pin devices which are the Grove Button and Grove Buzzer.

This hardware configuration would allow received cloud data to be processed between the two main devices, the ESP8266 and Arduino Uno. For example, the received time data from the web would be processed on the ESP8266 for desired time-zone calculations before being sent to the Grove LCD to be displayed to the user. Accelerometer data from the LSM6DS3 was also collected and stored in a data table on Amazon Web Services.

Research showed that most people tend to have the same morning routine so the time that a user got out of bed was determined by a single variable as the traffic time.

## Cloud Solution
Various AWS cloud services were utilized for the project. Namely, they include the IoT Core, IoT Analytics and
Data Pipeline, Lambda, Cloudsight dashboard, and DynamoDB. The device would send messages up to IoT Core via the MQTT protocol where the message would be interpreted by a rules engine and sent to various areas of the cloud architecture such as the analytics pipeline for accelerometer data, or the lambda function for the alarm logic

To derive whether to wake the user when the alarm is turned on the device constantly querys the cloud setting off a lambda function finding the user settings in the database. It then uses those settings as well as the information gathered from the MapQuest traffic API to make a decision whether to send a signal to the device triggering the buzzer if the alarm state is set to “on”.

## Hardware Components
- Sparkfun ESP8266 Thing
- Arduino UNO R3
- Seeed Studio Base Shield v2.1
- Sparkfun LSM6DS3
- Grove-LCD RGB Backlight v4.0
- Grove Button
- Grove Buzzer

## Challenges
Hardware:
Master to multiple slaves was troublesome. While getting the initial ESP8266->Arduino Uno (Master->Slave) was not too difficult, adding the other devices created issues. Until each device could successfully be called sharing the I2C channel at desired times, it was difficult to visualize the final implementation. One alternate solution could have been to remove the Arduino using only the ESP8266 and then use the LSM6DS3 with SPI communication and LCD with I2C.  After enough troubleshooting, keeping the Arduino turned out to be a working solution.

Understanding how delay can affect messages being sent between devices at different baud rates also caused issues. Bytes of data became scrambled if not enough time was given for any needed logic. Therefore, depending on what data was being sent, careful delay chosen between bounds fixed early device communication issues.

Cloud(AWS):
AWS is an enterprise level service which was very heavy weight for a small project like this. There were also missing features that could have been useful. Some include real time graphing solutions, official libraries for connecting the device to AWS for Arduino, and clear instructions on how to communicate with AWS from the device.

## Future Work/Improvements
Hardware:
There are small things that could be sharpened. For example, tidying up delay between the button press and what it signals to the device. Currently set at delay(300) means that an immediate second button press before 300ms would illicit no response. This choice was to alleviate pushing the button, turning the alarm off, and accidentally holding it too long instantly turning the alarm back on. Writing a better implementation where holding the button maintains the same new state would not be difficult.
Due to time constraints this was not a priority fix.

Cloud(AWS):
Right now, when the alarm is on, it constantly queries the cloud every 15 seconds to see if it should wake the user. This querying could be removed by having all of the querying take place in the cloud. Essentially, a schedule could be used where every minute or so it goes through all alarms that are in the database and checks if they are on. If they are on, then all alarms which are 2 hours away could be found from the desired arrival time and then use that collection and query mapquest sending a wake-up alert to those who meet the conditions to wake the user. This could be implemented as a scheduled action in the cloud without having the device constantly querying the lambda function.

Other Features:
There are other quality of life features that may be great to have and implemented with not too much trouble in development. One would be adding a second digital pin Grove Button device for handling a snooze feature.  Exploring more buttons could lead to other features. For example, a button could display data from one’s cloud data table such as average movement while sleeping, wake time, or other desired calculations. For a final device, depending on the demographic and use case, the possibilities of adding other sensors and doing something useful for the user with that information are many. E.g. Medical patient vs average consumer may want a different sensor and relative information for different reasons.

# cmake_minimum_required(VERSION <specify CMake version here>)
project(solar_frame_arduino)

set(CMAKE_CXX_STANDARD 14)

include_directories(Solar_Arduino_Sketches/Alt_motor_test)
include_directories(Solar_Arduino_Sketches/libraries/Adafruit_LSM9DS1-1.0.1)
include_directories(Solar_Arduino_Sketches/libraries/Adafruit_Sensor)
include_directories(Solar_Arduino_Sketches/libraries/Adafruit_Sensor/Adafruit_Sensor-master)
include_directories(Solar_Arduino_Sketches/libraries/Azande/examples)
include_directories(Solar_Arduino_Sketches/libraries/Azande/examples/CalculateWiFi)
include_directories(Solar_Arduino_Sketches/libraries/Azande/examples/Gball)
include_directories(Solar_Arduino_Sketches/libraries/Azande/examples/WifiSignalStrength)
include_directories(Solar_Arduino_Sketches/libraries/Azande/src)
include_directories(Solar_Arduino_Sketches/Solar_arduino)

add_executable(solar_frame_arduino
        Solar_Arduino_Sketches/Alt_motor_test/motors.cpp
        Solar_Arduino_Sketches/Alt_motor_test/motors.h
        Solar_Arduino_Sketches/libraries/Adafruit_LSM9DS1-1.0.1/Adafruit_LSM9DS1.cpp
        Solar_Arduino_Sketches/libraries/Adafruit_LSM9DS1-1.0.1/Adafruit_LSM9DS1.h
        Solar_Arduino_Sketches/libraries/Adafruit_Sensor/Adafruit_Sensor-master/Adafruit_Sensor.h
        Solar_Arduino_Sketches/libraries/Azande/examples/CalculateWiFi/arduino_secrets.h
        Solar_Arduino_Sketches/libraries/Azande/examples/Gball/arduino_secrets.h
        Solar_Arduino_Sketches/libraries/Azande/examples/WifiSignalStrength/arduino_secrets.h
        Solar_Arduino_Sketches/libraries/Azande/src/Azande.cpp
        Solar_Arduino_Sketches/libraries/Azande/src/Azande.h
        Solar_Arduino_Sketches/libraries/Azande/src/AzandeSystemConfig.h
        Solar_Arduino_Sketches/libraries/Azande/src/AzandeUserMacros.h
        Solar_Arduino_Sketches/Solar_arduino/motors.cpp
        Solar_Arduino_Sketches/Solar_arduino/motors.h
        Solar_Arduino_Sketches/Solar_arduino/sensors.cpp
        Solar_Arduino_Sketches/Solar_arduino/sensors.h)

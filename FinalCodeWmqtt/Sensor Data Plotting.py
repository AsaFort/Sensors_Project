import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import json
import time

#mqtt setup
client = mqtt.Client()
broker_address = "test.mosquitto.org"
port = 1883
topic = "afort/endpoint1"
client.connect(broker_address)

#data storage
filteredA_list = []
accA_list = []
gyroA_list = []
time_list = [] 

#time counter
start_time = time.time()

def on_message(client, userdata, message):
    global filteredA_list, accA_list, gyroA_list, time_list

    payload = json.loads(message.payload.decode())
    filteredA_list.append(payload["filteredA"])
    accA_list.append(payload["accA"])
    gyroA_list.append(payload["gyroA"])

    #elapsed time
    elapsed_time = time.time() - start_time
    time_list.append(elapsed_time)

    #update plot
    plt.clf() 
    plt.plot(time_list, filteredA_list, label="Filtered Acceleration", marker='o')
    plt.plot(time_list, accA_list, label="Accelerometer", marker='o')
    plt.plot(time_list, gyroA_list, label="Gyroscope", marker='o')
    plt.xlabel("Time (s)")
    plt.ylabel("Acceleration (g's)")
    plt.title("Sensor Data")
    plt.grid(True)
    plt.legend()
    plt.pause(0.1) 

client = mqtt.Client()
client.on_message = on_message

client.connect(broker_address, port)

client.subscribe(topic)

client.loop_start()

plt.figure()

try:
    
    plt.ion()  #interactive mode
    plt.show()

    while True:
        pass

except KeyboardInterrupt:
    print("Exiting...")

#stop loop and disconnect
client.loop_stop()
client.disconnect()

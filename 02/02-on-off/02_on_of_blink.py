import serial, sys
import time

serialName = 'COM10'
serialBaudRate = 115200
ser = serial.Serial(serialName, baudrate=serialBaudRate)
print('start')
print("\nМигание светодиода с заданным периодом:")

try:
    t = int(input("Период мигания (мс): ")) / 1000
    print("Мигание начато. Нажмите Ctrl+C для остановки.\n")
    while True:
        ser.write(b'e')
        time.sleep(t)
        ser.write(b'd')
        time.sleep(t)

except KeyboardInterrupt:
    print("\nОстановлено пользователем")

finally:
    ser.close()
    print("Порт закрыт")



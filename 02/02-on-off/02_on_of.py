import serial, sys
serialName = 'COM10'
serialBaudRate = 115200
ser = serial.Serial(serialName, baudrate=serialBaudRate)
print('start')
print("\nУправление светодиодом:")
print("  e - включить")
print("  d - выключить")
print("  v - Версия и название устройства")
print("  q - выход")

try:
    while True:
        cmd = input("Команда: ").strip().lower()# Считываем команду из пайтон терминала, стрип удаляет все пробелы, lower понижает регистр
        if cmd == 'q':
            print('Exit...')
            break

        elif cmd in ['e', 'd', 'v']:
            ser.write(cmd.encode('ascii')) # Отправляем команду через com-порт черз кодировку тк передача данных проходит в байтах
            response = ser.readline().decode().strip() #Получаем ответ
            if response:
                print(f"Ответ: {response}")

        else:
            print("Неверная команда")


finally:
    ser.close()
    print("Порт закрыт")



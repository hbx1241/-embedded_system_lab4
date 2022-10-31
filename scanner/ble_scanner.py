from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate

ID = b"\x00\x00"
class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device", dev.addr)
        elif isNewData:
            print("Recevied new data from", dev.addr)
    def handleNotification(cHandle, data):
        if (cHandle == 13):
            print("Heart rate: ", int.from_bytes(data, "big"), " bpm")
        elif (cHandle == 19):
            print("Magneto " + str(data, 'utf-8'))

#scanner = Scanner().withDelegate(ScanDelegate())

#

scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan()
n = 0
addr = []

        
#
for dev in devices:
    print("%d:Device %s (%s), RSSI=%d dB"% (n, dev.addr, dev.addrType, dev.rssi))
    addr.append(dev.addr)
    n += 1
    for (adtype, desc, value) in dev.getScanData():
        print(" %s = %s"%(desc, value))

number = input('Enter your device number:')
print('Device', number)
num = int(number)
print(addr[num])
print("Connecting...")
dev = Peripheral(addr[num], 'random')
dev.setDelegate(ScanDelegate)

#
print("Services...")
setup_data = b"\x01\x00"
for svc in dev.services:
    print(str(svc))
    for ch in svc.getCharacteristics():
        i = ch.getHandle()
        for dpt in ch.getDescriptors():
            i += 1
            if (str(dpt.uuid) == "00002902-0000-1000-8000-00805f9b34fb"):
                print("Found CCCD!")
                try:
                    dev.writeCharacteristic(i, setup_data, withResponse=True)
                    print("change CCCD value into 1")
                except:
                   print("fail to change CCCD value")
try:
    while True:
        if dev.waitForNotifications(1.0):
            print("Get Notification!") 
            continue
            # handleNotification() was called
        print("waiting")  
finally:
    dev.disconnect()

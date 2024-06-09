from os import getenv

Import('env')

device_id = ''

if not env.IsIntegrationDump():
    device_id = getenv('DEVICE_ID')

    if device_id is None:
        print('What is the device ID?')
        device_id = input()

env.Append(
    CPPDEFINES=[
        ('DEVICE_ID', env.StringifyMacro(device_id))
    ]
)
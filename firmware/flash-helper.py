from os import getenv

Import('env')

if env.IsIntegrationDump():
    Return()

device_id = getenv('DEVICE_ID')

if device_id is None:
    Return()

env.Append(
    CPPDEFINES=[
        ('DEVICE_ID', env.StringifyMacro(device_id))
    ]
)
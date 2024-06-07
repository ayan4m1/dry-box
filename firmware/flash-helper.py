Import('env')

if env.IsIntegrationDump():
    Return()

print(f'What is the device ID?')
device_id = input()

print('What is the WiFi SSID?')
ssid = input()

print('What is the WiFi PSK?')
psk = input()

print('What is the Discord webhook URL?')
webhook_url = input()

env.Append(
    CPPDEFINES=[
        ('DEVICE_ID', env.StringifyMacro(device_id)),
        ('WIFI_SSID', env.StringifyMacro(ssid)),
        ('WIFI_PSK', env.StringifyMacro(psk)),
        ('DISCORD_WEBHOOK_URL', env.StringifyMacro(webhook_url))
    ]
)
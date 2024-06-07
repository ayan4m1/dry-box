import os

Import('env')

if env.IsIntegrationDump():
    Return()

device_id = os.getenv('DEVICE_ID')
wifi_ssid = os.getenv('WIFI_SSID')
wifi_psk = os.getenv('WIFI_PSK')
discord_webhook_url = os.getenv('DISCORD_WEBHOOK_URL')

if device_id is None or wifi_ssid is None or wifi_psk is None or discord_webhook_url is None:
    Return()

env.Append(
    CPPDEFINES=[
        ('DEVICE_ID', env.StringifyMacro(device_id)),
        ('WIFI_SSID', env.StringifyMacro(wifi_ssid)),
        ('WIFI_PSK', env.StringifyMacro(wifi_psk)),
        ('DISCORD_WEBHOOK_URL', env.StringifyMacro(discord_webhook_url))
    ]
)
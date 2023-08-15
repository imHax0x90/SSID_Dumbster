import subprocess

# Run the command to list available Wi-Fi connections
network_info = subprocess.check_output(['nmcli', '-f', 'SSID,SECURITY', 'device', 'wifi', 'list']).decode('utf-8')

networks = network_info.strip().split('\n')[1:]

wifi_data = {}

# Parse and store Wi-Fi network information
for network in networks:
    parts = network.split(None, 1)  # Split at first whitespace
    if len(parts) >= 2:
        ssid, security = parts
        wifi_data[ssid] = security

# Print the dictionary of all Wi-Fi connections
for ssid, security in wifi_data.items():
    print("{:<30}|  {:<}".format(ssid, security))
    
    
def connection(essid, password):

    # Run the command to connect to the Wi-Fi network
    connect_command = f'sudo nmcli device wifi connect "{essid}" password "{password}"'
    process = subprocess.Popen(connect_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    # Display the output or the error (if any)
    if stdout:
        print("Output:")
        print(stdout.decode('utf-8'))
    else:
        print("Error:")
        print(stderr.decode('utf-8'))

    
connection("PlsDontTalk", "2meifIdidnotYet")

def connection(essid, password):

    # Run the command to connect to the Wi-Fi network
    connect_command = f'sudo nmcli device wifi connect "{essid}" password "{password}"'
    process = subprocess.Popen(connect_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    # Display the output or the error (if any)
    if stdout:
        print("Output:")
        print(stdout.decode('utf-8'))
    else:
        print("Error:")
        print(stderr.decode('utf-8'))
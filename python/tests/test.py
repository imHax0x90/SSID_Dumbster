import subprocess
import asyncio

async def connection(essid, password):
    connect_command = f'sudo nmcli device wifi connect "{essid}" password "{password}"'
    process = await asyncio.create_subprocess_shell(
        connect_command,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, stderr = await process.communicate()

    if stdout:
        output = stdout.decode('utf-8')
        print("Output:")
        print(output)
        if "Périphérique « wlp0s20f3 » activé" in output or "Device 'wlp0s20f3' successfully activated" in output:
            print(f"Successfully connected to network '{essid}' with password '{password}'.")
            return True
        else:
            print(f"Activation message not found for password '{password}'.")
    else:
        print("Error:")
        print(stderr.decode('utf-8'))
    
    return False

async def main():
    ssid = "PlsDontTalk"
    passwords = [
        "azeRTy!*000?/*",
        "AZerty000",
        "ABcdef000",
        "MeTooISuck",
        "IAintAPassword@!?.:"
    ]

    for password in passwords:
        if await connection(ssid, password):
            break

# Run the asyncio event loop
asyncio.run(main())

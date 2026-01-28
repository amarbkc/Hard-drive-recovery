import os

# Define a byte equivalent for reading
BYTE = bytes

# Open the drive to read
try:
    in_file = open('/dev/disk2', 'rb')
except IOError as e:
    print(f"Error opening hard drive: {e}")
    exit(1)

# Initialize output file to None
out_file = None

# Block of 512 bytes to check at once
buff = bytearray(512)

# Counter for naming serially
counter = 0

try:
    while in_file.readinto(buff) == 512:
        # If the first 4 bytes match the JPEG header
        if (buff[0] == 0xff and buff[1] == 0xd8 and buff[2] == 0xff and
            (buff[3] >= 0xe0 and buff[3] <= 0xef)):
            print("JPEG found")

            if out_file is not None:
                out_file.close()

            fname = f"{counter:03}.jpg"
            out_file = open(fname, "wb")
            counter += 1

            out_file.write(buff)

        # If the first 5 bytes match the PDF header
        elif (buff[0] == 0x25 and buff[1] == 0x50 and buff[2] == 0x44 and
              buff[3] == 0x46 and buff[4] == 0x2d):
            print("PDF found")

            if out_file is not None:
                out_file.close()

            fname = f"{counter:03}.pdf"
            out_file = open(fname, "wb")
            counter += 1

            out_file.write(buff)

        # If no new file is found, write to the existing file
        elif out_file is not None:
            out_file.write(buff)

finally:
    if out_file is not None:
        out_file.close()
    in_file.close()

# Mom's ESP32
My mom loves her garden. This project is to help her track the tempature, moisture, etc.

NOTE: I'm dumping all the project notes here, and will move them to a more appropriate place later.

# Goals
1. Create a device using ESP32 that will have an easy wifi setup mode, run on solar power, and include a web interface for calibrating the sensors.
2. Track the sensor data using a remote Laravel app probably running on a Coolify instance. I would use AWS serverless but I need more practice with modern Laravel.
3. Design and print an enclosure using food-safe PETG with a stainless steel nozzle (brass can leach lead), and wether treat it.

## Nice to have
1. OTA firmware updates.
2. Plugin for Middleware like discord/slack notifications and alerts.
3. Interface for solinoid or valve to control watering based on certain conditions.
4. Interface for camera/motion sensors like wildlife camera.
5. Integration with AI to analyze data or visually monitor plant health/pests.

# Usage
Added a script to compile html to C variables `./build-html.sh` This will make .h files out of any html files found in the html directory. Requires xxd from vim. Use `brew install vim` if xdd is not found on your system. If the need for a more complex web server is required, may replace this with webpack or something like that.

# Goal Progress
## ESP32
### Wifi Setup Mode - done
1. Hold the wifi setup button while rebooting the device.
2. Connect to the NickLabs_AP wifi network
3. Go to 192.168.4.1
Make sure VPN is off, I was connected to nord and it didn't work.

### Solar power
This will be done with a MCP73871 circuit and a li/ion battery. I'll try something like this: [Adafruit USB LiIon/LiPoly charger - v1.2](https://www.adafruit.com/product/259)

I have a solar panel with a microusb connector and a couple panels from some cheap LEDs I might be able to repurpose.

### Web Interface

I created a script to compile the html folder to c variables.

But I'm thinking maybe I should just allow the ESP to accept sensor calibration from the Laravel API. That way the only setup will be:

1. Wifi credentials.
2. API key from the Laravel app.

## Case
I'm picturing a spike you can stick into the dirt, and aim the camera toward the produce you want to view at night. There are always critters that eat mom's crops so having a way to position the infrared sensor and camera would be nice.

The solar panel should be able to block rain, like an umbrella at the top of the spike.
```
 _________ <- Solar Panel
     |
    | |
    | | <- ESP enclosure
     |
     |\
     | | <- Moisture sensor
```
She currently has some motion sensing lights that go on to try and scare off the animals. She calls it her "Rat Rave". I can at least build that functionality in.

I need to look up how to waterproof PETG I think there is some epoxy resin that I already have which may work.

How do I stop corosion for exposed cables? That seems to happen on a lot of stuff I get from amazon.

Printable parts:
1. Case
2. Threaded tip for spike to mount solar panel
3. Maybe a [positionable camera enclosure with ball joint](https://www.thingiverse.com/thing:3652452)
4. Maybe a plastic spike, [like the kind I printed for drip feeder](https://www.thingiverse.com/thing:26459). I'll have to see how they hold up under the sunlight and 105 degree heat.

Metal rod?
Screws for enclosure.

## Misc

### Wifi range
### Heat

## Laravel

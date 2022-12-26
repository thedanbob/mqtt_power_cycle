# MQTT Power Cycler for Home Assistant

This firmware turns a Sonoff Basic into an [MQTT Button](https://www.home-assistant.io/integrations/button.mqtt/) for Home Assistant which lets you power cycle a device. When the button is triggered in Home Assistant, the Sonoff will cut power for a configurable amount of time (default: 10 seconds). It can also be configured to do the opposite, turn *on* power for a certain amount of time.

[platformio.sample.ini](platformio.sample.ini) shows how to configure the project. All options and their defaults can be found in [config.h](include/config.h). No configuration on the Home Assistant side is necessary besides setting up the MQTT integration; all device-specific settings are shared via [MQTT discovery](https://www.home-assistant.io/docs/mqtt/discovery).

Add `upload_protocol = espota` to platformio.ini after flashing the firmware once to enable over-the-air updates. To update OTA a device running the `release` environment with an IP address of 192.168.1.3, you would run

```bash
pio run -e release -t upload --upload-port 192.168.1.3
```

You can also add `upload_port = 192.168.1.3` to platformio.ini instead of using the --upload-port flag.

## Button press confirmation

You may want to require confirmation when pressing the button on your Home Assistant dashboard to avoid accidental power cycles. This can be accomplished by customizing the button's [Tap Action](https://www.home-assistant.io/dashboards/actions/#tap-action) in YAML:

```yaml
type: button
name: Power Cycler
tap_action:
  action: call-service
  service: button.press
  service_data: {}
  target:
    entity_id: button.power_cycler
  confirmation:
    text: Are you sure you want to power cycle this device?
```

## Other firmware

[MQTT Light](https://github.com/thedanbob/mqtt_light) \
[MQTT Garage Door](https://github.com/thedanbob/mqtt_garage_door)

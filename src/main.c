/**
 * @brief This is the main.c source code of the application. The BMI270 Sensor sample template was the initial start of this file for nrf5340 SoC.
 * @code
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause.
 * @endcode
 *
 * @file main.c
 * @version 1.0
 * @author Shaun Lin (hl116@rice.edu)
 * @copyright Rice University & HealthSeers Inc. Ⓒ 2024
 */

/**
 * @page main_source_page Application Main Thread
 * @brief This page describes the Main Thread's functionality.
 *
 * @section For Building Release Version
 *
 * @subsection CONFIGS
 * 
 */

// ------------------ Includes ------------------

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h> // BMI270 IMU sensor driver
#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h> // GC9A01 display driver
#include <zephyr/drivers/gpio.h>
#include <lvgl.h> // Graphics library
#include <string.h>
#include <zephyr/logging/log.h>


// ------------------ Macros ------------------

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(app);

// button configuration
#ifdef CONFIG_GPIO
static struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_cb;
static uint32_t count; // button press count

// ------------------ Function Prototypes ------------------

/**
 * @brief Button press interrupt callback function.
 *
 * @param port Pointer to the GPIO device structure for the button.
 * @param cb Pointer to the GPIO callback structure.
 * @param pins Pin mask that triggered the callback.
 */
static void button_isr_callback(const struct device *port, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	count = 0;
}
#endif

/**
 * @brief Setup the hardware button with an interrupt.
 */
static void setup_button(void) {
	int err;
    if (device_is_ready(button.port)) {
        err = gpio_pin_configure_dt(&button, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return 0;
		}
		gpio_init_callback(&button_cb, button_isr_callback, BIT(button.pin));
		err = gpio_add_callback(button.port, &button_cb);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return 0;
		}
		err = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return 0;
		}
    }
}

/**
 * @brief Initialize the display and the LVGL library.
 *
 * @param display_dev Pointer to the display device structure.
 */
static void initialize_display(const struct device *display_dev) {
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device is not ready");
        return;
    }
    // lv_init();
    // lvgl_driver_init();
    LOG_INF("Display initialized");
}

/**
 * @brief Setup the BMI270 IMU sensor.
 *
 * @param sensor_dev Pointer to the sensor device structure.
 */
static void setup_sensors(const struct device *sensor_dev) {
    if (!device_is_ready(sensor_dev)) {
        LOG_ERR("Device %s is not ready\n", sensor_dev->name);
        return;
    }
    LOG_INF("Sensor initialized, device %p name is %s\n", sensor_dev, sensor_dev->name);

    // Configure sensor parameters here if necessary
}

/**
 * @brief Configure the IMU sensor's accelerometer and gyroscope.
 * 
 * @param sensor_dev Pointer to the sensor device structure.
 * @return int 0 on success, negative error code on failure.
 */
int configure_imu_sensor(const struct device *sensor_dev) {
	struct sensor_value full_scale, sampling_freq, oversampling;

    if (!device_is_ready(sensor_dev)) {
        LOG_ERR("Sensor device is not ready");
        return -ENODEV;
    }

    // Configure the accelerometer
	/* Setting scale in G, due to loss of precision if the SI unit m/s^2
	 * is used
	 */
    full_scale.val1 = 2;            /* G */
	full_scale.val2 = 0;
	sampling_freq.val1 = 100;       /* Hz. Performance mode */
	sampling_freq.val2 = 0;
	oversampling.val1 = 1;          /* Normal mode */
	oversampling.val2 = 0;

    sensor_attr_set(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_FULL_SCALE, &full_scale);
    sensor_attr_set(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_OVERSAMPLING, &oversampling);
	/* Set sampling frequency last as this also sets the appropriate
	 * power mode. If already sampling, change to 0.0Hz before changing
	 * other attributes
	 */
    sensor_attr_set(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &sampling_freq);

    // Configure the gyroscope
    /* Setting scale in degrees/s to match the sensor scale */
	full_scale.val1 = 500;          /* dps */
	full_scale.val2 = 0;
	sampling_freq.val1 = 100;       /* Hz. Performance mode */
	sampling_freq.val2 = 0;
	oversampling.val1 = 1;          /* Normal mode */
	oversampling.val2 = 0;

	sensor_attr_set(sensor_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_FULL_SCALE, &full_scale);
	sensor_attr_set(sensor_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_OVERSAMPLING, &oversampling);
	/* Set sampling frequency last as this also sets the appropriate
	 * power mode. If already sampling, change sampling frequency to
	 * 0.0Hz before changing other attributes
	 */
	sensor_attr_set(sensor_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &sampling_freq);

    LOG_INF("IMU sensor configured successfully.");
    return 0;
}

/**
 * @brief Displays Cairdio & Rice logo for 3 seconds, then removes it after a delay.
 * 
 * @param display_dev Pointer to the display device structure.
 * @return void
 */
void display_logo_animation(const struct device *display_dev) {
	LOG_INF("Loading logo...");

    // Initialize a style for the logo
    static lv_style_t style;
    lv_style_init(&style);

    // Create an image object and apply the style
    lv_obj_t *obj_logo = lv_img_create(lv_scr_act());
    lv_obj_add_style(obj_logo, &style, 0);

    // Declare the image source
    LV_IMG_DECLARE(cairdio_and_rice_logo);
    lv_img_set_src(obj_logo, &cairdio_and_rice_logo);

    // Center the image on the screen
    lv_obj_center(obj_logo);

    // Refresh the display to show the image
    lv_task_handler();
    if (display_dev) {
        display_blanking_off(display_dev);
    }

    // Delay for 3 seconds to display the logo
    k_sleep(K_MSEC(3000));

    // Delete the logo object to clear it from the screen
    lv_obj_del(obj_logo);
}

/**
 * @brief Display menu including bluetooth status, logo, battery status and text for 3 seconds, then clear them after a delay.
 *
 * @param display_dev Pointer to the display device structure.
 */
void menu(const struct device *display_dev) {
	LOG_INF("Loading menu...");

    // Initialize styles
    static lv_style_t style;
    lv_style_init(&style);

    // Create logo objects
    lv_obj_t *obj_cairdio_logo = lv_img_create(lv_scr_act()); // cairdio logo
    lv_obj_t *obj_bluetooth_status = lv_img_create(lv_scr_act()); // bluetooth status
    lv_obj_t *obj_battery_status = lv_img_create(lv_scr_act()); // battery status

    // Apply styles
    lv_obj_add_style(obj_cairdio_logo, &style, 0);
    lv_obj_add_style(obj_bluetooth_status, &style, 0);
    lv_obj_add_style(obj_battery_status, &style, 0);

    // Declare images
    LV_IMG_DECLARE(cairdio_logo);
    LV_IMG_DECLARE(bluetooth_connected);
    LV_IMG_DECLARE(battery_50_percentage);

    // Set sources
    lv_img_set_src(obj_cairdio_logo, &cairdio_logo);
    lv_img_set_src(obj_bluetooth_status, &bluetooth_connected);
    lv_img_set_src(obj_battery_status, &battery_50_percentage);

    // Set zoom and alignment
    lv_img_set_zoom(obj_cairdio_logo, 150);
    lv_img_set_zoom(obj_bluetooth_status, 100);
    lv_img_set_zoom(obj_battery_status, 100);

    lv_obj_align(obj_cairdio_logo, LV_ALIGN_CENTER, 0, -80); // top center
    lv_obj_align(obj_bluetooth_status, LV_ALIGN_CENTER, -65, -78); // top left
    lv_obj_align(obj_battery_status, LV_ALIGN_CENTER, 72, -78); // top right

    // Add text label for instructions
    lv_obj_t *count_label = lv_label_create(lv_scr_act());
    lv_label_set_text(count_label, "Hold the device\n for 10 seconds");
    lv_obj_align(count_label, LV_ALIGN_CENTER, 0, 0); // center

    // Refresh display
    lv_task_handler();
    if (display_dev) {
        display_blanking_off(display_dev);
    }

    // Delay for 3 seconds
    k_sleep(K_MSEC(3000));

    // Clean up
    lv_obj_del(obj_cairdio_logo);
    lv_obj_del(obj_bluetooth_status);
    lv_obj_del(obj_battery_status);
    lv_obj_del(count_label);
}

/**
 * @brief Display system status including bluetooth status, logo and battery status.
 *
 * @param display_dev Pointer to the display device structure.
 */
void system_status_bar(const struct device *display_dev, lv_obj_t *obj_cairdio_logo, lv_obj_t *obj_bluetooth_status, lv_obj_t *obj_battery_status) {
	// Initialize styles
    static lv_style_t style;
    lv_style_init(&style);

    // Apply styles
    lv_obj_add_style(obj_cairdio_logo, &style, 0);
    lv_obj_add_style(obj_bluetooth_status, &style, 0);
    lv_obj_add_style(obj_battery_status, &style, 0);

    // Declare images
    LV_IMG_DECLARE(cairdio_logo);
    LV_IMG_DECLARE(bluetooth_connected);
    LV_IMG_DECLARE(battery_50_percentage);

    // Set sources
    lv_img_set_src(obj_cairdio_logo, &cairdio_logo);
    lv_img_set_src(obj_bluetooth_status, &bluetooth_connected);
    lv_img_set_src(obj_battery_status, &battery_50_percentage);

    // Set zoom and alignment
    lv_img_set_zoom(obj_cairdio_logo, 150);
    lv_img_set_zoom(obj_bluetooth_status, 100);
    lv_img_set_zoom(obj_battery_status, 100);

    lv_obj_align(obj_cairdio_logo, LV_ALIGN_CENTER, 0, -80); // top center
    lv_obj_align(obj_bluetooth_status, LV_ALIGN_CENTER, -65, -78); // top left
    lv_obj_align(obj_battery_status, LV_ALIGN_CENTER, 72, -78); // top right
}

/**
 * @brief Process sensor data to fetch accelerometer and gyro readings.
 *
 * @param sensor_dev Pointer to the sensor device structure.
 * * @return Ay The accelerometer's Y-axis value, with type int.
 */
int process_sensor_data(const struct device *sensor_dev) {
    struct sensor_value acc[3], gyr[3];

    if (sensor_sample_fetch(sensor_dev) < 0) {
        LOG_ERR("Failed to fetch sensor data");
        return;
    }

    sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, acc);
    sensor_channel_get(sensor_dev, SENSOR_CHAN_GYRO_XYZ, gyr);

	return acc[1].val1;
}


// ------------------ Main Thread ------------------
/**
 * @brief Main thread of the application entry point.
 */
int main(void) {
    const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	const struct device *const sensor_dev = DEVICE_DT_GET_ONE(bosch_bmi270);
	
	// ------------------ Display Initialization ------------------
    initialize_display(display_dev);
    setup_button();

	// ------------------ BMI270 IMU Initialization ------------------
	setup_sensors(sensor_dev);

	int ret = configure_imu_sensor(sensor_dev);
	if (ret != 0) {
		LOG_ERR("Failed to configure IMU sensor: %d", ret);
	}

    display_logo_animation(display_dev);
	menu(display_dev);

	// initialize accelerometer's Y-axis value
	int Ay = 0;
	// initialize the count down label from 10
	int count = 10;
	int count2 = 0;

	LOG_INF("Starting orientation detection...");
	// TODO: refactor following code into modular functions
	// ------------------ Main Thread Loop ------------------
    while (true) {
		// fetch sensor data's Ay value
		Ay = process_sensor_data(sensor_dev);
		// print the Ay (accelerometer's Y-axis value) value in terminal
		LOG_INF("accelerometer's Y-axis value: %d", Ay);

		count2++;
		if (count2 % 5 == 0) {
			count--; // decrement count every 50ms
		}

		// Create system_status_bar's objects
		lv_obj_t *obj_cairdio_logo = lv_img_create(lv_scr_act()); // cairdio logo
		lv_obj_t *obj_bluetooth_status = lv_img_create(lv_scr_act()); // bluetooth status
		lv_obj_t *obj_battery_status = lv_img_create(lv_scr_act()); // battery status
		system_status_bar(display_dev, obj_cairdio_logo, obj_bluetooth_status, obj_battery_status);

		// create a custom slider
		static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, 0};
		static lv_style_transition_dsc_t transition_dsc;
		lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);
		static lv_style_t style_main;
		static lv_style_t style_indicator;
		static lv_style_t style_knob;
		static lv_style_t style_pressed_color;
		lv_style_init(&style_main); // background color of the slider
		lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
		lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
		lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
		lv_style_set_pad_ver(&style_main, -2); /*Makes the indicator larger*/
		lv_style_init(&style_indicator); // left side of the knob
		lv_style_set_bg_opa(&style_indicator, LV_OPA_TRANSP); // using transparent color to hide the left side of the knob
		lv_style_init(&style_pressed_color);
		lv_style_set_bg_color(&style_pressed_color, lv_palette_darken(LV_PALETTE_CYAN, 2));

		// Create a label below the slider
		lv_obj_t * label = lv_label_create(lv_scr_act());
		lv_obj_t * count2_label = lv_label_create(lv_scr_act());
		
		char count_str[50];
		sprintf(count_str, "Remaining: %d", count);
		lv_label_set_text(count2_label, count_str);
		LOG_INF("Remaining: %d seconds...", count);

		// orientation detection
		// set the text of the label to "move right" if Ay < -1
		if (Ay < -1) {
			lv_style_init(&style_knob);
			lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
			lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_RED, 3));
			lv_style_set_border_width(&style_knob, 2);
			lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
			lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
			lv_style_set_transition(&style_knob, &transition_dsc);

			lv_label_set_text(label, "move right");
			lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), LV_PART_MAIN|LV_STATE_DEFAULT);

			// reset the count to 10 if Ay < -1
			count = 10;
		}
		// set the text of the label to "move left" if Ay > 1
		else if (Ay > 1) {	
			lv_style_init(&style_knob);
			lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
			lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_RED, 3));
			lv_style_set_border_width(&style_knob, 2);
			lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
			lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
			lv_style_set_transition(&style_knob, &transition_dsc);

			lv_label_set_text(label, "move left");
			lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), LV_PART_MAIN|LV_STATE_DEFAULT);

			// reset the count to 10 if Ay > 1
			count = 10;
		}
		// set the text of the label to "stay still" if Ay is between -1 and 1
		else {
			lv_style_init(&style_knob);
			lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
			lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_GREEN, 3));
			lv_style_set_border_width(&style_knob, 2);
			lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
			lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
			lv_style_set_transition(&style_knob, &transition_dsc);

			lv_label_set_text(label, "stay still");
			lv_obj_set_style_text_color(label, lv_color_hex(0x00FF00), LV_PART_MAIN|LV_STATE_DEFAULT);
		}

		// Create the slider and apply styles
		lv_obj_t * slider = lv_slider_create(lv_scr_act());
		lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
		lv_obj_add_style(slider, &style_main, LV_PART_MAIN);
		lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
		lv_obj_add_style(slider, &style_pressed_color, LV_PART_INDICATOR | LV_STATE_PRESSED);
		lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
		lv_obj_add_style(slider, &style_pressed_color, LV_PART_KNOB | LV_STATE_PRESSED);
		lv_obj_set_size(slider, 200, 10);
		lv_slider_set_range(slider, 0, 200); // set the range of the slider to 0-200

		// map the value of AY to the slider, AY = -10 -> slider = 200, AY = 0 -> slider = 100, AY = 10 -> slider = 0
		int slider_value = 100 - Ay * 10;
		lv_slider_set_value(slider, slider_value, LV_ANIM_OFF);
		lv_obj_center(slider);
		lv_obj_add_event_cb(slider, NULL, LV_EVENT_VALUE_CHANGED, NULL);

		lv_obj_align_to(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // offset the label below the slider 10 pixels
		lv_obj_align_to(count2_label, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // offset the count2_label below the label 10 pixels

		if (count == 0) {
			// print the text "Complete recording" in terminal
			LOG_INF("Complete recording");

			lv_obj_del(slider); // delete the object
			lv_obj_del(label); // delete the object
			lv_obj_del(count2_label); // delete the object
			lv_obj_del(obj_cairdio_logo); // delete the object
			lv_obj_del(obj_bluetooth_status); // delete the object
			lv_obj_del(obj_battery_status); // delete the object

			// after 10 seconds complete, exit the orientation detection and display the text "Check your mobile for the result"
			lv_obj_t * check_mobile_label = lv_label_create(lv_scr_act());
			lv_label_set_text(check_mobile_label, "Check your mobile\n for the result");
			lv_obj_align(check_mobile_label, LV_ALIGN_CENTER, 0, 0); // align on the center of the screen

			lv_task_handler();
			display_blanking_off(display_dev);

			k_sleep(K_MSEC(5000));// delay 5 seconds
			lv_obj_del(check_mobile_label); // delete the object
			lv_obj_clean(lv_scr_act()); // Clear the screen
			break; // exit the loop
		}
		lv_task_handler();
		display_blanking_off(display_dev);

		lv_obj_del(slider); // delete the object
		lv_obj_del(label); // delete the object
		lv_obj_del(count2_label); // delete the object
		lv_obj_del(obj_cairdio_logo); // delete the object
		lv_obj_del(obj_bluetooth_status); // delete the object
		lv_obj_del(obj_battery_status); // delete the object
        
        k_sleep(K_MSEC(10));
    } // end of while loop.

    return 0;
}
// ------------------ End of Main Thread ------------------

// ------------------------ End of File ------------------------
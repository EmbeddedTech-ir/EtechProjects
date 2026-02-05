import serial
import numpy as np
import matplotlib.pyplot as plt
import time

# ==========================================
# CONFIGURATION
# ==========================================
COM_PORT = 'COM4'      # <--- CHANGE THIS
BAUD_RATE = 921600
MAX_DIST_MM = 1000
DRAW_FPS = 30          # Limit drawing FPS
# ==========================================

ser = None
current_mode = 8       # 4x4 or 8x8
last_draw_time = 0
DRAW_INTERVAL = 1.0 / DRAW_FPS


def on_key_press(event):
    global ser, current_mode
    if ser is None or not ser.is_open:
        return

    if event.key == '4':
        print("\n>> Requesting 4x4 Mode...")
        ser.write(b'4')
        current_mode = 4

    elif event.key == '8':
        print("\n>> Requesting 8x8 Mode...")
        ser.write(b'8')
        current_mode = 8


def run_heatmap():
    global ser, last_draw_time

    # ==========================================
    # SERIAL SETUP
    # ==========================================
    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
        ser.reset_input_buffer()
        print(f"Connected to {COM_PORT}")
    except serial.SerialException as e:
        print(f"ERROR: {e}")
        return

    print("--------------------------------------------")
    print("Press '4' -> 4x4 mode (60Hz)")
    print("Press '8' -> 8x8 mode (15Hz)")
    print("--------------------------------------------")

    # ==========================================
    # PLOT SETUP
    # ==========================================
    plt.ion()
    fig, ax = plt.subplots(figsize=(7, 6))
    fig.canvas.mpl_connect('key_press_event', on_key_press)

    data = np.zeros((4, 4))
    im = ax.imshow(
        data,
        cmap='jet_r',
        vmin=0,
        vmax=MAX_DIST_MM,
        interpolation='nearest'
    )

    cbar = plt.colorbar(im)
    cbar.set_label('Distance (mm)', rotation=270, labelpad=15)

    ax.set_title("VL53L7CX Heatmap (Press 4 / 8)")
    ax.set_xticks([])
    ax.set_yticks([])

    # ==========================================
    # MAIN LOOP
    # ==========================================
    try:
        while plt.fignum_exists(fig.number):

            # -------- Read latest serial line --------
            raw_line = None
            while ser.in_waiting > 0:
                raw_line = ser.readline().decode('utf-8', errors='ignore').strip()

            if not raw_line or ',' not in raw_line:
                continue

            # -------- Parse data --------
            try:
                values = np.fromstring(raw_line, dtype=np.int16, sep=',')
            except ValueError:
                continue

            if current_mode == 4 and values.size == 16:
                matrix = values.reshape(4, 4)

            elif current_mode == 8 and values.size == 64:
                matrix = values.reshape(8, 8)

            else:
                continue

            # -------- FPS-limited drawing --------
            now = time.time()
            if now - last_draw_time >= DRAW_INTERVAL:
                im.set_data(matrix)
                fig.canvas.draw_idle()
                fig.canvas.flush_events()
                last_draw_time = now

    except KeyboardInterrupt:
        print("\nStopping...")

    finally:
        if ser:
            ser.close()
        plt.close()


if name == "__main__":
    run_heatmap()
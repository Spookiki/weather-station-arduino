import serial
import json
import threading
from flask import Flask, jsonify, render_template
from datetime import datetime

SERIAL_PORT = "COM3"
BAUD_RATE   = 9600

app = Flask(__name__)
start_time = datetime.now()

latest_data = {
    "temp": None,
    "hum":  None,
    "gaz":  None,
    "gaz_ok": None,
    "pm25": None,
    "pm10": None,
    "last_update": None
}

def read_serial():
    global latest_data
    while True:
        try:
            ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
            print(f"[OK] Connecte sur {SERIAL_PORT} a {BAUD_RATE} baud")
            while True:
                line = ser.readline().decode("utf-8").strip()
                if line.startswith("{"):
                    try:
                        data = json.loads(line)
                        data["last_update"] = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
                        latest_data = data
                        print(f"[DATA] {data}")
                    except json.JSONDecodeError:
                        print(f"[WARN] JSON invalide : {line}")
        except serial.SerialException as e:
            print(f"[ERREUR] Serial : {e} — nouvelle tentative dans 5s")
            import time
            time.sleep(5)


@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/data")
def api_data():
    data = dict(latest_data)
    elapsed = datetime.now() - start_time
    s = int(elapsed.total_seconds())
    h, rem = divmod(s, 3600)
    m, sec = divmod(rem, 60)
    if h > 0:
        data["uptime"] = f"{h}h {m}min"
    elif m > 0:
        data["uptime"] = f"{m}min {sec}s"
    else:
        data["uptime"] = f"{sec}s"
    return jsonify(data)

if __name__ == "__main__":
    thread = threading.Thread(target=read_serial, daemon=True)
    thread.start()
    print("[OK] Serveur Flask sur http://localhost:5000")
    app.run(debug=False, host="0.0.0.0", port=5000)
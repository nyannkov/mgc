from flask import Flask, send_from_directory, jsonify
import yaml, os, argparse

parser = argparse.ArgumentParser(description="Animation Viewer for mgc anim_gen output")
parser.add_argument("--yaml", required=True, help="Animation YAML file path")
parser.add_argument("--port", type=int, default=5000)
parser.add_argument("--host", default="127.0.0.1", help="Host IP to bind (default: 127.0.0.1)")
args = parser.parse_args()

YAML_PATH = os.path.abspath(args.yaml)
RESOURCE_DIR = os.path.dirname(YAML_PATH)
STATIC_DIR = os.path.join(os.path.dirname(__file__), "static")

app = Flask(__name__, static_folder=STATIC_DIR)

@app.route("/api/anim")
def get_anim():
    with open(YAML_PATH, "r", encoding="utf-8") as f:
        data = yaml.safe_load(f)
    return jsonify({
        "yaml_name": os.path.basename(YAML_PATH),
        "data": data
    })

@app.route("/res/<path:filename>")
def get_resource(filename):
    return send_from_directory(RESOURCE_DIR, filename)

@app.route("/")
def index():
    return send_from_directory(STATIC_DIR, "index.html")

@app.route("/viewer.js")
def viewer_js():
    return send_from_directory(STATIC_DIR, "viewer.js")

if __name__ == "__main__":
    app.run(host=args.host, port=args.port, debug=True)

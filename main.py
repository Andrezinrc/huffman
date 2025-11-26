import os
import ctypes
from flask import Flask, request, render_template, send_file

app = Flask(__name__)

lib_path = os.path.join(os.path.dirname(__file__), "libhuffman.so")
huff = ctypes.CDLL(lib_path)

huff.compressToFile.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
huff.decompressFromFile.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
huff.compressToFile.restype = ctypes.c_int
huff.decompressFromFile.restype = ctypes.c_int

UPLOAD_FOLDER = os.path.join(os.path.dirname(__file__), "uploads")
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route("/", methods=["GET", "POST"])
def index():
    result = None
    download = None

    if request.method == "POST":
        file = request.files.get("file")
        action = request.form.get("action")

        if not file:
            result = "Nenhum arquivo enviado."
            return render_template("index.html", result=result)

        input_path = os.path.join(UPLOAD_FOLDER, file.filename)
        file.save(input_path)

        if action == "compress":
            output_path = input_path + ".adr"
            ok = huff.compressToFile(input_path.encode(), output_path.encode())
            if ok:
                return send_file(output_path, as_attachment=True)
            else:
                result = "Erro ao comprimir o arquivo."

        elif action == "decompress":
            if not file.filename.endswith(".adr"):
                result = "Somente arquivos .adr podem ser descomprimidos."
                return render_template("index.html", result=result)

            output_prefix = input_path.replace(".adr", "")
            ok = huff.decompressFromFile(input_path.encode(), output_prefix.encode())

            if ok:
                folder = os.path.dirname(output_prefix)
                base = os.path.basename(output_prefix)

                for f in os.listdir(folder):
                    if f.startswith(base):
                        fullpath = os.path.join(folder, f)
                        return send_file(fullpath, as_attachment=True)

                result = "Erro ao localizar arquivo descompactado."
            else:
                result = "Erro ao descomprimir o arquivo."

    return render_template("index.html", result=result)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)

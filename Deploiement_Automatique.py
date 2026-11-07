import os
import subprocess
import time
import http.server
import urllib.request
import webbrowser
import threading
import json
import re

# =========================================================
# CONFIGURATION AUTOMATIQUE
# =========================================================
PROJECT_PATH = os.path.dirname(os.path.abspath(__file__))
PROJECT_NAME = "NetXDuoEthernet"
SERVER_PORT = 8080
WEB_DIR = os.path.join(PROJECT_PATH, "WebDashboard")

def log(msg, color=""):
    print(f"{color}{msg}\033[0m")

def search_tool(filename, search_dirs):
    log(f"[*] Recherche de {filename}...", "\033[93m")
    for d in search_dirs:
        if os.path.exists(d):
            for root, dirs, files in os.walk(d):
                if filename in files:
                    return os.path.join(root, filename)
    return None

def update_simulation_mode(mode_value):
    sensor_h = os.path.join(PROJECT_PATH, "Core", "Src", "Sensors", "sensor_manager.h")
    if not os.path.exists(sensor_h):
        return
    
    with open(sensor_h, "r", encoding="utf-8") as f:
        content = f.read()
    
    # Remplacer la définition SENSOR_SIMULATION_MODE
    new_content = re.sub(r'#define\s+SENSOR_SIMULATION_MODE\s+\d', f'#define SENSOR_SIMULATION_MODE {mode_value}', content)
    
    if new_content != content:
        with open(sensor_h, "w", encoding="utf-8") as f:
            f.write(new_content)
        log(f"[+] Mode simulation défini sur {mode_value} dans sensor_manager.h", "\033[92m")

# =========================================================
# SERVEUR PROXY: Connecte le Dashboard Local au vrai STM32
# =========================================================
class STMProxyHandler(http.server.SimpleHTTPRequestHandler):
    stm32_ip = None

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB_DIR, **kwargs)

    def do_GET(self):
        # Si c'est une requête API, on la redirige vers la VRAIE carte STM32 !
        if self.path.startswith("/api/") or self.path.startswith("/Get"):
            if not self.stm32_ip:
                self.send_error(500, "IP STM32 non configurée")
                return
            
            try:
                # Interroge le STM32
                url = f"http://{self.stm32_ip}{self.path}"
                req = urllib.request.Request(url, method="GET")
                with urllib.request.urlopen(req, timeout=3) as response:
                    data = response.read()
                    
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.end_headers()
                self.wfile.write(data)
            except Exception as e:
                self.send_error(502, f"Erreur de communication avec STM32: {e}")
        else:
            # Sinon, on sert les fichiers HTML/CSS du dashboard situés sur le PC
            super().do_GET()

    def log_message(self, format, *args):
        pass # Cache les logs serveurs pour garder la console propre

def start_dashboard_server(ip_stm32):
    STMProxyHandler.stm32_ip = ip_stm32
    server = http.server.ThreadingHTTPServer(("", SERVER_PORT), STMProxyHandler)
    log(f"\n[+] Serveur Web Proxy démarré sur http://localhost:{SERVER_PORT}", "\033[92m")
    log(f"[+] Le Dashboard affichera les vraies données du STM32 ({ip_stm32})", "\033[96m")
    
    # Ouvre le navigateur
    webbrowser.open(f"http://localhost:{SERVER_PORT}")
    server.serve_forever()

# =========================================================
# SCRIPT PRINCIPAL
# =========================================================
def main():
    os.system('cls' if os.name == 'nt' else 'clear')
    log("=========================================================", "\033[96m")
    log("🚀 AUTO-DEPLOIMENT STM32 INDUSTRIAL MONITOR", "\033[96m")
    log("=========================================================\n", "\033[96m")

    # 1. Désactiver le mode simulation
    update_simulation_mode(0)

    # 2. Chercher STM32CubeIDE (stm32cubeidec.exe) et STM32_Programmer_CLI.exe
    search_paths = ["C:\\ST", "C:\\Program Files\\STMicroelectronics", "D:\\ST"]
    
    ide_path = search_tool("stm32cubeidec.exe", search_paths)
    cli_path = search_tool("STM32_Programmer_CLI.exe", search_paths)

    if not ide_path or not cli_path:
        log("\n[ERREUR] Impossible de trouver STM32CubeIDE ou STM32CubeProgrammer.", "\033[91m")
        log("Veuillez compiler et flasher manuellement la carte via STM32CubeIDE.", "\033[93m")
    else:
        log("\n[*] Outils trouvés :", "\033[92m")
        log(f"    IDE: {ide_path}")
        log(f"    CLI: {cli_path}\n")
        
        # 3. Compilation manuelle requise (car Eclipse verrouille l'espace de travail)
        log("\n=========================================================", "\033[93m")
        log("⚠️ ACTION REQUISE DANS STM32CubeIDE ⚠️", "\033[93m")
        log("=========================================================\n", "\033[93m")
        log("1. Allez dans votre fenêtre STM32CubeIDE.", "\033[97m")
        log("2. Cliquez sur le bouton 'Build' (l'icône Marteau 🔨).", "\033[97m")
        log("3. Attendez la fin de la compilation (Build Finished).", "\033[97m")
        
        input("\n\033[92mUne fois la compilation terminée, appuyez sur ENTRÉE ici pour flasher... \033[0m")
        
        # Vérification du fichier ELF pour être sûr qu'il est récent
        elf_file = os.path.join(PROJECT_PATH, "Debug", f"{PROJECT_NAME}.elf")
        if not os.path.exists(elf_file):
            log(f"\n[ERREUR] Le fichier {elf_file} n'existe pas. Compilation échouée.", "\033[91m")
            return
            
        mtime = os.path.getmtime(elf_file)
        if time.time() - mtime > 600:
            log("\n[ATTENTION] Le fichier .elf semble vieux. Êtes-vous sûr d'avoir cliqué sur Build ?", "\033[93m")

        # 4. Flashage
        log("\n=> FLASHAGE DE LA CARTE STM32... (Ne débranchez pas !)", "\033[96m")
        cmd_flash = [cli_path, "-c", "port=SWD", "-d", elf_file, "-v", "-rst"]
        res = subprocess.run(cmd_flash, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        if "File download complete" in res.stdout or "Verified OK" in res.stdout:
            log("[V] Flashage réussi ! La carte redémarre.", "\033[92m")
        else:
            log(f"\n[ERREUR] Impossible de flasher la carte. ST-Link branché et STM32CubeIDE en mode debug désactivé?", "\033[91m")
            print(res.stdout)
            return

    # 5. Connexion Dashboard Serveur
    log("\n=========================================================", "\033[96m")
    log("🔌 CONFIGURATION DU DASHBOARD INTELLIGENT", "\033[96m")
    log("=========================================================\n", "\033[96m")
    log("Le code est dans la carte STM32 ! Branchez le câble Ethernet.")
    log("Attendez 10 secondes le temps que la carte reçoive une IP...\n")
    
    ip_stm32 = input("\033[93mVeuillez entrer l'adresse IP de votre STM32 (vue sur le Port Série) : \033[0m")
    
    if ip_stm32.strip():
        # Lancer le serveur local
        start_dashboard_server(ip_stm32.strip())
    else:
        log("IP invalide. Fermeture du script.", "\033[91m")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        log("\n[!] Script annulé par l'utilisateur.", "\033[91m")
